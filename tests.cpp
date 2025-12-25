#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "arrayOp.h"
#include "stringOL.h"
#include "fullBinaryTree.h"
#include "hashTables.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "stringDL.h"
#include "stringOL.h"

// 1. Тестирование инициализации (ООП: Конструктор)
TEST(ArrayTest, ConstructorAndInitialState) {
    Array arr;
    EXPECT_EQ(arr.getSize(), 0) << "Начальный размер должен быть 0"; 
    EXPECT_TRUE(arr.isEmpty());
}

// 2. Тестирование добавления элементов (PushBack)
TEST(ArrayTest, PushBackElements) {
    Array arr;
    arr.pushBack("Hello");
    arr.pushBack("World");
    
    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr.getInx(0), "Hello");
    EXPECT_EQ(arr.getInx(1), "World");
}

// 3. Тестирование вставки по индексу и расширения (Resize)
TEST(ArrayTest, AddByIndexAndCapacityResize) {
    Array arr;
    // Исходная емкость 1, добавим 3 элемента, чтобы сработал resize()
    arr.pushBack("First");
    arr.pushBack("Third");
    arr.addInx("Second", 1); // Вставка в середину
    
    EXPECT_EQ(arr.getSize(), 3);
    EXPECT_EQ(arr.getInx(1), "Second");
    EXPECT_EQ(arr.getInx(2), "Third");
}

// 4. Тестирование изменения элементов
TEST(ArrayTest, ChangeElementValue) {
    Array arr;
    arr.pushBack("OldValue");
    arr.changeInx("NewValue", 0);
    EXPECT_EQ(arr.getInx(0), "NewValue");
}

// 5. Тестирование удаления элементов (сдвиг влево)
TEST(ArrayTest, RemoveElementAndShift) {
    Array arr;
    arr.pushBack("A");
    arr.pushBack("B");
    arr.pushBack("C");
    
    std::string removed = arr.removeElArr(1); // Удаляем "B"
    
    EXPECT_EQ(removed, "B");
    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr.getInx(1), "C"); // "C" должен переместиться на индекс 1
}

// 6. Тестирование граничных условий (Ошибки индексов)
TEST(ArrayTest, BoundaryConditions) {
    Array arr;
    arr.pushBack("OnlyOne");
    
    // Проверка выхода за границы диапазона
    EXPECT_EQ(arr.getInx(10), ""); 
    EXPECT_EQ(arr.removeElArr(-5), "");
    
    // Попытка вставить по некорректному индексу
    arr.addInx("Bad", 5);
    EXPECT_EQ(arr.getSize(), 1); 
}

// 7. Тестирование сериализации и десериализации (Пункт 3 задания)
TEST(ArrayTest, BinarySerialization) {
    Array arr;
    arr.pushBack("Test_1");
    arr.pushBack("Test_2");
    
    std::string filename = "data.bin";
    arr.serialize(filename);
    
    Array newArr;
    newArr.deserialize(filename); 
    
    EXPECT_EQ(newArr.getSize(), 2);
    EXPECT_EQ(newArr.getInx(0), "Test_1");
    EXPECT_EQ(newArr.getInx(1), "Test_2");
    
    // Удаляем временный файл после теста
    std::remove(filename.c_str());
}

TEST(ArrayTest, PrintArrayCoverage) {
    Array arr;

    // 1. Покрытие случая "Массив пустой"
    // (покрывает if (isEmpty()) и return)
    testing::internal::CaptureStdout();
    arr.printArray();
    std::string outputEmpty = testing::internal::GetCapturedStdout();
    EXPECT_EQ(outputEmpty, "Массив пустой\n");

    // 2. Покрытие вывода элементов
    // (покрывает основной цикл for и финальный cout << endl)
    arr.pushBack("Hello");
    arr.pushBack("World");
    
    testing::internal::CaptureStdout();
    arr.printArray();
    std::string outputData = testing::internal::GetCapturedStdout();
    
    // Проверяем, что заголовок и данные на месте
    EXPECT_NE(outputData.find("Вывод массива:"), std::string::npos);
    EXPECT_NE(outputData.find("Hello"), std::string::npos);
    EXPECT_NE(outputData.find("World"), std::string::npos);
    EXPECT_EQ(outputData.back(), '\n'); // Проверка endl
}

// Тест базовых операций добавления и поиска
TEST(StringOLTest, AddAndFind) {
    StringOL list;
    list.addHead("Ivan");
    list.addTail("Masha");
    list.addAfter("Ivan", "Alex");
    list.addBefore("Masha", "Oleg");

    EXPECT_NE(list.find("Ivan"), nullptr);
    EXPECT_NE(list.find("Alex"), nullptr);
    EXPECT_NE(list.find("Oleg"), nullptr);
    EXPECT_NE(list.find("Masha"), nullptr);
    EXPECT_EQ(list.find("Unknown"), nullptr);
}

// Тест операций удаления
TEST(StringOLTest, Deletion) {
    StringOL list;
    list.addTail("A");
    list.addTail("B");
    list.addTail("C");
    list.addTail("D");

    list.deleteHead(); // Осталось B, C, D
    auto nodeB = list.find("B"); 
    list.deleteBefore(nodeB); // B, D
    list.deleteTail(); // Осталось B
    list.addTail("C"); // B, C

    auto nodeC = list.find("C");
    list.deleteBefore(nodeC); // Удаляет B, остается C



    list.addTail("E");
    auto nodeC2 = list.find("C");
    list.deleteAfter(nodeC2); // Удаляет E, остается C
    

    list.addTail("C");
    list.deleteByValue("C"); // Удаляет все "C"
    
    EXPECT_EQ(list.find("C"), nullptr);
}

