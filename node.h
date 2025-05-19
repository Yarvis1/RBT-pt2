class Node{
 public:
  int token;
  int color; // 1 is red, 0 is black
  Node* right;
  Node* left;
  Node* parent;
  Node(int x, Node* p);
};
