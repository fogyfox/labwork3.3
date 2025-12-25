package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
)

// NodeS представляет узел стека
type NodeS struct {
	Key  string
	Next *NodeS
}

// Stack реализует структуру стека (LIFO)
type Stack struct {
	head      *NodeS
	stackSize int
}

// NewStack заменяет конструктор
func NewStack() *Stack {
	return &Stack{
		head:      nil,
		stackSize: 0,
	}
}

// IsEmpty возвращает true, если стек пуст
func (s *Stack) IsEmpty() bool {
	return s.head == nil
}

// Size возвращает текущее количество элементов в стеке
func (s *Stack) Size() int {
	return s.stackSize
}

// Push добавляет элемент на вершину стека (LIFO)
func (s *Stack) Push(data string) {
	newNode := &NodeS{
		Key:  data,
		Next: s.head,
	}
	s.head = newNode
	s.stackSize++
}

// Pop извлекает и возвращает значение с вершины стека
func (s *Stack) Pop() string {
	if s.IsEmpty() {
		fmt.Println("Стек пустой")
		return ""
	}

	val := s.head.Key
	s.head = s.head.Next
	s.stackSize--
	
	return val
}

// Peek возвращает значение на вершине без удаления
func (s *Stack) Peek() string {
	if s.IsEmpty() {
		return ""
	}
	return s.head.Key
}

// GetByIndex позволяет получить значение по индексу (от вершины вниз)
func (s *Stack) GetByIndex(inx int) string {
	if inx < 0 || inx >= s.stackSize {
		fmt.Println("Выход за диапазон")
		return ""
	}
	
	curr := s.head
	for i := 0; i < inx; i++ {
		curr = curr.Next
	}
	return curr.Key
}

// Print выводит содержимое стека в консоль
func (s *Stack) Print() {
	if s.IsEmpty() {
		fmt.Println("Стек пустой")
		return
	}
	
	fmt.Print("Стек: ")
	curr := s.head
	for curr != nil {
		fmt.Printf("%s ", curr.Key)
		curr = curr.Next
	}
	fmt.Println()
}

// SerializeText записывает элементы стека от вершины к основанию
func (s *Stack) SerializeText(w io.Writer) error {
	curr := s.head
	for curr != nil {
		if _, err := fmt.Fprintln(w, curr.Key); err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

// DeserializeText читает строки и помещает их в стек.
// Чтобы сохранить исходный порядок, мы читаем всё в слайс и пушим в обратном порядке.
func (s *Stack) DeserializeText(r io.Reader) error {
	var lines []string
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	
	for i := len(lines) - 1; i >= 0; i-- {
		s.Push(lines[i])
	}
	return scanner.Err()
}



// SerializeBinary сохраняет стек в бинарный поток
func (s *Stack) SerializeBinary(w io.Writer) error {
	// 1. Записываем размер стека
	if err := binary.Write(w, binary.LittleEndian, int32(s.stackSize)); err != nil {
		return err
	}

	// 2. Записываем элементы от вершины вниз
	curr := s.head
	for curr != nil {
		data := []byte(curr.Key)
		if err := binary.Write(w, binary.LittleEndian, int32(len(data))); err != nil {
			return err
		}
		if _, err := w.Write(data); err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

// DeserializeBinary восстанавливает стек из бинарного потока
func (s *Stack) DeserializeBinary(r io.Reader) error {
	var count int32
	if err := binary.Read(r, binary.LittleEndian, &count); err != nil {
		return err
	}

	elements := make([]string, count)
	for i := 0; i < int(count); i++ {
		var strLen int32
		if err := binary.Read(r, binary.LittleEndian, &strLen); err != nil {
			return err
		}

		buf := make([]byte, strLen)
		if _, err := io.ReadFull(r, buf); err != nil {
			return err
		}
		elements[i] = string(buf)
	}

	// Пушим в обратном порядке, чтобы восстановить структуру
	for i := len(elements) - 1; i >= 0; i-- {
		s.Push(elements[i])
	}
	return nil
}