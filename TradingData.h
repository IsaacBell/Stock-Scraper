#pragma once
#include "ConnectHttp.h"
#include "Macros.h"
#include "File.h"
#include "Date.h"
#include <thread>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctype.h>      /* isalpha */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* getenv */
#include <string>       /* to_string */ 
#include <locale>       /* std::tolower, std::locale */


constexpr unsigned N_FUNCTIONS {24};
constexpr bool OVERWRITE_DATA_REPORTS {true};

class TradingData
{
public:
  //TradingData(std::vector<std::string> name);
  TradingData(std::string name);
  ~TradingData();
  void EXECUTE();
  //void BatchRequests();
  std::string Book();
  std::string Charts();
  std::string Company();
  //std::string DelayedQuote();
  std::string Dividends();
  std::string Earnings();
  std::string EffectiveSpread();
  std::string Financials();
  //std::string HistoricalPrices();
  std::string KeyStats();
  std::string Intraday();
  std::string LargestTrades();
  std::string List();
  std::string Logo();
  std::string News();
  std::string OHLC();
  //std::string OpenClose(bool overwrite=true);
  std::string Peers();
  std::string Previous();
  std::string Price();
  std::string Quote();
  std::string Relevant();
  std::string Splits();
  std::string TimeSeries();
  std::string VolumeByVenue();
  std::string S_Previous();
  std::string S_Symbols();
  std::string name;
  
private:
  typedef std::string (TradingData::*overall)();
  overall p_overall[N_FUNCTIONS] = {
    &TradingData::Book,
    &TradingData::Charts,
    &TradingData::Company,
    &TradingData::Dividends,
    &TradingData::Earnings,
    &TradingData::EffectiveSpread,
    &TradingData::Financials,
    &TradingData::KeyStats,
    &TradingData::Intraday,
    &TradingData::LargestTrades,
    &TradingData::List,
    &TradingData::Logo,
    &TradingData::News,
    &TradingData::OHLC,
    &TradingData::Peers,
    &TradingData::Previous,
    &TradingData::Price,
    &TradingData::Quote,
    &TradingData::Relevant,
    &TradingData::Splits,
    &TradingData::TimeSeries,
    &TradingData::VolumeByVenue,
    &TradingData::S_Symbols,
    &TradingData::S_Previous
  };

  Date date;
  static ConnectHttp connect;
  std::string dayChart(time_t tempdate);

  std::thread p_thread[N_FUNCTIONS];

  std::string TMP_DIR;
  std::string mApiPublicKey;
  std::string mApiPrivateKey;

  std::string GetEnvVar(const std::string& key);
  std::string SendGetRequest(std::string key);
  std::string SendGetRequest(std::string key, std::string endpoint);
};

ConnectHttp TradingData::connect;

TradingData::TradingData(std::string name):
  name(name),
  date(),
  mApiPublicKey(
    [this](){ return TradingData::GetEnvVar("API_SANDBOX_PUB_KEY"); }
    ()
  )
{
  std::string TMP_DIR = "./tmp/";
  File::mkdir(TMP_DIR);
  TMP_DIR += name;
  File::mkdir(TMP_DIR);
}

TradingData::~TradingData()
{
}

std::string TradingData::Book()
{
  return SendGetRequest("Book");
}

// todo
std::string TradingData::Charts()
{
  bool overwrite = true;
  std::string c_path = ConnectHttp::path + name;
  File::mkdir(c_path);
  //std::string c_url = ConnectHttp::URL + "chart?" + _Parameters[Parameters::SYMBOLS] + "=" + _name + "&" + _Parameters[Parameters::RANGE] + "=" + _Range[Date::__range];
  std::string c_url = ConnectHttp::URL + name + "/chart/" + _Range[Date::__range];
  c_path = c_path + "/Chart";
  File::mkdir(c_path);
  c_path += "/chart"+EXT;
  connect.RequestAddJson(c_url, c_path,"date");
  c_path = ConnectHttp::path + name +"/Chart_1d";
  File::mkdir(c_path);
  time_t tempdate = Date::t_date30;
  std::vector<std::thread> t;
  while (Date::t_date >tempdate) {
    t.push_back(std::thread(&TradingData::dayChart, this,tempdate));
    tempdate += i_Range2;
  }
  std::cout << "Charts: status{finished}\n";
  for (int i = 0; i < t.size();i++) {
    t[i].join();
  }

  return "Not implemented yet";
}

inline std::string TradingData::Company()
{
  return SendGetRequest("Company");
}

inline std::string TradingData::Dividends()
{
  return SendGetRequest("Dividends");
}

std::string TradingData::Earnings()
{
  return SendGetRequest("Earnings", "earnings/5y");
}

std::string TradingData::EffectiveSpread()
{
  return SendGetRequest("EffectiveSpread", "/effective-spread");
}

inline std::string TradingData::Financials()
{
  return SendGetRequest("Financials");
}

inline std::string TradingData::Intraday()
{
  return SendGetRequest("IntradayPrices", "/intraday-prices");
}

inline std::string TradingData::KeyStats()
{
  return SendGetRequest("Stats");
}

inline std::string TradingData::LargestTrades()
{
  return SendGetRequest("LargestTrades", "/largest-trades");
}

