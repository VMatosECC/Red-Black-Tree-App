#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*  --------------------------------------------------------------
 This is a partial implementation of a Red-Black Tree (RBT).
 This version includes insertion and search functions only.
 Refer to the following resources for more details:
 "Introduction to Algorithms" by Cormen  ISBN-13: 978-0262033848

 For a visualization of RBT operations, visit the following link
 https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
*/



// Enum to represent colors of the nodes
enum class Color { RED, BLACK };
const bool debugFlag = false;    // Set to trur to enable debug output

// ------------------------ Node structure for Red-Black Tree ------------------------
template <typename T>
struct Node {
    T        data;
    Color    color;
    Node<T>* parent;
    Node<T>* left;
    Node<T>* right;

    // Constructor - Accept data, set color to RED and pointers to nullptr 
    Node(T val) {
        data   = val;
        color  = Color::RED;
        parent = left = right = nullptr;
    }

    // Helper function to get data and color of a node
    friend string getDataAndColor(const Node<T>* pn) {
        if (pn == nullptr)
            return "NULL(BLACK)";
        else
            return to_string(pn->data) 
                 + (pn->color == Color::RED ? "(RED)" : "(BLACK)");
    }

    void print() const {
        cout << " [ " << getDataAndColor(this)
            << "\t  P:" << getDataAndColor(parent)
            << "\t  L:" << getDataAndColor(left)
            << "\t  R:" << getDataAndColor(right) << " ] ";
    }

};

// Red-Black Tree class ==========================================================================
template <typename T>
class RedBlackTree {
private:
    Node<T>* root;

    // Private helper functions
    void     rotateLeft(Node<T>* x);
    void     rotateRight(Node<T>* x);
    void     fixInsertion(Node<T>* x);
    Node<T>* search(Node<T>* node, const T& val) const;

public:
     RedBlackTree() : root(nullptr) {}
    ~RedBlackTree();

    // Public interface
    void     insert(const T& val);
    Node<T>* search(const T& val) const;
    void     print() const;
};
// ------------------------------------------------------------------------------------------------
// Destructor
template <typename T>
RedBlackTree<T>::~RedBlackTree() {
    // TODO - Implement deletion of tree nodes to avoid memory leaks
}

// Insertion function
template <typename T>
void RedBlackTree<T>::insert(const T& val) {
    Node<T>* newNode = new Node<T>(val);

    if (root == nullptr) {
        // If tree is empty, make new node as root and color it black
        root = newNode;
        root->color = Color::BLACK;
        if(debugFlag) {root->print(); cout << "\tInserted as root." << endl;}
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
    if (debugFlag) { newNode->print(); cout << "\tInserted (fixed)." << endl; }
}

// Fix violations of Red-Black Tree properties after insertion --------------
template <typename T>
void RedBlackTree<T>::fixInsertion(Node<T>* x) {

    // Beginning with node x, continue fixing until the tree is a valid Red-Black Tree
    while (x != root && x->parent->color == Color::RED) {

        Node<T>* grandparent = x->parent->parent;
        Node<T>* parent = x->parent;
        if (debugFlag) {
            cout << " Fixing for " << getDataAndColor(x)
                << " with parent " << getDataAndColor(parent)
                << " and grandparent " << getDataAndColor(grandparent) << endl;
        }

        //Is the parent a left child?
        if (x->parent == x->parent->parent->left) {
            Node<T>* uncle = x->parent->parent->right;
            if (uncle && uncle->color == Color::RED) {
                if (debugFlag) cout << " Case 1: Parent and uncle are both red (RAF)" << endl;
                // Case 1: Parent and uncle are both red
                x->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                x = x->parent->parent;
            }
            else {
                // Case 2: Parent is red but uncle is black or absent

                if (x == x->parent->right) {
                    if (debugFlag) cout << " Case 2: Parent is red, uncle is black, and x is right child (BAR left)" << endl;
                    x = x->parent;
                    rotateLeft(x);
                }
                // Case 3: Parent is red, uncle is black, and x is left child
                if (debugFlag) cout << " Case 3: Parent is red, uncle is black, and x is left child (BAR right)" << endl;
                x->parent->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                rotateRight(x->parent->parent);
            }
        }
        else {
            // Symmetric cases for a parent that is a right child
            Node<T>* uncle = x->parent->parent->left;

            if (debugFlag) {
                cout << " Correcting for right child" << endl;
                cout << " Uncle: " << getDataAndColor(uncle) << endl;

            }

            if (uncle && uncle->color == Color::RED) {
                if (debugFlag) cout << " Case 1B: Parent and uncle are both red (RAF)" << endl;
                x->parent->color = Color::BLACK;
                uncle->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->left) {
                    if (debugFlag) cout << " Case 2B: Parent is red, uncle is black, and x is left child (BAR right)" << endl;
                    x = x->parent;
                    rotateRight(x);
                }
                if (debugFlag) cout << " Case 3B: Parent is red, uncle is black, and x is right child (BAR left)" << endl;
                x->parent->color = Color::BLACK;
                x->parent->parent->color = Color::RED;
                rotateLeft(x->parent->parent);
            }
        }
    }

    // Make sure the root is ALWAYS black
    root->color = Color::BLACK;
}

