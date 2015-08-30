#include <array>
#include <cmath>
#include <vector>

class factorization {
public:
  static std::vector<std::array<unsigned long, 2> > prime_factorize(unsigned long factor_this);
  static std::vector<unsigned long> deduce_divisors(unsigned long operand);
private:
  static unsigned long smallest_factor(unsigned long operand, unsigned long & high_score);
};
