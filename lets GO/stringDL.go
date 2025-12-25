package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
)

// NodeDL представляет узел двусвязного списка
type NodeDL struct {
	Person string
	Next   *NodeDL
	Prev   *NodeDL
}

// StringDL реализует двусвязный список
type StringDL struct {
	head *NodeDL
	tail *NodeDL
}

// NewStringDL заменяет конструктор
func NewStringDL() *StringDL {
	return &StringDL{head: nil, tail: nil}
}

// AddHead добавляет элемент в начало списка
func (s *StringDL) AddHead(name string) {
	newNode := &NodeDL{Person: name, Next: s.head, Prev: nil}
	if s.head != nil {
		s.head.Prev = newNode
	}
	s.head = newNode
	if s.tail == nil {
		s.tail = newNode
	}
}

// AddTail добавляет элемент в конец списка
func (s *StringDL) AddTail(name string) {
	newNode := &NodeDL{Person: name, Next: nil, Prev: s.tail}
	if s.tail != nil {
		s.tail.Next = newNode
	}
	s.tail = newNode
	if s.head == nil {
		s.head = newNode
	}
}

// Find ищет узел по значению
func (s *StringDL) Find(name string) *NodeDL {
	curr := s.head
	for curr != nil {
		if curr.Person == name {
			return curr
		}
		curr = curr.Next
	}
	return nil
}

// AddAfter добавляет новый узел после указанного
func (s *StringDL) AddAfter(ptr *NodeDL, name string) {
	if ptr == nil {
		return
	}
	newNode := &NodeDL{Person: name, Next: ptr.Next, Prev: ptr}
	if ptr.Next != nil {
		ptr.Next.Prev = newNode
	}
	ptr.Next = newNode
	if ptr == s.tail {
		s.tail = newNode
	}
}

// AddBefore добавляет новый узел перед указанным
func (s *StringDL) AddBefore(ptr *NodeDL, name string) {
	if ptr == nil {
		return
	}
	newNode := &NodeDL{Person: name, Next: ptr, Prev: ptr.Prev}
	if ptr.Prev != nil {
		ptr.Prev.Next = newNode
	}
	ptr.Prev = newNode
	if ptr == s.head {
		s.head = newNode
	}
}

// DeleteHead удаляет первый элемент
func (s *StringDL) DeleteHead() {
	if s.head == nil {
		return
	}
	if s.head == s.tail {
		s.head = nil
		s.tail = nil
		return
	}
	s.head = s.head.Next
	s.head.Prev = nil
}

// DeleteTail удаляет последний элемент
func (s *StringDL) DeleteTail() {
	if s.tail == nil {
		return
	}
	if s.head == s.tail {
		s.head = nil
		s.tail = nil
		return
	}
	s.tail = s.tail.Prev
	s.tail.Next = nil
}

// DeleteByValue удаляет узел с конкретным значением
func (s *StringDL) DeleteByValue(name string) {
	el := s.Find(name)
	if el == nil {
		return
	}

	if el == s.head {
		s.DeleteHead()
	} else if el == s.tail {
		s.DeleteTail()
	} else {
		el.Prev.Next = el.Next
		el.Next.Prev = el.Prev
	}
}

// Print выводит список
func (s *StringDL) Print() {
	fmt.Print("Вывод: ")
	curr := s.head
	for curr != nil {
		fmt.Printf("%s ", curr.Person)
		curr = curr.Next
	}
	fmt.Println()
}

// PrintRecursive выполняет рекурсивный вывод
func (s *StringDL) PrintRecursive() {
	fmt.Print("Вывод рекурсией: ")
	s.printRecursivePrivate(s.head)
	fmt.Println()
}

func (s *StringDL) printRecursivePrivate(ptr *NodeDL) {
	if ptr == nil {
		return
	}
	fmt.Printf("%s ", ptr.Person)
	s.printRecursivePrivate(ptr.Next)
}

// SerializeText записывает элементы списка построчно от головы к хвосту
func (s *StringDL) SerializeText(w io.Writer) error {
	curr := s.head
	for curr != nil {
		if _, err := fmt.Fprintln(w, curr.Person); err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

// DeserializeText читает строки из потока и восстанавливает двусвязный список
func (s *StringDL) DeserializeText(r io.Reader) error {
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		s.AddTail(scanner.Text())
	}
	return scanner.Err()
}


// SerializeBinary записывает структуру списка в двоичный формат
func (s *StringDL) SerializeBinary(w io.Writer) error {
	// Считаем количество элементов
	count := 0
	curr := s.head
	for curr != nil {
		count++
		curr = curr.Next
	}

	// 1. Записываем общее кол-во (int32)
	if err := binary.Write(w, binary.LittleEndian, int32(count)); err != nil {
		return err
	}

	// 2. Записываем данные каждого узла
	curr = s.head
	for curr != nil {
		data := []byte(curr.Person)
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

// DeserializeBinary восстанавливает двусвязный список из бинарного потока
func (s *StringDL) DeserializeBinary(r io.Reader) error {
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