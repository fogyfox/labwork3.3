// stack_test.go
package main

import (
	"bytes"
	"testing"
)

func TestStack(t *testing.T) {
	s := NewStack()

	s.Push("Bottom")
	s.Push("Top")

	if s.Peek() != "Top" {
		t.Errorf("Ожидалась вершина 'Top', получили %s", s.Peek())
	}

	if s.Pop() != "Top" {
		t.Error("Pop вернул неверный элемент")
	}

	s.Pop()
	if !s.IsEmpty() {
		t.Error("Стек должен быть пустым")
	}

    // Тестирование Print
    
    s.Print() 

    // Добавляем элементы для дальнейших тестов
    s.Push("Bottom")
    s.Push("Middle")
    s.Push("Top")

    //Покрываем вывод заполненного стека (цикл for curr != nil)
    s.Print()

    //Тестирование GetByIndex
    val := s.GetByIndex(0)
    if val != "Top" {
        t.Errorf("Ожидался 'Top', получили %s", val)
    }

    //Покрываем цикл прохода по индексам (например, индекс 1 — Middle)
    valMid := s.GetByIndex(1)
    if valMid != "Middle" {
        t.Errorf("Ожидался 'Middle', получили %s", valMid)
    }

    // Покрываем ошибки "Выход за диапазон"
    s.GetByIndex(-1)  // Условие inx < 0
    s.GetByIndex(10)  // Условие inx >= s.stackSize
}

func TestStackSerialization(t *testing.T) {
	st := NewStack()
	st.Push("Bottom")
	st.Push("Middle")
	st.Push("Top")

	// Тест бинарной сериализации
	t.Run("Binary Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)
		if err := st.SerializeBinary(buf); err != nil {
			t.Fatal(err)
		}

		newSt := NewStack()
		if err := newSt.DeserializeBinary(buf); err != nil {
			t.Fatal(err)
		}

		if newSt.Size() != 3 || newSt.Peek() != "Top" {
			t.Errorf("Ошибка: размер %d, вершина %s", newSt.Size(), newSt.Peek())
		}
	})

	// Тест текстовой сериализации
	t.Run("Text Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)
		st.SerializeText(buf)

		newSt := NewStack()
		if err := newSt.DeserializeText(buf); err != nil {
			t.Fatal(err)
		}

		if newSt.Pop() != "Top" || newSt.Pop() != "Middle" {
			t.Error("Порядок элементов нарушен при текстовом восстановлении")
		}
	})
}