/**
 * test program for our ProductServices
 */

#include <iostream>
#include "products.hpp"
#include "productservice.hpp"

using namespace std;

int main()
{
  // Create the 10Y treasury note
  date maturityDate(2025, Nov, 16);
  string cusip = "912828M56";
  Bond treasuryBond(cusip, CUSIP, "T", 2.25, maturityDate);


  // Create the 2Y treasury note
  date maturityDate2(2017, Nov, 5);
  string cusip2 = "912828TW0";
  Bond treasuryBond2(cusip2, CUSIP, "T", 0.75, maturityDate2);

  // Create a BondProductService
  BondProductService *bondProductService = new BondProductService();

  // Add the 10Y note to the BondProductService and retrieve it from the service
  bondProductService->Add(treasuryBond);
  Bond bond = bondProductService->GetData(cusip);
  cout << "CUSIP: " << bond.GetProductId() << " ==> " << bond << endl;

  // Add the 2Y note to the BondProductService and retrieve it from the service
  bondProductService->Add(treasuryBond2);
  bond = bondProductService->GetData(cusip2);
  cout << "CUSIP: " << bond.GetProductId() << " ==> " << bond << endl;

  // Create the Spot 10Y Outright Swap
  date effectiveDate(2015, Nov, 16);
  date terminationDate(2025, Nov, 16);
  string outright10Y = "Spot-Outright-10Y";
  IRSwap outright10YSwap(outright10Y, THIRTY_THREE_SIXTY, THIRTY_THREE_SIXTY, SEMI_ANNUAL, LIBOR, TENOR_3M, effectiveDate, terminationDate, USD, 10, SPOT, OUTRIGHT);

  // Create the IMM 2Y Outright Swap
  date effectiveDate2(2015, Dec, 20);
  date terminationDate2(2017, Dec, 20);
  string imm2Y = "IMM-Outright-2Y";
  IRSwap imm2YSwap(imm2Y, THIRTY_THREE_SIXTY, THIRTY_THREE_SIXTY, SEMI_ANNUAL, LIBOR, TENOR_3M, effectiveDate2, terminationDate2, USD, 2, IMM, OUTRIGHT);

  // Create a IRSwapProductService
  IRSwapProductService *swapProductService = new IRSwapProductService();

  // Add the Spot 10Y Outright Swap to the IRSwapProductService and retrieve it from the service
  swapProductService->Add(outright10YSwap);
  IRSwap swap = swapProductService->GetData(outright10Y);
  cout << "Swap: " << swap.GetProductId() << " == > " << swap << endl;

  // Add the IMM 2Y Outright Swap to the IRSwapProductService and retrieve it from the service
  swapProductService->Add(imm2YSwap);
  swap = swapProductService->GetData(imm2Y);
  cout << "Swap: " << swap.GetProductId() << " == > " << swap << endl;

  //create future objects
  Future future("FUT001", "Bond", 1000000.0f, date(2024, Dec, 15), "Physical", 100.5f, USD);
  EuroDollarFuture euroDollarFuture("EDF001", "EuroDollar", 1500000.0f, date(2025, Jan, 15), "Cash", 99.0f, USD, TENOR_3M, LIBOR);
  BondFuture bondFuture("BF001", "Bond", 750000.0f, date(2025, Feb, 25), "Physical", 101.0f, USD, "US912828U816", CUSIP, 5.0f, date(2030, Dec, 31));
  FutureProductService* futureProductService = new FutureProductService();
  EDFutureProductService* edFutureProductService = new EDFutureProductService();
  BondFutureProductService* bondFutureProductService = new BondFutureProductService();
  futureProductService->add(future);
  edFutureProductService->add(euroDollarFuture);
  bondFutureProductService->add(bondFuture);

  Future retrievedfuture1 = futureProductService->GetData("FUT001");
  cout << "Future: " << retrievedfuture1.GetProductId() << " == > " << retrievedfuture1 << endl;

  BondFuture retrievedfuture2 = bondFutureProductService->GetData("BF001");
  cout << "Bond Future: " << retrievedfuture2.GetProductId() << " == > " << retrievedfuture2 << endl;

  EuroDollarFuture retrievedfuture3 = edFutureProductService->GetData("EDF001");
  cout << "EuroDollar Future: " << retrievedfuture3.GetProductId() << " == > " << retrievedfuture3 << endl;

  delete bondProductService;
  delete swapProductService;
  delete futureProductService;


  return 0;
}
