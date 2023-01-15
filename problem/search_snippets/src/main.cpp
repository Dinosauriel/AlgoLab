#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <cassert>

void test_case() {
  int n;
  std::cin >> n;
  std::vector<int> m(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> m[i];
  }
  
  std::vector<std::vector<int>> p(n);
  
  for (int i = 0; i < n; ++i) {
    std::vector<int> p_i(m[i]);
    p[i] = p_i;
    for (int j = 0; j < m[i]; ++j) {
      std::cin >> p[i][j];
    }
  }
  
  int best_length = INT_MAX;
  
  std::vector<int> slider(n);
  std::vector<int> pos(n);

  for (int i = 0; i < n; ++i) {
    pos[i] = p[i][slider[i]];
  }
  int a = *std::min_element(pos.begin(), pos.end());
  int b = *std::max_element(pos.begin(), pos.end());
  
  while (true) {
    int l = b - a + 1;
    best_length = std::min(l, best_length);
    
    int first_word = 0;
    for (int i = 0; i < n; ++i) {
      if (pos[i] < pos[first_word]) {
        first_word = i;
      }
    }
    
    slider[first_word] += 1;
    if (slider[first_word] >= m[first_word]) {
      //this word does not appear any further. the length will only get longer from here
      //terminate!
      break;
    }
    pos[first_word] = p[first_word][slider[first_word]];
    
    b = std::max(b, pos[first_word]);
    a = *std::min_element(pos.begin(), pos.end());
  }

  std::cout << best_length << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}