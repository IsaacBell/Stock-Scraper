#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <unordered_map>
#include "curl/curl.h"
#include "TradingData.h"
#include "ConnectHttp.h"
#include "include/rapidjson/document.h"

double calculate_vwap(std::string book_data)
{
  Document document;
  document.Parse(book_data.c_str());

  auto aapl = document["AAPL"].GetObject();
  auto book = aapl["book"].GetObject();

  int volume_sum      = 0;
  double daily_open   = 0.0;
  double daily_close  = 0.0;
  double high_price   = 0.0;
  double low_price    = 0.0;
  double total_sum    = 0.0;

  auto symbol         = book["quote"]["symbol"].GetString();
  auto volume         = book["quote"]["volume"].GetInt();
  auto daily_low      = book["quote"]["low"].GetDouble();
  auto daily_high     = book["quote"]["high"].GetDouble();
  auto change_percent = book["quote"]["changePercent"].GetDouble();

  if (!book["quote"]["open"].IsNull())
    daily_open  = book["quote"]["open"].GetDouble();
  if (!book["quote"]["close"].IsNull())
    daily_close = book["quote"]["close"].GetDouble();

  std::cout << "\n----" << symbol << "Key Stats----\n" << std::endl;
  std::cout << "volume: "      << volume         << std::endl;
  std::cout << "volume sum: "  << volume_sum     << std::endl;
  std::cout << "daily open: "  << daily_open     << std::endl;
  std::cout << "daily close: " << daily_close    << std::endl;
  std::cout << "high price: "  << high_price     << std::endl;
  std::cout << "low price: "   << low_price      << std::endl;
  std::cout << "change %: "    << change_percent << std::endl;
  std::cout << "total sum: "   << total_sum      << std::endl;

  /* todo
  -------
  // for(auto& trade : book["trades"].GetArray()) {
  //   auto price = trade["price"].GetString();
  //   // get the price and volume according to table structure
  //   if (std::stod(price) > high_price)
  //     high_price = std::stod(price);
  //   if (std::stod(price) < low_price)
  //     low_price = std::stod(price);
  // }
  -------
  */

  double price = (high_price + low_price) / 2;
  // compute total sum and volume sum
  total_sum += price * volume;
  volume_sum += volume;

  if (volume_sum == 0)
    return 0.0;

  return total_sum / volume_sum;
}

double calc_twap(std::string book_data, int n_iterations)
{
  // wip
  Document document;
  document.Parse(book_data.c_str());

  auto aapl = document["AAPL"].GetObject();
  auto book = aapl["book"].GetObject();

  double price_sum    = 0.0;
  double daily_open   = 0.0;
  double daily_close  = 0.0;
  double high_price   = 0.0;
  double low_price    = 0.0;
  double total_sum    = 0.0;

  auto symbol         = book["quote"]["symbol"].GetString();
  auto volume         = book["quote"]["volume"].GetInt();
  auto daily_low      = book["quote"]["low"].GetDouble();
  auto daily_high     = book["quote"]["high"].GetDouble();
  auto change_percent = book["quote"]["changePercent"].GetDouble();

  if (!book["quote"]["open"].IsNull())
    daily_open  = book["quote"]["open"].GetDouble();
  if (!book["quote"]["close"].IsNull())
    daily_close = book["quote"]["close"].GetDouble();

  std::cout << "\n----" << symbol << "Key Stats----\n" << std::endl;
  std::cout << "volume: "      << volume         << std::endl;
  std::cout << "daily open: "  << daily_open     << std::endl;
  std::cout << "daily close: " << daily_close    << std::endl;
  std::cout << "high price: "  << high_price     << std::endl;
  std::cout << "low price: "   << low_price      << std::endl;
  std::cout << "change %: "    << change_percent << std::endl;
  std::cout << "total sum: "   << total_sum      << std::endl;

  return price_sum / n_iterations;
}

void importIEXData()
{
  std::string lineStr;
  TradingData aapl("aapl");
 
  // auto company    = aapl.Company();
  // auto book       = aapl.Book();
  // auto earnings   = aapl.Earnings();
  // auto financials = aapl.Financials();
  // auto intraday = aapl.Intraday();
  // double vwap     = calculate_vwap(book);

  // std::cout << "\n----Company----\n" << company << "\n";
  // std::cout << "\n----Book----\n" << book << "\n";
  // std::cout << "\n----Earnings----\n" << earnings << "\n";
  // std::cout << "\n----Financials----\n" << financials << "\n";
  // std::cout << "\n----Intraday Prices----\n" << intraday << "\n";
  // std::cout << "\n----VWAP----\n" << vwap << "\n";
}

int main1()
{
  // todo - store data in a matrix?
  std::vector<std::vector<std::string>> marketDataTable;

  importIEXData();

  curl_global_cleanup();

  return 0;
}

int main2()
{
  std::map<std::string, auto> marketDataTable;
  boost::asio::io_service io_service;

  
}

