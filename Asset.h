#include <algorithm>

#ifndef ASSET_H
#define ASSET_H

class Asset
{
public:
  Asset(double upIn, double downIn, int periodsToAve, std::istream& input1) : input(input1) { upPercent = upIn; downPercent = downIn; currentTime = 0; numDays = periodsToAve; quantity = 0;}
  // Constructor
  Asset(const Asset& myAsset);
  // Copy constructor
  ~Asset();
  // Destructor: only needs to delete things on heap

  void readPrices();
  // Reads the next price from the input stream and adds to it to our prices vector
  void observeMarket();
  // Observes the market price, by reading the price, by calling calcPriceLevels and then calls executeTrade
  double getProfit();
  // Returns currentProfit to the user
  void writeProfit();
  
 private:
  std::vector<double> prices; // Is a pointer to the first price in our array of prices
  std::vector<double> profits; // Is a pointer to the first profit in our array of profits
  double upPercent, downPercent; // What percentage up down from rolling mean should we buy sell at
  double buyPrice, sellPrice, avePrice; // What actual prices do should we buy/sell at
  double quantity; // Quantity of asset we currently have
  double currentProfit; // The current profit: amount from selling minus amount from buying by point t
  double currentPrice;
  int numDays; // How many days do we use for our rolling mean
  int currentTime; // A counter for where we are in our price vector
  std::istream& input; // The input stream for our asset
  void calcPriceLevels();
  // Calculates the price levels to buy or sell at based on the average of prices from t-x to t-1
  // Updates buyPrice and sellPrice
  void executeTrade();
  // Looks at prices[t] and sees if they are above/below buyPrice/sellPrice
  // If so buys/sells and updates profit array and currentProfit

};

#endif