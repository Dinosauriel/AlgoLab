#include <iostream>
#include <vector>
#include <deque>
#include <climits>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int InputType;
typedef CGAL::Gmpz ExactType;
// typedef CGAL::Gmpq InputType;
// typedef CGAL::Gmpq ExactType;

void test_case() {
  int n, m;
  
  std::cin >> n;
  std::cin >> m;
  
  std::vector<int> score(n);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> score[i];
    // std::cout << conv_score[i] << " ";
  }

  // create linear program
  CGAL::Quadratic_program<InputType> lp(CGAL::SMALLER, true, 0, true, 1); 
  
  lp.set_c0(0);
  for (int i = 0; i < n; ++i)
    lp.set_c(i, -score[i]);

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u;
    std::cin >> v;
    // a_u - a_v <= 0
    lp.set_a(u, i, 1);
    lp.set_a(v, i, -1);
    lp.set_b(i, 0);
  }

  CGAL::Quadratic_program_solution<ExactType> solution = CGAL::solve_linear_program(lp, ExactType());

  if (-solution.objective_value() <= 0) {
    std::cout << "impossible" << std::endl;
    return;
  }

  double final_score = - solution.objective_value().numerator().to_double() / solution.objective_value().denominator().to_double();

  std::cout << (int) final_score << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}
