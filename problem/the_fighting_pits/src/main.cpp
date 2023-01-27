#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>

int fighters_mask;

struct fighters {
  int f1;
  int f2;
  int f3;

  struct fighters insert(int f) {
    return {f, f1, f2};
  }
  
  int unique_size(int m) const {
    if (m == 2) {
      std::set<int> u = {0, f1, f2};
      return u.size() - 1;
    }
    std::set<int> u = {0, f1, f2, f3};
    return u.size() - 1;
  }

  // bool operator< (struct fighters const& b) const {
  //   if (f1 != b.f1) {
  //     return f1 < b.f1;
  //   }
  //   if (f2 != b.f2) {
  //     return f2 < b.f2;
  //   }
  //   if (f3 != b.f3) {
  //     return f3 < b.f3;
  //   }
    
  //   return false;
  // }
  
  // bool operator== (struct fighters const& b) const {
  //   return f1 == b.f1 && f2 == b.f2 && f3 == b.f3;
  // }
  
  int enc() const {
    // std::cout << "enc(" << f1 << ", " << f2 << ", " << f3 << ")" << std::endl;
    return (f3 << 6) + (f2 << 3) + f1;
  }
  
};

struct fighters dec_f(int enc) {
  int f1 = enc & 0b111;
  int f2 = (enc >> 3) & 0b111;
  int f3 = (enc >> 6) & 0b111;
  
  // std::cout << "dec(" << f1 << ", " << f2 << ", " << f3 << ")" << std::endl;
  return {f1, f2, f3};
}

struct state {
  int p; // p fighters have entered through north
  struct fighters n; // the last 3 fighters that have entered through n
  struct fighters s; // the last 3 fighters that have entered through s
  
  // bool operator< (struct state const& b) const {
  //   if (p != b.p)
  //     return p < b.p;

  //   if (!(n == b.n))
  //     return n < b.n;

  //   if (!(s == b.s))
  //     return s < b.s;
    
  //   return false;
  // }
  
  // bool operator== (struct state const& b) const {
  //   return p == b.p && n == b.n && s == b.s;
  // }
  
  int excitement(int m, bool north) const {
    int size;
    if (north)
      size = n.unique_size(m);
    else
      size = s.unique_size(m);

    return size * 1000 - int(std::pow(2, std::abs(p)));
  }
  
  int enc() const {
    return (p << 18) + ((n.enc() & fighters_mask) << 9) + (s.enc() & fighters_mask);
  }
};

struct state dec(int enc) {
  struct fighters s = dec_f(enc & fighters_mask);
  struct fighters n = dec_f((enc >> 9) & fighters_mask);
  int p = (enc >> 18);
  return {p, n, s};
}


void testcase() {
  int n, k, m;
  std::cin >> n >> k >> m;
  
  int b = 3 * (m - 1); // Number of bits needed to encode a queue
  fighters_mask = (1 << b) - 1;
  
  std::vector<int> queue(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> queue[i];
    queue[i] += 1;
  }
  
  std::map<int, int> round;
  
  struct state start = {
    .p = 0,
    .n = {0, 0, 0},
    .s = {0, 0, 0},
  };
  
  round[start.enc()] = 0;
  
  // std::cout << "table created" << std::endl;
  
  for (int f: queue) {
    std::map<int, int> next_round;

    // std::cout << "round " << a << std::endl;
    
    for (auto prev: round) {
      
      // std::cout << "prev: " << prev.second << std::endl;
      
      int prev_score = prev.second;
      struct state prev_st = dec(prev.first);
      
      struct state label_n = {
        .p = prev_st.p - 1,
        .n = prev_st.n.insert(f),
        .s = prev_st.s,
      };

      struct state label_s = {
        .p = prev_st.p + 1,
        .n = prev_st.n,
        .s = prev_st.s.insert(f),
      };

      int score_n = label_n.excitement(m, true);
      int score_s = label_s.excitement(m, false);

      // std::cout << "score_n  " << score_n << ", score_s " << score_s << std::endl;
      
      if (score_n >= 0) {
        int enc = label_n.enc();
        next_round[enc] = std::max(next_round[enc], prev_score + score_n);
      }
      if (score_s >= 0) {
        int enc = label_s.enc();
        next_round[enc] = std::max(next_round[enc], prev_score + score_s); 
      }
    }
    
    round = next_round;
  }
  
  int best = 0;
  for (auto st: round) {
    best = std::max(best, st.second);
  }
  
  std::cout << best << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}
