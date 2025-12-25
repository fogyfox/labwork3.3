package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
)

// Node аналогичен структуре Node в C++
type Node struct {
	Person string
	Next   *Node
}

// StringOL реализует односвязный список
type StringOL struct {
	head *Node
}

// NewStringOL заменяет конструктор
func NewStringOL() *StringOL {
	return &StringOL{head: nil}
}

// AddHead добавляет элемент в начало списка
func (s *StringOL) AddHead(name string) {
	s.head = &Node{Person: name, Next: s.head}
}

// AddTail добавляет элемент в конец списка
func (s *StringOL) AddTail(name string) {
	newNode := &Node{Person: name, Next: nil}
	if s.head == nil {
		s.head = newNode
		return
	}
	cur := s.head
	for cur.Next != nil {
		cur = cur.Next
	}
	cur.Next = newNode
}

// Find ищет узел по значению
func (s *StringOL) Find(name string) *Node {
	cur := s.head
	for cur != nil {
		if cur.Person == name {
			return cur
		}
		cur = cur.Next
	}
	return nil
}

// AddAfter добавляет новое значение после целевого
func (s *StringOL) AddAfter(targetValue, newValue string) {
	cur := s.head
	for cur != nil {
		if cur.Person == targetValue {
			cur.Next = &Node{Person: newValue, Next: cur.Next}
			return
		}
		cur = cur.Next
	}
}

// AddBefore добавляет новое значение перед целевым
func (s *StringOL) AddBefore(targetValue, newValue string) {
	if s.head == nil {
		return
	}
	if s.head.Person == targetValue {
		s.AddHead(newValue)
		return
	}
	var prev *Node
	cur := s.head
	for cur != nil && cur.Person != targetValue {
		prev = cur
		cur = cur.Next
	}
	if cur != nil && prev != nil {
		prev.Next = &Node{Person: newValue, Next: cur}
	}
}

// DeleteHead удаляет первый элемент
func (s *StringOL) DeleteHead() {
	if s.head == nil {
		return
	}
	s.head = s.head.Next
}

// DeleteTail удаляет последний элемент
func (s *StringOL) DeleteTail() {
	if s.head == nil {
		return
	}
	if s.head.Next == nil {
		s.head = nil
		return
	}
	cur := s.head
	for cur.Next != nil && cur.Next.Next != nil {
		cur = cur.Next
	}
	cur.Next = nil
}

// DeleteByValue удаляет все узлы с указанным значением
func (s *StringOL) DeleteByValue(name string) {
	for s.head != nil && s.head.Person == name {
		s.DeleteHead()
	}
	cur := s.head
	for cur != nil && cur.Next != nil {
		if cur.Next.Person == name {
			cur.Next = cur.Next.Next
		} else {
			cur = cur.Next
		}
	}
}

// Print выводит список в консоль
func (s *StringOL) Print() {
	cur := s.head
	first := true
	for cur != nil {
		if !first {
			fmt.Print(" ")
		}
		fmt.Print(cur.Person)
		first = false
		cur = cur.Next
	}
	fmt.Println()
}

// PrintRecursive публичный метод для рекурсивного вывода
func (s *StringOL) PrintRecursive() {
	s.printRecursivePrivate(s.head)
	fmt.Println()
}

func (s *StringOL) printRecursivePrivate(node *Node) {
	if node == nil {
		return
	}
	fmt.Print(node.Person)
	if node.Next != nil {
		fmt.Print(" ")
		s.printRecursivePrivate(node.Next)
	}
}

// DeleteAfter удаляет узел после указанного
func (s *StringOL) DeleteAfter(ptr *Node) {
	if s.head == nil || ptr == nil || ptr.Next == nil {
		return
	}
	ptr.Next = ptr.Next.Next
}

// DeleteBefore удаляет узел перед указанным
func (s *StringOL) DeleteBefore(ptr *Node) {
	if s.head == nil || ptr == nil || s.head == ptr {
		return
	}
	if s.head.Next == ptr {
		s.DeleteHead()
		return
	}
	var prev *Node
	cur := s.head
	for cur != nil && cur.Next != nil && cur.Next != ptr {
		prev = cur
		cur = cur.Next
	}
	if cur != nil && cur.Next == ptr && prev != nil {
		prev.Next = ptr
	}
}

// SerializeText записывает весь список построчно
func (s *StringOL) SerializeText(w io.Writer) error {
	cur := s.head
	for cur != nil {
		if _, err := fmt.Fprintln(w, cur.Person); err != nil {
			return err
		}
		cur = cur.Next
	}
	return nil
}

// DeserializeText читает все строки и восстанавливает порядок через addTail
func (s *StringOL) DeserializeText(r io.Reader) error {
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		s.AddTail(scanner.Text())
	}
	return scanner.Err()
}

// SerializeBinary записывает [количество][длина_строки][строка] для каждого узла
func (s *StringOL) SerializeBinary(w io.Writer) error {
	count := 0
	cur := s.head
	for cur != nil {
		count++
		cur = cur.Next
	}

	if err := binary.Write(w, binary.LittleEndian, int32(count)); err != nil {
		return err
	}

	cur = s.head
	for cur != nil {
		bytes := []byte(cur.Person)
		if err := binary.Write(w, binary.LittleEndian, int32(len(bytes))); err != nil {
			return err
		}
		if _, err := w.Write(bytes); err != nil {
			return err
		}
		cur = cur.Next
	}
	return nil
}

// DeserializeBinary считывает количество и восстанавливает все узлы по порядку
func (s *StringOL) DeserializeBinary(r io.Reader) error {
	var count int32
	if err := binary.Read(r, binary.LittleEndian, &count); err != nil {
		return err
	}

	for i := 0; i < int(count); i++ {
		var strLen int32
		if err := binary.Read(r, binary.LittleEndian, &strLen); err != nil {
			return err
		}

		buf := make([]byte, strLen)
		if _, err := io.ReadFull(r, buf); err != nil {
			return err
		}
		s.AddTail(string(buf))
	}
	return nil
}