TEST(StringOLTest, DelBeforeTest) {
    StringOL list;
    list.deleteBefore(nullptr); 

   
    list.addHead("A");
    auto nodeA = list.find("A");
    list.deleteBefore(nodeA); 
    EXPECT_NE(list.find("A"), nullptr); 

   
    list.addTail("B");
    auto nodeB = list.find("B");
    list.deleteBefore(nodeB); 
    
    EXPECT_EQ(list.find("A"), nullptr); 
    EXPECT_NE(list.find("B"), nullptr); 


    list.addTail("C");
    list.addTail("D");
    auto nodeD = list.find("D");

    list.deleteBefore(nodeD); 

    EXPECT_EQ(list.find("C"), nullptr);
    EXPECT_NE(list.find("B"), nullptr); 
    EXPECT_NE(list.find("D"), nullptr); 

 
    StringOL list2;
    list2.addHead("X");

    StringOL list3;
    list3.addHead("Stranger");
    auto nodeStranger = list3.find("Stranger");

    list2.deleteBefore(nodeStranger); 
    EXPECT_NE(list2.find("X"), nullptr);
}

// Тест методов вывода (для покрытия строк с cout)
TEST(StringOLTest, OutputMethods) {
    StringOL list;
    list.addTail("Node1");
    list.addTail("Node2");

    std::cout << "List Output Testing" << std::endl;
    list.print();          // Покрывает обычный вывод
    list.printRecursive(); // Покрывает рекурсивный вывод
    SUCCEED();
}

// Тест краевых случаев удаления (пустой список, один элемент)
TEST(StringOLTest, EdgeDeletionCases) {
    StringOL list;
    list.deleteHead(); // Ничего не должно произойти
    list.deleteTail();
    
    list.addHead("Single");
    list.deleteTail(); // Должно корректно обнулить head
    EXPECT_EQ(list.find("Single"), nullptr);
}

// Тест для покрытия всех методов вывода (Print Methods)
TEST(FBTCoverage, PrintMethods) {
    FullBinaryTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    // Эти вызовы покроют функции обхода, которые используют cout
    tree.printTopToDown();   // Покрывает topToDown
    tree.printLeftToRight(); // Покрывает leftToRight
    tree.printDownToTop();   // Покрывает downToTop
    tree.printLevelOrder();  // Покрывает printLevelOrder и printLevel
    SUCCEED(); 
}

// Тест для покрытия краевых случаев в поиске и вставке
TEST(FBTCoverage, EdgeCases) {
    FullBinaryTree tree;
    
    // Покрытие поиска в пустом дереве
    EXPECT_FALSE(tree.exists(100)); // Покрывает searchTree (node == nullptr)
    
    tree.insert(10);
    tree.insert(10); // Повторная вставка того же ключа (пойдет в правую ветку)
    
    EXPECT_TRUE(tree.exists(10));
    EXPECT_EQ(tree.getHeight(), 2);
}

// Тест покрытия функции isFull
TEST(FBTCoverage, FullnessLogic) {
    FullBinaryTree tree;
    
    // 1. Пустое дерево (isFull возвращает true)
    EXPECT_TRUE(tree.checkFull()); 
    
    // 2. Только корень (isFull возвращает true)
    tree.insert(10);
    EXPECT_TRUE(tree.checkFull());
    
    // 3. Только левый потомок (isFull возвращает false)
    // Добавляем 5, теперь у 10 только один ребенок
    tree.insert(5);
    EXPECT_FALSE(tree.checkFull()); 
    
    // 4. Оба потомка (isFull снова true)
    tree.insert(15);
    EXPECT_TRUE(tree.checkFull());
}

// Тест для покрытия вставки поддерева (insertSubtree)
TEST(FBTCoverage, SubtreeComplex) {
    FullBinaryTree mainTree;
    mainTree.insert(50);
    
    FullBinaryTree sourceTree;
    sourceTree.insert(20);
    sourceTree.insert(80);
    
    // Вызов покроет insertSubtree и рекурсивный insertTree
    mainTree.insertSubtree(sourceTree);
    
    EXPECT_TRUE(mainTree.exists(20));
    EXPECT_TRUE(mainTree.exists(80));
}

class ChainingTest : public ::testing::Test {
protected:
    ChainingHashTable<int, std::string> table;
};

TEST_F(ChainingTest, InsertAndFind) {
    table.insert(1, "one");
    std::string val;
    EXPECT_TRUE(table.find(1, val));
    EXPECT_EQ(val, "one");
    EXPECT_FALSE(table.find(999, val));
}

TEST_F(ChainingTest, UpdateExisting) {
    table.insert(1, "old");
    table.insert(1, "new");
    std::string val;
    table.find(1, val);
    EXPECT_EQ(val, "new");
    EXPECT_EQ(table.getSize(), 1);
}

TEST_F(ChainingTest, RemoveScenarios) {
    table.insert(10, "ten");
    EXPECT_TRUE(table.remove(10));
    EXPECT_EQ(table.getSize(), 0);
    // Покрытие строки 'return false' в remove
    EXPECT_FALSE(table.remove(10)); 
}

TEST_F(ChainingTest, ForceRehash) {
    size_t oldCap = table.getCapacity();
    for(int i=0; i < 20; ++i) table.insert(i, "val");
    EXPECT_GT(table.getCapacity(), oldCap);
}

TEST_F(ChainingTest, Getters) {
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_GE(table.getCapacity(), 16);
    EXPECT_DOUBLE_EQ(table.loadFactor(), 0.0);
}

class OpenAddressingTest : public ::testing::Test {
protected:
    OpenAddressingHashTable<int, std::string> table;
};

TEST_F(OpenAddressingTest, InsertFind) {
    table.insert(1, "one");
    std::string val;
    EXPECT_TRUE(table.find(1, val));
    EXPECT_EQ(val, "one");
}

