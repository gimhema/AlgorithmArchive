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
class KMO_Stack {
private:
    KMO_Node<T>* top;

public:
    KMO_Stack() {
        top = nullptr;
    }

    ~KMO_Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(T value) {
        KMO_Node<T>* newNode = new KMO_Node<T>(value);
        newNode->next = top;
        top = newNode;
    }

    void pop() {
        if (isEmpty()) {
            std::cout << "Stack underflow!\n";
            return;
        }
        KMO_Node<T>* temp = top;
        top = top->next;
        delete temp;
    }

    T peek() {
        if (isEmpty()) {
            std::cout << "Stack is empty!\n";
            return T();  // 기본값 반환 (0, "", nullptr 등)
        }
        return top->data;
    }

    bool isEmpty() {
        return top == nullptr;
    }
};
