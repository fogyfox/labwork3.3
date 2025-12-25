package main

import (
	"bufio"
	"encoding/gob"
	"fmt"
	"io"
	"strings"
)

// Константы состояний ячейки
type EntryState int

const (
	Empty EntryState = iota
	Occupied
	Deleted
)

// Entry представляет ячейку таблицы
type Entry[K comparable, V any] struct {
	Key   K
	Value V
	State EntryState
}

// OpenAddressingHashTable реализует хеш-таблицу с двойным хешированием
type OpenAddressingHashTable[K comparable, V any] struct {
	table               []Entry[K, V]
	size                int
	capacity            int
	loadFactorThreshold float64
}

// NewOpenAddressingHashTable — конструктор (аналог ChainingHashTable и OpenAddressingHashTable в C++)
func NewOpenAddressingHashTable[K comparable, V any](initialCapacity int, loadFactor float64) *OpenAddressingHashTable[K, V] {
	return &OpenAddressingHashTable[K, V]{
		table:               make([]Entry[K, V], initialCapacity),
		size:                0,
		capacity:            initialCapacity,
		loadFactorThreshold: loadFactor,
	}
}

// hash реализует двойное хеширование (аналог h1 + attempt * h2)
func (ht *OpenAddressingHashTable[K, V]) hash(key K, attempt int) int {
	// Упрощенная логика получения числового хеша в Go для примера
	hStr := fmt.Sprintf("%v", key)
	h1Val := 0
	for i := 0; i < len(hStr); i++ {
		h1Val = 31*h1Val + int(hStr[i])
	}
	if h1Val < 0 {
		h1Val = -h1Val
	}

	h1 := h1Val % ht.capacity
	h2 := 1 + (h1Val % (ht.capacity - 1))
	return (h1 + attempt*h2) % ht.capacity
}

// loadFactor возвращает текущий коэффициент заполнения
func (ht *OpenAddressingHashTable[K, V]) loadFactor() float64 {
	return float64(ht.size) / float64(ht.capacity)
}

// rehash увеличивает размер таблицы и перераспределяет элементы
func (ht *OpenAddressingHashTable[K, V]) rehash() {
	oldTable := ht.table
	ht.capacity *= 2
	ht.table = make([]Entry[K, V], ht.capacity)
	ht.size = 0

	for _, entry := range oldTable {
		if entry.State == Occupied {
			ht.Insert(entry.Key, entry.Value)
		}
	}
}

// Insert добавляет ключ и значение в таблицу
func (ht *OpenAddressingHashTable[K, V]) Insert(key K, value V) {
	if ht.loadFactor() >= ht.loadFactorThreshold {
		ht.rehash()
	}

	for attempt := 0; attempt < ht.capacity; attempt++ {
		index := ht.hash(key, attempt)
		
		// Если ключ уже есть, обновляем значение
		if ht.table[index].State == Occupied && ht.table[index].Key == key {
			ht.table[index].Value = value
			return
		}
		
		// Если ячейка не занята, вставляем данные
		if ht.table[index].State != Occupied {
			ht.table[index].Key = key
			ht.table[index].Value = value
			ht.table[index].State = Occupied
			ht.size++
			return
		}
	}
	panic("Таблица переполнена") // Аналог runtime_error в C++
}

// Find ищет значение по ключу. Возвращает значение и флаг успеха
func (ht *OpenAddressingHashTable[K, V]) Find(key K) (V, bool) {
	for attempt := 0; attempt < ht.capacity; attempt++ {
		index := ht.hash(key, attempt)
		if ht.table[index].State == Empty {
			break
		}
		if ht.table[index].State == Occupied && ht.table[index].Key == key {
			return ht.table[index].Value, true
		}
	}
	var zero V
	return zero, false
}

// Remove помечает ячейку как удаленную (DELETED)
func (ht *OpenAddressingHashTable[K, V]) Remove(key K) bool {
	for attempt := 0; attempt < ht.capacity; attempt++ {
		index := ht.hash(key, attempt)
		if ht.table[index].State == Empty {
			return false
		}
		if ht.table[index].State == Occupied && ht.table[index].Key == key {
			ht.table[index].State = Deleted
			ht.size--
			return true
		}
	}
	return false
}

// Display выводит только занятые ячейки
func (ht *OpenAddressingHashTable[K, V]) Display() {
	fmt.Println("\nХЕШ-ТАБЛИЦА С ОТКРЫТОЙ АДРЕСАЦИЕЙ")
	for i := 0; i < len(ht.table); i++ {
		if ht.table[i].State == Occupied {
			fmt.Printf("Ячейка [%d]: {%v = %v}\n", i, ht.table[i].Key, ht.table[i].Value)
		}
	}
}

// SerializeText записывает только занятые (Occupied) ячейки
func (ht *OpenAddressingHashTable[K, V]) SerializeText(w io.Writer) error {
	for i := 0; i < ht.capacity; i++ {
		if ht.table[i].State == Occupied {
			if _, err := fmt.Fprintf(w, "%v:%v\n", ht.table[i].Key, ht.table[i].Value); err != nil {
				return err
			}
		}
	}
	return nil
}

// DeserializeText читает пары ключ:значение и вставляет их в таблицу
func (ht *OpenAddressingHashTable[K, V]) DeserializeText(r io.Reader, parseKey func(string) K, parseVal func(string) V) error {
	scanner := bufio.NewScanner(r)
	for scanner.Scan() {
		line := scanner.Text()
		parts := strings.SplitN(line, ":", 2)
		if len(parts) == 2 {
			k := parseKey(parts[0])
			v := parseVal(parts[1])
			ht.Insert(k, v)
		}
	}
	return scanner.Err()
}

// SerializeBinary сохраняет активные данные в двоичный поток
func (ht *OpenAddressingHashTable[K, V]) SerializeBinary(w io.Writer) error {
	var activeEntries []Entry[K, V]
	for i := 0; i < ht.capacity; i++ {
		if ht.table[i].State == Occupied {
			activeEntries = append(activeEntries, ht.table[i])
		}
	}

	encoder := gob.NewEncoder(w)
	return encoder.Encode(activeEntries)
}

// DeserializeBinary восстанавливает таблицу из двоичного потока
func (ht *OpenAddressingHashTable[K, V]) DeserializeBinary(r io.Reader) error {
	var activeEntries []Entry[K, V]
	decoder := gob.NewDecoder(r)
	if err := decoder.Decode(&activeEntries); err != nil {
		return err
	}

	for _, entry := range activeEntries {
		ht.Insert(entry.Key, entry.Value)
	}
	return nil
}