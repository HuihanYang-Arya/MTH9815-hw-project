/**
 * @file AlgoStreamingService.hpp
 * @brief Header file for the AlgoStreamingService class template.
 *
 * This file contains the definition of the AlgoStreamingService class template, which is responsible for managing and publishing two-way prices for financial products.
 */

#ifndef ALGO_STREAMING_SERVICE_HPP
#define ALGO_STREAMING_SERVICE_HPP

#include <map>
#include <random>
#include <string>
#include "utils/SOA.hpp"
#include "utils/Products.hpp"
#include "PricingService.hpp"
#include "StreamingService.hpp"

template <class V>
class AlgoStreamingService : public Service<string, PriceStream<V>>
{
private:
    map<string, PriceStream<V>> pricestreams;
    random_device rd; // Random device for generating random numbers
    default_random_engine generator{ rd() }; // Random number generator

public:
    // Get data on our service given a key
    PriceStream<V>& GetData(string key);

    // The callback that a Connector should invoke for any new or updated data
    void OnMessage(PriceStream<V>& data);

    // Publish two-way prices
    void PublishPrice(Price<V>& data);
};

template <typename V>
PriceStream<V>& AlgoStreamingService<V>::GetData(string key)
{
    return pricestreams[key];
}

template <typename V>
void AlgoStreamingService<V>::OnMessage(PriceStream<V>& data)
{
    pricestreams[data.GetProduct().GetProductId()] = data;
}

/**
 * @brief Publishes the price data to the streaming service.
 * 
 * This function takes a Price object, extracts the product, calculates the bid and ask prices,
 * generates a random visible size for the orders, creates bid and ask PriceStreamOrder objects,
 * and then creates a PriceStream object. The PriceStream object is stored in the pricestreams
 * map and the service is notified with the new price stream.
 * 
 * @tparam V The type of the product.
 * @param data The Price object containing the product and price information.
 */
template <typename V>
void AlgoStreamingService<V>::PublishPrice(Price<V>& data)
{
    V product = data.GetProduct();
    double bid_price = data.GetMid() - data.GetBidOfferSpread() / 2;
    double ask_price = data.GetMid() + data.GetBidOfferSpread() / 2;
    uniform_int_distribution<long> distribution(1000000, 1999999);
    long visible_size = distribution(generator); // Generating random visible size
    PriceStreamOrder bid_order(bid_price, visible_size, 2 * visible_size, BID);
    PriceStreamOrder ask_order(ask_price, visible_size, 2 * visible_size, OFFER);
    PriceStream<V> price_stream(data.GetProduct(), bid_order, ask_order);

    pricestreams[price_stream.GetProduct().GetProductId()] = price_stream;
    Service<string, PriceStream<V>>::Notify(price_stream);
}

#endif