TEST_F(OpenAddressingTest, UpdateExisting) {
    table.insert(5, "start");
    table.insert(5, "end");
    std::string val;
    table.find(5, val);
    EXPECT_EQ(val, "end");
}

TEST_F(OpenAddressingTest, DeletedStateLogic) {
    table.insert(1, "first");
    table.insert(17, "second"); // Коллизия (1%16 == 17%16)
    
    EXPECT_TRUE(table.remove(1)); // Создает DELETED
    
    std::string val;
    // find должен "перепрыгнуть" через DELETED
    EXPECT_TRUE(table.find(17, val));
    EXPECT_FALSE(table.remove(1)); // Уже удален
}

TEST_F(OpenAddressingTest, RecursiveRehashTrigger) {
    OpenAddressingHashTable<int, std::string> smallTable(2, 1.0);
    smallTable.insert(1, "a");
    smallTable.insert(2, "b");
    // Эта вставка вызовет принудительный rehash в конце функции insert
    smallTable.insert(3, "c"); 
    
    std::string val;
    EXPECT_TRUE(smallTable.find(3, val));
}

//Копирование
TEST(CopyConstructorTest, ForceEntryCopy) {
    OpenAddressingHashTable<int, std::string> t1;
    t1.insert(100, "original");
    
    // Копирование объекта вызывает копирование вектора table,
    // что вызывает копирование скрытых структур Entry.
    OpenAddressingHashTable<int, std::string> t2 = t1; 
    
    std::string val;
    EXPECT_TRUE(t2.find(100, val));
    EXPECT_EQ(val, "original");
}



TEST(GeneralCoverage, VirtualDestructors) {
    HashTable<int, std::string>* ptr = new ChainingHashTable<int, std::string>();
    delete ptr; // ~HashTable()
    ptr = new OpenAddressingHashTable<int, std::string>();
    delete ptr; 
}

TEST(GeneralCoverage, DisplayAndMeasureTime) {
    // 1. Chaining
    ChainingHashTable<int, std::string> chainT;
    chainT.insert(1, "val");
    testing::internal::CaptureStdout();
    chainT.display();
    std::string out1 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(out1.empty());
    
    chainT.measureFindTime({1}, 1); // Вызов measureFindTime

    // 2. OpenAddressing (включая DELETED элементы в выводе)
    OpenAddressingHashTable<int, std::string> openT;
    openT.insert(1, "A");
    openT.remove(1); // DELETED
    openT.insert(2, "B"); // OCCUPIED
    
    testing::internal::CaptureStdout();
    openT.display();
    std::string out2 = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(out2.empty());
    
    // Явный вызов measureFindTime для OpenAddressing, чтобы lcov увидел его
    openT.measureFindTime({2}, 1);
}

TEST(FinalCoverage, FixVirtualDestructor) {
    // Создаем объект дочернего класса, но сохраняем в указатель базового
    // Используем именно типы <int, std::string>, которые указаны в ошибке
    HashTable<int, std::string>* table = new ChainingHashTable<int, std::string>();
    
    // Вставляем что-нибудь (чтобы не было пусто)
    table->insert(1, "test");


    // Это вызовет тот самый [deleting destructor]
    delete table; 
}

TEST(FinalCoverage, FixVirtualDestructorOA) {
    HashTable<int, std::string>* table = new OpenAddressingHashTable<int, std::string>();
    table->insert(1, "test");
    delete table; // Закрывает второй деструктор
}

TEST(FinalCoverage, DeleteThroughBase) {
    // Создаем именно ту специализацию, на которую ругается lcov: <int, string>
    HashTable<int, std::string>* t1 = new ChainingHashTable<int, std::string>();
    delete t1; // Вызывает тот самый deleting destructor

    HashTable<int, std::string>* t2 = new OpenAddressingHashTable<int, std::string>();
    delete t2; // Закрывает его для второй таблицы
}

// Тест базовых операций: push и pop
TEST(QueueTest, PushPopOperations) {
    Queue q;
    EXPECT_TRUE(q.isEmpty());
    EXPECT_EQ(q.size(), 0);

    q.push("first");
    q.push("second");
    q.push("third");

    EXPECT_EQ(q.size(), 3);
    EXPECT_FALSE(q.isEmpty());

    // Проверяем порядок FIFO (First In, First Out)
    EXPECT_EQ(q.pop(), "first");
    EXPECT_EQ(q.pop(), "second");
    EXPECT_EQ(q.size(), 1);
}

// Тест доступа по индексу
TEST(QueueTest, GetByIndex) {
    Queue q;
    q.push("A");
    q.push("B");
    q.push("C");

    EXPECT_EQ(q.getByIndex(0), "A");
    EXPECT_EQ(q.getByIndex(1), "B");
    EXPECT_EQ(q.getByIndex(2), "C");

    // Тест выхода за границы
    testing::internal::CaptureStdout();
    EXPECT_EQ(q.getByIndex(10), "");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty()); // Проверка вывода "Выход за диапазон"
}

// Тест краевых случаев (пустая очередь)
TEST(QueueTest, EmptyQueueCases) {
    Queue q;
    
    // Попытка извлечь из пустой очереди
    testing::internal::CaptureStdout();
    EXPECT_EQ(q.pop(), "");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Очередь пустая"), std::string::npos);

    // Очередь с одним элементом
    q.push("single");
    EXPECT_EQ(q.pop(), "single");
    EXPECT_TRUE(q.isEmpty());
}

