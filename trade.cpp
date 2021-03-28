#include "Utils.h"
#include "Strategy.h"

// Pseudo-code
auto cointegration() {
  // Dynamic equilibrium if one price can be expressed
  // as a linear function of another in a manner that
  // indicates stationarity, as per ADF test

  double google[] = google daily price quotes;
  double microsoft[] = microsoft daily price quotes;

  double slope = get_slope(google[], microsoft[]);
  double intercept = avg(google[] - (slope*avg(microsoft[]) ));
  
  // how far does google's price deviate from the "fair value" as per this relationship, per day?
  double[] cointegration;
  for (auto day : microsoft[]) cointegration[i] = intercept * slope * microsoft[i];

  // A series we want to be stationary. This series is the diff between
  // actual price of Google and its dynamic equilibrium. This should show
  // how reliable pair trading with this pair would be
  double[] divergences;
  for (auto day : google[]) {
    divergences[i] = cointegration[i] - google[i];

    // if (divergences[i] > 0) then 
    // the price of google is undervalued relative to msoft
    // Long Goog and short MSOFT

    // if (divergences[i] < 0) then the opposite
    // Long MSOFT and short GOOG
  }

  for (int i = 1; i < array_size; i++) {
    double dx = divergences[i] - divergences[i-1]; // delta x
    double lagged_x = divergences[i-1]; // previous value of x
  }

  // DF Test
  // We're hoping for significantly negative result
  double coefficient = 0, std_error = 0;
  
  // Regress delta x onto lagged x
  // https://www.ablebits.com/office-addins-blog/2018/07/25/excel-linest-function-formula-examples/#simple-linear-regression-slope-intercept
  // https://stackoverflow.com/questions/18939869/how-to-get-the-slope-of-a-linear-regression-line-using-c
  [coefficient, std_error] = linest(dx, lagged_x);

  // todo - what range are we looking for as a green light?
  double t_stat = coefficient / std_error;

  // Dry-running the test: long the under, short the over
  // See above comments on when divergences is pos/neg
  double[] returns {0};
  for (int i = 1; i < array.size(); i++) {
    returns[i] = divergences[i] > 0 ? google[i] - microsoft[i] : microsoft[i] - google[i];
  }

  double[] portfolio_val {100};
  for (int i = 1; i < array.size(); i++) {
    portfolio_val[i] = portfolio_val[i-1]*(1+returns[i]));
  }

  // Compare our portfolio value to market returns
  // Do we actually gain by using this strat instead of tracking the S&P 500 directly?
  
  double[] sp_500_daily_prices; // Should be in the same date range as our portfolio
  double[] market_returns  {0};
  for (int i = 1; i < array.size(); i++) {
    market_returns[i] = sp_500_daily_prices[i]/sp_500_daily_prices[i-1] - 1;
  }

  // final comparison
  int years = 10; // todo - how many years did we simulate?
  double beta = slope(returns, market_returns);
  double annualized_stategy_return = std::pow(returns[returns.size() - 1] / returns[1], 1/years) - 1;

  // Next steps may include testing and fitting slope
  // in order to lower 
}

// unit_root_test() {}
// dickey_fuller_test() {}
// augmented_dickey_fuller_test() {}

int main() {
  MarketType m_type = MarketType::equities;
  StrategyOptions opts{mean_reversion()};
  auto mean_reversion_strategy = std::make_shared<Strategy> (opts, m_type);
}

// Pseudo-code
auto getApiInfoForStock()
{
  constexpr std_const IEX_CLOUD_ENDPOINTS[2] {"intraday-prices", "quote"};
}