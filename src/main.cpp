#include <iostream> 
#include "dp.h"
#include "bst.h"
#include "linklist.h"

int main() {
    std::cout << "hello, welcome to cpp world\n";

    bst BST;
    BST.insert(5);
    BST.insert(3);
    BST.insert(7);
    BST.insert(2);
    BST.insert(4);
    BST.insert(6);
    BST.insert(8);
    BST.traversal(IN_ORDER);
    std::vector<int> input = {15, 13, 17, 12, 14, 16, 18};
    BST.create(input);
    BST.traversal(IN_ORDER);

    BST.delete_node(15);
    BST.delete_node(13);
    BST.traversal(IN_ORDER);

    return 0;

}