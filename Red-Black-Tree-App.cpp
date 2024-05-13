#include <iostream>
using namespace std;

// Enum to represent colors of the nodes
enum class Color { RED, BLACK };

// ------------------------ Node structure for Red-Black Tree ------------------------
// Node structure for Red-Black Tree
template <typename T>
struct Node {
    T data;
    Color color;
    Node<T>* parent;
    Node<T>* left;
    Node<T>* right;

    // Constructor
    Node(T val) : data(val), color(Color::RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

// Red-Black Tree class ==========================================================================
template <typename T>
class RedBlackTree {
private:
    Node<T>* root;

    // Private helper functions
    void rotateLeft(Node<T>* x);
    void rotateRight(Node<T>* x);
    void fixInsertion(Node<T>* x);
    Node<T>* search(Node<T>* node, const T& val) const;

public:
    RedBlackTree() : root(nullptr) {}
    ~RedBlackTree();

    // Public interface
    void insert(const T& val);
    Node<T>* search(const T& val) const;
    void print() const;
};
// ------------------------------------------------------------------------------------------------
// Destructor
template <typename T>
RedBlackTree<T>::~RedBlackTree() {
    // Implement deletion of tree nodes to avoid memory leaks
}

// Insertion function
template <typename T>
void RedBlackTree<T>::insert(const T& val) {
    Node<T>* newNode = new Node<T>(val);

    if (root == nullptr) {
        // If tree is empty, make new node as root and color it black
        root = newNode;
        root->color = Color::BLACK;
        return;
    }

    // Traverse to find the appropriate position for the new node
    Node<T>* current = root;
    Node<T>* parent = nullptr;

    while (current != nullptr) {
        parent = current;
        if (val < current->data)
            current = current->left;
        else
            current = current->right;
    }

    // Set the parent for the new node
    newNode->parent = parent;

    // Insert the new node as left or right child of the parent
    if (val < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    // Fix any violations of Red-Black Tree properties
    fixInsertion(newNode);
}

// Fix violations of Red-Black Tree properties after insertion
template <typename T>
void RedBlackTree<T>::fixInsertion(Node<T>* x) {
    while (x != root && x->parent->color == Color::RED) {
        if (x->parent == x->parent->parent->left) {
            Node<T>* uncle = x->parent->parent->right;
            if (uncle && uncle->color == Color::RED) {
                // Case 1: Parent and uncle are both red
                x->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                x = x->parent->parent;
            }
            else {
                // Case 2: Parent is red but uncle is black or absent
                if (x == x->parent->right) {
                    x = x->parent;
                    rotateLeft(x);
                }
                // Case 3: Parent is red, uncle is black, and x is left child
                x->parent->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                rotateRight(x->parent->parent);
            }
        }
        else {
            // Symmetric cases for right child
            Node<T>* uncle = x->parent->parent->left;
            if (uncle && uncle->color == Color::RED) {
                x->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->left) {
                    x = x->parent;
                    rotateRight(x);
                }
                x->parent->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                rotateLeft(x->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}

// Left rotation
template <typename T>
void RedBlackTree<T>::rotateLeft(Node<T>* x) {
    Node<T>* y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Right rotation
template <typename T>
void RedBlackTree<T>::rotateRight(Node<T>* x) {
    Node<T>* y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// Search function
template <typename T>
Node<T>* RedBlackTree<T>::search(Node<T>* node, const T& val) const {
    if (node == nullptr || node->data == val)
        return node;

    if (val < node->data)
        return search(node->left, val);
    else
        return search(node->right, val);
}

// Wrapper for search function
template <typename T>
Node<T>* RedBlackTree<T>::search(const T& val) const {
    return search(root, val);
}

// Helper function to print tree in inorder traversal
template <typename T>
void inorderPrint(Node<T>* root) {
    if (root != nullptr) {
        inorderPrint(root->left);
        cout << root->data << "(" << (root->color == Color::RED ? "RED" : "BLACK") << ") ";
        inorderPrint(root->right);
    }
}

// Public function to print tree
template <typename T>
void RedBlackTree<T>::print() const {
    inorderPrint(root);
    cout << endl;
}

// ==================== Main function ===============================================================

// Main function
int main() {
    // Create Red-Black Tree instance
    RedBlackTree<int> tree;
    // Insert elements into the tree
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(60);
    tree.insert(70);
    tree.insert(80);
    tree.insert(90);
    tree.insert(100);

    // Print the tree
    tree.print();

    // Search for an element in the tree
    int val_to_search = 50;
    Node<int>* result = tree.search(val_to_search);
    if (result)
        cout << "Found " << val_to_search << " in the tree." << endl;
    else
        cout << val_to_search << " not found in the tree." << endl;

    return 0;
}
