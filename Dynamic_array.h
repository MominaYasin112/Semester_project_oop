#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
using namespace std;

template <typename T>
class Dynamic_array {
    T* arr;
    int len, cap;

public:
    Dynamic_array() {
        cap = 2;
        len = 0;
        arr = new T[cap];
    }
    Dynamic_array(std::initializer_list<T> list) {
        cap = list.size() > 0 ? list.size() * 2 : 2;
        len = list.size();
        arr = new T[cap];
        int i = 0;
        for (const T& val : list) {
            arr[i++] = val;
        }
    }
    Dynamic_array(int count, T value) {
        cap = count;
        len = count;
        arr = new T[cap];
        for (int i = 0; i < count; i++) {
            arr[i] = value;
        }
    }

    Dynamic_array(const Dynamic_array& other) {
        cap = other.cap;
        len = other.len;
        arr = new T[cap];
        for (int i = 0; i < len; i++) {
            arr[i] = other.arr[i];
        }
    }

    ~Dynamic_array() {
        delete[] arr;
    }

    Dynamic_array& operator=(const Dynamic_array& other) {
        if (this != &other) {
            delete[] arr;
            cap = other.cap;
            len = other.len;
            arr = new T[cap];
            for (int i = 0; i < len; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    void push(T value) {
        if (len >= cap) {
            double_capacity();
        }
        arr[len++] = value;
    }

    void pop() {
        if (len > 0) {
            len--;
        }
    }

    void double_capacity() {
        cap *= 2;
        T* newArr = new T[cap];
        for (int i = 0; i < len; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }

    void display() const {
        for (int i = 0; i < len; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    void clear() {
        delete[] arr;
        cap = 2;
        len = 0;
        arr = new T[cap];
    }

    int size() const {
        return len;
    }

    int capacity() const {
        return cap;
    }

    bool isEmpty() const {
        return len == 0;
    }

    T& operator[](int index) {
        return arr[index];
    }

    const T& operator[](int index) const {
        return arr[index];
    }

    T* begin() {
        return arr;
    }

    T* end() {
        return arr + len;
    }

    const T* begin() const {
        return arr;
    }

    const T* end() const {
        return arr + len;
    }

    void removeAt(int index) {
        if (index >= 0 && index < len) {
            for (int i = index; i < len - 1; i++) {
                arr[i] = arr[i + 1];
            }
            len--;
        }
    }

    void insertAt(int index, const T& value) {
        if (index >= 0 && index <= len) {
            if (len >= cap) {
                double_capacity();
            }
            for (int i = len; i > index; i--) {
                arr[i] = arr[i - 1];
            }
            arr[index] = value;
            len++;
        }
    }

    bool contains(const T& value) const {
        for (int i = 0; i < len; i++) {
            if (arr[i] == value)
                return true;
        }
        return false;
    }

    int findIndex(const T& value) const {
        for (int i = 0; i < len; i++) {
            if (arr[i] == value)
                return i;
        }
        return -1;
    }
};

#endif // DYNAMIC_ARRAY_H
