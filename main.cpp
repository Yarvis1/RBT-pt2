#include <iostream>
#include <queue>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>
#include "node.h"

using namespace std;

void transplant();
Node* getSibling();
void fixdeleteforRBT();
void RBInsert(Node*& root, int key);
Node* BSTinsert(Node* root, Node* newNode);
void print(Node* root, int space, int count);
Node* search(Node* root, int val);
Node* findMin(Node* node);
Node* deleteNodeHelper(Node* root, int val);
void deleteNode(Node*& root, int val);
void fixBSTforRBTinsert(Node*& root, Node* z);
void rightRotate(Node*& root, Node* x);
void leftRotate(Node*& root, Node* y);

int main() {
    vector<int> numbers;
    Node* root = nullptr;
    char input[10];
    bool quit = false;

    while (!quit) {
        cout << "Options: print(p), delete(d), insert(i), search(s), exit(e)" << endl;
        cin.getline(input, 10);

        if (strcmp(input, "p") == 0) {
            print(root, 0, 3);
        } else if (strcmp(input, "d") == 0) {
            int deleteVal;
            cout << "Enter value to delete: ";
            cin >> deleteVal;
            cin.ignore();
            deleteNode(root, deleteVal);
        } else if (strcmp(input, "i") == 0) {
            cout << "From file(F) or input manually(M)?" << endl;
            cin.getline(input, 10);

            if (strcmp(input, "F") == 0) {
                cout << "Using numbers from file \"number.txt\"" << endl;
                int val;
                ifstream number("number.txt");
                while (number >> val) {
                    numbers.push_back(val);
                }
                for (int i = 0; i < numbers.size(); i++) {
                    RBInsert(root, numbers[i]);
                }
            } else if (strcmp(input, "M") == 0) {
                string inputLine;
                cout << "Enter numbers separated by spaces: ";
                getline(cin, inputLine);
                stringstream ss(inputLine);
                int num;
                while (ss >> num) {
                    RBInsert(root, num);
                }
            }
        } else if (strcmp(input, "s") == 0) {
            int searchVal;
            cout << "Enter value to search: ";
            cin >> searchVal;
            cin.ignore();
            Node* found = search(root, searchVal);
            if (found) {
                cout << "Value " << found->token << " found." << endl;
            } else {
                cout << "Value not found." << endl;
            }
        } else if (strcmp(input, "e") == 0) {
            quit = true;
        }
    }
    return 0;
}

void RBInsert(Node*& root, int key) {
    Node* newNode = new Node(key, nullptr);
    newNode->color = 1; // red
    root = BSTinsert(root, newNode);
    fixBSTforRBTinsert(root, newNode);
}

Node* BSTinsert(Node* root, Node*  newNode) {
    if (root == nullptr) return newNode;

    if (newNode->token < root->token) {
        root->left = BSTinsert(root->left, newNode);
        root->left->parent = root;
    } else if (newNode->token > root->token) {
        root->right = BSTinsert(root->right, newNode);
        root->right->parent = root;
    }
    // Ignore duplicates
    return root;
}

void fixBSTforRBTinsert(Node*& root, Node* z){
    while (z->parent && z->parent->color == 1) {
        Node* grandparent = z->parent->parent;
        if (!grandparent) break; // if no grandparent parent is root

        if (z->parent == grandparent->left) {
            Node* uncle = grandparent->right;
            if (uncle != nullptr && uncle->color == 1) {
                // three cases, 1, uncle red
                z->parent->color = 0;
                uncle->color = 0;
                grandparent->color = 1;
                z = grandparent;
            } else {
                if (z == z->parent->right) {
                    // 2, z is right child
                    z = z->parent;
                    leftRotate(root, z);
                }
                // 3, z is left child
                z->parent->color = 0;
                grandparent->color = 1;
                rightRotate(root, grandparent);
            }
        } else {
            Node* uncle = grandparent->left;
            if (uncle != nullptr && uncle->color == 1) {
                // 1
                z->parent->color = 0;
                uncle->color = 0;
                grandparent->color = 1;
                z = grandparent;
            } else {
                if (z == z->parent->left) {
                    // 2
                    z = z->parent;
                    rightRotate(root, z);
                }
                // 3
                z->parent->color = 0;
                grandparent->color = 1;
                leftRotate(root, grandparent);
            }
        }
    }
    root->color = 0;
}

