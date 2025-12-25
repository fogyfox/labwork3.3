package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"io"
)

type NodeA struct {
	Value string
}

type Array struct {
	data     []NodeA
	size     int
	capacity int
}

func NewDynamicArray() *Array {
	return &Array{
		data:     make([]NodeA, 1),
		size:     0,
		capacity: 1,
	}
}

// PushBack добавляет в конец
func (da *Array) PushBack(val string) {
	if da.size == da.capacity {
		da.resize()
	}
	da.data[da.size] = NodeA{Value: val}
	da.size++
}

// AddInx — вставка по индексу (требуется тестом)
func (da *Array) AddInx(inx int, val string) {
	if inx < 0 || inx > da.size {
		fmt.Println("Выход за диапазон")
		return
	}
	if da.size == da.capacity {
		da.resize()
	}
	// Сдвигаем элементы вправо
	for i := da.size; i > inx; i-- {
		da.data[i] = da.data[i-1]
	}
	da.data[inx] = NodeA{Value: val}
	da.size++
}

func (da *Array) resize() {
	da.capacity *= 2
	newData := make([]NodeA, da.capacity)
	copy(newData, da.data)
	da.data = newData
}

func (da *Array) GetInx(inx int) string {
	if inx < 0 || inx >= da.size {
		return "Выход за диапазон"
	}
	return da.data[inx].Value
}

// ChangeInx — замена значения (требуется тестом)
func (da *Array) ChangeInx(inx int, val string) {
	if inx < 0 || inx >= da.size {
		fmt.Println("Выход за диапазон")
		return
	}
	da.data[inx].Value = val
}

// RemoveElArr теперь возвращает строку, которая была удалена
func (da *Array) RemoveElArr(inx int) string {
	if inx < 0 || inx >= da.size {
		return "Выход за диапазон"
	}
	
	val := da.data[inx].Value // Сохраняем значение перед удалением
	
	for i := inx; i < da.size-1; i++ {
		da.data[i] = da.data[i+1]
	}
	da.size--
	return val // Возвращаем сохраненное значение
}

func (da *Array) GetSize() int {
	return da.size
}

func (da *Array) PrintArray() {
	if da.size == 0 {
		fmt.Println("Массив пустой")
		return
	}
	fmt.Print("Вывод массива: ")
	for i := 0; i < da.size; i++ {
		fmt.Printf("%s ", da.data[i].Value)
	}
	fmt.Println()
}

func (da *Array) Clear() {
	da.size = 0
	da.capacity = 1
	da.data = make([]NodeA, 1)
}


func (da *Array) SerializeBinary(w io.Writer) error {
	binary.Write(w, binary.LittleEndian, int32(da.size))
	for i := 0; i < da.size; i++ {
		b := []byte(da.data[i].Value)
		binary.Write(w, binary.LittleEndian, int32(len(b)))
		w.Write(b)
	}
	return nil
}

func (da *Array) DeserializeBinary(r io.Reader) error {
	da.Clear()
	var count int32
	if err := binary.Read(r, binary.LittleEndian, &count); err != nil {
		return err
	}
	for i := 0; i < int(count); i++ {
		var l int32
		binary.Read(r, binary.LittleEndian, &l)
		buf := make([]byte, l)
		io.ReadFull(r, buf)
		da.PushBack(string(buf))
	}
	return nil
}

func (da *Array) SerializeText(w io.Writer) error {
	for i := 0; i < da.size; i++ {
		fmt.Fprintln(w, da.data[i].Value)
	}
	return nil
}

func (da *Array) DeserializeText(r io.Reader) error {
	da.Clear()
	sc := bufio.NewScanner(r)
	for sc.Scan() {
		da.PushBack(sc.Text())
	}
	return sc.Err()
}