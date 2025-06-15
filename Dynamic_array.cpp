//#include "Dynamic_array.h"
//template <typename T>
//Dynamic_array<T>::Dynamic_array() : arr(nullptr), len(0), cap(0) {
//}
//
//// Constructor with count and value
//template <typename T>
//Dynamic_array<T>::Dynamic_array(int count, T value) : len(count), cap(count) {
//    if (count <= 0) {
//        arr = nullptr;
//        len = cap = 0;
//        return;
//    }
//    arr = new T[cap];
//    for (int i = 0; i < len; i++) {
//        arr[i] = value;
//    }
//}
//
//// Copy constructor
//template <typename T>
//Dynamic_array<T>::Dynamic_array(const Dynamic_array& other) : len(other.len), cap(other.cap) {
//    if (cap == 0) {
//        arr = nullptr;
//        return;
//    }
//    arr = new T[cap];
//    for (int i = 0; i < len; i++) {
//        arr[i] = other.arr[i];
//    }
//}
//
//// Destructor
//template <typename T>
//Dynamic_array<T>::~Dynamic_array() {
//    delete[] arr;
//}
//
//// Assignment operator
//template <typename T>
//Dynamic_array<T>& Dynamic_array<T>::operator=(const Dynamic_array& other) {
//    if (this != &other) {
//        delete[] arr;
//        len = other.len;
//        cap = other.cap;
//        if (cap == 0) {
//            arr = nullptr;
//        }
//        else {
//            arr = new T[cap];
//            for (int i = 0; i < len; i++) {
//                arr[i] = other.arr[i];
//            }
//        }
//    }
//    return *this;
//}
//
//// Push element
//template <typename T>
//void Dynamic_array<T>::push(T value) {
//    if (len >= cap) {
//        double_capacity();
//    }
//    arr[len++] = value;
//}
//
//// Pop element
//template <typename T>
//void Dynamic_array<T>::pop() {
//    if (len > 0) {
//        len--;
//    }
//}
//
//// Double capacity
//template <typename T>
//void Dynamic_array<T>::double_capacity() {
//    int new_cap = (cap == 0) ? 1 : cap * 2;
//    T* new_arr = new T[new_cap];
//
//    for (int i = 0; i < len; i++) {
//        new_arr[i] = arr[i];
//    }
//
//    delete[] arr;
//    arr = new_arr;
//    cap = new_cap;
//}
//
//// Display elements
//template <typename T>
//void Dynamic_array<T>::display() const {
//    cout << "[";
//    for (int i = 0; i < len; i++) {
//        cout << arr[i];
//        if (i < len - 1) {
//            cout << ", ";
//        }
//    }
//    cout << "]" << endl;
//}
//
//// Clear all elements
//template <typename T>
//void Dynamic_array<T>::clear() {
//    len = 0;
//    // Note: We keep the capacity and memory allocated for efficiency
//    // If you want to free memory too, uncomment the lines below:
//    // delete[] arr;
//    // arr = nullptr;
//    // cap = 0;
//}
//
//// Get size
//template <typename T>
//int Dynamic_array<T>::size() const {
//    return len;
//}
//
//// Get capacity
//template <typename T>
//int Dynamic_array<T>::capacity() const {
//    return cap;
//}
//
//// Index operator (non-const)
//template <typename T>
//T& Dynamic_array<T>::operator[](int index) {
//    if (index < 0 || index >= len) {
//        throw out_of_range("Index out of bounds");
//    }
//    return arr[index];
//}
//
//// Index operator (const)
//template <typename T>
//const T& Dynamic_array<T>::operator[](int index) const {
//    if (index < 0 || index >= len) {
//        throw out_of_range("Index out of bounds");
//    }
//    return arr[index];
//}