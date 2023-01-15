#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <tuple>

long value(long P, long H, long W, long p, long h, long w) {
  return std::min(P, p) + std::min(H, h) + std::min(W, w);
}


void test_case() {
  int n, m;
  std::cin >> n;
  std::cin >> m;
  
  long a, b;
  std::cin >> a;
  std::cin >> b;
  
  long P, H, W;
  std::cin >> P;
  std::cin >> H;
  std::cin >> W;
  
  std::vector<std::tuple<long, long, long>> Potions(n + m);
  std::vector<bool> Available(n + m, true);

  
  for (int i = 0; i < n; ++i) {
    long p_i;
    long h_i;
    std::cin >> p_i;
    std::cin >> h_i;
    Potions[i] = std::make_tuple(p_i, h_i, -a);
  }
  
  for (int i = n; i < n + m; ++i) {
    long w_i;
    std::cin >> w_i;
    Potions[i] = std::make_tuple(-b, 0, w_i);
  }

  int res = 0;
  
  long p = 0;
  long h = 0;
  long w = 0;
  while (p < P || h < H || w < W) {
    
    bool empty = std::none_of(Available.begin(), Available.end(), [](bool b){ return b; });
    if (empty) {
      res = -1;
      break;
    }
    
    // always take the potion that is most valuable to me
    long best_value = LONG_MIN;
    int mvp = -1;
    //look for best potion
    for (int i = 0; i < n + m; ++i) {
      if (!Available[i]) {
        //potion no longer available
        // std::cout << "potion " << i << " no longer available" << std::endl;
        continue;
      }

      long pot_p = std::get<0>(Potions[i]);
      long pot_h = std::get<1>(Potions[i]);
      long pot_w = std::get<2>(Potions[i]);
      long value_i = value(P, H, W, p + pot_p, h + pot_h, w + pot_w)
            - value(P, H, W, p, h, w);
      
      if (value_i > best_value) {
        best_value = value_i;
        mvp = i;
      }
    }
    // take potion mvp
    // std::cout << "potion " << mvp << " is mvp with value " << best_value << std::endl;
    
    p += std::get<0>(Potions[mvp]);
    h += std::get<1>(Potions[mvp]);
    w += std::get<2>(Potions[mvp]);
    Available[mvp] = false;
    
    ++res;
  }
  
  std::cout << res << std::endl; 
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}