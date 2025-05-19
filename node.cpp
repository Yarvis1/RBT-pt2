#include"node.h"
#include <iostream>
using namespace std;
Node::Node(int x,Node* p){
  token = x;
  left= NULL;
  parent = p;
  right= NULL;
  color = 1;
};
