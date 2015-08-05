#include <array>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

long smallest_factor(long operand);
vector<array<long, 2> > prime_factorize(long factor_this);
vector<long> deduce_divisors(long operand_dd);

int main() {
  long to_p_factor;
  cin >> to_p_factor;
  vector<array<long, 2> > p_factors = prime_factorize(to_p_factor);
  for (int i = 0; i < p_factors.size(); i++) {
    cout << p_factors[i][0] << ", " << p_factors[i][1] << endl;
  }
}

vector<array<long, 2> > prime_factorize(long factor_this) {
  long original = factor_this;
  vector<array<long, 2> > p_factors;
  long s_factor;
  while ((s_factor = smallest_factor(factor_this)) != 1) {
    if (p_factors.size() > 0 &&
        p_factors[p_factors.size() - 1][0] == s_factor) {
      p_factors[p_factors.size() - 1][1]++;
    }
    else {
      array<long, 2> new_array;
      new_array[0] = s_factor;
      new_array[1] = 1;
      p_factors.push_back(new_array);
    }
    factor_this /= s_factor;
  }
  return p_factors;
}

long smallest_factor(long operand) {
  if (operand < 2) {
    return operand;
  }
  int fac;
  if (!(operand % (fac = 2)) || !(operand % (fac = 3)) || !(operand % (fac = 5))) {
    return fac;
  }
  for (long j = 7; j < sqrt(operand); j += 30) { //could set a "high score" mechanic? so earlier ranges don't need to be revisited
    if (!(operand % j)) return j;
    if (!(operand % j + 4)) return j + 4;
    if (!(operand % j + 6)) return j + 6;
    if (!(operand % j + 10)) return j + 10;
    if (!(operand % j + 12)) return j + 12;
    if (!(operand % j + 16)) return j + 16;
    if (!(operand % j + 22)) return j + 22;
    if (!(operand % j + 24)) return j + 24;
  }
  return operand;
}
