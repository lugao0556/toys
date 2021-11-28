#ifndef SORT_H_
#define SORT_H_

#include <vector>

namespace algorithm {

// the impl of quick sort
extern void qsort(std::vector<int>& data);

// the impl of merge sort
extern void msort(std::vector<int>& data);

// the impl of heap sort
extern void hsort(std::vector<int>& data);



}

#endif