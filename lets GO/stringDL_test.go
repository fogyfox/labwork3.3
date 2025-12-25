package main

import (
	"bytes"
	"testing"
)

func TestStringDL(t *testing.T) {
	list := NewStringDL()

	// Добавление
	list.AddHead("B")
	list.AddHead("A")
	list.AddTail("C")

	// Поиск
	nodeB := list.Find("B")
	if nodeB == nil || nodeB.Person != "B" {
		t.Error("Узел B не найден")
	}

	// AddBefore
	list.AddBefore(nodeB, "A.5")
	if nodeB.Prev.Person != "A.5" {
		t.Error("AddBefore не сработал")
	}
	list.AddBefore(list.head, "First") // Тест вставки перед головой

	// Удаление
	list.DeleteByValue("B")
	list.DeleteHead()
	list.DeleteTail()

	// Крайние случаи
	list.DeleteByValue("non-existent")
	emptyList := NewStringDL()
	emptyList.DeleteHead()
	emptyList.DeleteTail()
	
	// Список из одного элемента
	single := NewStringDL()
	single.AddHead("One")
	single.DeleteByValue("One")
	if single.head != nil || single.tail != nil {
		t.Error("Ошибка очистки списка из одного элемента")
	}

	// Покрываем Print и PrintRecursive для пустого списка
    list.Print()
    list.PrintRecursive()

    // Добавляем элементы
    list.AddHead("A")
    list.AddTail("C")

    // Покрываем AddAfter для вставки в середину (между A и C)
    ptrA := list.Find("A")
    list.AddAfter(ptrA, "B")
    
    // Покрываем AddAfter для вставки в самый конец (обновление tail)
    ptrC := list.Find("C")
    list.AddAfter(ptrC, "D")
    
    if list.tail.Person != "D" {
        t.Error("Tail не обновился после AddAfter")
    }

    //Покрываем проверку на nil в AddAfter
    list.AddAfter(nil, "Error")

    // Покрываем циклы и рекурсию для заполненного списка
    // Это пройдет по всем узлам A -> B -> C -> D
    list.Print()
    list.PrintRecursive()
}

func TestStringDLSerialization(t *testing.T) {
	// Создаем список: Alice <-> Bob <-> Charlie
	list := NewStringDL()
	list.AddTail("Alice")
	list.AddTail("Bob")
	list.AddTail("Charlie")

	// Тест бинарной сериализации
	t.Run("Binary Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)
		if err := list.SerializeBinary(buf); err != nil {
			t.Fatal(err)
		}

		newList := NewStringDL()
		if err := newList.DeserializeBinary(buf); err != nil {
			t.Fatal(err)
		}

		// Проверка связей: идем до конца и возвращаемся назад
		if newList.head.Person != "Alice" || newList.tail.Person != "Charlie" {
			t.Error("Голова или хвост установлены неверно")
		}
		if newList.tail.Prev.Person != "Bob" {
			t.Errorf("Обратная связь повреждена: ожидался Bob, получен %s", newList.tail.Prev.Person)
		}
	})

	// Тест текстовой сериализации
	t.Run("Text Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)
		list.SerializeText(buf)

		newList := NewStringDL()
		newList.DeserializeText(buf)

		if newList.head.Next.Next.Person != "Charlie" {
			t.Error("Последовательность элементов нарушена")
		}
	})
}