#ifndef SORT_H_
#define SORT_H_

#include <vector>

namespace algorithm {

// the impl of the quick sort
extern void qsort(std::vector<int>& data);

// the impl of merge sort
extern void msort(std::vector<int>& data);


// for debug use
extern void copy(int low, int mid, int high, std::vector<int>& data, std::vector<int>& tmp);

}

#endif