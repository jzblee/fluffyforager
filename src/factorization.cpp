#include "factorization.h"

std::vector<std::array<unsigned long, 2> > factorization::prime_factorize(unsigned long factor_this) {
  std::vector<std::array<unsigned long, 2> > p_factors;
  unsigned long s_factor;
  unsigned long high_score = 7;
  while ((s_factor = factorization::smallest_factor(factor_this, high_score)) != 1) {
    if (p_factors.size() > 0 &&
        p_factors[p_factors.size() - 1][0] == s_factor) {
      p_factors[p_factors.size() - 1][1]++;
    }
    else {
      std::array<unsigned long, 2> new_array;
      new_array[0] = s_factor;
      new_array[1] = 1;
      p_factors.push_back(new_array);
    }
    factor_this /= s_factor;
  }
  return p_factors;
}

const int hs_offsets[8] = {0, 4, 6, 10, 12, 16, 22, 24}; // Sieve of Eratosthenes

unsigned long factorization::smallest_factor(unsigned long operand, unsigned long & high_score) {
  if (operand < 2) {
    return operand;
  }
  int fac;
  if (high_score == 7
    && (!(operand % (fac = 2)) || !(operand % (fac = 3)) || !(operand % (fac = 5)))) {
    return fac;
  }
  while (high_score <= sqrt(operand)) {
    /* "High score" mechanic ensures that earlier ranges are not revisited */
    for (int i = 0; i < 8; i++) {
      if (!(operand % (high_score + hs_offsets[i]))) return high_score + hs_offsets[i];
    }
    high_score += 30;
  }
  return operand;
}

std::vector<unsigned long> factorization::deduce_divisors(unsigned long operand) {
  std::vector<unsigned long> divisors;
  std::vector<std::array<unsigned long, 2> > p_factors = factorization::prime_factorize(operand);
  std::vector<int> multiplicities, counter_state;
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
    unsigned long new_divisor = 1;
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
        unsigned long temp = divisors[i];
        divisors[i] = divisors[i + 1];
        divisors[i + 1] = temp;
        keep_sorting = true;
      }
    }
  }
  return divisors;
}
