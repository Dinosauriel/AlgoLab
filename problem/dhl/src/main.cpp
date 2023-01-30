#include <iostream>
#include <vector>
#include <climits>

void testcase() {
  int n; // n <= 300
  std::cin >> n;
  
  std::vector<int> a(n);
  std::vector<int> b(n);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
    a[i] -= 1;
  }
    
  for (int i = 0; i < n; ++i) {
    std::cin >> b[i];
    b[i] -= 1;
  }

  std::vector<std::vector<int>> M(n, std::vector<int>(n));
  // M[i][j] = a[i] * b[j]
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      M[i][j] = a[i] * b[j];
  }
  
  std::vector<std::vector<long>> S(n + 1, std::vector<long>(n + 1, 0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      S[i + 1][j + 1] = S[i][j + 1] + S[i + 1][j] - S[i][j] + M[i][j];
    }
  }
  
  // for (int i = 0; i < n + 1; ++i) {
  //   for (int j = 0; j < n + 1; ++j) {
  //     std::cout << S[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
  
  std::vector<std::vector<long>> T(n + 1, std::vector<long>(n + 1, -1));
  // T[i][j] = min cost for going from (0, 0) to (a_i, b_j)
  T[0][0] = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      for (int k = 0; k <= i; ++k) {
        for (int l = 0; l <= j; ++l) {
          long cost = T[k][l];
          if (cost == -1)
            continue;
          long submatrix_cost = S[i + 1][j + 1] - S[i + 1][l] - S[k][j + 1] + S[k][l];
          if (T[i + 1][j + 1] == -1) {
            T[i + 1][j + 1] = cost + submatrix_cost;
            continue;
          }
          T[i + 1][j + 1] = std::min(T[i + 1][j + 1], cost + submatrix_cost);
        }
      }
      
      // for (int ii = 0; ii < n + 1; ++ii) {
      //   for (int jj = 0; jj < n + 1; ++jj) {
      //     std::cout << T[ii][jj] << " ";
      //   }
      //   std::cout << std::endl;
      // }
    }
  }
  
  std::cout << T[n][n] << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
