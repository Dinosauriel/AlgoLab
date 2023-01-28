#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
  int n;
  std::cin >> n;
  
  std::vector<int> t(n);
  for (int i = 0; i < n; ++i)
    std::cin >> t[i];

  std::vector<int> l(n);
  // bomb i needs to be defused at the latest at time l[i] so all parents can be defused
  l[0] = t[0];
  for (int i = 1; i < n; ++i) {
    int p = (i - 1) / 2;
    l[i] = std::min(t[i], l[p] - 1);
  }
  
  std::sort(l.begin(), l.end());
  
  for (int i = 0; i < n; ++i) {
    // i + 1 bombs need to be defused in at most l[i] time
    if (i + 1 > l[i]) {
      std::cout << "no" << std::endl;
      return;
    }
  }
  
  std::cout << "yes" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}