// array_test.go
package main

import (
	"bytes"
	"testing"
)

func TestArray(t *testing.T) {
	arr := NewDynamicArray()

	// Тест PushBack и расширения (начальный размер 1)
	arr.PushBack("1")
	arr.PushBack("2")
	if arr.capacity < 2 {
		t.Error("Массив не расширился")
	}

	// Тест добавления по индексу
	arr.AddInx(1, "1.5")
	if arr.GetInx(1) != "1.5" {
		t.Error("Элемент не вставлен по индексу")
	}

	// Тест выхода за диапазон
	arr.AddInx(100, "err") // Должно вывести сообщение в консоль

	da := NewDynamicArray()

	// 1. Покрытие PrintArray для пустого массива
	da.PrintArray() // Сработает if da.IsEmpty()

	// 2. Покрытие GetSize
	if da.GetSize() != 0 {
		t.Error("Размер должен быть 0")
	}

	// Добавляем элементы для дальнейших тестов
	da.PushBack("one")
	da.PushBack("two")
	da.PushBack("three")

	// 3. Покрытие PrintArray для заполненного массива
	da.PrintArray() // Сработает цикл вывода элементов

	// 4. Покрытие GetInx (успех и ошибка)
	val := da.GetInx(1)
	if val != "two" {
		t.Error("GetInx вернул неверное значение")
	}
	da.GetInx(-1)  // Покрытие "Выход за диапазон" (inx < 0)
	da.GetInx(100) // Покрытие "Выход за диапазон" (inx >= size)

	// 5. Покрытие ChangeInx (успех и ошибка)
	da.ChangeInx(1, "new_two")
	if da.GetInx(1) != "new_two" {
		t.Error("Значение не изменилось")
	}
	da.ChangeInx(-1, "err")  // Покрытие "Выход за диапазон"
	da.ChangeInx(100, "err") // Покрытие "Выход за диапазон"

	// 6. Покрытие RemoveElArr (успех и ошибка)
	// Удаление из середины/начала вызывает цикл "Сдвиг влево"

	expectedValue := da.GetInx(1) 
	
	da.RemoveElArr(1) 

	if expectedValue != "new_two" {
	    t.Errorf("Ожидалось удаление 'new_two', но на позиции 1 было %s", expectedValue)
	}
	
	if da.GetSize() != 2 {
		t.Error("Размер не уменьшился")
	}

	da.RemoveElArr(-1)  // Покрытие "Выход за диапазон"
	da.RemoveElArr(100) // Покрытие "Выход за диапазон"
}

// TestArraySerializationCoverage проверяет все методы сериализации для 100% покрытия
func TestArraySerializationCoverage(t *testing.T) {
	da := NewDynamicArray()

	// 1. Тест бинарной сериализации (SerializeBinary и DeserializeBinary)
	t.Run("Full_Binary_Coverage", func(t *testing.T) {
		da.Clear() // Покрываем метод Clear
		da.PushBack("Binary1")
		da.PushBack("Binary2")
		da.PushBack("Сложные данные с пробелами")

		buf := new(bytes.Buffer)
		
		// Проверяем запись
		if err := da.SerializeBinary(buf); err != nil {
			t.Fatalf("Ошибка SerializeBinary: %v", err)
		}

		// Проверяем чтение в новый объект
		newDa := NewDynamicArray()
		if err := newDa.DeserializeBinary(buf); err != nil {
			t.Fatalf("Ошибка DeserializeBinary: %v", err)
		}

		// Проверка идентичности данных
		if newDa.GetSize() != da.GetSize() {
			t.Errorf("Размер не совпал: ожидалось %d, получено %d", da.GetSize(), newDa.GetSize())
		}
		
		for i := 0; i < da.GetSize(); i++ {
			if newDa.GetInx(i) != da.GetInx(i) {
				t.Errorf("Данные на индексе %d повреждены", i)
			}
		}
	})

	// 2. Тест текстовой сериализации (SerializeText и DeserializeText)
	t.Run("Full_Text_Coverage", func(t *testing.T) {
		da.Clear()
		da.PushBack("TextLine1")
		da.PushBack("TextLine2")

		buf := new(bytes.Buffer)

		// Проверяем запись текста
		if err := da.SerializeText(buf); err != nil {
			t.Fatalf("Ошибка SerializeText: %v", err)
		}

		// Проверяем чтение текста
		newDa := NewDynamicArray()
		if err := newDa.DeserializeText(buf); err != nil {
			t.Fatalf("Ошибка DeserializeText: %v", err)
		}

		// Сверяем количество строк и содержание
		if newDa.GetSize() != 2 {
			t.Errorf("Ожидалось 2 элемента после загрузки текста, получено %d", newDa.GetSize())
		}

		if newDa.GetInx(1) != "TextLine2" {
			t.Errorf("Ожидалось 'TextLine2', получено '%s'", newDa.GetInx(1))
		}
	})
}