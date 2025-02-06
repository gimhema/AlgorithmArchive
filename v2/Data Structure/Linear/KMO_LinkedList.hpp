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
class KMO_LinkedList {
private:
    KMO_Node<T>* head;
    int size;

public:
    KMO_LinkedList() {
        head = nullptr;
        size = 0;
    }

    ~KMO_LinkedList() {
        while (!isEmpty()) {
            removeFront();
        }
    }

    void insertFront(T value) {
        KMO_Node<T>* newNode = new KMO_Node<T>(value);
        newNode->next = head;
        head = newNode;
        size++;
    }

    void insertBack(T value) {
        KMO_Node<T>* newNode = new KMO_Node<T>(value);
        if (isEmpty()) {
            head = newNode;
        } else {
            KMO_Node<T>* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
    }

    void insertAt(int index, T value) {
        if (index < 0 || index > size) {
            std::cout << "Invalid index!\n";
            return;
        }
        if (index == 0) {
            insertFront(value);
            return;
        }
        KMO_Node<T>* newNode = new KMO_Node<T>(value);
        KMO_Node<T>* temp = head;
        for (int i = 0; i < index - 1; i++) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
        size++;
    }

    void removeFront() {
        if (isEmpty()) {
            std::cout << "List is empty!\n";
            return;
        }
        KMO_Node<T>* temp = head;
        head = head->next;
        delete temp;
        size--;
    }

    void removeBack() {
        if (isEmpty()) {
            std::cout << "List is empty!\n";
            return;
        }
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
        } else {
            KMO_Node<T>* temp = head;
            while (temp->next->next != nullptr) {
                temp = temp->next;
            }
            delete temp->next;
            temp->next = nullptr;
        }
        size--;
    }

    void removeAt(int index) {
        if (index < 0 || index >= size) {
            std::cout << "Invalid index!\n";
            return;
        }
        if (index == 0) {
            removeFront();
            return;
        }
        KMO_Node<T>* temp = head;
        for (int i = 0; i < index - 1; i++) {
            temp = temp->next;
        }
        KMO_Node<T>* target = temp->next;
        temp->next = target->next;
        delete target;
        size--;
    }

    void printList() {
        KMO_Node<T>* temp = head;
        while (temp != nullptr) {
            std::cout << temp->data << " -> ";
            temp = temp->next;
        }
        std::cout << "nullptr\n";
    }

    bool isEmpty() {
        return head == nullptr;
    }

    int getSize() {
        return size;
    }
};
