package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
)

// NodeQ представляет узел очереди
type NodeQ struct {
	Value string
	Next  *NodeQ
}

// StringQueue реализует структуру очереди (FIFO)
type StringQueue struct {
	head      *NodeQ
	tail      *NodeQ
	queueSize int
}

// NewStringQueue заменяет конструктор
func NewStringQueue() *StringQueue {
	return &StringQueue{
		head:      nil,
		tail:      nil,
		queueSize: 0,
	}
}

// IsEmpty проверяет, пуста ли очередь
func (q *StringQueue) IsEmpty() bool {
	return q.head == nil
}

// Size возвращает текущее количество элементов
func (q *StringQueue) Size() int {
	return q.queueSize
}

// Push добавляет элемент в конец очереди
func (q *StringQueue) Push(data string) {
	newNode := &NodeQ{Value: data, Next: nil}
	if q.IsEmpty() {
		q.head = newNode
		q.tail = newNode
	} else {
		q.tail.Next = newNode
		q.tail = newNode
	}
	q.queueSize++
}

// Pop извлекает значение из начала очереди
func (q *StringQueue) Pop() string {
	if q.IsEmpty() {
		fmt.Println("Очередь пустая")
		return ""
	}

	val := q.head.Value
	q.head = q.head.Next

	// Если после удаления очередь стала пустой, обнуляем tail
	if q.head == nil {
		q.tail = nil
	}

	q.queueSize--
	return val
}

// GetByIndex возвращает значение по индексу без удаления
func (q *StringQueue) GetByIndex(inx int) string {
	if inx < 0 || inx >= q.queueSize {
		fmt.Println("Выход за диапазон")
		return ""
	}
	curr := q.head
	for i := 0; i < inx; i++ {
		curr = curr.Next
	}
	return curr.Value
}

// Print выводит все элементы очереди
func (q *StringQueue) Print() {
	if q.IsEmpty() {
		fmt.Println("Очередь пустая")
		return
	}
	fmt.Print("Очередь: ")
	curr := q.head
	for curr != nil {
		fmt.Printf("%s ", curr.Value)
		curr = curr.Next
	}
	fmt.Println()
}

// SerializeText записывает все элементы очереди в текстовый поток (по одному на строку)
func (q *StringQueue) SerializeText(w io.Writer) error {
	curr := q.head
	for curr != nil {
		if _, err := fmt.Fprintln(w, curr.Value); err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

// DeserializeText читает строки из текстового потока и добавляет их в очередь
func (q *StringQueue) DeserializeText(r io.Reader) error {
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		q.Push(scanner.Text())
	}
	return scanner.Err()
}


// SerializeBinary записывает очередь в бинарный поток
func (q *StringQueue) SerializeBinary(w io.Writer) error {
	// 1. Записываем общее количество элементов (int32)
	if err := binary.Write(w, binary.LittleEndian, int32(q.queueSize)); err != nil {
		return err
	}

	// 2. Последовательно записываем каждый элемент
	curr := q.head
	for curr != nil {
		data := []byte(curr.Value)
		// Сначала длина строки
		if err := binary.Write(w, binary.LittleEndian, int32(len(data))); err != nil {
			return err
		}
		// Затем сами байты строки
		if _, err := w.Write(data); err != nil {
			return err
		}
		curr = curr.Next
	}
	return nil
}

// DeserializeBinary восстанавливает очередь из бинарного потока
func (q *StringQueue) DeserializeBinary(r io.Reader) error {
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
		q.Push(string(buf))
	}
	return nil
}