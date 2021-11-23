#ifndef DP_H_
#define DP_H_

#include <vector>
#include <string>

namespace dp{
extern void qsort(std::vector<int>&, std::size_t, std::size_t);

extern int calc_fibonacci(int);

extern int tribonacci(int);

extern int coin_change(std::vector<int>&,  int);

extern int max_sub_array(std::vector<int>&);

extern int minCostClimbingStairs(std::vector<int>&);

extern int LIS(const std::vector<int>&);

extern int LCS(const std::string&, const std::string&);

extern int max_profit(std::vector<int> &);

extern int massage(std::vector<int>&);

extern std::string longestPalindrome(const std::string&);

extern std::vector<std::string> generateParenthesis(int);

extern int trap(std::vector<int>&);

extern int numSquares(int);

extern std::vector<std::vector<std::string>> partition(std::string &);

}



#endif 