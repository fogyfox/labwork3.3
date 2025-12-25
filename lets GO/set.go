package main

import (
	"bufio"
	"encoding/binary" 
	"fmt"
	"io"
	"os"        
	"strconv"
)

// NodeSet представляет узел в цепочке хеш-таблицы
type NodeSet struct {
	Key  int
	Next *NodeSet
}

// Set реализует множество целых чисел
type Set struct {
	buckets   []*NodeSet
	tableSize int
	itemCount int
}

// NewSet — конструктор множества
func NewSet(initialSize int) *Set {
	if initialSize <= 0 {
		initialSize = 101
	}
	return &Set{
		buckets:   make([]*NodeSet, initialSize),
		tableSize: initialSize,
		itemCount: 0,
	}
}

// hashFunction — простая хеш-функция для int
func (s *Set) hashFunction(key int) int {
	h := key
	if h < 0 {
		h = -h
	}
	return h % s.tableSize
}

// rehashIfNeeded проверяет load factor и увеличивает таблицу
func (s *Set) rehashIfNeeded() {
	loadFactor := float64(s.itemCount) / float64(s.tableSize)
	if loadFactor > 0.7 {
		oldBuckets := s.buckets
		s.tableSize *= 2
		s.buckets = make([]*NodeSet, s.tableSize)
		s.itemCount = 0 // Сбросим и пересчитаем при повторной вставке

		for _, head := range oldBuckets {
			curr := head
			for curr != nil {
				s.Insert(curr.Key)
				curr = curr.Next
			}
		}
	}
}

// Insert добавляет ключ, если его еще нет
func (s *Set) Insert(key int) bool {
	if s.Contains(key) {
		return false
	}
	s.rehashIfNeeded()
	index := s.hashFunction(key)
	s.buckets[index] = &NodeSet{Key: key, Next: s.buckets[index]}
	s.itemCount++
	return true
}

// Contains проверяет наличие ключа
func (s *Set) Contains(key int) bool {
	index := s.hashFunction(key)
	curr := s.buckets[index]
	for curr != nil {
		if curr.Key == key {
			return true
		}
		curr = curr.Next
	}
	return false
}

// Remove удаляет ключ из множества
func (s *Set) Remove(key int) bool {
	index := s.hashFunction(key)
	var prev *NodeSet
	curr := s.buckets[index]

	for curr != nil {
		if curr.Key == key {
			if prev == nil {
				s.buckets[index] = curr.Next
			} else {
				prev.Next = curr.Next
			}
			s.itemCount--
			return true
		}
		prev = curr
		curr = curr.Next
	}
	return false
}

func (s *Set) Size() int {
	return s.itemCount
}

func (s *Set) IsEmpty() bool {
	return s.itemCount == 0
}

func (s *Set) Print() {
	fmt.Print("{ ")
	for _, head := range s.buckets {
		curr := head
		for curr != nil {
			fmt.Printf("%d ", curr.Key)
			curr = curr.Next
		}
	}
	fmt.Println("}")
}

// ToVector преобразует множество в слайс (аналог std::vector)
func (s *Set) ToVector() []int {
	result := make([]int, 0, s.itemCount)
	for _, head := range s.buckets {
		curr := head
		for curr != nil {
			result = append(result, curr.Key)
			curr = curr.Next
		}
	}
	return result
}

// SaveToFile сохраняет элементы в файл
func (s *Set) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	for _, val := range s.ToVector() {
		fmt.Fprintln(file, val)
	}
	return nil
}

// LoadFromFile загружает элементы из файла
func (s *Set) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		val, err := strconv.Atoi(scanner.Text())
		if err == nil {
			s.Insert(val)
		}
	}
	return scanner.Err()
}

// Partition — алгоритм разбиения на подмножества с равной суммой
func (s *Set) Partition(subsetSum int) ([][]int, bool) {
	nums := s.ToVector()
	total := 0
	for _, n := range nums {
		total += n
	}

	if total%subsetSum != 0 {
		return nil, false
	}

	k := total / subsetSum
	subsets := make([][]int, k)
	sums := make([]int, k)

	if backtrack(nums, subsets, sums, subsetSum, 0) {
		return subsets, true
	}
	return nil, false
}

func backtrack(nums []int, subsets [][]int, sums []int, target int, idx int) bool {
	if idx == len(nums) {
		return true
	}

	for i := 0; i < len(subsets); i++ {
		if sums[i]+nums[idx] <= target {
			subsets[i] = append(subsets[i], nums[idx])
			sums[i] += nums[idx]

			if backtrack(nums, subsets, sums, target, idx+1) {
				return true
			}

			// Backtrack
			sums[i] -= nums[idx]
			subsets[i] = subsets[i][:len(subsets[i])-1]

			// Оптимизация: если подмножество пустое, нет смысла пробовать другие пустые
			if sums[i] == 0 {
				break
			}
		}
	}
	return false
}


// SerializeText записывает все элементы множества в текстовый поток (по одному на строку)
func (s *Set) SerializeText(w io.Writer) error {
	elements := s.ToVector()
	for _, val := range elements {
		if _, err := fmt.Fprintln(w, val); err != nil {
			return err
		}
	}
	return nil
}

// DeserializeText читает числа из текстового потока и добавляет их в множество
func (s *Set) DeserializeText(r io.Reader) error {
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		val, err := strconv.Atoi(scanner.Text())
		if err == nil {
			s.Insert(val)
		}
	}
	return scanner.Err()
}


// SerializeBinary записывает множество в бинарный поток: [кол-во элементов][число1][число2]...
func (s *Set) SerializeBinary(w io.Writer) error {
	elements := s.ToVector()
	
	// Записываем количество элементов (int32)
	if err := binary.Write(w, binary.LittleEndian, int32(len(elements))); err != nil {
		return err
	}

	// Записываем сами элементы
	for _, val := range elements {
		if err := binary.Write(w, binary.LittleEndian, int32(val)); err != nil {
			return err
		}
	}
	return nil
}

// DeserializeBinary восстанавливает множество из бинарного потока
func (s *Set) DeserializeBinary(r io.Reader) error {
	var count int32
	if err := binary.Read(r, binary.LittleEndian, &count); err != nil {
		return err
	}

	for i := 0; i < int(count); i++ {
		var val int32
		if err := binary.Read(r, binary.LittleEndian, &val); err != nil {
			return err
		}
		s.Insert(int(val))
	}
	return nil
}