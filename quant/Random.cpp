// Computes Gaussian deviates w/ the Box-Muller method

#include "Random.h"
#include <cstdlib>
#include <cmath>

double SampleBoxMuller()
{
  double result;
  double x;
  double y;
  double xy_square;

  do {
    x = 2.0 * std::rand() / static_cast<double>(RAND_MAX) - 1;
    y = 2.0 * std::rand() / static_cast<double>(RAND_MAX) - 1;
    xy_square = x*x + y*y;
  } while (xy_square >= 1.0);
  
  result = x*sqrt(-2*log(xy_square)/xy_square);
  return result;
}