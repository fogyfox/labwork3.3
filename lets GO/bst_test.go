package main

import (
	"testing"
	"bytes"
)

func TestFullBinaryTree(t *testing.T) {
	fbt := NewFullBinaryTree()

	// Тест вставки
	keys := []int{5, 3, 7, 2, 4, 6, 8}
	for _, k := range keys {
		fbt.Insert(k)
	}

	// Проверка высоты
	if fbt.GetHeight() != 3 {
		t.Errorf("Ожидалась высота 3, получили %d", fbt.GetHeight())
	}

	// Проверка методов печати (покрывает рекурсивные обходы)
	fbt.PrintTopToDown()
	fbt.PrintLeftToRight()
	fbt.PrintDownToTop()
	fbt.PrintByLevels()

	// Тест копирования дерева
	fbt2 := NewFullBinaryTree()
	fbt2.InsertTree(fbt)
	if fbt2.GetHeight() != 3 {
		t.Error("Ошибка копирования дерева")
	}

	// Тест на пустом дереве
	emptyTree := NewFullBinaryTree()
	if emptyTree.GetHeight() != 0 {
		t.Error("Высота пустого дерева должна быть 0")
	}
	emptyTree.PrintTopToDown()
	
}

func TestBSTSerialization(t *testing.T) {
	originalTree := NewFullBinaryTree()
	keys := []int{20, 10, 30, 25}
	for _, k := range keys {
		originalTree.Insert(k)
	}

	// Тест текстовой сериализации
	t.Run("Text Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)
		
		// Сохраняем
		originalTree.SerializeText(buf)

		// Восстанавливаем в новое дерево
		restoredTree := NewFullBinaryTree()
		err := restoredTree.DeserializeText(buf)
		if err != nil {
			t.Fatalf("Failed to deserialize text: %v", err)
		}

		// Проверяем структуру
		validateBST(t, restoredTree)
	})

	// Тест бинарной сериализации
	t.Run("Binary Serialization", func(t *testing.T) {
		buf := new(bytes.Buffer)

		// Сохраняем
		err := originalTree.SerializeBinary(buf)
		if err != nil {
			t.Fatalf("Failed to serialize binary: %v", err)
		}

		// Восстанавливаем
		restoredTree := NewFullBinaryTree()
		err = restoredTree.DeserializeBinary(buf)
		if err != nil {
			t.Fatalf("Failed to deserialize binary: %v", err)
		}

		// Проверяем структуру
		validateBST(t, restoredTree)
	})
}

// Вспомогательная функция для проверки целостности дерева
func validateBST(t *testing.T, tree *FullBinaryTree) {
	if tree.Root == nil {
		t.Fatal("Root is nil")
	}
	if tree.Root.Key != 20 {
		t.Errorf("Expected root 20, got %d", tree.Root.Key)
	}
	if tree.Root.Left.Key != 10 {
		t.Errorf("Expected left child 10, got %d", tree.Root.Left.Key)
	}
	if tree.Root.Right.Key != 30 {
		t.Errorf("Expected right child 30, got %d", tree.Root.Right.Key)
	}
	if tree.Root.Right.Left.Key != 25 {
		t.Errorf("Expected nested child 25, got %d", tree.Root.Right.Left.Key)
	}
}