#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <cfloat>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int InputType;
typedef CGAL::Gmpz ExactType;

void test_case() {
  int n;
  
  std::cin >> n;
  
  std::vector<int> Required(n);
  std::vector<int> K(n);
  std::vector<int> L(n);
  std::vector<int> M(n);

  for (int i = 0; i < n; ++i) {
    int f, c, k, l, m;
    std::cin >> f;
    std::cin >> c;
    std::cin >> k;
    std::cin >> l;
    std::cin >> m;
    
    Required[i] = f - c;
    K[i] = k;
    L[i] = l;
    M[i] = m;
  }
  
  
  std::vector<int> A(25, 0);
  std::vector<int> P(25, 0);
  
  for (int i = 1; i < 25; ++i) {
    int a;
    std::cin >> a;
    A[i] = A[i - 1] + a;
  }

  for (int i = 1; i < 25; ++i) {
    int p;
    std::cin >> p;
    P[i] = P[i - 1] + p;
  }
  
  int best_solution = INT_MAX;
  
  for (int w_a = 24; w_a >= 0; --w_a) {
    int w_p;
    for (w_p = 24; w_p >= 0; --w_p) {
      if (w_a + w_p >= best_solution) {
        continue;
      }
      
      // std::cout << "(" << w_a << ", " << w_p << ")" << std::endl;
      // create linear program
      CGAL::Quadratic_program<InputType> lp (CGAL::LARGER, true, 0, true, 1);
      // set objective function
      // dont optimize
      lp.set_c0(0);
      lp.set_c(0, 0);
      lp.set_c(1, 0);
      lp.set_c(2, 0);

      for (int i = 0; i < n; ++i) {
        int k = K[i] + (A[w_a] * A[w_a]);
        int l = L[i] + (P[w_p] * P[w_p]);
        int m = M[i] + (A[w_a] * P[w_p]);
        
        //h1
        lp.set_a(0, i, k);
        //h2
        lp.set_a(1, i, l);
        //h3
        lp.set_a(2, i, m);
        lp.set_b(i, Required[i]);
      }
      
      CGAL::Quadratic_program_solution<ExactType> solution = CGAL::solve_linear_program(lp, ExactType());
      // std::cout << solution;
      
      if (solution.is_infeasible()) {
        // too little work w_p, break
        break;
      }
      // we have a solution
      best_solution = std::min(best_solution, w_a + w_p);
    }
    if (w_p == 24) {
      break;
    }
  }
  
  if (best_solution == INT_MAX) {
    std::cout << "Impossible!" << std::endl;
    return;
  }
  
  std::cout << best_solution << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    test_case();
  }
}