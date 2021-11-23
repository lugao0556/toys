#include "bst.h"
#include <iostream>

void bst::create(const std::vector<int>& input) {
    if (input.empty()) {
        return;
    }

    if (_root) {
        std::cout << "warning: _root is not null, clean old data in bst\n";
        clear();
    }

    for (int data : input) {
        insert(data);
    }
}

void bst::clear() {
    if (!_root) {
        return;
    }

    tree_node* node = _root;
    clear_helper(node);
    _root = nullptr;  
}

void bst::delete_node(int val) {
    if (!_root) {
        std::cout << "_root is null, no data in bst.\n";
        return;
    }

    tree_node* parent = nullptr;
    tree_node* node   = _root;

    while (node) {
        if (node->val == val) {
            delete_node_helper(parent, node);
            return;
        } else if (node->val < val) {
            parent = node;
            node = node->right;
        } else {
            parent = node;
            node = node->left;
        }    
    }

    std::cout << "no find data = "<< val <<" in bst.\n";
}

void bst::insert(int val) {
    if (!_root) {
        _root = new tree_node(val);
        return;
    }

    tree_node* parent = _root;
    tree_node* node   = _root;
    while (node) {
        parent = node;
        if (node->val > val) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if (parent->val > val) {
        parent->left = new tree_node(val);
    } else {
        parent->right = new tree_node(val);
    } 
}

void bst::traversal(int type) {
    switch (type)
    {
    case IN_ORDER:
        traversal_in_order(_root);
        std::cout << std::endl;
        break;
    case PRE_ORDER:
        traversal_pre_order(_root);
        std::cout << std::endl;
        break;
    case POST_ORDER:
        traversal_post_order(_root);
        std::cout << std::endl;
        break;
    
    default:
        std::cout << "Can't traversal due to type is " << type << std::endl;
        break;
    }
}

/*-----------------------------------------------------------
 * internal function part should define at below
 *----------------------------------------------------------*/
void bst::traversal_in_order(tree_node* root) {
    if (!root) {
        return;
    }

    traversal_in_order(root->left);
    std::cout << root->val << " ";
    traversal_in_order(root->right);
}

void bst::traversal_pre_order(tree_node* root) {
    if (!root) {
        return;
    }

    std::cout << root->val << " ";
    traversal_in_order(root->left);
    traversal_in_order(root->right);
}

void bst::traversal_post_order(tree_node* root) {
    if (!root) {
        return;
    }

    traversal_in_order(root->left);
    traversal_in_order(root->right);
    std::cout << root->val << " ";
}

void bst::clear_helper(tree_node* node) {
    if (!node) {
        return;
    }

    tree_node* left = node->left;
    tree_node* right = node->right;
    delete(node);
    clear_helper(left);
    clear_helper(right);
}


void bst::delete_node_helper(tree_node* parent, tree_node* node) {
    if (!node->left && !node->right) {
        // node is a leaf-node itself, just delete it
        if (parent) {
            if (parent->left == node) {
                parent->left = nullptr;
            } else if (parent->right == node) {
                parent->right = nullptr;
            } else {
                 std::cout << "Internal Error occured in delete node.\n";
            }
        }
        delete(node);
    } else if (node->left && !node->right) {
        // node only have left child, on right child
        if (!parent) {
            // node is a root node
            _root = node->left;    
        } else {
            // node is not a root node
            if (parent->left == node) {
                parent->left = node->left;
            } else if (parent->right == node) {
                parent->right = node->left;
            } else {
                std::cout << "Internal Error occured in delete node.\n";
            }
        }
        delete(node);
    } else {
        // node have right child
        if (!parent) {
            tree_node* leftside = node->left;
            _root = node->right;

            tree_node* tmp = node->right;
            tree_node* tmp_parent = node->right;
            while (tmp) {
                tmp_parent = tmp;
                tmp = tmp->left;
            }
            tmp_parent->left = leftside;
        } else {

            if (parent->left == node) {
                parent->left = node->right;
            } else if (parent->right == node) {
                parent->right = node->right;
            } else {
                std::cout << "Internal Error occured in delete node.\n";
            }

            tree_node* leftside = node->left;
            tree_node* tmp = node->right;
            tree_node* tmp_parent = node->right;
            while (tmp) {
                tmp_parent = tmp;
                tmp = tmp->left;
            }
            tmp_parent->left = leftside;
        }

        delete node;
    }   
}






