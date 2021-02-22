#include "Random.h"
#include <iostream>
#include <cmath>
#include <algorithm>

int main()
{
  std::cout << "Monte Carlo Equity Basket\n\n";

  double T   = 1;       // maturity
  double K   = 100;     // strike
  double S10 = 120;     // spot equity 1
  double S20 = 100;     // spot equity 2
  double r   = 0.05;    // interest rate
  double rho = 0.5;     // correlation
  double sigma1 = 0.10; // volatility
  double sigma2 = 0.15; // volatility
  
  int N = 300;   // # steps
  int M = 10000; // # simulations

  double S1[N+1];
  double S2[N+1];
  double sum_payoff = 0;
  double premium = 0;
  double dt = T / N;

  for (int j = 0; j < M; j++)
  {
    S1[0] = S10;
    S2[0] = S20;

    for (int i = 0; i < N; i++)
    {
      double epsilon1 = SampleBoxMuller();
      double epsilon2 = SampleBoxMuller();
      S1[i+1] = S1[i] * (1 + r*dt + sigma1*std::sqrt(dt)*epsilon1);
      epsilon2 = epsilon1*rho + std::sqrt(1 - rho*rho)*epsilon2;
      S1[i+1] = S1[i] * (1 + r*dt + sigma1*std::sqrt(dt)*epsilon1);
      S2[i+1] = S2[i] * (1 + r*dt + sigma2*std::sqrt(dt)*epsilon2);
    }

    sum_payoff += std::max(S1[N], S2[N]);
  }

  // discounted expected payoff
  premium = std::exp(-r*T)*(sum_payoff / M);

  std::cout << "Premium: " << premium << "\n";

  return 0;
}