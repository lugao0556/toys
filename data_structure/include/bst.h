#ifndef BST_H_
#define BST_H_

#include <vector>

#define IN_ORDER   0
#define PRE_ORDER  1
#define POST_ORDER 2


struct tree_node
{
    int val;
    tree_node* left;
    tree_node* right;

    tree_node() : val(0), left(nullptr), right(nullptr) {}
    tree_node(int x) : val(x), left(nullptr), right(nullptr) {}
    tree_node(int x, tree_node* left, tree_node* right) : val(x), left(left), right(right) {}
};

class bst
{
private:
    tree_node* _root;

    //internal function
    void traversal_in_order(tree_node* root);
    void traversal_pre_order(tree_node* root);
    void traversal_post_order(tree_node* root);
    void clear_helper(tree_node* root);
    void delete_node_helper(tree_node* parent, tree_node* node);
public:
    bst() : _root(nullptr) {}
    ~bst() {}
    void create(const std::vector<int>& input);
    void insert(int val);
    void clear();
    void delete_node(int val);

    void traversal(int traversal_type);
};







#endif