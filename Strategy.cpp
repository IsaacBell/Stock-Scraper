#include <string>
#include <memory>
#include <utility>
#include "Strategy.h"

struct Strategy::Impl
{
  std::unique_ptr<StrategyOptions> m_strategy_opts;

  int m_days = 0;
  int m_highBreakoutCount;
  double m_position_size;
  std::string m_volatility_index;

  template <typename SO = StrategyOptions>
  Impl(
    std::unique_ptr<SO> strategy_opts,
    MarketType market_type
  ) : m_strategy_opts(std::forward<SO>(strategy_opts)), m_market_type(market_type) {};
};

template <typename SO>
Strategy::Strategy(
  std::unique_ptr<SO> strategy_opts
) : pimpl(
  std::make_unique<Impl>(
    std::forward<SO>(strategy_opts),
    market_type
  )
) {};

Strategy::~Strategy() = default;

Strategy::Strategy(const Strategy& other) : pimpl(std::make_unique<Impl>(*other.pimpl)) {}

Strategy& Strategy::operator=(Strategy rhs)
{
  swap(pimpl, rhs.pimpl);
  return *this;
}

double Strategy::NDayHigh(int n)
{}
