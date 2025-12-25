package main

import (
	"bytes"
	"strconv"
	"testing"
)

func TestChainingHashTable(t *testing.T) {
	// Инициализация (маленький размер для теста рехеша)
	ht := NewChainingHashTable[string, int](2, 0.7)

	// Тест вставки и поиска
	ht.Insert("apple", 10)
	val, found := ht.Find("apple")
	if !found || val != 10 {
		t.Error("Ошибка вставки или поиска")
	}

	// Тест обновления существующего ключа
	ht.Insert("apple", 20)
	val, _ = ht.Find("apple")
	if val != 20 {
		t.Error("Значение не обновилось")
	}

	// Тест рехеширования (вставляем больше элементов, чем позволяет LF)
	ht.Insert("banana", 30)
	ht.Insert("cherry", 40) 
	if ht.capacity <= 2 {
		t.Error("Рехеширование не сработало")
	}

	// Тест поиска несуществующего ключа
	_, found = ht.Find("unknown")
	if found {
		t.Error("Найден несуществующий ключ")
	}

	// Тест удаления
	if !ht.Remove("banana") {
		t.Error("Не удалось удалить существующий ключ")
	}
	if ht.Remove("banana") {
		t.Error("Удален уже удаленный ключ")
	}

	// Тест вывода (для покрытия кода печати)
	ht.Display()
}

func TestChainingTableSerialization(t *testing.T) {
	// Создаем таблицу [string]int
	ht := NewChainingHashTable[string, int](10, 0.7)
	ht.Insert("apple", 100)
	ht.Insert("banana", 200)

	// 1. Тест БИНАРНОЙ сериализации
	t.Run("Binary (Gob)", func(t *testing.T) {
		buf := new(bytes.Buffer)
		if err := ht.SerializeBinary(buf); err != nil {
			t.Fatalf("Binary serialize error: %v", err)
		}

		newHt := NewChainingHashTable[string, int](10, 0.7)
		if err := newHt.DeserializeBinary(buf); err != nil {
			t.Fatalf("Binary deserialize error: %v", err)
		}

		if val, found := newHt.Find("apple"); !found || val != 100 {
			t.Errorf("Expected 100 for apple, got %d", val)
		}
	})

	// 2. Тест ТЕКСТОВОЙ сериализации
	t.Run("Text (Key:Value)", func(t *testing.T) {
		buf := new(bytes.Buffer)
		if err := ht.SerializeText(buf); err != nil {
			t.Fatalf("Text serialize error: %v", err)
		}

		// Функции для конвертации строк обратно в типы K и V
		parseKey := func(s string) string { return s }
		parseVal := func(s string) int {
			v, _ := strconv.Atoi(s)
			return v
		}

		newHt := NewChainingHashTable[string, int](10, 0.7)
		if err := newHt.DeserializeText(buf, parseKey, parseVal); err != nil {
			t.Fatalf("Text deserialize error: %v", err)
		}

		if val, found := newHt.Find("banana"); !found || val != 200 {
			t.Errorf("Expected 200 for banana, got %d", val)
		}
	})
}