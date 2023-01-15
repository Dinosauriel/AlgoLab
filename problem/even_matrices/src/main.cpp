#include <iostream>
#include <array>

void testcase() {
  int n;
  std::cin >> n;
  
  std::array<int, (n + 1) * (n + 1)> M = {};
  std::array<int, (n + 1) * (n + 1)> S = {};

  for (int row = 1; row < n + 1; ++row) {
    for (int col = 1; col < n + 1; ++col) {
      int v;
      std::cin >> v;
      M[row * (n + 1) + col] = v;
      S[row * (n + 1) + col] = v;
    }
  }
  
  //sum horizontally
  for (int row = 1; row < n + 1; ++row) {
    for (int col = 1; col < n + 1; ++col) {
      S[row * (n + 1) + col] += S[row * (n + 1) + col - 1];
    }
  }
  
  //sum vertically
  for (int row = 1; row < n + 1; ++row) {
    for (int col = 1; col < n + 1; ++col) {
      S[row * (n + 1) + col] += S[(row - 1) * (n + 1) + col];
    }
  }

  int k;
  
  for (int matrix_width = n; matrix_width > 0; --matrix_width) {
    
  }
  


  std::cout << result << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}