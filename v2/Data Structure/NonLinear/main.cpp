#include "KMO_BinaryTree.hpp"

int main() {
    KMO_BinaryTree<int> tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    std::cout << "Inorder traversal: ";
    tree.inorderTraversal();

    std::cout << "Preorder traversal: ";
    tree.preorderTraversal();

    std::cout << "Postorder traversal: ";
    tree.postorderTraversal();

    std::cout << "Search 40: " << (tree.search(40) ? "Found" : "Not Found") << std::endl;
    std::cout << "Remove 40" << std::endl;
    tree.remove(40);

    std::cout << "Inorder traversal after deletion: ";
    tree.inorderTraversal();

    return 0;
}
