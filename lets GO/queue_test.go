package main

import (
	"bytes"
	"testing"
)

func TestQueue(t *testing.T) {
	q := NewStringQueue()

	// Пустая очередь
	if !q.IsEmpty() {
		t.Error("Очередь должна быть пустой")
	}
	q.Pop() // Покрытие Println "Очередь пустая"

	// Push/Pop
	q.Push("First")
	q.Push("Second")
	if q.Size() != 2 {
		t.Error("Неверный размер")
	}

	if q.Pop() != "First" {
		t.Error("Нарушен принцип FIFO")
	}

	// GetByIndex
	val := q.GetByIndex(0)
	if val != "Second" {
		t.Error("Ошибка получения по индексу")
	}
	q.GetByIndex(10) // Покрытие ошибки индекса
	q.GetByIndex(-1)

	q = NewStringQueue()

    // Покрываем ветку "Очередь пустая"
    q.Print() 

    // Добавляем элементы
    q.Push("A")
    q.Push("B")

    // Покрываем основной цикл вывода
    q.Print()

	
}

func TestQueueSerialization(t *testing.T) {
	// Создаем и наполняем очередь
	q := NewStringQueue()
	q.Push("First")
	q.Push("Second")
	q.Push("Third")

	// Тест бинарной сериализации
	t.Run("Binary Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)
		if err := q.SerializeBinary(buf); err != nil {
			t.Fatalf("Ошибка сохранения: %v", err)
		}

		newQ := NewStringQueue()
		if err := newQ.DeserializeBinary(buf); err != nil {
			t.Fatalf("Ошибка загрузки: %v", err)
		}

		if newQ.Size() != 3 {
			t.Errorf("Ожидался размер 3, получено %d", newQ.Size())
		}
		if val := newQ.Pop(); val != "First" {
			t.Errorf("Ожидался 'First', получено '%s'", val)
		}
	})

	// Тест текстовой сериализации
	t.Run("Text Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)
		q.SerializeText(buf)

		newQ := NewStringQueue()
		if err := newQ.DeserializeText(buf); err != nil {
			t.Fatalf("Ошибка загрузки текста: %v", err)
		}

		// Проверяем порядок (последовательно извлекая элементы)
		newQ.Pop() // Пропускаем 'First'
		if val := newQ.Pop(); val != "Second" {
			t.Errorf("Порядок нарушен: ожидался 'Second', получено '%s'", val)
		}
	})
}