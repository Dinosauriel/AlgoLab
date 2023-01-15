#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

void test_case(int n) {
  long x;
  long y;
  long a;
  long b;
  
  std::cin >> x;
  std::cin >> y;
  std::cin >> a;
  std::cin >> b;
  
  
  K::Point_2 phin_src(x, y), phin_targ(a, b);
  K::Ray_2 phineas(phin_src, phin_targ);
  
  std::vector<K::Segment_2> segments(n);
  
  for (int i = 0; i < n; ++i) {
    long r, s, t, u;
    std::cin >> r;
    std::cin >> s;
    std::cin >> t;
    std::cin >> u;
    
    K::Point_2 p_a(r, s), p_b(t, u);
    K::Segment_2 segment(p_a, p_b);
    
    segments[i] = segment;
  }
  
  for (int i = 0; i < n; ++i) {
    if (CGAL::do_intersect(phineas, segments[i])) {
      std::cout << "yes" << std::endl;
      return;
    }
  }
  
  std::cout << "no" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  while (true) {
    int n;
    std::cin >> n;
    
    if (n == 0) {
      break;
    }
    
    test_case(n);
  }
}