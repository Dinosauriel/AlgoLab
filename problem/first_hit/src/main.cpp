//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>

//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;

double floor_to_double(const EK::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void test_case(int n) {
  long x;
  long y;
  long a;
  long b;
  
  std::cin >> x;
  std::cin >> y;
  std::cin >> a;
  std::cin >> b;
  
  
  EK::Point_2 phin_src(x, y), phin_targ(a, b);
  EK::Ray_2 phineas(phin_src, phin_targ);
  
  std::vector<EK::Segment_2> segments(n);
  
  for (int i = 0; i < n; ++i) {
    long r, s, t, u;
    std::cin >> r;
    std::cin >> s;
    std::cin >> t;
    std::cin >> u;
    
    EK::Point_2 p_a(r, s), p_b(t, u);
    EK::Segment_2 segment(p_a, p_b);
    
    segments[i] = segment;
  }
  
  EK::FT best_distance = -1.;
  EK::Point_2 best_inter(0, 0);
  
  for (int i = 0; i < n; ++i) {
    if (CGAL::do_intersect(phineas, segments[i])) {
      auto intersection = CGAL::intersection(phineas, segments[i]);
      EK::Point_2* inter_point = boost::get<EK::Point_2>(&*intersection);

      if (inter_point == NULL) {
        //intersection is a segment
        EK::Segment_2* inter_seg = boost::get<EK::Segment_2>(&*intersection);
        assert(inter_seg != NULL);
        EK::FT dist_src = CGAL::squared_distance(inter_seg->source(), phin_src);
        EK::FT dist_targ = CGAL::squared_distance(inter_seg->target(), phin_src);
        
        if (dist_src < best_distance || best_distance == -1.) {
          best_inter = inter_seg->source();
          best_distance = dist_src;
        }
        
        if (dist_targ < best_distance || best_distance == -1.) {
          best_inter = inter_seg->target();
          best_distance = dist_targ;
        }
      } else {
        EK::FT dist = CGAL::squared_distance(*inter_point, phin_src);
        if (dist < best_distance || best_distance == -1.) {
          best_inter = *inter_point;
          best_distance = dist;
        }
      }
    }
  }

  if (best_distance == -1.) {
    std::cout << "no" << std::endl;
    return;
  }
  
  std::cout << (long) floor_to_double(best_inter.x()) << " " << (long) floor_to_double(best_inter.y()) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n;
  std::cin >> n;
  while (n != 0) {
    test_case(n);
    std::cin >> n;
  }
}