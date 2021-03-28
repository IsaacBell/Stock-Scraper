#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include "Utils.h"

// Read a market history file and check trends & volatility over time

// Reveal market anomalies over time

// Trading systems developed in certain market conditions (up/down trends, hi/lo volatility)
// often lose profitability in other conditions

// We want to see these conditions vary on a regular and random basis

// Slow wandering leads to dangerous non-stationarity; markets may stay in one state a long time then suddenly change overnight, impacting our indicators


constexpr std::vector<float> TEST_CLOSES {131.1, 131.92, 133.71, 136.86};

constexpr int N_GAPS {11}; // # gaps in analysis
// ConstArray<n_gaps, [](){for (int i = 1; i < arr; i++) arr[i] = arr[i-1]*2;}> gap_size;
constexpr std::vector<int> GAP_SIZES {1,2,4,8,16,32,64,128,256,512,std::numeric_limits<int>::max()}; // INT_MAX => over 512

void gap_analyze(
  int n, // #indicators
  double *x,
  double thresh,
  int n_gaps,
  std::vector<int> *gap_size,
  int *gap_count
) {
  int i, j, above_below, new_above_below, count;

  for (int i = 0; i < n_gaps; i++)
    gap_count[i] = 0;
  count = 1;
  above_below = (x[0] >= thresh) ? 1 : 0;

  for (int i = 1; i <= n; i++)
  {
    if (i == n) // Passing end of array counts as a change
      new_above_below = 1-above_below;
    else
      new_above_below = (x[i] >= thresh) ? 1 : 0;

    if (new_above_below == above_below)
      ++count;
    else {
      for (int j = 0; j < n_gaps-1; j++) {
        if (count <= gap_size[j]) break;
        ++gap_count[j];
        count = 1;
        above_below = new_above_below;
      }
    }
  }
}

template<typename T=float>
T find_slope(T x[2], T y[2])
{
  return (T) ( (y[1]-y[0]) / (x[1] - x[0]));
}


// ./statn [lookback fractile version filename]

// ./statn 100 0.5 0 file.csv

int main(int argc, char* argv[])
{
  if (argc < 5)
  {
    std::cout << "Not enough arguments provided.\n\nPlease use format "<<argv[0]<<" [lookback] [fractile] [version] [filename]";
    return 1;
  }

  int full_lookback {0};
  int n_prices {0};

  int lookback = std::stoi(argv[1]);  
  float fractile = std::stof(argv[2]);
  int version = std::stoi(argv[3]);

  // Read file here

  if (version == 0)
    full_lookback = lookback;
  else if (version == 1)
    full_lookback = lookback * 2;
  else if (version > 1)
    full_lookback = version * lookback;

  const int n_indicators = n_prices - full_lookback + 1;

  // Compute indicators for trend
  // For each pass, k is the index of the current value of the indicator
  std::vector<double> trend(n_indicators);
  std::vector<double> trend_sorted(n_indicators);

  for (int i = 0; i < n_indicators; i++)
  {
    int k = full_lookback - 1 + i;
    if (version == 0)
      trend[i] = find_slope(lookback, close + k);
    else if (version == 1)
      trend[i] = find_slope(lookback, close + k) - find_slope(lookback, close + k - lookback);
    else
      trend[i] = find_slope(lookback, close + k) - find_slope(full_lookback, close + k);

    trend_sorted[i] = trend[i];
  }

  // Sort vals to find user-specified quantile and then tally counts in each bin
  // Note - C's qsort not as performant as C++'s std::sort
  qsort(0, n_indicators-1, trend_sorted);

  int k = (int) (fractile * n_indicators+1)) - 1;
  if (k < 0) k = 0;
  double trend_quantile = trend_sorted[k];

  gap_analyze(n_indicators, trend, trend_quantile, N_GAPS, GAP_SIZES, GAP_SIZES.size());


  return 0;
}