// Left rotation
template <typename T>
void RedBlackTree<T>::rotateLeft(Node<T>* x) {
    /*  Rotate left around x  (x goes to the left side) -------------

                        XP                      XP
                      /                        /
                     X        ------>         Y
                    /  \                    /  \
                   XL   Y                  X   YR
                       /  \              /  \
                      YL   YR           XL   YL

         x is the node to be rotated
         y is the right child of x
         x's right child becomes y's left child
         y's parent becomes x's parent
         y's left child becomes x
         x's parent becomes y
         x's parent's child becomes y
         y becomes the parent of x
         x becomes the left child of y
     ---------------------------------------------------
     */
     // y is the right child of x
    Node<T>* y = x->right;

    x->right = y->left;
    if (y->left != nullptr) y->left->parent = x;

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
    /* ---------------------------------------------------------

    Right rotate around x (x goes to the right side) 
            XP                      XP
            |                        |
            X        ------>         Y
           /  \                     /  \
          Y    XR                  YL   X
         /  \                          /  \
        YL   YR                      YR   XR

         x is the node to be rotated
         y is the left child of x
         x's left child becomes y's right child
         y's parent becomes x's parent
         y's right child becomes x
         x's parent becomes y
         x's parent's child becomes y
         y becomes the parent of x
         x becomes the right child of y
    --------------------------------------------------------- */

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

// Helper function to print tree in preorder traversal
template <typename T>
void inorderPrint(Node<T>* pn) {
    if (pn != nullptr) {
        cout << getDataAndColor(pn) << " ";
        inorderPrint(pn->left);
        inorderPrint(pn->right);
    }
}

// Public function to print tree
template <typename T>
void RedBlackTree<T>::print() const {
    inorderPrint(root);
    cout << endl;
}
// --------------------------------------------------------------------------------------------------

RedBlackTree<int> loadSample1() {
    RedBlackTree<int> tree;
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
    return tree;
}

RedBlackTree<int> loadSample2() {
    RedBlackTree<int> tree;
    tree.insert(40);
    tree.insert(20);
    tree.insert(70);
    tree.insert(10);
    tree.insert(30);
    tree.insert(35);
    tree.insert(37);
    return tree;
}

// ==================== Main function ===============================================================


int main() {
    
    RedBlackTree<int> tree;
    


    //Sample 1. Create a RBT. This sample shows left rotation & color flip 
    //tree = loadSample1(); 
    
    //SAMPLE2 2. Create a RBT. This sample shows right rotation & color flip 
    tree = loadSample2();

    // Print the tree in [Root-Left-Right] order (aka. peorder)
    cout << "\n Pre-Order tree ==> ";
    tree.print();

    // Search for an element in the tree
    int searchKey = 20;
    Node<int>* result = tree.search(searchKey);
    if (result) {
        cout << " Key " << searchKey << " was found in the tree." << endl;
        result->print();  cout << endl;
    }
    else
        cout << " Key " << searchKey << " not found in the tree." << endl;

    return 0;
}
