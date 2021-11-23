#include "dp.h"
#include <iostream>
#include <climits>
#include <algorithm>

namespace dp {

// internal function
void swap(std::vector<int>& v, std::size_t lhs, std::size_t rhs) {
    int tmp = v[lhs];
    v[lhs] = v[rhs];
    v[rhs] = tmp;
}

// internal function 
int partion(std::vector<int>& v, std::size_t lb, std::size_t rb) {
    int base = rb; // random search to do

    while (lb < rb) {
        int base_value = v[base];

        for (std::size_t i = lb; i < base; i++) {
            if (v[i] > v[base]) {
                swap(v, i, base);
            }    
        }

        for (std::size_t i = base + 1; i <= rb; i++) {
            if (v[i] < v[base]) {
                swap(v, i, base);
            }
        }
    }
}

// exported api
void qsort(std::vector<int>& v, std::size_t lb, std::size_t rb) {

    int index = partion(v, lb, rb);

    qsort(v, lb, index - 1);
    qsort(v, index + 1, rb);
}


// exported api
int calc_fibonacci(int n) {
    if (n <= 2) {
        return 1;
    }

    std::vector<int> dp(n+1, 0);
    dp[0] = 1;
    dp[1] = 1;
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }
    return dp[n];
}

// exported api
int coin_change(std::vector<int>& coins,  int amount) {
    // dp[n] means that when amount == n, the smallest number of coins
    // so the status_transfer_equation is as follow
    // dp[0] = 0;
    // dp[n] =  min{ dp[n-coin] + 1,  coin in coins && (n-coin > 0)}

    std::vector<int> dp(amount+1, amount+1);
    dp[0] = 0;

    for (std::size_t i = 1; i <= amount; i++) {
        for (int coin : coins) {         
            if ((int)i - coin < 0) {
                continue;
            } else {
                dp[i] = std::min(dp[i-coin] + 1, dp[i]);
            }
        }
    }

    return (dp[amount] == amount+1) ? -1 : dp[amount];
} 

/*
* 输入一个整型数组，数组中的一个或连续多个整数组成一个子数组。求所有子数组的和的最大值。
* 要求时间复杂度为O(n)。
*/
int max_sub_array(std::vector<int>& nums) {
    // dp[n] 表示以索引n为结束点子数组的最大值 
    // dp[i] = max(dp[i-1] + nums[i], nums[i])

    
    std::size_t size = nums.size();
    if (size <= 0) {
        return INT32_MIN;
    }
    std::vector<int> dp(size, INT32_MIN);
    dp[0] = nums[0];
    int sum_max = nums[0];
    for (int i = 1; i < size; i++) {
        dp[i] = std::max(dp[i-1] + nums[i], nums[i]);
        sum_max = std::max(dp[i], sum_max);
    }

    return sum_max;
}

/*
* 数组的每个下标作为一个阶梯，第 i 个阶梯对应着一个非负数的体力花费值 cost[i]（下标从 0 开始）。
* 每当爬上一个阶梯都要花费对应的体力值，一旦支付了相应的体力值，就可以选择向上爬一个阶梯或者爬两个阶梯。
* 请找出达到楼层顶部的最低花费。在开始时，你可以选择从下标为 0 或 1 的元素作为初始阶梯。
*/
int minCostClimbingStairs(std::vector<int>& cost) {
    // dp[n] means that spent smallest effort when arrive the n steps
    // dp[n] = cost[n] when n == 0;
    // dp[n] = min {cost[n], cost[n-1]} when n==1;
    // dp[n] = min {dp[n-1], dp[n-2] + cost[n]}

    std::size_t n = cost.size();
    if (n == 0) {
        return 0;
    }

    if (n == 1) {
        return cost[0];     
    }

    std::vector<int> dp(n, 0);
    dp[0] = cost[0];
    dp[1] = cost[1];
    if (n == 2) {
      return std::min(dp[0], dp[1]);
    }

    for (int i = 2; i < n; i++) {
        dp[i] = std::min(dp[i-1], dp[i-2] + cost[i]);
    }

    return dp[n-1];
}

