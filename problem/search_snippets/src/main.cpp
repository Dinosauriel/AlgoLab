#include <iostream>
#include <climits>
#include <vector>
#include <map>

void test_case() {
  int n;
  // n words in query
  std::cin >> n;
  std::vector<int> m(n);
  // word i appears m[i] times in query
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
  
  std::vector<int> slider(n, 0);
  
  // M: pos -> word_i
  std::map<int, int> M;
  
  for (int i = 0; i < n; ++i)
    M.insert(std::make_pair(p[i][0], i));
  
  while (true) {
    std::pair<int, int> beg = *(M.begin());
    std::pair<int, int> bac = *(M.rbegin());
    
    int a = beg.first;
    int a_word = beg.second;
    
    int b = bac.first;
    
    int l = b - a + 1;
    best_length = std::min(l, best_length);
    
    slider[a_word] += 1;
    if (slider[a_word] == m[a_word]) {
      break;
    }
    
    M.erase(a);
    M.insert(std::make_pair(p[a_word][slider[a_word]], a_word));
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
