#include <iostream>

template <typename T>
struct KMO_TreeNode {
    T data;
    KMO_TreeNode* left;
    KMO_TreeNode* right;
    KMO_TreeNode(T value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class KMO_BinaryTree {
private:
    KMO_TreeNode<T>* root;

    void inorderTraversal(KMO_TreeNode<T>* node) {
        if (node) {
            inorderTraversal(node->left);
            std::cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

    void preorderTraversal(KMO_TreeNode<T>* node) {
        if (node) {
            std::cout << node->data << " ";
            preorderTraversal(node->left);
            preorderTraversal(node->right);
        }
    }

    void postorderTraversal(KMO_TreeNode<T>* node) {
        if (node) {
            postorderTraversal(node->left);
            postorderTraversal(node->right);
            std::cout << node->data << " ";
        }
    }

    KMO_TreeNode<T>* insert(KMO_TreeNode<T>* node, T value) {
        if (!node) return new KMO_TreeNode<T>(value);
        if (value < node->data)
            node->left = insert(node->left, value);
        else
            node->right = insert(node->right, value);
        return node;
    }

    KMO_TreeNode<T>* search(KMO_TreeNode<T>* node, T value) {
        if (!node || node->data == value) return node;
        if (value < node->data)
            return search(node->left, value);
        return search(node->right, value);
    }

    KMO_TreeNode<T>* findMin(KMO_TreeNode<T>* node) {
        while (node && node->left)
            node = node->left;
        return node;
    }

    KMO_TreeNode<T>* remove(KMO_TreeNode<T>* node, T value) {
        if (!node) return node;
        if (value < node->data)
            node->left = remove(node->left, value);
        else if (value > node->data)
            node->right = remove(node->right, value);
        else {
            if (!node->left) {
                KMO_TreeNode<T>* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                KMO_TreeNode<T>* temp = node->left;
                delete node;
                return temp;
            }
            KMO_TreeNode<T>* temp = findMin(node->right);
            node->data = temp->data;
            node->right = remove(node->right, temp->data);
        }
        return node;
    }

public:
    KMO_BinaryTree() : root(nullptr) {}

    void insert(T value) { root = insert(root, value); }
    void inorderTraversal() { inorderTraversal(root); std::cout << std::endl; }
    void preorderTraversal() { preorderTraversal(root); std::cout << std::endl; }
    void postorderTraversal() { postorderTraversal(root); std::cout << std::endl; }
    bool search(T value) { return search(root, value) != nullptr; }
    void remove(T value) { root = remove(root, value); }
    
    KMO_TreeNode<T>* getLeftNode(T value) {
        KMO_TreeNode<T>* node = search(root, value);
        return node ? node->left : nullptr;
    }

    KMO_TreeNode<T>* getRightNode(T value) {
        KMO_TreeNode<T>* node = search(root, value);
        return node ? node->right : nullptr;
    }
};