// todo
inline std::string TradingData::List()
{
  bool overwrite = true;
  std::string c_path = ConnectHttp::path + name + "/List/";
  std::string c_url = ConnectHttp::URL + name;
  File::mkdir(c_path);
  
  connect.RequestWriteJson(c_url + "/mostactive", c_path+"MostActive" + EXT);
  connect.RequestWriteJson(c_url + "/gainers", c_path + "Gainers" + EXT);
  connect.RequestWriteJson(c_url + "/losers", c_path + "Losers" + EXT);
  connect.RequestWriteJson(c_url + "/iexvolume", c_path + "iexVolume" + EXT);
  connect.RequestWriteJson(c_url + "/iexpercent", c_path + "iexpercent" + EXT);
  std::cout << "List: status{finished}\n";

  return "Not implemented yet";
}

inline std::string TradingData::Logo()
{
  return SendGetRequest("Logo");
}

inline std::string TradingData::News()
{
  return SendGetRequest("News", "/news/last/500");
}

inline std::string TradingData::OHLC()
{
  return SendGetRequest("OHLC");
}

inline std::string TradingData::Peers()
{
  return SendGetRequest("Peers");
}

inline std::string TradingData::Previous()
{
  return SendGetRequest("Previous");
}

inline std::string TradingData::Price()
{
  return SendGetRequest("Price");
}

inline std::string TradingData::Quote()
{
  return SendGetRequest("Quote");
}

inline std::string TradingData::Relevant()
{
  return SendGetRequest("Relevant");
}

inline std::string TradingData::Splits()
{
  return SendGetRequest("Splits", "splits/5y");
}

inline std::string TradingData::TimeSeries()
{
  return SendGetRequest("TimeSeries", "time-series");
}

inline std::string TradingData::VolumeByVenue()
{
  return SendGetRequest("VolumeByVenue", "/volume-by-venue");
}

inline std::string TradingData::S_Previous()
{
  return SendGetRequest("Market", "market/previous");
}

// todo
inline std::string TradingData::S_Symbols()
{
  bool overwrite = false;
  std::string c_path = ConnectHttp::path + "Symbols";
  File::mkdir(c_path);
  c_path += "/Symbols" + EXT;
  std::string c_url = "https://api.iextrading.com/1.0/ref-data/symbols";
  if(overwrite||!File::isFile(c_path))
  connect.RequestWriteJson(c_url, c_path);
  std::cout << "S_Symbols: status{finished}\n";

  return "Not implemented yet";
}

// todo
inline std::string TradingData::dayChart(time_t tempdate)
{
  tm* temp = localtime(&tempdate);
  std::stringstream mon;mon << std::setfill('0') << std::setw(2) << temp->tm_mon;
  std::stringstream day;day << std::setfill('0') << std::setw(2) << temp->tm_mday;
  std::string s_date(std::to_string(temp->tm_year + 1900) + mon.str() + day.str());
  std::string c_url = ConnectHttp::URL + name + "/chart/date/" + s_date;
  std::string c_path = ConnectHttp::path + name +"/Chart_1d/" +s_date+EXT;
  connect.RequestAddJson(c_url, c_path, "minute");

  return "Not yet implemented";
}

inline void TradingData::EXECUTE()
{
  for (int i = 0; i < N_FUNCTIONS; i++) {
    p_thread[i] = std::thread(p_overall[i],this);
  }
  for (int i = 0; i < N_FUNCTIONS; i++) {
    p_thread[i].join();
  }
  std::cout << "finished successfully" << std::endl;
}

std::string TradingData::GetEnvVar(const std::string &key)
{
  const char * val = std::getenv( key.c_str() );

  if ( val == nullptr ) {
    // invalid to assign nullptr to std::string
    return "";
  } else {
    return val;
  }
}

inline std::string TradingData::SendGetRequest(std::string key)
{
  return SendGetRequest(key, "");
}

inline std::string TradingData::SendGetRequest(std::string key, std::string endpoint="")
{
  std::string output = "";
  const auto start = std::chrono::system_clock::now();

  std::string c_path     = ConnectHttp::path + name + "/" + key + "/";
  std::string parentPath = ConnectHttp::path + name;

  std::locale loc;

  std::string lowercaseEndpoint = "";
  for (int i = 0; i < key.length(); ++i) {
    if ( isalpha(key[i]) )
      lowercaseEndpoint += std::tolower(key[i],loc);
  }

  std::string processedEndpoint = endpoint == "" ? lowercaseEndpoint : endpoint;

  File::mkdir(parentPath);
  File::mkdir(c_path);
  c_path += key + EXT;
  std::string c_url = ConnectHttp::URL + "/batch?symbols=" + name +
                      endpoint + "&types=" + lowercaseEndpoint + "&token=";

  c_url += mApiPublicKey;

  if( OVERWRITE_DATA_REPORTS || !File::isFile(c_path) ) {
    output += connect.RequestReadJson(c_url);
    // connect.RequestWriteJson(c_url, c_path);
  }

  // Benchmarking
  auto end = std::chrono::system_clock::now();
  auto elapsedTmp = end - start;
  long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTmp).count();
  auto benchmark = "[" + name + "] " + key + ": " ;
  benchmark += std::to_string(elapsed) + "ms\n";
  std::cout << benchmark << std::endl;

  return output;
}
