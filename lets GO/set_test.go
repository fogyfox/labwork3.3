package main

import (
	"os"
	"testing"
	"bytes"
)

func TestSet(t *testing.T) {
	s := NewSet(4)

	// Вставка и дубликаты
	s.Insert(10)
	s.Insert(10) // Покрытие проверки на существование
	if s.itemCount != 1 {
		t.Error("Множество не должно содержать дубликатов")
	}

	// Rehash
	for i := 0; i < 10; i++ {
		s.Insert(i)
	}

	// Удаление
	s.Remove(10)
	if s.Contains(10) {
		t.Error("Элемент не удален")
	}

	// Файловые операции
	fname := "test_set.txt"
	s.SaveToFile(fname)
	defer os.Remove(fname)

	s2 := NewSet(10)
	s2.LoadFromFile(fname)
	if !s2.Contains(1) {
		t.Error("Ошибка загрузки из файла")
	}
	s2.LoadFromFile("non_existent.txt") // Покрытие ошибки открытия

	// Тест Partition (Алгоритм разбиения)
	sPart := NewSet(5)
	sPart.Insert(1)
	sPart.Insert(2)
	sPart.Insert(3)
	// Сумма 6, делим на подмножества с суммой 3
	res, ok := sPart.Partition(3)
	if !ok || len(res) != 2 {
		t.Error("Partition не нашел решение")
	}
	
	sPart.Partition(100) // Покрытие ветки total%subsetSum != 0

	// Тестируем пустое множество
    s = NewSet(5)
    
    if s.Size() != 0 {
        t.Errorf("Ожидался размер 0, получили %d", s.Size())
    }
    
    if !s.IsEmpty() {
        t.Error("IsEmpty должен возвращать true для нового множества")
    }
    
    // Покрываем Print для пустого состояния
    s.Print() 

    // Тестируем заполненное множество
    s.Insert(10)
    s.Insert(20)
    s.Insert(30)

    if s.Size() != 3 {
        t.Errorf("Ожидался размер 3, получили %d", s.Size())
    }

    if s.IsEmpty() {
        t.Error("IsEmpty должен возвращать false после вставки")
    }

    // Покрываем вложенные циклы в Print
    // Внутренний цикл 'for curr != nil' сработает для каждой цепочки (bucket)
    s.Print()
}

func TestSetSerialization(t *testing.T) {
	// Создаем множество и добавляем данные
	s := NewSet(10)
	s.Insert(10)
	s.Insert(20)
	s.Insert(30)
	s.Insert(20) // Дубликат, не должен влиять на размер

	// Тест бинарной сериализации
	t.Run("Binary Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)
		if err := s.SerializeBinary(buf); err != nil {
			t.Fatalf("Ошибка сохранения: %v", err)
		}

		newS := NewSet(10)
		if err := newS.DeserializeBinary(buf); err != nil {
			t.Fatalf("Ошибка загрузки: %v", err)
		}

		if !newS.Contains(10) || !newS.Contains(30) {
			t.Error("Данные потеряны при бинарной десериализации")
		}
		// Проверяем количество (itemCount должен быть 3)
		if len(newS.ToVector()) != 3 {
			t.Errorf("Неверное количество элементов: %d", len(newS.ToVector()))
		}
	})

	// Тест текстовой сериализации
	t.Run("Text Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)
		s.SerializeText(buf)

		newS := NewSet(10)
		if err := newS.DeserializeText(buf); err != nil {
			t.Fatalf("Ошибка загрузки текста: %v", err)
		}

		if !newS.Contains(20) {
			t.Error("Элемент 20 не найден после текстовой загрузки")
		}
	})
}