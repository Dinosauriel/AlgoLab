#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <cfloat>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// typedef int InputType;
// typedef CGAL::Gmpz ExactType;
typedef CGAL::Gmpq InputType;
typedef CGAL::Gmpq ExactType;


struct coords {
  int x;
  int y;
};

double max_a(struct coords const& p1, struct coords const& p2, int h, int w) {
  int delta_x = std::abs(p1.x - p2.x);
  int delta_y = std::abs(p1.y - p2.y);
  
  double r = ((double) delta_y) / ((double) delta_x);
  
  if (r > ((double) h) / ((double) w)) {
    // p1 is "above" or "below" p2
    return ((double) delta_y) / ((double) h);
  }
  // p1 is "right" or "left" of p2
  return ((double) delta_x) / ((double) w);
}

void test_case() {
  int n;
  int m;
  int h;
  int w;
  
  std::cin >> n;
  std::cin >> m;
  std::cin >> h;
  std::cin >> w;
  
  std::vector<struct coords> Free(n);
  std::vector<struct coords> Occupied(m);

  for (int i = 0; i < n; ++i) {
    std::cin >> Free[i].x;
    std::cin >> Free[i].y;
  }
  
  for (int i = 0; i < m; ++i) {
    std::cin >> Occupied[i].x;
    std::cin >> Occupied[i].y;
  }
  
  // create linear program
  CGAL::Quadratic_program<InputType> lp (CGAL::SMALLER, true, 1, false, 0); 
  
  // set objective function
  // maximize a_0 + ... + a_(n-1)
  lp.set_c0(0);
  for (int i = 0; i < n; ++i) {
    lp.set_c(i, -1.);
  }
  
  int constraint_i = 0;
  // set constraints for all n x n
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      double a = max_a(Free[i], Free[j], h, w);
      // a_i + a_j <= 2 * a
      lp.set_a(i, constraint_i, 1);
      lp.set_a(j, constraint_i, 1);
      lp.set_b(constraint_i, 2 * a);
      ++constraint_i;
    }
  }

  if (m > 0) {
    // set constraints for some n x m
    for (int i = 0; i < n; ++i) {
      double a = DBL_MAX;

      for (int j = 0; j < m; ++j) {
        a = std::min(a, max_a(Free[i], Occupied[j], h, w));
      }
      
      // a_i <= 2a - 1
      lp.set_a(i, constraint_i, 1);
      lp.set_b(constraint_i, (2 * a) - 1);
      ++constraint_i;
    }
  }
  
  CGAL::Quadratic_program_solution<ExactType> solution = CGAL::solve_linear_program(lp, ExactType());
  // std::cout << solution;
  
  double perimeter = 0;
  for (auto opt = solution.variable_values_begin(); opt < solution.variable_values_end(); ++opt) {
    CGAL::Quotient<ExactType> res = *opt;
    // std::cout << res.numerator() << "/" << res.denominator() << std::endl;
    double a = res.numerator().to_double() / res.denominator().to_double();
    // std::cout << a << ", ";
    perimeter += (double) (a * 2 * (w + h));
  }
  
  std::cout << (long) std::ceil(perimeter) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}