#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <set>

void test_case() {
  int n;
  int m;
  std::cin >> n;
  std::cin >> m;
  
  std::vector<int> S(n);
  std::vector<int> W(m);
  std::vector<bool> is_transported(m, false);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> S[i];
  }
  
  for (int i = 0; i < m; ++i) {
    std::cin >> W[i];
  }
  
  std::sort(W.begin(), W.end(), std::greater<int>());
  std::sort(S.begin(), S.end(), std::greater<int>());

  // for (int i = 0; i < m; ++i) {
  //     std::cout << "W[" << i << "] = " << W[i] << std::endl;
  // }
  // for (int i = 0; i < n; ++i) {
  //   std::cout << "S[" << i << "] = " << S[i] << std::endl;
  // }
  
  if (W[0] > S[0]) {
    std::cout << "impossible" << std::endl;
    return;
  }
  
  // greedily take the heaviest box available
  int minutes = 0;
  bool boxes_are_available;
  int frnd;
  while (true) {
    boxes_are_available = false;
    frnd = 0;
    for (int box = 0; box < m; ++box) {
      if (is_transported[box]) {
        continue;
      }
      boxes_are_available = true;
      
      if (S[frnd] >= W[box]) {
        // take the box!
        is_transported[box] = true;
        ++frnd;
      }
      
      if (frnd >= n) {
        break;
      }
    }
    if (!boxes_are_available) {
      break;
    }
    
    minutes += 3;
  }
  
  std::cout << minutes - 1 << std::endl; 
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}