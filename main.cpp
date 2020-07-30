
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <time.h>
#include "TradingData.h"
#include "ConnectHttp.h"

void importIEXData()
{
  std::string lineStr;
  TradingData tsla("TSLA");

  tsla.Company();
  tsla.Book();
  tsla.Earnings();
  tsla.Financials();
}

// calculate vwap value
double calc_vwap(std::vector<std::vector<std::string>> &marketDataTable)
{
  int n = marketDataTable.size() - 1; // skip the first title line
  double total_sum = 0.0;
  int volume_sum = 0;
  for (int i = 1; i <= n; i++)
  {
    // get the price and volume according to table structure
    double high_price = atof(marketDataTable[i][9].c_str());
    double low_price = atof(marketDataTable[i][10].c_str());
    double price = (high_price + low_price) / 2;
    int volume = atoi(marketDataTable[i][11].c_str());
    // compute total sum and volume sum
    total_sum += price * volume;
    volume_sum += volume;
  }
  return total_sum / volume_sum;
}

// calculate twap value
double calc_twap(std::vector<std::vector<std::string>> &marketDataTable)
{
  int n = marketDataTable.size() - 1; // skip the first title line
  double price_sum = 0.0;
  for (int i = 1; i <= n; i++)
  {
    // get the price and volume according to table structure
    double high_price = atof(marketDataTable[i][9].c_str());
    double low_price = atof(marketDataTable[i][10].c_str());
    double price = (high_price + low_price) / 2;
    // compute price sum and time sum
    // here use the 1 min K-line data, so total time is n minutes
    price_sum += price;
  }
  return price_sum / n;
}

int main()
{
  std::cout << "StockScraper - BEGIN" << std::endl;

  std::vector<std::vector<std::string>> marketDataTable;

  importIEXData();
  // std::cout << "calculating TWAP and VWAP" << std::endl;
  // std::cout << "VWAP [0][0]: " << marketDataTable[0][0] << std::endl;
  // std::cout << "TWAP [0][0]: " << marketDataTable[0][0] << std::endl;
  // std::cout << "VWAP: " << calc_twap(marketDataTable) << std::endl;
  // std::cout << "TWAP: " << calc_vwap(marketDataTable) << std::endl;
  return 0;
}
