#include <gtest/gtest.h>

int main(int argc, char **argv) {
    // Инициализация Google Test
    ::testing::InitGoogleTest(&argc, argv);
    
    // Запуск всех тестов, определенных в tests.cpp
    return RUN_ALL_TESTS();
}