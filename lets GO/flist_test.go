package main

import (
	"testing"
	"bytes"
)

func TestStringOL(t *testing.T) {
	list := NewStringOL()

	// Добавление
	list.AddHead("B")
	list.AddHead("A")
	list.AddTail("C")

	// Поиск
	node := list.Find("B")
	if node == nil || node.Person != "B" {
		t.Error("Узел не найден")
	}

	// Добавление после узла
	list.AddAfter("B", "B1")
	if list.Find("B1") == nil {
		t.Error("AddAfter не сработал")
	}

	// Удаление головы
	list.DeleteHead()
	if list.Find("A") != nil {
		t.Error("DeleteHead не удалил голову")
	}

	// Удаление после узла
	list.DeleteAfter(list.Find("B"))
	if list.Find("B1") != nil {
		t.Error("DeleteAfter не удалил узел")
	}

	// Удаление по значению (включая повторяющиеся и голову)
	list.AddHead("X")
	list.AddTail("X")
	list.DeleteByValue("X")
	if list.Find("X") != nil {
		t.Error("DeleteByValue не удалил все вхождения")
	}

	// Печать
	list.Print()
	list.PrintRecursive()
    
    // Граничные случаи
    list.DeleteAfter(nil)
    list.AddAfter("non-existent", "val")

	// Тестирование AddBefore
	l1 := NewStringOL()
	l1.AddBefore("Any", "New") // 1. Покрывает if s.head == nil

	l1.AddHead("Target")
	l1.AddBefore("Target", "NewHead") // 2. Покрывает if s.head.Person == targetValue
	if l1.head.Person != "NewHead" {
		t.Error("AddBefore не вставил в начало")
	}

	l1.AddTail("End")
	l1.AddBefore("End", "Middle") // Покрывает цикл for и вставку в середину
	if l1.Find("Middle") == nil {
		t.Error("AddBefore не вставил в середину")
	}
	
	l1.AddBefore("NonExistent", "Ignore") // Покрывает случай, когда cur == nil (не нашли)

	// Тестирование DeleteTail 
	l2 := NewStringOL()
	l2.DeleteTail() //  Покрывает if s.head == nil

	l2.AddHead("OnlyOne")
	l2.DeleteTail() //  Покрывает if s.head.Next == nil
	if l2.head != nil {
		t.Error("DeleteTail не удалил единственный элемент")
	}

	l2.AddHead("A")
	l2.AddTail("B")
	l2.AddTail("C")
	l2.DeleteTail() // Покрывает цикл прохода до предпоследнего и cur.Next = nil
	if l2.Find("C") != nil {
		t.Error("DeleteTail не удалил хвост")
	}

	// Тестирование DeleteBefore
	l3 := NewStringOL()
	l3.DeleteBefore(nil) // 1. Покрывает ptr == nil

	target := &Node{Person: "Isolated"}
	l3.DeleteBefore(target) // 2. Покрывает s.head == nil

	l3.AddHead("A")
	l3.DeleteBefore(l3.head) // 3. Покрывает s.head == ptr

	l3.AddTail("B")
	l3.DeleteBefore(l3.head.Next) // 4. Покрывает if s.head.Next == ptr (DeleteHead)
	if l3.head.Person != "B" {
		t.Error("DeleteBefore не удалил голову перед вторым элементом")
	}

	l3.AddHead("NewA")
	l3.AddTail("C")
	ptrC := l3.Find("C")
	l3.DeleteBefore(ptrC) // 5. Покрывает цикл и prev.Next = ptr (удаление "B")
	if l3.Find("B") != nil {
		t.Error("DeleteBefore не удалил узел в середине")
	}

}

func TestStringOLDS(t *testing.T) {
	// Создаем тестовый набор данных
	testNames := []string{"Alice", "Bob", "Charlie", "David", "Eve"}
	
	list := NewStringOL()
	for _, name := range testNames {
		list.AddTail(name)
	}

	t.Run("Text_Check", func(t *testing.T) {
		buf := new(bytes.Buffer)
		if err := list.SerializeText(buf); err != nil {
			t.Fatal(err)
		}

		newList := NewStringOL()
		if err := newList.DeserializeText(buf); err != nil {
			t.Fatal(err)
		}

		curr := newList.head
		for i, expectedName := range testNames {
			if curr == nil {
				t.Fatalf("Список оборвался на элементе %d, ожидалось: %s", i, expectedName)
			}
			if curr.Person != expectedName {
				t.Errorf("Ошибка на позиции %d: получили %s, ожидалось %s", i, curr.Person, expectedName)
			}
			curr = curr.Next
		}
		if curr != nil {
			t.Error("В списке остались лишние элементы после десериализации")
		}
	})

	t.Run("Binary_Check", func(t *testing.T) {
		buf := new(bytes.Buffer)
		if err := list.SerializeBinary(buf); err != nil {
			t.Fatal(err)
		}

		newList := NewStringOL()
		if err := newList.DeserializeBinary(buf); err != nil {
			t.Fatal(err)
		}

		curr := newList.head
		for i, expectedName := range testNames {
			if curr == nil {
				t.Fatalf("Бинарный список короче чем нужно на позиции %d", i)
			}
			if curr.Person != expectedName {
				t.Errorf("Бинарная ошибка на позиции %d: %s", i, curr.Person)
			}
			curr = curr.Next
		}
	})
}