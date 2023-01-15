#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>

void test_case() {
  int n;
  std::cin >> n;
  
  std::vector<std::pair<int, int>> W(n);

  for (int i = 0; i < n; ++i) {
    int l;
    int p;
    std::cin >> l;
    std::cin >> p;
    W[i].first = p;
    W[i].second = l;
  }
  
  std::sort(W.begin(), W.end());
  
  int boats = 0;
  
  int front = INT_MIN;
  int earliest_end = INT_MAX;
  
  for (int i = 0; i < n; ++i) {
    int p_i = W[i].first;
    int l_i = W[i].second;
    
    if (p_i < front) {
      //cant take this boat for sure. skip
      // std::cout << "skipping boat! i = " << i << std::endl; 
      continue;
    }
    assert(p_i >= front);
    if (p_i >= earliest_end) {
      // take a boat
      ++boats;
      front = earliest_end;
      earliest_end = INT_MAX;
      // std::cout << "took boat! front = " << front << ", i = " << i << std::endl; 
    }
    
    int best_end_for_i = std::max(p_i, front + l_i);
    earliest_end = std::min(earliest_end, best_end_for_i);
  }

  //maybe take the last boat if possible
  if (W[n - 1].first >= front) {
    ++boats;
  }

  std::cout << boats << std::endl; 
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}