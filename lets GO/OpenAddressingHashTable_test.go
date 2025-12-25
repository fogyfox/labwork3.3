// OpenAddressingHashTable_test.go
package main

import (
	"bytes"
	"strconv"
	"testing"
)

func TestOpenAddressing(t *testing.T) {
	// Создаем маленькую таблицу для провокации рехеша (размер 2, порог 0.5)
	ht := NewOpenAddressingHashTable[string, int](2, 0.5)

	// Тест вставки и поиска
	ht.Insert("A", 1)
	val, found := ht.Find("A")
	if !found || val != 1 {
		t.Error("Не удалось найти вставленный элемент")
	}

	// Тест рехеширования (вставляем второй элемент, вызываем расширение)
	ht.Insert("B", 2)
	if ht.capacity <= 2 {
		t.Error("Рехеширование не увеличило емкость")
	}

	// Тест удаления и состояния Deleted
	ht.Remove("A")
	_, found = ht.Find("A")
	if found {
		t.Error("Элемент 'A' не должен быть найден после удаления")
	}

	// Тест вставки в Deleted ячейку
	ht.Insert("A", 100)
	val, _ = ht.Find("A")
	if val != 100 {
		t.Errorf("Ожидалось 100, получено %v", val)
	}
	// Создаем таблицу
    ht = NewOpenAddressingHashTable[string, int](10, 0.75)

    // Вставляем данные, чтобы сработал if ht.table[i].State == Occupied
    ht.Insert("apple", 100)
    ht.Insert("banana", 200)

    // Вызываем метод, содержащий твой цикл (например, Display)
    // Это "прогонит" счетчик покрытия через fmt.Printf
    ht.Display()
}

func TestOpenAddressingSerialization(t *testing.T) {
	// Инициализируем таблицу: [int]string
	ht := NewOpenAddressingHashTable[int, string](10, 0.7)
	ht.Insert(1, "Data1")
	ht.Insert(42, "Data42")
	ht.Remove(1) // Проверим, что удаленные не сохраняются
	ht.Insert(2, "Data2")

	// Бинарный тест
	t.Run("Binary Test", func(t *testing.T) {
		buf := new(bytes.Buffer)
		if err := ht.SerializeBinary(buf); err != nil {
			t.Fatal(err)
		}

		newHt := NewOpenAddressingHashTable[int, string](10, 0.7)
		if err := newHt.DeserializeBinary(buf); err != nil {
			t.Fatal(err)
		}

		if val, found := newHt.Find(42); !found || val != "Data42" {
			t.Error("Ошибка восстановления ключа 42")
		}
		if _, found := newHt.Find(1); found {
			t.Error("Удаленный ключ 1 не должен был восстановиться")
		}
	})

	// Текстовый тест
	t.Run("Text Test", func(t *testing.T) {
		buf := new(bytes.Buffer)
		ht.SerializeText(buf)

		parseK := func(s string) int { v, _ := strconv.Atoi(s); return v }
		parseV := func(s string) string { return s }

		newHt := NewOpenAddressingHashTable[int, string](10, 0.7)
		if err := newHt.DeserializeText(buf, parseK, parseV); err != nil {
			t.Fatal(err)
		}

		if val, _ := newHt.Find(2); val != "Data2" {
			t.Errorf("Ожидалось Data2, получено %v", val)
		}
	})
}