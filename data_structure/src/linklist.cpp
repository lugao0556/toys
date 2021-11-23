#include<iostream>
#include"linklist.h"

void linklist::insert_at_back(int val) {
    if (empty()) {
        head = new linknode(val);
        return;
    }

    linknode* p = head;
    while (p->next) {
        p = p->next;
    }

    linknode* node = new linknode(val);
    p->next = node;
    _size++;
    return;
}

void linklist::insert_at_front(int val) {
    if (empty()) {
        head = new linknode(val);
        return;
    }

    linknode* node = new linknode(val, head);
    head = node;
}

void linklist::insert_by_index(int index) {
    


}

void linklist::clear() {
    if (empty()) {
        return;
    }

    linknode* p = head;
    while (p) {
        linknode* next = p->next;
        delete p;
        p = next;
    }

    head = nullptr;
    _size = 0;
}

void linklist::show_internal_data_in_list() {
    std::cout << "show internal data in list as follow: \n";
    std::cout << "------------------------------------- \n";
    linknode* p = head;
    while(p) {
        std:: cout << p->val << " ";
        p = p->next; 
    }
    std::cout << std::endl;
    std::cout << "------------------------------------- \n";
}