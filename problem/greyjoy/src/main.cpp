#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <algorithm>

void testcase() {
  int n, k, w;
  std::cin >> n >> k >> w;
  
  std::vector<int> c(n);
  for (int i = 0; i < n; ++i)
    std::cin >> c[i];
  
  int max_l = 0;
  std::vector<std::vector<long>> W(w);
  for (int i = 0; i < w; ++i) {
    int l;
    std::cin >> l;
    W[i].reserve(l);
    max_l = std::max(l, max_l);
    for (int j = 0; j < l; ++j) {
      int r;
      std::cin >> r;
      W[i].push_back(r);
    }
  }
  
  int best_plan = 0;
  
  // PART 1
  // sliding window for each waterway
  for (int i = 0; i < w; ++i) {
    // sum over [a, b[
    int b = 1;
    int sum = c[W[i][0]];
    for (int a = 0; a < W[i].size(); ++a) {
      while (b < W[i].size() && sum < k) {
        sum += c[W[i][b]];
        ++b;
      }
      
      if (sum == k)
        best_plan = std::max(b - a, best_plan);
      sum -= c[W[i][a]];
    }
  }
  
  // skip PART 2 if c[0] is too large
  if (k < c[0]) {
    std::cout << best_plan << std::endl;
    return;
  }
  
  // PART 2
  // consider pairs of 2 waterways via the root island
  std::vector<std::vector<long>> W_sum = W;
  for (int i = 0; i < w; ++i) {
    W_sum[i][0] = 0;
    for (size_t j = 1; j < W[i].size(); ++j) {
      W_sum[i][j] = W_sum[i][j - 1] + c[W[i][j]];
    }
  }

  std::vector<std::vector<std::pair<int, int>>> T(k - c[0] + 1);
  for (int i = 0; i < w; ++i) {
    for (size_t j = 0; j < W_sum[i].size(); ++j) {
      if (W_sum[i][j] > k - c[0])
        break;
      T[W_sum[i][j]].push_back(std::make_pair(i, j));
    }
  }

  for (long i = 0; i <= k - c[0]; ++i) {
    // k = k1 + k2 + c[0]
    long k1 = i;
    long k2 = k - c[0] - k1;
    
    if (k1 > k2)
      break;

    for (std::pair<int, int> part1: T[k1]) {
      for (std::pair<int, int> part2: T[k2]) {
        if (part1.first != part2.first)
          best_plan = std::max(best_plan, part1.second + part2.second + 1);
      }
    }
  }

  std::cout << best_plan << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}
