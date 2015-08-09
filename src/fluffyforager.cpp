#include <iostream>
#include <array>
#include <vector>
#include "factorization.h"
using namespace std;

int main() {
  long to_p_factor;
  cout << "Enter a number to get its prime factors and divisors: ";
  cin >> to_p_factor;
  cout << endl << "Prime Multiplicity Index:" << endl;
  vector<array<long, 2> > p_factors = factorization::prime_factorize(to_p_factor);
  for (int i = 0; i < p_factors.size(); i++) {
    cout << p_factors[i][0] << ", " << p_factors[i][1] << endl;
  }
  cout << endl;
  vector<long> divisors = factorization::deduce_divisors(to_p_factor);
  cout << divisors.size() << " divisors: ";
  for (int i = 0; i < divisors.size(); i++) {
    cout << divisors[i];
    if (i != divisors.size() - 1) {
      cout << ", ";
    }
    else {
      cout << endl;
    }
  }
  return 0;
}
