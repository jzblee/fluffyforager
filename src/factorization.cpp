#include "factorization.h"

vector<array<long, 2> > factorization::prime_factorize(long factor_this) {
  long original = factor_this;
  vector<array<long, 2> > p_factors;
  long s_factor;
  while ((s_factor = factorization::smallest_factor(factor_this)) != 1) {
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

long factorization::smallest_factor(long operand) {
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

vector<long> factorization::deduce_divisors(long operand) {
  vector<long> divisors;
  vector<array<long, 2> > p_factors = factorization::prime_factorize(operand);
  vector<int> multiplicities, counter_state;
  for (int i = 0; i < p_factors.size(); i++) {
    multiplicities.push_back(p_factors[i][1]);
    counter_state.push_back(0);
  }
  int current_index = 0;
  divisors.push_back(1);
  while (counter_state != multiplicities) {
    if (counter_state[current_index] < multiplicities[current_index]) {
      /*
      * When a digit is successfully incremented, all digits to the
      * left of the incremented digit are set to zero. The current
      * index position is returned to the leftmost counter digit.
      */
      if (current_index > 0) {
        for (int i = 0; i < current_index; i++) {
          counter_state[i] = 0;
        }
      }
      counter_state[current_index]++;
      current_index = 0;
    }
    else {
      current_index++;
      /*
      * The current index is shifted to the right by one place if
      * the digit at the current index may not be incremented.
      * The incrementor acts again without storing a new divisor.
      */
      continue;
    }
    long new_divisor = 1;
    for (int i = 0; i < p_factors.size(); i++) {
      new_divisor *= pow(p_factors[i][0], counter_state[i]);
    }
    divisors.push_back(new_divisor);
  }
  bool keep_sorting = true;
  while (keep_sorting) {
    keep_sorting = false;
    for (int i = 0; i < divisors.size() - 1; i++) {
      if (divisors[i] > divisors[i + 1]) {
        long temp = divisors[i];
        divisors[i] = divisors[i + 1];
        divisors[i + 1] = temp;
        keep_sorting = true;
      }
    }
  }
  return divisors;
}