void leftRotate(Node*& root, Node* x){
    Node* y = x->right;
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

//same but swap x and y;
void rightRotate(Node*& root, Node* y){
    Node* x = y->left;
    y->left = x->right;
    if (x->right != nullptr)
        x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == nullptr)
        root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}

//print
void print(Node* root, int space, int count) {
    if (root == nullptr) return;
    space += count;
    print(root->right, space, count);
    cout << endl;
    for (int i = count; i < space; i++) cout << " ";
    cout << root->token;
    if(root->color==0){
	cout<<"(b)"<<endl;
    }
    else{
	cout<<"(r)"<<endl;
    }
    print(root->left, space, count);
}

//search
Node* search(Node* root, int val) {
    if (root == nullptr || root->token == val) return root;
    if (val < root->token) return search(root->left, val);
    return search(root->right, val);
}

// Find min value in tree
Node* findMin(Node* node) {
    while (node && node->left != nullptr) {
        node = node->left;
    }
    return node;
}

void transplant(Node*& root, Node* u, Node* v){
if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

Node* getSibling(Node* node){
if(!node || !node->parent)return NULL;
if(node==node->parent->left) return node->parent->right;
else{
    return node->parent->left;
   }
}

void fixdeleteforRBT(Node*& root, Node* x){
 // Loop until fix reaches root or is red
    while (fix != root && (!fix || fix->color == 0)) {
        Node* sibling = getSibling(fix);

        // If fix is a left child
        if (fix == fix->parent->left) {
            // Case 1: Sibling is red
            if (sibling && sibling->color == 1) {
               
            }

            // Case 2: Sibling and its children are black
            if ((!sibling->left || sibling->left->color == 0) &&
                (!sibling->right || sibling->right->color == 0)) {
               
            } else {
                // Case 3: Sibling's right child is black, left is red
                if (!sibling->right || sibling->right->color == 0) {
                  
                }

                // Case 4: Sibling's right child is red
               
            }
        } 
	else {
//same as above but for right child

            
}

void deleteNode(Node*& root, int val) {
  Node* z = search(root,key);//check if in list
  if(!z) return;

  Node* del=z;
  Node* fix= nullptr;
  int delcolor = del->color;

   if (z->left == nullptr) {
        // if no LC transplant with RC
        fix = z->right;
        transplant(root, z, z->right);
    } else if (z->right == nullptr) {
        // if no RC transplant with LC
         fix = z->left;
        transplant(root, z, z->left);
    } else {
        // if have both children find min to the right
        del = findMin(z->right);
        delOriginalColor = del->color;
        fix = del->right;

        // if del node is a direct child of z
        if (del->parent == z) {
            if (fix) fix->parent = del;
        } else {
            // transplant del with its RC
            transplant(root, del, del->right);
            del->right = z->right;
            del->right->parent = del;
        }

        // tranplant z with del
      transplant(root, z, del);
        del->left = z->left;
        del->left->parent = del;
        del->color = z->color;//copy color
    }

    delete z;

    if (delOriginalColor == 0) {
        fixDelete(root,x);
    }

	
}

Node* deleteNodeHelper(Node* root, int val) {
    if (root == nullptr) return root;

    if (val < root->token) {
        root->left = deleteNodeHelper(root->left, val);
    } else if (val > root->token) {
        root->right = deleteNodeHelper(root->right, val);
    } else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = findMin(root->right);
        root->token = temp->token;
        root->right = deleteNodeHelper(root->right, temp->token);
    }
    return root;
}

