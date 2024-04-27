
cmake_minimum_required(VERSION 3.22)
project(BIGHW)

set(CMAKE_CXX_STANDARD 20)  # Устанавливаем стандарт C++ на C++20

# Добавляем флаги компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -g -fsanitize=address")

# Добавляем директорию для заголовочных файлов
include_directories(.)

# Добавляем исполняемый файл
add_executable(BIGHW # Предполагаем, что ваш файл называется main.cpp
        x86_64-linux-gnu/libexpression_parser.a
        expression.h
        test.cpp
        CPos.cpp
        CPos.h
        ASTTree.h)

# Указываем путь к библиотеке expression_parser
target_link_directories(BIGHW PRIVATE ./x86_64-linux-gnu)

# Линкуем библиотеку expression_parser
target_link_libraries(BIGHW PRIVATE expression_parser)