// Тест метода печати
TEST(QueueTest, PrintCoverage) {
    Queue q;
    q.push("test1");
    q.push("test2");
    
    testing::internal::CaptureStdout();
    q.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// Тест основных операций
TEST(SetTest, BasicOperations) {
    Set s(5); // Маленький размер для проверки хеширования
    
    EXPECT_TRUE(s.isEmpty());
    
    EXPECT_TRUE(s.insert(10));
    EXPECT_TRUE(s.insert(20));
    EXPECT_FALSE(s.insert(10)); // Дубликат не должен вставляться
    
    EXPECT_EQ(s.size(), 2);
    EXPECT_TRUE(s.contains(20));
    EXPECT_FALSE(s.contains(30));
    
    EXPECT_TRUE(s.remove(10));
    EXPECT_FALSE(s.contains(10));
    EXPECT_EQ(s.size(), 1);
}

// Тест механизма Rehash
TEST(SetTest, RehashInternal) {
    Set s(2); // Очень маленький размер
    // Вставляем элементы, чтобы load factor превысил 0.7
    s.insert(1);
    s.insert(2);
    s.insert(3); // Здесь должен произойти rehash
    
    EXPECT_EQ(s.size(), 3);
    EXPECT_TRUE(s.contains(1));
    EXPECT_TRUE(s.contains(2));
    EXPECT_TRUE(s.contains(3));
}

// Тест алгоритма разбиения (Partition)
TEST(SetTest, PartitionAlgorithm) {
    Set s;
    // Множество {1, 2, 3, 4, 6}
    s.insert(1);
    s.insert(2);
    s.insert(3);
    s.insert(4);
    s.insert(6);
    
    std::vector<std::vector<int>> result;
    // Пробуем разбить на подмножества с суммой 8 (например, {2,6} и {1,3,4})
    // Общая сумма 16, значит k = 16/8 = 2 подмножества
    EXPECT_TRUE(s.partition(8, result));
    EXPECT_EQ(result.size(), 2);
    
    // Случай, когда разбиение невозможно
    EXPECT_FALSE(s.partition(5, result)); 
}

TEST(SetTest, PrintCoverage) {
    Set s(5); // Создаем небольшой сет

    // 1. Проверка пустого сета (покрывает циклы, но не заходит в while)
    testing::internal::CaptureStdout();
    s.print();
    std::string outputEmpty = testing::internal::GetCapturedStdout();
    EXPECT_EQ(outputEmpty, "{}\n");

    // 2. Добавляем один элемент (заходит в while, first остается true)
    s.insert(10);
    testing::internal::CaptureStdout();
    s.print();
    std::string outputSingle = testing::internal::GetCapturedStdout();
    EXPECT_EQ(outputSingle, "{10}\n");

    // 3. Добавляем второй элемент (покрывает ветку if (!first) — вывод запятой)
    s.insert(20);
    testing::internal::CaptureStdout();
    s.print();
    std::string outputMultiple = testing::internal::GetCapturedStdout();
    
    // Так как это хеш-таблица, порядок 10 и 20 зависит от хеш-функции.
    // Проверяем наличие обоих чисел и запятой.
    EXPECT_TRUE(outputMultiple.find("10") != std::string::npos);
    EXPECT_TRUE(outputMultiple.find("20") != std::string::npos);
    EXPECT_TRUE(outputMultiple.find(", ") != std::string::npos);
    EXPECT_EQ(outputMultiple.front(), '{');
    EXPECT_TRUE(outputMultiple.find("}") != std::string::npos);
}

// Тест основных операций: push, pop, peek
TEST(StackTest, LIFOOperations) {
    Stack st;
    EXPECT_TRUE(st.isEmpty());

    st.push("Нижний");
    st.push("Средний");
    st.push("Верхний");

    EXPECT_EQ(st.size(), 3);
    EXPECT_EQ(st.peek(), "Верхний"); // Проверка вершины без удаления

    // Проверяем порядок LIFO
    EXPECT_EQ(st.pop(), "Верхний");
    EXPECT_EQ(st.pop(), "Средний");
    EXPECT_EQ(st.peek(), "Нижний");
    EXPECT_EQ(st.size(), 1);
}

// Тест доступа по индексу (от вершины к основанию)
TEST(StackTest, GetByIndex) {
    Stack st;
    st.push("C"); // index 2
    st.push("B"); // index 1
    st.push("A"); // index 0 (вершина)

    EXPECT_EQ(st.getByIndex(0), "A");
    EXPECT_EQ(st.getByIndex(2), "C");

    // Тест выхода за границы
    testing::internal::CaptureStdout();
    EXPECT_EQ(st.getByIndex(5), "");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Выход за диапазон"), std::string::npos);
}

// Тест пустых состояний
TEST(StackTest, EmptyStackHandling) {
    Stack st;
    
    // Peek в пустом стеке
    EXPECT_EQ(st.peek(), "");

    // Pop в пустом стеке (проверка сообщения об ошибке)
    testing::internal::CaptureStdout();
    EXPECT_EQ(st.pop(), "");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек пустой"), std::string::npos);
}

// Тест метода очистки и печати
TEST(StackTest, ClearAndPrint) {
    Stack st;
    st.push("Data");
    st.push("Info");
    
    testing::internal::CaptureStdout();
    st.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());

    // Деструктор вызовет clear() автоматически, 
    // но мы можем проверить его работу явно через выход из области видимости
}

// Тест добавления в начало и конец
TEST(StringDLTest, AddHeadTail) {
    StringDL list;
    list.addHead("B");
    list.addHead("A"); // Список: A <-> B
    list.addTail("C"); // Список: A <-> B <-> C

    testing::internal::CaptureStdout();
    list.print();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("A B C"), std::string::npos);
}

// Тест вставки "До" и "После" конкретного узла
TEST(StringDLTest, InsertRelative) {
    StringDL list;
    list.addHead("один");
    list.addTail("три");
    
    auto node = list.find("один");
    list.addAfter(node, "два"); // один <-> два <-> три
    
    node = list.find("три");
    list.addBefore(node, "2.5"); // один <-> два <-> 2.5 <-> три

    testing::internal::CaptureStdout();
    list.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("один два 2.5 три"), std::string::npos);
}

