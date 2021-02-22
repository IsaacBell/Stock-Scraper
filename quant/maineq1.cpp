/*
$ /usr/bin/clang++ -std=c++17 -stdlib=libc++ -lcurl maineq1.cpp Random.cpp -o outfile
*/

#include "Random.h"
#include <iostream>
#include <cmath>
#include <algorithm>

int main() {
  std::cout << "Monte Carlo European Call\n\n";

  double T  = 1;       // maturity
  double K  = 100;     // strike
  double S0 = 100;     // spot
  double r  = 0.05;    // interest rate
  double sigma = 0.10; // volatility
  
  int N = 500;   // # steps
  int M = 10000; // # simulations

  double S[N+1];
  double sum_payoff = 0;
  double premium = 0;
  double dt = T / N;

  // Main Simulation Loop
  for (int j = 0; j < M; j++)
  {
    S[0] = S0;

    // Time Integration Loop
    for (int i = 0; i < N; i++)
    {
      double epsilon = SampleBoxMuller(); // get Gaussian draw
      S[i+1] = S[i] * (1 + r*dt + sigma*std::sqrt(dt)*epsilon);
    }

    // Compute Payoff
    sum_payoff += std::max(S[N] - K, 0.0); // compute and add payoff
  }

  // Compute discounted expected payoff
  premium = std::exp(-r*T)*(sum_payoff / M);

  std::cout << "Premium: " << premium << "\n";

  return 0;
}