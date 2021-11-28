#ifndef ALG_UTILS_H_
#define ALG_UTILS_H_

#include <vector>

namespace algorithm {

static void swap(std::vector<int>& data, int i, int j) {
    int tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
}
}

#endif