#include <iostream>

template <typename T>
class KMO_Node {
public:
    T data;
    KMO_Node* next;

    KMO_Node(T value) {
        data = value;
        next = nullptr;
    }
};

template <typename T>
class KMO_LinkedQueue {
private:
    KMO_Node<T>* front;
    KMO_Node<T>* rear;
    int size;

public:
    KMO_LinkedQueue() {
        front = rear = nullptr;
        size = 0;
    }

    ~KMO_LinkedQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(T value) {
        KMO_Node<T>* newNode = new KMO_Node<T>(value);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    void dequeue() {
        if (isEmpty()) {
            std::cout << "Queue underflow!\n";
            return;
        }
        KMO_Node<T>* temp = front;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        size--;
    }

    T peek() {
        if (isEmpty()) {
            std::cout << "Queue is empty!\n";
            return T();  // 기본값 반환
        }
        return front->data;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    int getSize() {
        return size;
    }
};

