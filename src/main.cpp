#include <iostream> 
#include <unistd.h>
#include <assert.h>
#include "bst.h"
#include "linklist.h"
#include "logger.h"
#include "logger_test.h"
#include "producer_consumer_pattern.h"
#include "algo.h"

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

    // utc_timer Timer;
    // std::cout << Timer.utc_fmt << std::endl;
    // sleep(5);
    // int A = Timer.get_curr_time(nullptr);
    // std::cout << Timer.utc_fmt << std::endl;


    //pc::test();

    //logger_test::test();

    // std::vector<int> data = {3,5,2,6,1,7,0,9};
    // algorithm::msort(data);
    // for (int i = 0; i < data.size(); i++) {
    //     std::cout << data[i] << " ";
    // }
    // std::cout << std::endl;
    // return 0;

    //std::vector<int> data = {6,4,2,8, 9, 10, 3,5,1,7};
    std::vector<int> data = {6,4,2,8, 9, 3,5,1,7};
    algorithm::msort(data);
    // algorithm::copy(0, 0, 1, data, tmp);
    // algorithm::copy(2, 2, 3, data, tmp);
    // algorithm::copy(4, 4, 5, data, tmp);
    // algorithm::copy(6, 6, 7, data, tmp);
    // algorithm::copy(0, 1, 3, data, tmp);
    // algorithm::copy(4, 5, 7, data, tmp);
    // algorithm::copy(0, 3, 7, data, tmp);
    for (int i = 0; i < data.size(); i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
    return 0;

}