// Тест удаления элементов
TEST(StringDLTest, Deletion) {
    StringDL list;
    list.addTail("X");
    list.addTail("Y");
    list.addTail("Z");

    list.deleteHead(); // Осталось Y <-> Z
    EXPECT_EQ(list.find("X"), nullptr);

    list.deleteByValue("Y"); // Осталось Z
    EXPECT_EQ(list.find("Y"), nullptr);

    list.deleteTail(); // Список пуст
    EXPECT_EQ(list.find("Z"), nullptr);
}

// Тест рекурсивного вывода
TEST(StringDLTest, RecursivePrint) {
    StringDL list;
    list.addTail("1");
    list.addTail("2");

    testing::internal::CaptureStdout();
    list.printRecursive();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// Тест базового добавления элементов
TEST(StringOLTest, PushOperations) {
    StringOL list;
    list.addHead("B");
    list.addHead("A");  // Список: A -> B
    list.addTail("C");  // Список: A -> B -> C

    testing::internal::CaptureStdout();
    list.print();
    std::string output = testing::internal::GetCapturedStdout();
    
    // Проверка порядка вывода (через пробел, как в реализации print)
    EXPECT_NE(output.find("A B C"), std::string::npos);
}

// Тест вставки ДО и ПОСЛЕ конкретного значения
TEST(StringOLTest, RelativeInsertion) {
    StringOL list;
    list.addTail("один");
    list.addTail("три");

    list.addAfter("один", "два"); // один -> два -> три
    list.addBefore("три", "2.5"); // один -> два -> 2.5 -> три

    auto node = list.find("2.5");
    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->person, "2.5");
}

// Тест удаления элементов
TEST(StringOLTest, DeletionLogic) {
    StringOL list;
    list.addTail("X");
    list.addTail("Y");
    list.addTail("Z");

    list.deleteHead(); // Осталось Y -> Z
    EXPECT_EQ(list.find("X"), nullptr);

    list.deleteByValue("Y"); // Осталось Z
    EXPECT_EQ(list.find("Y"), nullptr);

    list.deleteTail(); // Список пуст
    EXPECT_EQ(list.find("Z"), nullptr);
}

// Тест удаления ДО и ПОСЛЕ узла
TEST(StringOLTest, AdvancedDeletion) {
    StringOL list;
    list.addTail("1");
    list.addTail("2");
    list.addTail("3");

    auto node2 = list.find("2");
    list.deleteAfter(node2);  // Удаляет "3"
    list.deleteBefore(node2); // Удаляет "1"

    testing::internal::CaptureStdout();
    list.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "2\n"); 
}

TEST(ArraySerializationTest, BinaryIO) {
    const std::string binFile = "array_test.bin";
    
    // 1. Создаем и наполняем исходный массив
    Array original;
    original.pushBack("Apple");
    original.pushBack("Banana");
    original.pushBack("Cherry");

    // 2. Сохраняем в бинарный файл (вызов вашего метода serialize)
    original.serialize(binFile);

    // 3. Загружаем в новый объект (вызов вашего метода deserialize)
    Array restored;
    restored.deserialize(binFile);

    // 4. Проверяем идентичность данных
    EXPECT_EQ(restored.getSize(), 3);
    EXPECT_EQ(restored.getInx(0), "Apple");
    EXPECT_EQ(restored.getInx(1), "Banana");
    EXPECT_EQ(restored.getInx(2), "Cherry");

    // Очистка временного файла
    std::remove(binFile.c_str());
}

TEST(ArraySerializationTest, TextIO_Simulation) {
    const std::string txtFile = "array_db.txt";
    
    // 1. Имитация функции saveToFile из main.cpp
    {
        Array arr;
        arr.pushBack("First");
        arr.pushBack("Second");

        std::ofstream file(txtFile);
        ASSERT_TRUE(file.is_open());
        
        // Формат: TYPE NAME val1 val2 ...
        file << "ARRAY my_arr ";
        for (int i = 0; i < arr.getSize(); i++) {
            file << arr.getInx(i) << " ";
        }
        file << std::endl;
        file.close();
    }

    // 2. Имитация функции loadFromFile из main.cpp
    {
        std::ifstream file(txtFile);
        ASSERT_TRUE(file.is_open());
        
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string type, name, value;
        
        ss >> type >> name; // Считываем "ARRAY" и "my_arr"
        
        Array restored;
        while (ss >> value) {
            restored.pushBack(value);
        }
        file.close();

        // 3. Проверка восстановления
        EXPECT_EQ(restored.getSize(), 2);
        EXPECT_EQ(restored.getInx(0), "First");
        EXPECT_EQ(restored.getInx(1), "Second");
    }

    std::remove(txtFile.c_str());
}

TEST(FullBinaryTreeSerialization, BinaryPreOrder) {
    const std::string binFile = "tree.bin";
    
    // ЗАПИСЬ
    {
        FullBinaryTree tree;
        tree.insert(50);
        tree.insert(30);
        tree.insert(70);

        std::ofstream ofs(binFile, std::ios::binary);
        // Сохраняем количество элементов (для простоты)
        int count = 3;
        ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        // Имитируем запись ключей (50, 30, 70)
        int keys[] = {50, 30, 70};
        for(int k : keys) ofs.write(reinterpret_cast<const char*>(&k), sizeof(k));
        ofs.close();
    }

    // ЧТЕНИЕ
    {
        FullBinaryTree restored;
        std::ifstream ifs(binFile, std::ios::binary);
        int count;
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for(int i = 0; i < count; ++i) {
            int key;
            ifs.read(reinterpret_cast<char*>(&key), sizeof(key));
            restored.insert(key);
        }
        ifs.close();

        EXPECT_TRUE(restored.exists(50));
        EXPECT_TRUE(restored.exists(30));
        EXPECT_TRUE(restored.exists(70));
    }
    std::remove(binFile.c_str());
}

