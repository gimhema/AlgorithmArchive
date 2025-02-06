#include <iostream>

template <typename T>
class KMO_Node {
public:
    T data;
    KMO_Node* prev;
    KMO_Node* next;

    KMO_Node(T value) {
        data = value;
        prev = nullptr;
        next = nullptr;
    }
};

template <typename T>
class KMO_Deque {
private:
    KMO_Node<T>* front;
    KMO_Node<T>* rear;
    int size;

public:
    KMO_Deque() {
        front = rear = nullptr;
        size = 0;
    }

    ~KMO_Deque() {
        while (!isEmpty()) {
            popFront();
        }
    }

    void pushFront(T value) {
        KMO_Node<T>* newNode = new KMO_Node<T>(value);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            newNode->next = front;
            front->prev = newNode;
            front = newNode;
        }
        size++;
    }

    void pushBack(T value) {
        KMO_Node<T>* newNode = new KMO_Node<T>(value);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            newNode->prev = rear;
            rear = newNode;
        }
        size++;
    }

    void popFront() {
        if (isEmpty()) {
            std::cout << "Deque underflow!\n";
            return;
        }
        KMO_Node<T>* temp = front;
        front = front->next;
        if (front) {
            front->prev = nullptr;
        } else {
            rear = nullptr;  // 마지막 요소 제거 시 rear도 초기화
        }
        delete temp;
        size--;
    }

    void popBack() {
        if (isEmpty()) {
            std::cout << "Deque underflow!\n";
            return;
        }
        KMO_Node<T>* temp = rear;
        rear = rear->prev;
        if (rear) {
            rear->next = nullptr;
        } else {
            front = nullptr;  // 마지막 요소 제거 시 front도 초기화
        }
        delete temp;
        size--;
    }

    T getFront() {
        if (isEmpty()) {
            std::cout << "Deque is empty!\n";
            return T();
        }
        return front->data;
    }

    T getBack() {
        if (isEmpty()) {
            std::cout << "Deque is empty!\n";
            return T();
        }
        return rear->data;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    int getSize() {
        return size;
    }
};
