package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
	"strconv"
)

type NodeFBT struct {
	Key   int
	Left  *NodeFBT
	Right *NodeFBT
}

type FullBinaryTree struct {
	Root *NodeFBT
}

func NewFullBinaryTree() *FullBinaryTree {
	return &FullBinaryTree{Root: nil}
}

// Переименовали в InsertNode (с большой буквы) для доступа из тестов
func (fbt *FullBinaryTree) InsertNode(node *NodeFBT, key int) *NodeFBT {
	if node == nil {
		return &NodeFBT{Key: key}
	}
	if key < node.Key {
		node.Left = fbt.InsertNode(node.Left, key)
	} else if key > node.Key {
		node.Right = fbt.InsertNode(node.Right, key)
	}
	return node
}

func (fbt *FullBinaryTree) Insert(key int) {
	fbt.Root = fbt.InsertNode(fbt.Root, key)
}

// Исправленная версия InsertTree для тестов
func (fbt *FullBinaryTree) InsertTree(source *FullBinaryTree) {
	if source == nil || source.Root == nil {
		return
	}
	fbt.copyNode(source.Root)
}

func (fbt *FullBinaryTree) copyNode(node *NodeFBT) {
	if node == nil {
		return
	}
	fbt.Insert(node.Key)
	fbt.copyNode(node.Left)
	fbt.copyNode(node.Right)
}

// Высота дерева
func (fbt *FullBinaryTree) height(node *NodeFBT) int {
	if node == nil {
		return 0
	}
	lHeight := fbt.height(node.Left)
	rHeight := fbt.height(node.Right)
	if lHeight > rHeight {
		return lHeight + 1
	}
	return rHeight + 1
}

func (fbt *FullBinaryTree) GetHeight() int {
	return fbt.height(fbt.Root)
}

// Обходы
func (fbt *FullBinaryTree) topToDown(node *NodeFBT) {
	if node == nil { return }
	fmt.Printf("%d ", node.Key)
	fbt.topToDown(node.Left)
	fbt.topToDown(node.Right)
}

func (fbt *FullBinaryTree) leftToRight(node *NodeFBT) {
	if node == nil { return }
	fbt.leftToRight(node.Left)
	fmt.Printf("%d ", node.Key)
	fbt.leftToRight(node.Right)
}

func (fbt *FullBinaryTree) downToTop(node *NodeFBT) {
	if node == nil { return }
	fbt.downToTop(node.Left)
	fbt.downToTop(node.Right)
	fmt.Printf("%d ", node.Key)
}

func (fbt *FullBinaryTree) PrintTopToDown()   { fbt.topToDown(fbt.Root); fmt.Println() }
func (fbt *FullBinaryTree) PrintLeftToRight() { fbt.leftToRight(fbt.Root); fmt.Println() }
func (fbt *FullBinaryTree) PrintDownToTop()   { fbt.downToTop(fbt.Root); fmt.Println() }

// Поуровневый вывод (PrintByLevels)
func (fbt *FullBinaryTree) PrintByLevels() {
	h := fbt.GetHeight()
	for i := 1; i <= h; i++ {
		fbt.printLevel(fbt.Root, i)
	}
	fmt.Println()
}

func (fbt *FullBinaryTree) printLevel(node *NodeFBT, level int) {
	if node == nil { return }
	if level == 1 {
		fmt.Printf("%d ", node.Key)
	} else if level > 1 {
		fbt.printLevel(node.Left, level-1)
		fbt.printLevel(node.Right, level-1)
	}
}

// SerializeText сохраняет дерево в текстовом виде (Pre-order с маркерами null)
func (fbt *FullBinaryTree) SerializeText(w io.Writer) {
	fbt.serializeTextRecursive(fbt.Root, w)
}

func (fbt *FullBinaryTree) serializeTextRecursive(node *NodeFBT, w io.Writer) {
	if node == nil {
		fmt.Fprint(w, "null ")
		return
	}
	fmt.Fprintf(w, "%d ", node.Key)
	fbt.serializeTextRecursive(node.Left, w)
	fbt.serializeTextRecursive(node.Right, w)
}

// DeserializeText восстанавливает дерево из текстового потока
func (fbt *FullBinaryTree) DeserializeText(r io.Reader) error {
	scanner := bufio.NewScanner(r)
	scanner.Split(bufio.ScanWords)
	
	var readNode func() *NodeFBT
	readNode = func() *NodeFBT {
		if !scanner.Scan() {
			return nil
		}
		valStr := scanner.Text()
		if valStr == "null" {
			return nil
		}
		val, _ := strconv.Atoi(valStr)
		node := &NodeFBT{Key: val}
		node.Left = readNode()
		node.Right = readNode()
		return node
	}
	
	fbt.Root = readNode()
	return scanner.Err()
}

// SerializeBinary сохраняет дерево в бинарный поток
func (fbt *FullBinaryTree) SerializeBinary(w io.Writer) error {
	return fbt.serializeBinaryRecursive(fbt.Root, w)
}

func (fbt *FullBinaryTree) serializeBinaryRecursive(node *NodeFBT, w io.Writer) error {
	if node == nil {
		// Используем специальное значение (например, -1), если ключи только положительные,
		// или записываем флаг наличия узла.
		return binary.Write(w, binary.LittleEndian, int32(-1)) 
	}
	
	// Записываем ключ узла
	if err := binary.Write(w, binary.LittleEndian, int32(node.Key)); err != nil {
		return err
	}
	// Рекурсивно записываем потомков
	if err := fbt.serializeBinaryRecursive(node.Left, w); err != nil {
		return err
	}
	return fbt.serializeBinaryRecursive(node.Right, w)
}

// DeserializeBinary восстанавливает дерево из бинарного потока
func (fbt *FullBinaryTree) DeserializeBinary(r io.Reader) error {
	var readNode func() (*NodeFBT, error)
	readNode = func() (*NodeFBT, error) {
		var key int32
		err := binary.Read(r, binary.LittleEndian, &key)
		if err != nil {
			return nil, err
		}
		
		if key == -1 { // Маркер пустого узла
			return nil, nil
		}
		
		node := &NodeFBT{Key: int(key)}
		node.Left, err = readNode()
		if err != nil { return nil, err }
		node.Right, err = readNode()
		if err != nil { return nil, err }
		
		return node, nil
	}

	root, err := readNode()
	if err != nil && err != io.EOF {
		return err
	}
	fbt.Root = root
	return nil
}