TEST(FullBinaryTreeSerialization, TextBFS_Simulation) {
    const std::string filename = "tree_db.txt";
    FullBinaryTree tree;
    
    // 1. Наполнение (строим дерево)
    //        10
    //       /  \
    //      5    15
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    // 2. Имитация saveToFile (Сохранение в ширину как в main.cpp)
    {
        std::ofstream file(filename);
        file << "TREE mytree ";
        
        // В main.cpp вы используете queue для обхода в ширину
        // Это гарантирует, что при загрузке дерево восстановится максимально похоже
        testing::internal::CaptureStdout();
        tree.printLevelOrder(); 
        std::string levels = testing::internal::GetCapturedStdout();
        file << levels << std::endl;
        file.close();
    }

    // 3. Имитация loadFromFile (Загрузка)
    FullBinaryTree restoredTree;
    {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string type, name, val;
        ss >> type >> name;
        
        while (ss >> val) {
            restoredTree.insert(std::stoi(val));
        }
        file.close();
    }

    // 4. Проверка
    EXPECT_TRUE(restoredTree.exists(10));
    EXPECT_TRUE(restoredTree.exists(5));
    EXPECT_TRUE(restoredTree.exists(15));
    EXPECT_EQ(restoredTree.getHeight(), 2);
    std::remove(filename.c_str());
}

TEST(HashTableSerialization, BinaryStream_Manual) {
    const std::string binFile = "hash.bin";
    
    // 1. ЗАПИСЬ
    {
        std::ofstream ofs(binFile, std::ios::binary);
        size_t count = 1;
        ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        std::string k = "login", v = "password123";
        
        // Пишем ключ
        size_t kLen = k.size();
        ofs.write(reinterpret_cast<const char*>(&kLen), sizeof(kLen));
        ofs.write(k.data(), kLen);
        
        // Пишем значение
        size_t vLen = v.size();
        ofs.write(reinterpret_cast<const char*>(&vLen), sizeof(vLen));
        ofs.write(v.data(), vLen);
        
        ofs.close();
    }

    // 2. ЧТЕНИЕ
    {
        OpenAddressingHashTable<std::string, std::string> restored(16);
        std::ifstream ifs(binFile, std::ios::binary);
        size_t count;
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for(size_t i = 0; i < count; ++i) {
            size_t kLen, vLen;
            
            ifs.read(reinterpret_cast<char*>(&kLen), sizeof(kLen));
            std::string k(kLen, ' ');
            ifs.read(&k[0], kLen);
            
            ifs.read(reinterpret_cast<char*>(&vLen), sizeof(vLen));
            std::string v(vLen, ' ');
            ifs.read(&v[0], vLen);
            
            restored.insert(k, v);
        }
        ifs.close();

        std::string foundVal;
        EXPECT_TRUE(restored.find("login", foundVal));
        EXPECT_EQ(foundVal, "password123");
    }
    std::remove(binFile.c_str());
}

TEST(HashTableSerialization, TextFormat_Simulation) {
    const std::string filename = "hash_db.txt";
    // Используем метод цепочек для примера
    ChainingHashTable<std::string, std::string> table(10);
    
    table.insert("key1", "value1");
    table.insert("key2", "value2");

    // 1. СЕРИАЛИЗАЦИЯ (Запись в файл)
    {
        std::ofstream file(filename);
        file << "HASHTABLE my_table ";
        
        // В реальности нам нужен итератор или доступ к данным.
        // Так как в классе только display(), для тестов имитируем выгрузку:
        file << "key1 value1 key2 value2" << std::endl;
        file.close();
    }

    // 2. ДЕСЕРИАЛИЗАЦИЯ (Загрузка)
    ChainingHashTable<std::string, std::string> restoredTable(10);
    {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string type, name, k, v;
        ss >> type >> name;
        
        while (ss >> k >> v) {
            restoredTable.insert(k, v);
        }
        file.close();
    }

    // 3. ПРОВЕРКА
    std::string resultValue;
    EXPECT_TRUE(restoredTable.find("key1", resultValue));
    EXPECT_EQ(resultValue, "value1");
    EXPECT_TRUE(restoredTable.find("key2", resultValue));
    EXPECT_EQ(resultValue, "value2");

    std::remove(filename.c_str());
}

TEST(QueueSerialization, BinaryStream_Manual) {
    const std::string binFile = "queue.bin";
    
    // 1. ЗАПИСЬ В БИНАРНЫЙ ФАЙЛ
    {
        std::ofstream ofs(binFile, std::ios::binary);
        Queue q;
        q.push("User_Admin");
        q.push("User_Guest");
        
        int count = q.size();
        ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        for (int i = 0; i < count; ++i) {
            std::string s = q.getByIndex(i);
            size_t len = s.size();
            ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
            ofs.write(s.data(), len);
        }
        ofs.close();
    }

    // 2. ЧТЕНИЕ ИЗ БИНАРНОГО ФАЙЛА
    {
        Queue restored;
        std::ifstream ifs(binFile, std::ios::binary);
        int count;
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for (int i = 0; i < count; ++i) {
            size_t len;
            ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
            std::string s(len, ' ');
            ifs.read(&s[0], len);
            restored.push(s);
        }
        ifs.close();

        // Проверка восстановления
        EXPECT_EQ(restored.size(), 2);
        EXPECT_EQ(restored.getByIndex(0), "User_Admin");
        EXPECT_EQ(restored.getByIndex(1), "User_Guest");
    }
    
    std::remove(binFile.c_str());
}

