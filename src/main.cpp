#include <iostream> 
#include <unistd.h>
#include "dp.h"
#include "bst.h"
#include "linklist.h"
#include "logger.h"

int main() {
    std::cout << "hello, welcome to cpp world\n";

    // bst BST;
    // BST.insert(5);
    // BST.insert(3);
    // BST.insert(7);
    // BST.insert(2);
    // BST.insert(4);
    // BST.insert(6);
    // BST.insert(8);
    // BST.traversal(IN_ORDER);
    // std::vector<int> input = {15, 13, 17, 12, 14, 16, 18};
    // BST.create(input);
    // BST.traversal(IN_ORDER);

    // BST.delete_node(15);
    // BST.delete_node(13);
    // BST.traversal(IN_ORDER);

    utc_timer Timer;
    std::cout << Timer.utc_fmt << std::endl;
    sleep(5);
    int A = Timer.get_curr_time(nullptr);
    std::cout << Timer.utc_fmt << std::endl;



    return 0;

}