#ifndef LINKLIST_H_
#define LINKLIST_H_

struct linknode
{
    int val;
    linknode* next;
    linknode(int val) : val(val), next(nullptr) {}
    linknode(int val, linknode* next) : val(val), next(next) {}
};

class linklist
{
private:
    linknode* head;
    unsigned _size;
public:
    linklist() : head(nullptr), _size(0) {} ;
    ~linklist() {clear();};

    bool empty() {return head == nullptr;}
    int  size() {return _size;};

    // insert op
    void insert_at_back(int val);
    void insert_at_front(int val);
    void insert_by_index(int index);

    void clear();

    // use for debug
    void show_internal_data_in_list();

    
};





#endif