#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/number_utils.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <cfloat>
#include <set>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

struct biker {
  int i;
  long y0;
  K::FT r;
  
};
bool operator <(struct biker const& a, struct biker const& b) {
  return a.i < b.i;
}

K::FT r(struct biker const& a, long y1, long x1) {
  K::Direction_2 dir(x1, y1 - a.y0);
  return dir.dy() / dir.dx();
}

bool less_i(struct biker const& a, struct biker const& b) {
  return a.i < b.i;
}

bool less_y0(struct biker const& a, struct biker const& b) {
  return a.y0 < b.y0;
}

void test_case() {
  int n;
  std::cin >> n;

  std::vector<struct biker> B(n);
  
  for (int i = 0; i < n; ++i) {
    long y0;
    long x1;
    long y1;
    std::cin >> y0;
    std::cin >> x1;
    std::cin >> y1;
    B[i].y0 = y0;
    B[i].r = r(B[i], y1, x1);
    B[i].i = i;
  }
  
  std::sort(B.rbegin(), B.rend(), &less_y0);
  
  int lowest_r = 0;
  int highest_r = 0;
  
  for (int i = 0; i < n; ++i) {
    if (CGAL::abs(B[i].r) <= CGAL::abs(B[lowest_r].r))
      lowest_r = i;
    if (CGAL::abs(B[i].r) >= CGAL::abs(B[highest_r].r))
      highest_r = i;
  }

  K::FT abs_upper = B[highest_r].r;
  K::FT range_lower = B[lowest_r].r;
  
  std::set<struct biker> Winners;

  for (int i = 0; i < lowest_r; ++i) {
    abs_upper = CGAL::min(abs_upper, CGAL::abs(B[i].r));
    if (B[i].r >= range_lower) {
      if (CGAL::abs(B[i].r) <= abs_upper)
        Winners.insert(B[i]);
    }
  }
  
  abs_upper = B[highest_r].r;
  
  for (int i = n - 1; i >= lowest_r; --i) {
    abs_upper = CGAL::min(abs_upper, CGAL::abs(B[i].r));
    if (B[i].r <= range_lower) {
      if (CGAL::abs(B[i].r) <= abs_upper)
        Winners.insert(B[i]);
    }
  }

  for (struct biker const& b: Winners) {
    std::cout << b.i << " ";
  }

  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}