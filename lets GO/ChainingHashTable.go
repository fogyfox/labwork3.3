package main

import (
	"bufio"
	"encoding/gob"
	"fmt"
	"io"
	"strings"
)

// Entry представляет пару ключ-значение в цепочке
type ChainEntry[K comparable, V any] struct {
	Key   K
	Value V
}

// ChainingHashTable реализует хеш-таблицу с методом цепочек
type ChainingHashTable[K comparable, V any] struct {
	table               [][]ChainEntry[K, V]
	size                int
	capacity            int
	loadFactorThreshold float64
}

// NewChainingHashTable — конструктор таблицы
func NewChainingHashTable[K comparable, V any](initialCapacity int, loadFactor float64) *ChainingHashTable[K, V] {
	return &ChainingHashTable[K, V]{
		table:               make([][]ChainEntry[K, V], initialCapacity),
		size:                0,
		capacity:            initialCapacity,
		loadFactorThreshold: loadFactor,
	}
}

// hash — простая хеш-функция. В Go для дженериков нет универсального std::hash,
// поэтому для примера используем встроенную логику (для реальных задач нужны спец. библиотеки)
func (ht *ChainingHashTable[K, V]) hash(key K) int {
	// Это упрощенная демонстрация. В Go для получения хеша от интерфейса
	// обычно используют пакет hash/fnv или аналоги.
	h := fmt.Sprintf("%v", key)
	hashVal := 0
	for i := 0; i < len(h); i++ {
		hashVal = 31*hashVal + int(h[i])
	}
	if hashVal < 0 {
		hashVal = -hashVal
	}
	return hashVal % ht.capacity
}

// loadFactor вычисляет текущую заполненность
func (ht *ChainingHashTable[K, V]) loadFactor() float64 {
	return float64(ht.size) / float64(ht.capacity)
}

// rehash увеличивает размер таблицы вдвое при достижении порога
func (ht *ChainingHashTable[K, V]) rehash() {
	newCapacity := ht.capacity * 2
	newTable := make([][]ChainEntry[K, V], newCapacity)

	// Переносим все элементы в новую таблицу
	for _, chain := range ht.table {
		for _, entry := range chain {
			// Пересчитываем индекс для новой емкости
			h := fmt.Sprintf("%v", entry.Key)
			hashVal := 0
			for i := 0; i < len(h); i++ {
				hashVal = 31*hashVal + int(h[i])
			}
			if hashVal < 0 {
				hashVal = -hashVal
			}
			index := hashVal % newCapacity
			newTable[index] = append(newTable[index], entry)
		}
	}

	ht.table = newTable
	ht.capacity = newCapacity
}

// Insert добавляет или обновляет значение
func (ht *ChainingHashTable[K, V]) Insert(key K, value V) {
	if ht.loadFactor() >= ht.loadFactorThreshold {
		ht.rehash()
	}

	index := ht.hash(key)
	// Проверяем, существует ли ключ
	for i := range ht.table[index] {
		if ht.table[index][i].Key == key {
			ht.table[index][i].Value = value
			return
		}
	}

	// Добавляем в конец цепочки
	ht.table[index] = append(ht.table[index], ChainEntry[K, V]{Key: key, Value: value})
	ht.size++
}

// Find ищет значение по ключу
func (ht *ChainingHashTable[K, V]) Find(key K) (V, bool) {
	index := ht.hash(key)
	for _, entry := range ht.table[index] {
		if entry.Key == key {
			return entry.Value, true
		}
	}
	var zero V
	return zero, false
}

// Remove удаляет элемент из таблицы
func (ht *ChainingHashTable[K, V]) Remove(key K) bool {
	index := ht.hash(key)
	chain := ht.table[index]
	for i, entry := range chain {
		if entry.Key == key {
			// Удаление из слайса
			ht.table[index] = append(chain[:i], chain[i+1:]...)
			ht.size--
			return true
		}
	}
	return false
}

// Display выводит содержимое таблицы
func (ht *ChainingHashTable[K, V]) Display() {
	fmt.Println("\nХЕШ-ТАБЛИЦА С МЕТОДОМ ЦЕПОЧЕК")
	for i, chain := range ht.table {
		if len(chain) > 0 {
			fmt.Printf("Ячейка [%d]: ", i)
			for _, entry := range chain {
				fmt.Printf("{%v = %v} ", entry.Key, entry.Value)
			}
			fmt.Println()
		}
	}
}

// SerializeText записывает все элементы в формате "ключ:значение"
func (ht *ChainingHashTable[K, V]) SerializeText(w io.Writer) error {
	for _, chain := range ht.table {
		for _, entry := range chain {
			// Используем fmt.Fprintln для записи каждой пары на новой строке
			if _, err := fmt.Fprintf(w, "%v:%v\n", entry.Key, entry.Value); err != nil {
				return err
			}
		}
	}
	return nil
}

// DeserializeText восстанавливает таблицу из текста.
// Так как K и V — дженерики, нам нужны функции для парсинга строк в конкретные типы.
func (ht *ChainingHashTable[K, V]) DeserializeText(r io.Reader, parseKey func(string) K, parseVal func(string) V) error {
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


// SerializeBinary преобразует данные таблицы в бинарный формат
func (ht *ChainingHashTable[K, V]) SerializeBinary(w io.Writer) error {
	// Собираем все непустые элементы в один слайс для сериализации
	var allEntries []ChainEntry[K, V]
	for _, chain := range ht.table {
		allEntries = append(allEntries, chain...)
	}

	encoder := gob.NewEncoder(w)
	return encoder.Encode(allEntries)
}

// DeserializeBinary восстанавливает таблицу из бинарного потока
func (ht *ChainingHashTable[K, V]) DeserializeBinary(r io.Reader) error {
	var allEntries []ChainEntry[K, V]
	decoder := gob.NewDecoder(r)
	if err := decoder.Decode(&allEntries); err != nil {
		return err
	}

	for _, entry := range allEntries {
		ht.Insert(entry.Key, entry.Value)
	}
	return nil
}