#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

struct A {
  long p;
  long h;
};

void test_case() {
  int n, m;
  std::cin >> n >> m;
  long a, b, P, H, W;
  std::cin >> a >> b >> P >> H >> W;

  std::vector<struct A> pA(n);
  std::vector<long> pB(m);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> pA[i].p >> pA[i].h;
  }
  
  for (int i = 0; i < m; ++i) {
    std::cin >> pB[i];
  }
  
  std::sort(pB.rbegin(), pB.rend());
  
  std::vector<std::vector<long>> max_P(n + 1, std::vector<long>(H + 1, -1));
  // for exactly nA potions of type A and happiness exactly h, max_P[nA][h] power can be achieved at most
  max_P[0][0] = 0;
  
  
  // std::vector<std::vector<std::vector<int>>> trace(n + 1, std::vector<std::vector<int>>(H + 1));
  
  for (int i = 0; i < n; ++i) {
    // std::cout << "potion A " << i << ": " << pA[i].p << " " << pA[i].h << std::endl;
    auto old_max_P = max_P;
    // auto old_trace = trace;
    
    for (int nA = 0; nA < n; ++nA) {
      for (long h = 0; h <= H; ++h) {
        if (old_max_P[nA][h] != -1) {
          long new_h = std::min(H, h + pA[i].h);
          long new_p = old_max_P[nA][h] + pA[i].p;

          if (new_p > max_P[nA + 1][new_h]) {
            max_P[nA + 1][new_h] = new_p;
            // trace[nA + 1][new_h] = old_trace[nA][h];
            // trace[nA + 1][new_h].push_back(i);
          }
        }
      }
    }
  }
  
  // for (int i = 0; i < n + 1; ++i) {
  //   for (int j = 0; j < H + 1; ++j) {
  //     std::cout << max_P[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
  
  int best = INT_MAX;
  
  for (int nA = 0; nA <= n; ++nA) {
    long p = max_P[nA][H];
    long w = nA * -a;
    // std::cout << "using " << nA << " type A potions, power " << p << " can be reached" << std::endl;
    // std::cout << "trace: ";
    // long trace_sum = 0;
    // for (int i: trace[nA][H]) {
    //   std::cout << i << " ";
      // trace_sum += pA[i].p;
    // }
    // std::cout << std::endl;
    // if (p != -1 && trace_sum != p) {
    //   std::cout << "WARNING: trace sum " << trace_sum << std::endl;
    // }
    if (p == -1 || p < P)
      continue;

    // calculate the number of B potions needed
    int nB = -1;
    for (int i = 0; i < m; ++i) {
      w += pB[i];
      p -= b;
      if (p < P)
        break;
      
      if (p >= P && w >= W) {
        // we know that w >= W and p >= P
        // all conditions met
        nB = i + 1;
        break;
      }
    }
    
    if (nB != -1) {
      best = std::min(best, nA + nB);
      break;
    }
  }
  
  if (best == INT_MAX) {
    std::cout << -1 << std::endl;
    return;
  }
  std::cout << best << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}
