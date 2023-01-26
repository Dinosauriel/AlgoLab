#include <iostream>
#include <vector>

void testcase() {
  int n;
  std::cin >> n;
  
  std::vector<int> h(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> h[i];
  }
  
  long limit = 2;
  long i_fallen = 1;
  while (i_fallen <= n && i_fallen < limit) {
    limit = std::max(limit, i_fallen + h[i_fallen - 1]);
    ++i_fallen;
  }

  std::cout << i_fallen - 1 << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}
