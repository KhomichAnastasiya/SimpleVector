#pragma once

#include <cassert>
#include <stdexcept>
#include <iterator>
#include <initializer_list>
#include <utility>
#include <algorithm>

#include "array_ptr.h"

class ReserveProxyObj {
public:
    explicit ReserveProxyObj(size_t capacity_to_reserve)
        :capacity_(capacity_to_reserve)
    {
    }

    size_t ReserveCapasity() {
        return capacity_;
    }

private:
    size_t capacity_;

};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    SimpleVector(size_t size, const Type& value)
        : smart_ptr_(size), size_(size), capacity_(size) {
        std::fill(smart_ptr_.Get(), smart_ptr_.Get() + size, value);
    }

    explicit SimpleVector(size_t size)
        : SimpleVector(size, Type()) {
    }

    SimpleVector(std::initializer_list<Type> init)
        :smart_ptr_(init.size()), size_(init.size()), capacity_(init.size()) {
        std::copy(init.begin(), init.end(), smart_ptr_.Get());
    }

    SimpleVector(const SimpleVector& other) {
        assert(size_ == 0);
        SimpleVector<Type> vec_copy(other.GetSize());
        std::copy((other.smart_ptr_).Get(), ((other.smart_ptr_).Get() + other.GetSize()), (vec_copy.smart_ptr_).Get());
        vec_copy.capacity_ = other.capacity_;
        swap(vec_copy);
    }

    SimpleVector(SimpleVector&& other) {
        assert(size_ == 0);
        smart_ptr_ = std::move(other.smart_ptr_);
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return Iterator(&smart_ptr_[0]);
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return Iterator(&smart_ptr_[size_]);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return ConstIterator(&smart_ptr_[0]);
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return ConstIterator(&smart_ptr_[size_]);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return ConstIterator(&smart_ptr_[0]);
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return ConstIterator(&smart_ptr_[size_]);
    }

    SimpleVector(ReserveProxyObj capacity_to_reserve) {
        Reserve(capacity_to_reserve.ReserveCapasity());
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        ArrayPtr<Type> temp(new_capacity);
        for (std::size_t index = 0; index < size_; ++index) {
            temp[index] = smart_ptr_[index];
        }
        smart_ptr_.swap(temp);
        capacity_ = new_capacity;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (size_ == capacity_) {
            ArrayPtr<Type> temp(size_ + 1);
            std::move(smart_ptr_.Get(), smart_ptr_.Get() + size_, temp.Get());
            smart_ptr_.swap(temp);
            capacity_ = size_ + 1;
        }
        *(end()) = item;
        size_++;
    }

    void PushBack(Type&& item) {
        if (size_ == capacity_) {
            ArrayPtr<Type> temp(size_ + 1);
            std::move(smart_ptr_.Get(), smart_ptr_.Get() + size_, temp.Get());
            smart_ptr_.swap(temp);
            capacity_ = size_ + 1;
        }
        *(end()) = std::move(item);
        size_++;
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        auto dist = pos - begin();
        if (capacity_ == size_) {
            if (size_ == 0) {
                capacity_ = 1;
            }
            else capacity_ = 2 * capacity_;
            ArrayPtr<Type> temp(capacity_);
            std::move(smart_ptr_.Get(), smart_ptr_.Get() + size_, temp.Get());
            smart_ptr_.swap(temp);
        }
        std::copy_backward(std::make_move_iterator(begin() + dist), std::make_move_iterator(end()), end() + 1);
        auto* it = begin() + dist;
        *it = std::move(value);
        size_++;
        return it;
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        auto dist = pos - begin();
        if (capacity_ == size_) {
            if (size_ == 0) {
                capacity_ = 1;
            }
            else capacity_ = 2 * capacity_;
            ArrayPtr<Type> temp(capacity_);
            std::move(smart_ptr_.Get(), smart_ptr_.Get() + size_, temp.Get());
            smart_ptr_.swap(temp);
        }
        std::move_backward(std::make_move_iterator(begin() + dist), std::make_move_iterator(end()), end() + 1);
        auto* it = begin() + dist;
        *it = std::move(value);
        size_++;
        return it;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        if (pos == end()) {
            --size_;
            return end();
        }
        auto dist = std::distance(begin(), Iterator(pos));
        std::copy(std::make_move_iterator(Iterator(pos + 1)), std::make_move_iterator(end()), Iterator(pos));
        --size_;
        return Iterator(&smart_ptr_[dist]);
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (!this->IsEmpty()) {
            --size_;
        }
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        smart_ptr_.swap(other.smart_ptr_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("out_of_range");
        }
        return smart_ptr_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("out_of_range");
        }
        return smart_ptr_[index];
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size == size_) {
            return;
        }
        else if (new_size < size_) {
            for (auto it = &smart_ptr_[new_size]; it != &smart_ptr_[size_]; ++it) {
                *(it) = std::move(Type());
            }
        }
        else if (new_size > capacity_) {
            if (new_size >= 2 * capacity_) {
                capacity_ = new_size;
            }
            ArrayPtr<Type> temp(capacity_);
            std::move(&smart_ptr_[0], &smart_ptr_[size_], &temp[0]);
            for (auto it = &temp[size_]; it != &temp[new_size]; ++it) {
                *(it) = std::move(Type());
            }
            smart_ptr_.swap(temp);
        }
        size_ = new_size;
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        if (size_ == 0) {
            return true;
        }
        return false;
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            SimpleVector temp(rhs);
            swap(temp);
        }
        return *this;
    }

    SimpleVector& operator=(SimpleVector&&) = default;

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return smart_ptr_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return smart_ptr_[index];
    }

private:
    ArrayPtr<Type> smart_ptr_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

template <typename Type>
void swap(SimpleVector<Type>& lhs, SimpleVector<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <typename Type>
bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}