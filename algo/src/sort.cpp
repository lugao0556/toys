
#include <vector>
#include <iostream>
#include "sort.h"
#include "utils.h"

namespace algorithm {
// qick sort
int partion(int l, int r, std::vector<int>& data) {
    int pivot = data[l];
    while (l < r)
    {
        while(l < r && data[r] >= pivot) {
            r--;
        }
        data[l] = data[r];

        while (l < r && data[l] <= pivot) {
            l++;
        }
        data[r] = data[l];
    }

    data[l] = pivot;
    return l;  
}

void qsort_helper(int l, int r, std::vector<int>& data) {
    if (l < r) {
        int mid = partion(l, r, data);
        qsort_helper(l, mid - 1, data);
        qsort_helper(mid + 1, r, data);
    }

}

void qsort(std::vector<int>& data) {
    qsort_helper(0, data.size() - 1, data);
}

// merge sort
void copy(int low, int mid, int high, std::vector<int>& data, std::vector<int>& tmp) {
    std::cout << "[DEBUG INFO]:" << "low:" << low << ", mid:"<<mid<< ", high:" << high<<std::endl;
    int i = low, j = mid + 1;
    for (int index = low; index < high+1; index++) {
        if (i <= mid && j <= high && data[i] < data[j]) {
            tmp[index] = data[i];
            i++;
        } else if (i <= mid && j <= high && data[i] >= data[j]) {
            tmp[index] = data[j];
            j++;
        } else {
            if (i <= mid) {
                tmp[index] = data[i];
                i++;
            }

            if (j <= high) {
                tmp[index] = data[j];
                j++;
            }
        }
    }

    for (int i = low; i < high + 1; i++) {
        data[i] = tmp[i];
    }

    std::cout << "[DEBUG_INFO]:";
    for (int i = 0; i < data.size(); i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

void msort_helper(int low, int high, std::vector<int>& data, std::vector<int>& tmp) {

    if (low < high) {
        int mid = (low + high) / 2;
        msort_helper(low, mid, data, tmp);
        msort_helper(mid+1, high, data, tmp);
        copy(low, mid, high, data, tmp);
    }

}

void msort(std::vector<int>& data) {
    int n = data.size();
    std::vector<int> tmp(n, data[0]);
    msort_helper(0, n-1, data, tmp); 
    data = tmp;
}



// heap sort
void create_max_heap(std::vector<int>& data, int r) {
    for (int i = (r-1)/2; i >= 0; i--) {
        int lchild_index = 2*i + 1;
        int rchild_index = 2*i + 2;
        int parent = i;
        while (lchild_index <= r) {
            int child = lchild_index;
            if (rchild_index <= r && data[rchild_index] > data[lchild_index]) {
                child = rchild_index;
            } 

            if (data[parent] < data[child]) {
                swap(data, parent, child);
            }

            parent = child;
            lchild_index = 2*parent + 1;
            rchild_index = 2*parent + 2;

        }
    }
}

void hsort(std::vector<int>& data) {
    // the left child of data[i] is data[2i+1]
    // the right child of data[i] is data[2i+2]
    int len = data.size();
    for (int i = len - 1; i >= 0; i--) {
        create_max_heap(data, i);
        swap(data, 0, i);
    }
} 


} // end namespace