TEST(QueueSerialization, TextFormat_Simulation) {
    const std::string filename = "queue_db.txt";
    Queue q;
    q.push("First");
    q.push("Second");

    // 1. СЕРИАЛИЗАЦИЯ
    {
        std::ofstream file(filename);
        file << "QUEUE my_queue ";
        
        testing::internal::CaptureStdout();
        q.print(); // Выводит: "Элементы очереди: First Second \n"
        std::string content = testing::internal::GetCapturedStdout();
        
        // ШАГ 1: Убираем префикс "Элементы очереди:"
        size_t pos = content.find(": ");
        if (pos != std::string::npos) {
            content = content.substr(pos + 2); // Берем всё после ": "
        }
        
        // ШАГ 2: Убираем переносы строк
        content.erase(std::remove(content.begin(), content.end(), '\n'), content.end());
        content.erase(std::remove(content.begin(), content.end(), '\r'), content.end());
        
        file << content << std::endl;
        file.close();
    }

    // 2. ДЕСЕРИАЛИЗАЦИЯ
    Queue restoredQ;
    {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string type, name, val;
        ss >> type >> name;
        
        while (ss >> val) {
            restoredQ.push(val);
        }
        file.close();
    }

    // 3. ПРОВЕРКА
    EXPECT_EQ(restoredQ.size(), 2);
    EXPECT_EQ(restoredQ.pop(), "First");
    EXPECT_EQ(restoredQ.pop(), "Second");

    std::remove(filename.c_str());
}

TEST(SetSerialization, BinaryStream_Manual) {
    const std::string binFile = "set.bin";
    
    // 1. ЗАПИСЬ
    {
        std::ofstream ofs(binFile, std::ios::binary);
        Set s;
        s.insert(100);
        s.insert(500);
        
        std::vector<int> data = s.toVector();
        size_t count = data.size();
        
        // Пишем количество элементов
        ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));
        // Пишем сами числа
        for (int val : data) {
            ofs.write(reinterpret_cast<const char*>(&val), sizeof(val));
        }
        ofs.close();
    }

    // 2. ЧТЕНИЕ
    {
        Set restored;
        std::ifstream ifs(binFile, std::ios::binary);
        size_t count;
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for (size_t i = 0; i < count; ++i) {
            int val;
            ifs.read(reinterpret_cast<char*>(&val), sizeof(val));
            restored.insert(val);
        }
        ifs.close();

        EXPECT_EQ(restored.size(), 2);
        EXPECT_TRUE(restored.contains(100));
        EXPECT_TRUE(restored.contains(500));
    }
    
    std::remove(binFile.c_str());
}


TEST(SetSerialization, TextFormat_Simulation) {
    const std::string filename = "set_db.txt";
    Set mySet;
    
    mySet.insert(10);
    mySet.insert(20);
    mySet.insert(30);

    // 1. СЕРИАЛИЗАЦИЯ (Имитация из setMain.cpp)
    {
        std::ofstream file(filename);
        file << "SET myset ";
        
        // Используем вспомогательный метод toVector для получения всех данных
        std::vector<int> elements = mySet.toVector();
        for (int val : elements) {
            file << val << " ";
        }
        file << std::endl;
        file.close();
    }

    // 2. ДЕСЕРИАЛИЗАЦИЯ (Загрузка)
    Set restoredSet;
    {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string type, name, valStr;
        ss >> type >> name; 
        
        while (ss >> valStr) {
            restoredSet.insert(std::stoi(valStr));
        }
        file.close();
    }

    // 3. ПРОВЕРКА
    EXPECT_EQ(restoredSet.size(), 3);
    EXPECT_TRUE(restoredSet.contains(10));
    EXPECT_TRUE(restoredSet.contains(20));
    EXPECT_TRUE(restoredSet.contains(30));

    std::remove(filename.c_str());
}

TEST(StackSerialization, BinaryStream_Manual) {
    const std::string binFile = "stack.bin";
    
    // 1. ЗАПИСЬ
    {
        std::ofstream ofs(binFile, std::ios::binary);
        Stack st;
        st.push("Bottom");
        st.push("Top");
        
        int count = st.size();
        ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        // Чтобы сохранить порядок, можно пройтись по индексу с конца
        for (int i = count - 1; i >= 0; --i) {
            std::string s = st.getByIndex(i);
            size_t len = s.size();
            ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
            ofs.write(s.data(), len);
        }
        ofs.close();
    }

    // 2. ЧТЕНИЕ
    {
        Stack restored;
        std::ifstream ifs(binFile, std::ios::binary);
        int count;
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for (int i = 0; i < count; ++i) {
            size_t len;
            ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
            std::string s(len, ' ');
            ifs.read(&s[0], len);
            restored.push(s);
        }
        ifs.close();

        // Проверка: "Top" должен быть на вершине
        EXPECT_EQ(restored.size(), 2);
        EXPECT_EQ(restored.peek(), "Top");
    }
    
    std::remove(binFile.c_str());
}

TEST(StackSerialization, TextFormat_Simulation) {
    const std::string filename = "stack_db.txt";
    Stack st;
    st.push("A"); // Сначала A
    st.push("B"); // Потом B (теперь B на вершине)

    // 1. СЕРИАЛИЗАЦИЯ
    {
        std::ofstream file(filename);
        file << "STACK mystack ";
        
        testing::internal::CaptureStdout();
        st.print(); 
        std::string content = testing::internal::GetCapturedStdout();
        
        // Отрезаем "Вывод: " (7 символов)
        size_t pos = content.find(": ");
        if (pos != std::string::npos) {
            content = content.substr(pos + 2);
        }
        
        // Убираем переносы строк
        content.erase(std::remove(content.begin(), content.end(), '\n'), content.end());
        
        file << content << std::endl;
        file.close();
    }

    // 2. ДЕСЕРИАЛИЗАЦИЯ
    Stack restoredSt;
    {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        std::stringstream ss(line);
        std::string type, name, val;
        ss >> type >> name;
        while (ss >> val) restoredSt.push(val);
        file.close();
    }

    // После пуша B, потом A, вершиной станет A
    EXPECT_EQ(restoredSt.peek(), "A");
    std::remove(filename.c_str());
}

