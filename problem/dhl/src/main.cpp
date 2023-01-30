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
  
  std::vector<std::vector<long>> T(n + 1, std::vector<long>(n + 1, LONG_MAX));
  // T[i][j] = min cost for going from (0, 0) to (a_i, b_j)
  T[0][0] = 0;

  for (int i = 1; i < n + 1; ++i) {
    for (int j = 1; j < n + 1; ++j) {
      long cost = std::min(T[i - 1][j - 1], std::min(T[i - 1][j], T[i][j - 1]));
      T[i][j] = std::min(T[i][j], cost + M[i - 1][j - 1]);
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
