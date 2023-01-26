#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

void testcase() {
  int n, k;
  std::cin >> n >> k;
  
  std::vector<int> v(n);
  for (int i = 0; i < n; ++i)
    std::cin >> v[i];

  int best_val = INT_MAX;
  int best_i = 0;
  int best_j = 0;

  int i = 0;
  int j = 0;
  int sum = v[i];

  while (best_val != 0 && i < n && j < n) {
    if (std::abs(k - sum) < best_val) {
      best_val = std::abs(k - sum);
      best_i = i;
      best_j = j;
    }
    
    if (sum > k) {
      sum -= v[i];
      ++i;
    } else if (sum < k) {
      if (j == n - 1)
        break;

      sum += v[j + 1];
      ++j;
    }
  }

  std::cout << best_i << " " << best_j << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}
