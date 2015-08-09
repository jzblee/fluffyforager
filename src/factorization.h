#include <array>
#include <cmath>
#include <vector>
using namespace std;

class factorization {
public:
  static vector<array<long, 2> > prime_factorize(long factor_this);
  static long smallest_factor(long operand);
  static vector<long> deduce_divisors(long operand);
};
