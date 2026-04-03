# SimpleVector

RAII wrapper for dynamic array. Simplified analog of std::vector.

## Core Features

1. Store elements in a dynamic array
2. Automatic memory management (allocation/deallocation)
3. Element access by index (operator[]) and with bounds checking (at)
4. Size (resize) and capacity (reserve) management
5. Add elements to the end (pushBack) and remove from the end (popBack)
6. Insert and erase elements at arbitrary positions (insert, erase)
7. Pointer-based iterators (begin, end, cbegin, cend)
8. Lexicographic container comparison (operators ==, !=, <, >, <=, >=)

## Class Description

1. SimpleVector - template container class
2. ArrayPtr - RAII wrapper over raw pointer
3. ReserveProxyObj - helper object for memory reservation

## Usage

- Copy the files to your project
- Use the template class SimpleVector<Type>
- Tests are implemented in main.cpp

## System Requirements

- C++17+

## Tech Stack

- C++17+ (STL)

