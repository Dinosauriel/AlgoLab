#include <iostream>
#include <vector>
#include <algorithm>

struct seg {
  int a;
  int b;
};

void testcase() {
  int n, m, k;
  std::cin >> n >> m >> k;

  std::vector<int> v(n);
  for (int i = 0; i < n; ++i) {
    int v_i;
    std::cin >> v_i;
    v[i] = v_i;
  }

  // std::cout << std::hex << v.data() << std::dec << " v" << std::endl;
  
  // sliding window for [i, j[
  int sum = 0;
  std::vector<struct seg> segments(0);
  
  int j = 0;
  for (int i = 0; i < n; ++i) {
    while (j < n && sum < k) {
      sum += v[j];
      ++j;
    }
    if (sum == k) {
      segments.push_back({ .a = i, .b = j - 1 });
    }
    sum -= v[i];
  }
  
  int s = segments.size();
  
  std::vector<std::vector<int>> hoc(s + 1, std::vector<int>(m + 1, -1));
  // hoc[i][j] := considering the first i segments and j members of the army, hoc[i][j] hocruxes can be destroyed
  
  for (int i = 0; i < s + 1; ++i) {
    hoc[i][0] = 0;
  }
  
  // the segment with the highest index that doesnt overlap with new_seg
  int highest_compatible_seg = -1;
  // highest_compatible_seg == -1 -> no compatible segment found

  for (int i = 1; i < s + 1; ++i) {
    struct seg new_seg = segments[i - 1];
    
    while (highest_compatible_seg + 1 < s && segments[highest_compatible_seg + 1].b < new_seg.a) {
      ++highest_compatible_seg;
    }

    for (int j = 1; j <= std::min(m, i); ++j) {
      // either take new_seg or not
      int take;
      if (hoc[highest_compatible_seg + 1][j - 1] != -1) {
        take = (new_seg.b - new_seg.a + 1) + hoc[highest_compatible_seg + 1][j - 1];
      } else {
        take = -1;
      }
      int dont_take = hoc[i - 1][j];
      
      hoc[i][j] = std::max(take, dont_take);
    }
  }
  
  if (hoc[s][m] > 0) {
    std::cout << hoc[s][m] << std::endl;
  } else {
    std::cout << "fail" << std::endl;
  }
}


int main() {
  std::ios_base::sync_with_stdio(false); // Always!
  int t; std::cin >> t; // Read the number of test cases
  for (int i = 0; i < t; ++i)
      testcase(); // Solve a particular test case
}