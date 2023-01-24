#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <set>

struct batch {
  int w;
  int b;
  int f;
};

bool operator <(struct batch const& a, struct batch const& b) {
  return a.w < b.w;
}

void test_case() {
  int n;
  int m;
  std::cin >> n;
  std::cin >> m;
  
  std::vector<int> S(n);
  std::vector<int> W(m);

  for (int i = 0; i < n; ++i) {
    std::cin >> S[i];
  }

  S.push_back(0);

  for (int i = 0; i < m; ++i) {
    std::cin >> W[i];
  }
  std::sort(S.rbegin(), S.rend());
  std::sort(W.rbegin(), W.rend());

  if (W[0] > S[0]) {
    std::cout << "impossible" << std::endl;
    return;
  }
  
  long r = 0;
  long b = 0;
  for (long f = 1; f <= n; ++f) {
    while (b < m && W[b] > S[f]) {
      ++b;
    }
    // b boxes need to be carried by f friends
    r = std::max(r, (b + f - 1) / f);
  }
  
  std::cout << (r * 3) - 1 << std::endl; 
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}