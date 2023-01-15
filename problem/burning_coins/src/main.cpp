#include <algorithm>
#include <climits>
#include <vector>
#include <iostream>
#include <cassert>


void test_case() {
  
  int n;
  std::cin >> n;
  std::vector<int> coins(n);

  for (int i = 0; i < n; ++i) {
    int c;
    std::cin >> c;
    coins[i] = c;
  }
  
  std::vector<std::vector<int>> amount_won(n, std::vector<int>(n));
  
  for (int l = 0; l < n; ++l) {
    for (int i = 0; i < n - l; ++i) {
      if (l == 0) {
        amount_won[l][i] = coins[i];
        continue;
      }
      if (l == 1) {
        amount_won[l][i] = std::max(coins[i], coins[i + 1]);
        continue;
      }
      assert(l >= 2);
      
      // both take top
      int tt = coins[i + l] + amount_won[l - 2][i];
      // i take top, enemy takes bottom
      int tb = coins[i + l] + amount_won[l - 2][i + 1];
      // both take bottom
      int bb = coins[i] + amount_won[l - 2][i + 2];
      // i take bottom, enemy takes top
      int bt = coins[i] + amount_won[l - 2][i + 1];
      
      int top = std::min(tt, tb);
      int bot = std::min(bb, bt);
      
      amount_won[l][i] = std::max(top, bot);
    }
  }
  
  // for (int i = 0; i < n; ++i) {
  //   for (int j = 0; j < n; ++j) {
  //     std::cout << amount_won[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
  
  std::cout << amount_won[n - 1][0] << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}