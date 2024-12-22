/**
 * @file AlgoExecutionService.hpp
 * @brief Header file for the AlgoExecutionService class template.
 *
 * This file contains the definition and implementation of the AlgoExecutionService class template,
 * which is responsible for executing orders based on market data.
 */

#ifndef ALGO_EXECUTION_SERVICE_HPP
#define ALGO_EXECUTION_SERVICE_HPP

#include <string>
#include <vector>
#include <map>
#include "SOA.hpp"
#include "MarketDataService.hpp"
#include "ExecutionService.hpp"

using namespace std;

template <class T>
class AlgoExecutionService : public Service<string, ExecutionOrder<T>>
{
private:
    map<string, ExecutionOrder<T>> execution_orders;
    int counter;
    double spread_tol;

public:

    AlgoExecutionService();

    ExecutionOrder<T>& GetData(string key);

    void OnMessage(ExecutionOrder<T>& data);

    void ExecuteOrder(OrderBook<T>& data);
};

template <typename T>
AlgoExecutionService<T>::AlgoExecutionService() : counter(0), spread_tol(1.0 / 128)
{
    execution_orders = map<string, ExecutionOrder<T>>();
}

template <typename T>
ExecutionOrder<T>& AlgoExecutionService<T>::GetData(string key)
{
    return execution_orders[key];
}

template <typename T>
void AlgoExecutionService<T>::OnMessage(ExecutionOrder<T>& data)
{
    execution_orders[data.GetOrderId()] = data;
}

/**
 * @brief Executes an order based on the given OrderBook data.
 *
 * This function processes the bid and offer stacks from the provided OrderBook,
 * determines the best bid and offer prices, and executes an order if the spread
 * between the best bid and offer prices exceeds a predefined tolerance.
 *
 * @tparam T The type of the product being traded.
 * @param data The OrderBook containing bid and offer stacks for the product.
 *
 * The function performs the following steps:
 * 1. Retrieves the product from the OrderBook.
 * 2. Extracts the bid and offer stacks from the OrderBook.
 * 3. Identifies the best bid and offer prices from the stacks.
 * 4. If the spread between the best bid and offer prices exceeds the tolerance,
 *    it executes an order based on the current counter value (alternating between
 *    bid and offer orders).
 * 5. Creates an ExecutionOrder with the determined price, quantity, and side.
 * 6. Stores the ExecutionOrder in the execution_orders map and notifies the service.
 */
template <typename T>
void AlgoExecutionService<T>::ExecuteOrder(OrderBook<T>& data)
{
    T product = data.GetProduct();
    vector<Order> bid_stack = data.GetBidStack();
    vector<Order> offer_stack = data.GetOfferStack();
    Order best_bid = bid_stack.empty() ? Order() : bid_stack[0];
    Order best_offer = offer_stack.empty() ? Order() : offer_stack[0];

    for (const auto& e : bid_stack)
    {
        if (e.GetPrice() > best_bid.GetPrice())
            best_bid = e;
    }
    for (const auto& e : offer_stack)
    {
        if (e.GetPrice() < best_offer.GetPrice())
            best_offer = e;
    }

    double price, quantity;
    PricingSide side;
    if (!bid_stack.empty() && !offer_stack.empty() && (best_offer.GetPrice() - best_bid.GetPrice() > spread_tol))
    {
        if (counter % 2 == 0) // bid order
        {
            price = best_bid.GetPrice();
            quantity = best_bid.GetQuantity();
            side = BID;
        }
        else // offer order
        {
            price = best_offer.GetPrice();
            quantity = best_offer.GetQuantity();
            side = OFFER;
        }

        string tradeID = "TRADEID_" + to_string(counter);
        ExecutionOrder<T> execu_order(product, side, tradeID, MARKET, price, quantity, 2 * quantity, "", false);
        execution_orders[execu_order.GetOrderId()] = execu_order;
        ++counter;
        Service<string, ExecutionOrder<T>>::Notify(execu_order);
    }
}

#endif
