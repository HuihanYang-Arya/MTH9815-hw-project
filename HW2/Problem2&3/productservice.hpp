#ifndef PRODUCTSERVICE_HPP
#define PRODUCTSERVICE_HPP

#include <string>
#include <vector>
#include <map>
#include "products.hpp"


using namespace std;

/**
 * productservice.hpp defines Bond and IRSwap ProductServices
 */

#include <iostream>
#include <map>
#include "products.hpp"
#include "soa.hpp"

/**
 * Bond Product Service to own reference data over a set of bond securities.
 * Key is the productId string, value is a Bond.
 */
class BondProductService : public Service<string, Bond>
{
public:
  // BondProductService ctor
  BondProductService();

  // Return the bond data for a particular bond product identifier
  Bond& GetData(string productId) override;

  // Add a bond to the service (convenience method)
  void Add(Bond &bond);

  // Get all Bonds with the specified ticker
  vector<Bond> GetBonds(string& _ticker);

private:
  map<string, Bond> bondMap; // cache of bond products
};

/**
 * Interest Rate Swap Product Service to own reference data over a set of IR Swap products
 * Key is the productId string, value is a IRSwap.
 */
class IRSwapProductService : public Service<string, IRSwap>
{
public:
  // IRSwapProductService ctor
  IRSwapProductService();

  // Return the IR Swap data for a particular bond product identifier
  IRSwap& GetData(string productId) override;

  // Add a bond to the service (convenience method)
  void Add(IRSwap &swap);

  // Get all Swaps with the specified fixed leg day count convention
  vector<IRSwap> GetSwaps(DayCountConvention _fixedLegDayCountConvention);

  // Get all Swaps with the specified fixed leg payment frequency
  vector<IRSwap> GetSwaps(PaymentFrequency _fixedLegPaymentFrequency);

  // Get all Swaps with the specified floating index
  vector<IRSwap> GetSwaps(FloatingIndex _floatingIndex);

  // Get all Swaps with a term in years greater than the specified value
  vector<IRSwap> GetSwapsGreaterThan(int _termYears);

  // Get all Swaps with a term in years less than the specified value
  vector<IRSwap> GetSwapsLessThan(int _termYears);

  // Get all Swaps with the specified swap type
  vector<IRSwap> GetSwaps(SwapType _swapType);

  // Get all Swaps with the specified swap leg type
  vector<IRSwap> GetSwaps(SwapLegType _swapLegType);

private:
  map<string, IRSwap> swapMap; // cache of IR Swap products
};

vector<IRSwap> IRSwapProductService::GetSwaps(DayCountConvention _fixedLegDayCountConvention) {
  vector<IRSwap> result;
  for (const auto& pair : swapMap) {
    if (pair.second.GetFixedLegDayCountConvention() == _fixedLegDayCountConvention) {
      result.push_back(pair.second);
    }
  }
  return result;
}

vector<IRSwap> IRSwapProductService::GetSwaps(PaymentFrequency _fixedLegPaymentFrequency) {
  vector<IRSwap> result;
  for (const auto& pair : swapMap) {
    if (pair.second.GetFixedLegPaymentFrequency() == _fixedLegPaymentFrequency) {
      result.push_back(pair.second);
    }
  }
  return result;
}

vector<IRSwap> IRSwapProductService::GetSwaps(FloatingIndex _floatingIndex) {
  vector<IRSwap> result;
  for (const auto& pair : swapMap) {
    if (pair.second.GetFloatingIndex() == _floatingIndex) {
      result.push_back(pair.second);
    }
  }
  return result;
}

vector<IRSwap> IRSwapProductService::GetSwapsGreaterThan(int _termYears) {
  vector<IRSwap> result;
  for (const auto& pair : swapMap) {
    if (pair.second.GetTermYears() > _termYears) {
      result.push_back(pair.second);
    }
  }
  return result;
}

vector<IRSwap> IRSwapProductService::GetSwapsLessThan(int _termYears) {
  vector<IRSwap> result;
  for (const auto& pair : swapMap) {
    if (pair.second.GetTermYears() < _termYears) {
      result.push_back(pair.second);
    }
  }
  return result;
}

vector<IRSwap> IRSwapProductService::GetSwaps(SwapType _swapType) {
  vector<IRSwap> result;
  for (const auto& pair : swapMap) {
    if (pair.second.GetSwapType() == _swapType) {
      result.push_back(pair.second);
    }
  }
  return result;
}

vector<IRSwap> IRSwapProductService::GetSwaps(SwapLegType _swapLegType) {
  vector<IRSwap> result;
  for (const auto& pair : swapMap) {
    if (pair.second.GetSwapLegType() == _swapLegType) {
      result.push_back(pair.second);
    }
  }
  return result;
}

BondProductService::BondProductService()
{
  bondMap = map<string,Bond>();
}

Bond& BondProductService::GetData(string productId)
{
  return bondMap[productId];
}

void BondProductService::Add(Bond &bond)
{
  bondMap.insert(pair<string,Bond>(bond.GetProductId(), bond));
}


IRSwapProductService::IRSwapProductService()
{
  swapMap = map<string,IRSwap>();
}

IRSwap& IRSwapProductService::GetData(string productId)
{
  return swapMap[productId];
}

void IRSwapProductService::Add(IRSwap &swap)
{
  swapMap.insert(pair<string,IRSwap>(swap.GetProductId(), swap));
}

class FutureProductService : public Service<string, Future>
{
private: 
  map<string, Future> futureMap;

public:
  FutureProductService();

  Future& GetData(string productId) override;

  void add(Future &future);
};

FutureProductService::FutureProductService()
{
  futureMap = map<string, Future>();
}

Future& FutureProductService::GetData(string productId)
{
  return futureMap[productId];
}

void FutureProductService::add(Future &future)
{
  futureMap.insert(pair<string, Future>(future.GetProductId(), future));
}



class EDFutureProductService : public Service<string, EuroDollarFuture>
{
private: 
  map<string, EuroDollarFuture> edFutureMap;

public:
  EDFutureProductService();

  EuroDollarFuture& GetData(string productId) override;

  void add(EuroDollarFuture &euroDollarFuture);
};

EDFutureProductService::EDFutureProductService()
{
  edFutureMap = map<string, EuroDollarFuture>();
}

EuroDollarFuture& EDFutureProductService::GetData(string productId)
{
  return edFutureMap[productId];
}

void EDFutureProductService::add(EuroDollarFuture &euroDollarFuture)
{
  edFutureMap.insert(pair<string, EuroDollarFuture>(euroDollarFuture.GetProductId(), euroDollarFuture));
}

//

class BondFutureProductService : public Service<string,BondFuture>
{
public:
  // IRSwapProductService ctor
  BondFutureProductService();

  // Return the IR Swap data for a particular bond product identifier
  BondFuture& GetData(string productId) override;

  // Add a bond to the service (convenience method)
  void add(BondFuture &bondFuture);

private:
  map<string,BondFuture> bondFutureMap; // cache of IR Swap products

};

BondFutureProductService::BondFutureProductService()
{
  bondFutureMap = map<string,BondFuture>();
}

BondFuture& BondFutureProductService::GetData(string productId)
{
  return bondFutureMap[productId];
}

void BondFutureProductService::add(BondFuture &bondFuture)
{
  bondFutureMap.insert(pair<string,BondFuture>(bondFuture.GetProductId(), bondFuture));
}

#endif // PRODUCTSERVICE_HPP