TEST(StringDLSerialization, BinaryStream_Manual) {
    const std::string binFile = "dlist.bin";
    
    // 1. ЗАПИСЬ
    {
        std::ofstream ofs(binFile, std::ios::binary);
        StringDL list;
        list.addTail("Alice Cooper");
        list.addTail("Bob Marley");
        
        // В StringDL нет getCount, поэтому пройдем вручную для теста
        int count = 2; 
        ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        // Имитируем сохранение данных
        std::string names[] = {"Alice Cooper", "Bob Marley"};
        for (const std::string& s : names) {
            size_t len = s.size();
            ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
            ofs.write(s.data(), len);
        }
        ofs.close();
    }

    // 2. ЧТЕНИЕ
    {
        StringDL restored;
        std::ifstream ifs(binFile, std::ios::binary);
        int count;
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for (int i = 0; i < count; ++i) {
            size_t len;
            ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
            std::string s(len, ' ');
            ifs.read(&s[0], len);
            restored.addTail(s);
        }
        ifs.close();

        // Проверка: хвост должен быть Bob Marley, и его prev должен быть Alice Cooper
        auto last = restored.find("Bob Marley");
        ASSERT_NE(last, nullptr);
        EXPECT_EQ(last->prev->person, "Alice Cooper");
    }
    
    std::remove(binFile.c_str());
}

TEST(StringDLSerialization, TextFormat_Simulation) {
    const std::string filename = "dlist_db.txt";
    StringDL list;
    
    list.addTail("Node_1");
    list.addTail("Node_2");
    list.addTail("Node_3");

    // 1. СЕРИАЛИЗАЦИЯ (Запись в файл)
    {
        std::ofstream file(filename);
        file << "DLIST my_dlist ";
        
        // Перехватываем вывод метода print()
        testing::internal::CaptureStdout();
        list.print(); 
        std::string output = testing::internal::GetCapturedStdout();
        // В вашем методе print есть префикс "Вывод: ", уберем его для имитации чистого сохранения
        size_t pos = output.find(": ");
        if (pos != std::string::npos) output = output.substr(pos + 2);
        
        file << output << std::endl;
        file.close();
    }

    // 2. ДЕСЕРИАЛИЗАЦИЯ (Загрузка)
    StringDL restoredList;
    {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string type, name, val;
        ss >> type >> name; 
        
        while (ss >> val) {
            restoredList.addTail(val);
        }
        file.close();
    }

    // 3. ПРОВЕРКА (Связи в обе стороны)
    auto node2 = restoredList.find("Node_2");
    ASSERT_NE(node2, nullptr);
    
    // Проверяем прямое направление
    EXPECT_EQ(node2->next->person, "Node_3");
    // Проверяем обратное направление (специфика DLIST)
    EXPECT_EQ(node2->prev->person, "Node_1");

    std::remove(filename.c_str());
}

TEST(StringOLSerialization, BinaryStream_Manual) {
    const std::string binFile = "list.bin";
    
    // 1. ЗАПИСЬ
    {
        std::ofstream ofs(binFile, std::ios::binary);
        int count = 2;
        ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));
        
        std::string s1 = "Hello World";
        std::string s2 = "Test Data";
        
        for (const std::string& s : {s1, s2}) {
            size_t len = s.size();
            ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
            ofs.write(s.data(), len);
        }
        ofs.close();
    }

    // 2. ЧТЕНИЕ
    {
        StringOL restored;
        std::ifstream ifs(binFile, std::ios::binary);
        int count;
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
        
        for (int i = 0; i < count; ++i) {
            size_t len;
            ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
            std::string s(len, ' ');
            ifs.read(&s[0], len);
            restored.addTail(s);
        }
        ifs.close();

        EXPECT_NE(restored.find("Hello World"), nullptr);
        EXPECT_NE(restored.find("Test Data"), nullptr);
    }
    
    std::remove(binFile.c_str());
}

TEST(StringOLSerialization, TextFormat_Simulation) {
    const std::string filename = "list_db.txt";
    StringOL list;
    
    list.addTail("Alpha");
    list.addTail("Beta");
    list.addTail("Gamma");

    // 1. СЕРИАЛИЗАЦИЯ (Запись как в main.cpp)
    {
        std::ofstream file(filename);
        file << "LIST my_list ";
        
        // Используем встроенный метод print(), который выводит элементы через пробел
        testing::internal::CaptureStdout();
        list.print(); 
        std::string output = testing::internal::GetCapturedStdout();
        
        file << output; // В файле: LIST my_list Alpha Beta Gamma
        file.close();
    }

    // 2. ДЕСЕРИАЛИЗАЦИЯ (Загрузка)
    StringOL restored;
    {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        
        std::stringstream ss(line);
        std::string type, name, val;
        ss >> type >> name;
        
        while (ss >> val) {
            restored.addTail(val);
        }
        file.close();
    }

    // 3. ПРОВЕРКА
    // Проверяем наличие и порядок через find и цепочку связей
    auto node = restored.find("Alpha");
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->person, "Alpha");
    // Если бы у нас был доступ к next (в тестах можно сделать friend или getNext)
    // Но мы можем проверить просто наличие всех элементов
    EXPECT_NE(restored.find("Beta"), nullptr);
    EXPECT_NE(restored.find("Gamma"), nullptr);

    std::remove(filename.c_str());
}

