#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <cfloat>
#include <tuple>

struct move {
  long d;
  long t;
};

bool move_less(struct emp_move const& a, struct emp_move const& b) {
  return a.r < b.r;
}

std::vector<std::tuple<long, long, long>> perm_list(std::vector<struct move> Mov, int n) {
  std::vector<std::tuple<long, long, long>> list(1 << n);
  
  for (int combo = (1 << n) - 1; combo >= 0; --combo) {
    long d = 0;
    long t = 0;
    long k = 0;
    for (int i = 0; i < n; ++i) {
      int flag = ((combo >> i) & 1);
      d += flag * Mov[i].d;
      t += flag * Mov[i].t;
      k += flag;
    }
    list[combo] = std::make_tuple(d, t, k);
  }
  return list;
}

std::pair<long, long> evaluate_sips(std::vector<std::tuple<long, long, long>> list_A,
                                    std::vector<std::tuple<long, long, long>> list_B,
                                    std::vector<long> S, long D, long T, int sips) {
  for (unsigned long i = 0; i < list_A.size(); ++i) {
    long d_A = std::get<0>(list_A[i]);
    long t_A = std::get<1>(list_A[i]);
    long k_A = std::get<2>(list_A[i]);
    
    for (unsigned long j = 0; j < list_B.size(); ++j) {
      long d_B = std::get<0>(list_B[j]);
      long t_B = std::get<1>(list_B[j]);
      long k_B = std::get<2>(list_B[j]);
      
      long d = d_A + d_B + ((k_A + k_B) * S[sips]);
      long t = t_A + t_B;
      
      if (d >= D && t < T) {
        return std::make_pair(d, t);
      }
    }
  }

  return std::make_pair(-1, LONG_MAX);
}

void test_case() {
  int n, m;
  long D, T;
  std::cin >> n;
  std::cin >> m;
  std::cin >> D;
  std::cin >> T;

  // std::cout << "n = " << n << ", m = " << m << ", D = " << D << ", T = " << T << std::endl;
  
  // split and list
  int n_A = n / 2;
  int n_B = n - n_A;
  std::vector<struct move> Mov_A(n_A);
  std::vector<struct move> Mov_B(n_B);
  
  for (int i = 0; i < n_A; ++i) {
    std::cin >> Mov_A[i].d;
    std::cin >> Mov_A[i].t;
  }

  for (int i = 0; i < n_B; ++i) {
    std::cin >> Mov_B[i].d;
    std::cin >> Mov_B[i].t;
  }
  
  std::vector<long> S(m + 1, 0);
  
  for (int i = 0; i < m; ++i) {
    std::cin >> S[i + 1];
  }

  auto list_A = perm_list(Mov_A, n_A);
  auto list_B = perm_list(Mov_B, n_B);
  
  // the number of sips lies somewhere in [sips_lower, sips_upper]
  int sips = m / 2;
  int sips_lower = 0;
  int sips_upper = m;
  
  //perform binary search on the number of sips required
  while (sips_lower < sips_upper) {
    auto pair_d_t = evaluate_sips(list_A, list_B, S, D, T, sips);
    long d = std::get<0>(pair_d_t);
    long t = std::get<1>(pair_d_t);
    
    if (t >= T || d < D) {
      // we couldn't reach Panoramix with this number of sips
      // increase sips
      // std::cout << "we couldn't reach Panoramix with " << sips << " sips" << std::endl;
      sips_lower = sips + 1;
      sips = (sips_lower + sips_upper) / 2;
    } else {
      // we reached Panoramix in time.
      // see if we can use less sips
      // std::cout << "we reached Panoramix in time with " << sips << " sips" << std::endl;
      sips_upper = sips;
      sips = (sips_lower + sips_upper) / 2;
    }
  }
  // std::cout << "sips = " << sips << " is in [" << sips_lower << ", " << sips_upper << "]" << std::endl;
  
  
  auto pair_d_t = evaluate_sips(list_A, list_B, S, D, T, sips_lower);
  long d = std::get<0>(pair_d_t);
  long t = std::get<1>(pair_d_t);
  
  // std::cout << "d = " << d << ", t = " << t << std::endl;
  
  if (t >= T || d < D) {
    std::cout << "Panoramix captured" << std::endl;
    return;
  }
  
  std::cout << sips_lower << std::endl; 
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}