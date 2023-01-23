#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/number_utils.h>
#include <iostream>
#include <vector>
#include <algorithm>

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

bool less_r(struct biker const& a, struct biker const& b) {
  if (a.r == b.r) {
    if (a.r >= 0)
      return a.y0 < b.y0;
    return a.y0 > b.y0;
  }
  
  if (CGAL::abs(a.r) == CGAL::abs(b.r)) {
    return a.r > 0;
  }
  
  return CGAL::abs(a.r) < CGAL::abs(b.r);
}

void test_case() {
  int n;
  std::cin >> n;

  std::vector<struct biker> B(n);

  for (int i = 0; i < n; ++i) {
    long y0, x1, y1;
    std::cin >> y0 >> x1 >> y1;
    B[i].y0 = y0;
    B[i].r = r(B[i], y1, x1);
    B[i].i = i;
  }

  std::sort(B.begin(), B.end(), less_r);
  // for (int i = 0; i < n; ++i) {
  //   std::cout << B[i].r << ", ";
  // }
  // std::cout << std::endl;

  std::vector<struct biker> Winners;
  Winners.reserve(n);

  long y_low = B[0].y0;
  long y_up = B[0].y0;
  for (int i = 0; i < n; ++i) {
    if (B[i].r >= 0) {
      if (B[i].y0 >= y_up)
        Winners.push_back(B[i]);
    }
    
    if (B[i].r <= 0) {
      if (B[i].y0 <= y_low)
        Winners.push_back(B[i]);
    }

    y_low = std::min(y_low, B[i].y0);
    y_up = std::max(y_up, B[i].y0);
  }
  
  std::sort(Winners.begin(), Winners.end(), less_i);

  for (struct biker const& b: Winners)
    std::cout << b.i << " ";
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