// 最长递增子数列
int LIS(const std::vector<int>& array) {
    // dp[n] means that end with index n the length of Longest Increasing Subsequence
    int n = array.size();
    std::vector<int> dp(n, 1);

    dp[0] = 1;
    int res = dp[0];
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (array[j] < array[i]) {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
        res = std::max(dp[i], res);
    }

    return res;
}

// 最长公共子序列 Longest Common Subsequence
int LCS(const std::string& text1, const std::string& text2) {
    // dp[m][n] means that the LCS of end with index m in text1 and 
    // end with index n in text2 

    if (text1.empty() || text2.empty()) {
        return 0;
    }

    int m = text1.size();
    int n = text2.size();

    int* dp = new int[m * n]();
    dp[0] = (text1[0] == text2[0]) ? 1 : 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {

            if (text1[i] == text2[j]) {

                if (i > 0 && j > 0) {
                    dp[i*m + j] = std::max(dp[i*m + j - 1] + 1, dp[(i-1)*m + j] + 1);
                    dp[i*m + j] = std::max(dp[(i-1)*m + j-1] + 1, dp[i*m + j]);
                }

                if ((i > 0 && j == 0) || (i == 0 && j > 0)) {
                    dp[i*m + j] = 1;
                }
                
            } else {
                
                if (i > 0 && j > 0) {
                    dp[i*m + j] = std::max(dp[i*m + j - 1], dp[(i-1)*m + j]);
                    dp[i*m + j] = std::max(dp[(i-1)*m + j-1], dp[i*m + j]);
                }

                if ((i > 0 && j == 0) || (i == 0 && j > 0)) {
                    dp[i*m + j] = 0;
                }
                
            }
        }
    }

    return dp[m*n - 1];
}


int tribonacci(int n) {
    if (n ==  0) {
        return 0;
    }
    if (n == 1 && n == 2) {
        return 1;
    }
    std::vector<int> dp(n+1, 0);

    dp[0] = 0; dp[1] = 1; dp[2] = 1;
    for (int i = 3; i <= n; i++) {
        dp[i] = dp[i-1] + dp[i-2] + dp[i-3];
    }
    return dp[n];
}


/*
* dp[i] means the max profit get when sell out the stocks at the end of i-th day
* so let diff = price[i] - price[i+1]
* dp[i+1] = dp[i] + (-diff) when diff < 0
* dp[i+1] = dp[i] - diff  when diff > 0 & dp[i] > diff
* dp[i+1] = 0 when when diff > 0 & dp[i] < diff
*/
int max_profit(std::vector<int>& prices) {
    int len = prices.size();
    if (len <= 1) {
        return 0;
    }

    std::vector<int> dp(len, 0);
    int max = 0;

    for (int i = 1; i < len; i++) {
        if (prices[i] > prices[i-1]) {
            dp[i] = dp[i-1] + (prices[i] - prices[i-1]);
            max = max > dp[i] ? max : dp[i];
        } else {
            int diff = prices[i-1] - prices[i];
            if (dp[i-1] > diff) {
                dp[i] = dp[i-1] - diff;
            } else {
                dp[i] = 0;
            }
        }
    }

    return max;
}


/*
* dp[i] means that the best result get by end of index i in the array nums,so
* dp[0] = nums[0]
* dp[1] = nums[1]
* dp[i] = max{dp[0],dp[1],...,dp[i-2]} + nums[i]
* result = max{dp[0], dp[1], ..., dp[n-1]}
*/
int massage(std::vector<int>& nums) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }

    if (n == 1) {
        return nums[0];
    }

    if (n == 2) {
        return std::max(nums[0], nums[1]);
    }

    std::vector<int> dp(n, 0);
    dp[0] = nums[0];
    dp[1] = nums[1];
    int max = dp[0];
    int result = max;

    for (int i = 2; i < n; i++) {
        max = std::max(dp[i-2], max);
        dp[i] = max + nums[i]; 
        result = std::max(dp[i], dp[i-1]); 
    }

    return result;
}


