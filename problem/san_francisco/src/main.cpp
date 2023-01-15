#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>

void test_case() {
  int n;
  int m;
  long x;
  int k;
  
  std::cin >> n;
  std::cin >> m;
  std::cin >> x;
  std::cin >> k;
  
  // std::cout << n << " " << m << " " << x << " " << k << std::endl;

  std::vector<std::vector<int>> N(n, std::vector<int>());
  std::vector<std::vector<int>> P(n, std::vector<int>());
  
  for (int i = 0; i < m; ++i) {
    int u;
    int v;
    int p;
    std::cin >> u;
    std::cin >> v;
    std::cin >> p;
    P[u].push_back(p);
    N[u].push_back(v);
  }
  
  // for (int i = 0; i < n; ++i) {
  //   for (int j = 0; j < n; ++j) {
  //     std::cout << P[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
  
  std::vector<std::vector<long>> T(k + 1, std::vector<long>(n));
  
  for (int moves_left = 1; moves_left < k + 1; ++moves_left) {
    for (int u = 0; u < n; ++u) {
      // if (moves_left == 1) {
      //   int best_canal = *std::max_element(P[u].begin(), P[u].end());
      //   if (best_canal == -1) {
      //     //no outgoing canal, restart at hole 0
      //     best_canal = *std::max_element(P[0].begin(), P[0].end());
      //     assert(best_canal != -1);
      //   }
      //   T[moves_left][u] = best_canal;
      //   continue;
      // }
      // assert(moves_left > 1);
      
      long best_value = -1;
      for (long unsigned int i = 0; i < N[u].size(); ++i) {
        int v = N[u][i];
        
        long u_v_value = P[u][i] + T[moves_left - 1][v];
        best_value = std::max(u_v_value, best_value);
      }
      if (best_value == -1) {
        //no outgoing canals, restart at hole 0
        best_value = T[moves_left][0];
      }
      T[moves_left][u] = best_value;
    }
  }
  
  for (int moves = 0; moves < k + 1; ++moves) {
    long v = T[moves][0];
    if (v >= x) {
        std::cout << moves << std::endl;
        return;
    }
  }
  std::cout << "Impossible" << std::endl; 
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}