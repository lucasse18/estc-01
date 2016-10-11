#include "bernoulli.h"

__attribute__((always_inline))
inline int get_bernoulli_value(const double *p) {
  if(drand48() >= (*p))
    return 0;
  return 1;
}