std::string longestPalindrome(const std::string& s) {
    // if (s.empty()) {
    //     return "";
    // }

    // int n = s.size();
    // if (n == 1) {
    //     return s.substr(0, 1);
    // }

    // std::vector<std::string> dp(n, "");

    // dp[0] = s.substr(0, 1);
    // dp[1] = (s[0] == s[1]) ? s.substr(0,2) : s.substr(1,1);

    // std::string max = dp[0].size() > dp[1].size() ? dp[0] : dp[1];

    // for (int i = 2; i < n; i++) {
    //     int len = dp[i-1].size();
    //     if (s[i-1-len] == s[i]) {
    //         dp[i] = s.substr(i-1-len, len+2);
    //         max = dp[i].size() > max.size() ? dp[i] : max;
    //     } else if (s[i] == s[i-1]){
    //         dp[i] = s.substr(i-1, 2);
    //         max = dp[i].size() > max.size() ? dp[i] : max;
    //     } else {
    //         dp[i] = s.substr(i,1);
    //     }
    // }

    // return max;
    return "";
}

std::vector<std::string> generateParenthesis(int n) {
    if (n <= 0) {
        std::vector<std::string> empty;
        return empty;
    }

    std::vector<std::string> result;
    result.push_back("()");

    for (int i = 2; i <=n; i++) {
        std::vector<std::string> result_tmp;
        for (int j = 0; j < result.size(); j++) {
            std::string initstr = result[j];
            std::string newstr1 = "(" + initstr + ")";
            std::string newstr2 = "()" + initstr;
            std::string newstr3 = initstr + "()";
            if (find(result_tmp.begin(), result_tmp.end(), newstr1) == result_tmp.end()) {
                result_tmp.push_back(newstr1);
            }
            if (find(result_tmp.begin(), result_tmp.end(), newstr2) == result_tmp.end()) {
                result_tmp.push_back(newstr2);
            }
            if (find(result_tmp.begin(), result_tmp.end(), newstr3) == result_tmp.end()) {
                result_tmp.push_back(newstr3);
            }
        }
        result = result_tmp;
    }

    return result;
 
}

int trap(std::vector<int>& height) {

    int n = height.size();
    if (n <= 2) {
        return 0;
    }

    std::vector<int> leftMax(n, 0);
    std::vector<int> rightMax(n, 0);

    leftMax[0] = height[0];
    rightMax[n-1] = height[n-1];

    for (int i = 1; i < n; i++) {
        leftMax[i] = std::max(height[i], leftMax[i-1]);
    }

    for (int i = n-2; i >= 0; i--) {
        rightMax[i] = std::max(height[i], rightMax[i+1]);
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        ans += (std::min(leftMax[i], rightMax[i]) - height[i]);
    }

    return ans;
}


int numSquares(int  n) {
    if (n <= 3) {
        return n;
    }

    std::vector<int> dp(n+1, 0);
    dp[1] = 1; dp[2] = 2; dp[3] = 3;

    for (int i = 4; i < n+1; i++) {
        int min = i;
        for (int k = 1; k*k <= i; k++) {
            min = std::min(dp[i-k*k] + 1, min);
        }
        dp[i] = min;
    }

    return dp[n];
}

std::vector<std::vector<std::string>> partition(std::string &s) {
    int n = s.size();
    std::vector<std::vector<std::string>> res;
    if (n == 0) {
        return res;
    }

    std::vector<bool> dp_inside(n, false);
    std::vector<std::vector<bool>> dp(n, dp_inside);

    for (int i = 0; i < n; i++) {
        dp[i][i] = true;
    }

    for (int i = 0; i < n-1; i++) {
        dp[i][i+1] = (s[i] == s[i+1]);
        for (int j = i+2; j < n; j++) {
            dp[i][j] = dp[i+1][j-1] && (s[i] == s[j]);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (dp[i][j]) {
                std::string str = s.substr(i, j-i+1);
                
            }

        }
    }

}



}