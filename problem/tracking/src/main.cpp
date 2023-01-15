#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>

void test_case() {
  int n;
  int m;
  int k;
  int x;
  int y;
  
  std::cin >> n;
  std::cin >> m;
  std::cin >> k;
  std::cin >> x;
  std::cin >> y;
  
  // std::cout << n << " " << m << " " << x << " " << k << std::endl;

  std::vector<std::vector<int>> rivers(n, std::vector<int>(n, 0));
  std::vector<std::vector<int>> cost(n, std::vector<int>(n, 0));
  
  for (int i = 0; i < m; ++i) {
    int a;
    int b;
    int c;
    int d;
    std::cin >> a;
    std::cin >> b;
    std::cin >> c;
    std::cin >> d;
  
    rivers[a][b] = d;
    rivers[b][a] = d;
    cost[a][b] = c;
    cost[b][a] = c;
  }
  
  // for (int i = 0; i < n; ++i) {
  //   for (int j = 0; j < n; ++j) {
  //     std::cout << P[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
  
  std::vector<std::vector<long>> T(k + 1, std::vector<long>(n, -1));
  
  while (true) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < k + 1; ++j) {
        if (T[j][i] == -1) {
          continue;
        }
        
        for (int nb = 0; nb < n; ++nb) {
          if (cost[i][nb] == 0) {
            continue;
          }
          
          int target = T[j + rivers[i][nb]][nb];
          int source = T[j][i] + cost[i][nb];
          
          if (target == -1) {
            T[j + rivers[i][nb]][nb] = source;
          }
          
          T[j + rivers[i][nb]][nb] = std::min(target, source);
        }
      }
    }

    if (T[k][y] > -1) {
      std::cout << T[k][y] << std::endl;
      return;
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}