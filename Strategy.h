#include <memory>
#include <string>

constexpr unsigned DEFAULT_INITIAL_EQUITY {10'000};

enum class TradeSetting {
  backtest = 0,
  paper    = 1,
  live     = 2
};

enum class MarketType {
  equities = 0,
  forex    = 1,
  futures  = 2,
  crypto   = 3
};

struct StrategyOptions {
  int max_positions{10};
  bool allow_shrinking{true};

  std::string volatility_index{""};
  bool limit_by_volatility{false};
  
  // 50 = 50% margin (standard w/ US stocks) OR 100 = no margin
  int margin{50};

  long int initial_equity{DEFAULT_INITIAL_EQUITY};

  MarketType m_market_type{MarketType::equities};
  TradeSetting m_trade_setting {TradeSetting::backtest};
};

StrategyOptions mean_reversion() {
  return StrategyOptions{10, true, "VIX", true, 50, DEFAULT_INITIAL_EQUITY};
}

class Strategy {
public:
  template <typename SO = StrategyOptions>
  explicit Strategy(std::unique_ptr<SO> strategy_opts);
  ~Strategy();

  Strategy(const Strategy& other);
  Strategy& operator=(Strategy rhs);

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;

  int SetPositionSize();
  double NDayHigh(int n);
  bool ApplyStop(std::string stopType, int stopPoint);
};
