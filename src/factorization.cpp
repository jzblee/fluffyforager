#include "factorization.h"

vector<array<long, 2> > factorization::prime_factorize(long factor_this) {
  vector<array<long, 2> > p_factors;
  long s_factor;
  long high_score = 7;
  while ((s_factor = factorization::smallest_factor(factor_this, high_score)) != 1) {
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

long factorization::smallest_factor(long operand, long & high_score) {
  if (operand < 2) {
    return operand;
  }
  int fac;
  if (!(operand % (fac = 2)) || !(operand % (fac = 3)) || !(operand % (fac = 5))) {
    return fac;
  }
  while (high_score < sqrt(operand)) {
    /* "High score" mechanic ensures that earlier ranges are not revisited */
    if (!(operand % high_score)) return high_score;
    if (!(operand % (high_score + 4))) return high_score + 4;
    if (!(operand % (high_score + 6))) return high_score + 6;
    if (!(operand % (high_score + 10))) return high_score + 10;
    if (!(operand % (high_score + 12))) return high_score + 12;
    if (!(operand % (high_score + 16))) return high_score + 16;
    if (!(operand % (high_score + 22))) return high_score + 22;
    if (!(operand % (high_score + 24))) return high_score + 24;
    high_score += 30;
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
  divisors.push_back(1); // 1 is a divisor of every integer
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
