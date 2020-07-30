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


#define N_FUNCTIONS 23
#define OVERWRITE_DATA_REPORTS true

class TradingData
{
public:
  //TradingData(std::vector<std::string> name);
  TradingData(std::string name);
  ~TradingData();
  void EXECUTE();
  //void BatchRequests();
  void Book();
  void Charts();
  void Company();
  //void DelayedQuote();
  void Dividends();
  void Earnings();
  void EffectiveSpread();
  void Financials();
  //void HistoricalPrices();
  void KeyStats();
  void LargestTrades();
  void List();
  void Logo();
  void News();
  void OHLC();
  //void OpenClose(bool overwrite=true);
  void Peers();
  void Previous();
  void Price();
  void Quote();
  void Relevant();
  void Splits();
  void TimeSeries();
  void VolumeByVenue();
  void S_Previous();
  void S_Symbols();
  std::string name;
  
private:
  typedef void (TradingData::*overall)();
  overall p_overall[N_FUNCTIONS] = {
    &TradingData::Book,
    &TradingData::Charts,
    &TradingData::Company,
    &TradingData::Dividends,
    &TradingData::Earnings,
    &TradingData::EffectiveSpread,
    &TradingData::Financials,
    &TradingData::KeyStats,
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
  void dayChart(time_t tempdate);

  std::thread p_thread[N_FUNCTIONS];

  std::string TMP_DIR;
  std::string mApiPublicKey;
  std::string mApiPrivateKey;

  std::string GetEnvVar(const std::string& key);
  void SendGetRequest(std::string key);
  void SendGetRequest(std::string key, std::string endpoint);
};

ConnectHttp TradingData::connect;

TradingData::TradingData(std::string name):
  name(name),
  date(),
  mApiPublicKey(
    [this](){ return TradingData::GetEnvVar("API_SANDBOX_PUB_KEY"); }()
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

void TradingData::Book()
{
  SendGetRequest("Book");
}

void TradingData::Charts()
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
}

inline void TradingData::Company()
{
  SendGetRequest("Company");
}

inline void TradingData::Dividends()
{
  SendGetRequest("Dividends");
}

void TradingData::Earnings()
{
  SendGetRequest("Earnings", "earnings/5y");
}

void TradingData::EffectiveSpread()
{
  SendGetRequest("EffectiveSpread", "/effective-spread");
}

inline void TradingData::Financials()
{
  SendGetRequest("Financials");
}

inline void TradingData::KeyStats()
{
  SendGetRequest("Stats");
}

inline void TradingData::LargestTrades()
{
  SendGetRequest("LargestTrades", "/largest-trades");
}

inline void TradingData::List()
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
}

inline void TradingData::Logo()
{
  SendGetRequest("Logo");
}

inline void TradingData::News()
{
  SendGetRequest("News", "/news/last/500");
}

inline void TradingData::OHLC()
{
  SendGetRequest("OHLC");
}

inline void TradingData::Peers()
{
  SendGetRequest("Peers");
}

inline void TradingData::Previous()
{
  SendGetRequest("Previous");
}

inline void TradingData::Price()
{
  SendGetRequest("Price");
}

inline void TradingData::Quote()
{
  SendGetRequest("Quote");
}

inline void TradingData::Relevant()
{
  SendGetRequest("Relevant");
}

inline void TradingData::Splits()
{
  SendGetRequest("Splits", "splits/5y");
}

inline void TradingData::TimeSeries()
{
  SendGetRequest("TimeSeries", "time-series");
}

inline void TradingData::VolumeByVenue()
{
  SendGetRequest("VolumeByVenue", "/volume-by-venue");
}

inline void TradingData::S_Previous()
{
  SendGetRequest("Market", "market/previous");
}

inline void TradingData::S_Symbols()
{
  bool overwrite = false;
  std::string c_path = ConnectHttp::path + "Symbols";
  File::mkdir(c_path);
  c_path += "/Symbols" + EXT;
  std::string c_url = "https://api.iextrading.com/1.0/ref-data/symbols";
  if(overwrite||!File::isFile(c_path))
  connect.RequestWriteJson(c_url, c_path);
  std::cout << "S_Symbols: status{finished}\n";
}

inline void TradingData::dayChart(time_t tempdate)
{
  tm* temp = localtime(&tempdate);
  std::stringstream mon;mon << std::setfill('0') << std::setw(2) << temp->tm_mon;
  std::stringstream day;day << std::setfill('0') << std::setw(2) << temp->tm_mday;
  std::string s_date(std::to_string(temp->tm_year + 1900) + mon.str() + day.str());
  std::string c_url = ConnectHttp::URL + name + "/chart/date/" + s_date;
  std::string c_path = ConnectHttp::path + name +"/Chart_1d/" +s_date+EXT;
  connect.RequestAddJson(c_url, c_path, "minute");
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

inline void TradingData::SendGetRequest(std::string key)
{
  return SendGetRequest(key, "");
}

inline void TradingData::SendGetRequest(std::string key, std::string endpoint="")
{
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

  std::string c_url = ConnectHttp::URL + "/stock/" + name +
                      endpoint + "?token=";

  c_url += mApiPublicKey;

  if( OVERWRITE_DATA_REPORTS || !File::isFile(c_path) ) {
    connect.RequestWriteJson(c_url, c_path);
  }

  // Benchmarking
  auto end = std::chrono::system_clock::now();
  auto elapsedTmp = end - start;
  long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTmp).count();
  auto benchmark = "[" + name + "] " + key + ": " ;
  benchmark += std::to_string(elapsed) + "ms\n";
  std::cout << benchmark << std::endl;

  return;
}
