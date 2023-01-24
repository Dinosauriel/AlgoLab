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

  for (int i = 0; i < m; ++i) {
    std::cin >> W[i];
  }
  std::sort(S.begin(), S.end());
  std::sort(W.begin(), W.end());

  std::vector<struct batch> batches;
  batches.reserve(n);

  // assign friends to batches
  for (int i = 0; i < n; ++i) {
    if (i == 0 || S[i] != S[i - 1]) {
      batches.push_back({ .w = S[i], .b = 0, .f = 0 });
    }
    batches.back().f += 1;
  }

  // assign boxes to batches
  size_t b_i = 0;
  for (int i = 0; i < m; ++i) {
    while (W[i] > batches[b_i].w) {
      ++b_i;
      if (b_i >= batches.size()) {
        std::cout << "impossible" << std::endl;
        return;
      }
    }
    batches[b_i].b += 1;
  }
  
  long r = 0;
  long b = 0;
  long f = 0;
  
  for (int i = batches.size() - 1; i >= 0; --i) {
    f += batches[i].f;
    long boxes_delivered = r * f;
    
    b += batches[i].b;
    if (b > boxes_delivered) {
      long rounds_to_complete = ((b - boxes_delivered) + f - 1) / f;
      r += rounds_to_complete;
    }
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