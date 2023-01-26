#include <iostream>
#include <vector>

void testcase() {
  int n;
  std::cin >> n;

  std::vector<int> X(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> X[i];
  }
  
  std::vector<int> pre_sum(n + 1, 0);
  for (int i = 1; i < n + 1; ++i) {
    pre_sum[i] = pre_sum[i - 1] + X[i - 1];
  }
  
  int n_odd = 0;
  int n_even = 0;
  
  int n_even_pairs = 0;
  for (int i = 1; i < n + 1; ++i) {
    if (pre_sum[i] % 2 == 0) {
      n_even_pairs += (n_even + 1);
      ++n_even;
    } else {
      n_even_pairs += n_odd;
      ++n_odd;
    }
  }
  
  std::cout << n_even_pairs << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}
