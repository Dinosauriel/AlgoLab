#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Delaunay_triangulation_2<K> Triang;

struct warehouse {
  K::Point_2 p;
  int s;
  int a;
};

struct stadium {
  K::Point_2 p;
  int d;
  int u;
};

bool circle_less_than(K::Circle_2 const& a, K::Circle_2 const& b) {
  return a.squared_radius() < b.squared_radius();  
};


void testcase() {
  auto cp0 = std::chrono::steady_clock::now();

  int n, m, c;
  std::cin >> n >> m >> c;
  
  // std::cout << "(" << n << ", " << m << ", " << c << ")" << std::endl;
  
  std::vector<struct warehouse> w(n);
  std::vector<struct stadium> s(m);
  std::vector<std::vector<int>> r(n, std::vector<int>(m));
  std::vector<K::Circle_2> l;
  
  std::vector<K::Point_2> pts(n + m);

  for (int i = 0; i < n; ++i) {
    int x, y, s, a;
    std::cin >> x >> y >> s >> a;
    
    w[i].p = K::Point_2(x, y);
    w[i].s = s;
    w[i].a = a;

    pts[i] = w[i].p;
    // std::cout << "w((" << w[i].p.x() << ", " << w[i].p.y() << "), " << w[i].s << ", " << w[i].a << ")" << std::endl; 
  }
  
  for (int i = 0; i < m; ++i) {
    int x, y, d, u;
    std::cin >> x >> y >> d >> u;
    
    s[i].p = K::Point_2(x, y);
    s[i].d = d;
    s[i].u = u;

    pts[n + i] = s[i].p;
  }
  
  for (int i = 0; i < n; ++i) {
   for (int j = 0; j < m; ++j) {
     int rev;
     std::cin >> rev;
     r[i][j] = rev;
   }
  }
  
  Triang triang;
  triang.insert(pts.begin(), pts.end());

  for (int i = 0; i < c; ++i) {
   int x, y, r;
   std::cin >> x >> y >> r;
   K::FT r_sq = (long) r * (long) r;
   K::Point_2 center = K::Point_2(x, y);
   
   auto pt = triang.nearest_vertex(center)->point();
   if (CGAL::squared_distance(center, pt) <= r_sq) {
    l.push_back(K::Circle_2(center, r_sq));
   } 
  }

  auto cp1 = std::chrono::steady_clock::now();

  std::vector<std::vector<int>> t(n, std::vector<int>(m, 0));
  for (size_t i = 0; i < l.size(); ++i) {
    for (int j = 0; j < n; ++j) {
      for (int k = 0; k < m; ++k) {
        if (l[i].has_on_bounded_side(w[j].p) != l[i].has_on_bounded_side(s[k].p)) {
          t[j][k] += 1;
        }
      }
    }
  }
  auto cp2 = std::chrono::steady_clock::now();
  
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  //maximize revenue -> minimize negative
  lp.set_c0(0);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      int a = (i * m) + j;
      
      float revenue = 100 * r[i][j] - t[i][j];
      // std::cout << "revenue: " << revenue << std::endl;
      lp.set_c(a, -revenue);
    }
  }
  
  //enforce stadium bounds
  for (int j = 0; j < m; ++j) {
    for (int i = 0; i < n; ++i) {
      int a = (i * m) + j;
      
      // upper bound
      lp.set_a(a, 3 * j, 1);
      lp.set_b(3 * j, s[j].d);
      
      // lower bound
      lp.set_a(a, 3 * j + 1, -1);
      lp.set_b(3 * j + 1, -s[j].d);
      
      // alcohol upper bound
      lp.set_a(a, 3 * j + 2, w[i].a);
      lp.set_b(3 * j + 2, 100 * s[j].u);
    }
  }
  
  int const_i = 3 * m;
  
  // enforce warehouse bounds
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      int a = (i * m) + j;
      lp.set_a(a, const_i + i, 1.);
      lp.set_b(const_i + i, w[i].s);
    }
  }
  
  // CGAL::print_linear_program(std::cout, lp, "first_lp");

  // solve the program, using ET as the exact type
  Solution sol = CGAL::solve_linear_program(lp, ET());
  // output solution
  // std::cout << sol;

  auto cp3 = std::chrono::steady_clock::now();

  // std::cout << "read input: " << std::chrono::duration_cast<std::chrono::milliseconds>(cp1 - cp0).count() << "ms" << std::endl;
  // std::cout << "circles:    " << std::chrono::duration_cast<std::chrono::milliseconds>(cp2 - cp1).count() << "ms" << std::endl;
  // std::cout << "solve lp:   " << std::chrono::duration_cast<std::chrono::milliseconds>(cp3 - cp2).count() << "ms" << std::endl;
  
  if (sol.is_infeasible()) {
    std::cout << "RIOT!" << std::endl;
    return;
  }
  
  auto val = sol.objective_value();
  
  std::cout << (long) std::floor(-(val.numerator().to_double() / val.denominator().to_double()) / 100.) << std::endl;
  // std::cout << val << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false); // Always!

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
}
