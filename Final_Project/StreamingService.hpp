#ifndef STREAMING_SERVICE_HPP
#define STREAMING_SERVICE_HPP

#include <string>
#include <map>
#include "SOA.hpp"
#include "MarketDataService.hpp"

/**
 * @class PriceStreamOrder
 * @brief Represents an order in a price stream with price, visible quantity, hidden quantity, and side.
 * 
 * This class encapsulates the details of a price stream order, including the price, visible quantity,
 * hidden quantity, and the side (buy or sell) of the order.
 */

/**
 * @brief Default constructor for PriceStreamOrder.
 */
 
/**
 * @brief Parameterized constructor for PriceStreamOrder.
 * 
 * @param price The price of the order.
 * @param visibleQuantity The visible quantity of the order.
 * @param hiddenQuantity The hidden quantity of the order.
 * @param side The side (buy or sell) of the order.
 */

/**
 * @brief Get the side of the order.
 * 
 * @return The side of the order.
 */

/**
 * @brief Get the price of the order.
 * 
 * @return The price of the order.
 */

/**
 * @brief Get the visible quantity of the order.
 * 
 * @return The visible quantity of the order.
 */

/**
 * @brief Get the hidden quantity of the order.
 * 
 * @return The hidden quantity of the order.
 */
class PriceStreamOrder
{
public:
    // ctor
    PriceStreamOrder() = default;
    PriceStreamOrder(double _price, long _visibleQuantity, long _hiddenQuantity, PricingSide _side);

    // The side on this order
    PricingSide GetSide() const;

    // Get the price on this order
    double GetPrice() const;

    // Get the visible quantity on this order
    long GetVisibleQuantity() const;

    // Get the hidden quantity on this order
    long GetHiddenQuantity() const;

private:
    double price;
    long visibleQuantity;
    long hiddenQuantity;
    PricingSide side;
};


/**
 * Price Stream with a two-way market.
 * Type T is the product type.
 */
template<typename T>
class PriceStream
{
public:
    // ctor
    PriceStream() = default;
    PriceStream(const T& _product, const PriceStreamOrder& _bidOrder, const PriceStreamOrder& _offerOrder);

    // Get the product
    const T& GetProduct() const;

    // Get the bid order
    const PriceStreamOrder& GetBidOrder() const;

    // Get the offer order
    const PriceStreamOrder& GetOfferOrder() const;

private:
    T product;
    PriceStreamOrder bidOrder;
    PriceStreamOrder offerOrder;
};


/**
 * Streaming service to publish two-way prices.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class StreamingService : public Service<string, PriceStream <T> >
{
private:
    map<string, PriceStream<T>> pricestreams;

public:
    // Get data on our service given a key
    PriceStream<T>& GetData(string key);

    // The callback that a Connector should invoke for any new or updated data
    void OnMessage(PriceStream<T>& data);

    // Publish two-way prices
    void PublishPrice(PriceStream<T>& priceStream); 
};


PriceStreamOrder::PriceStreamOrder(double price, long visibleQuantity, long hiddenQuantity, PricingSide side)
{
    price = price;
    visibleQuantity = visibleQuantity;
    hiddenQuantity = hiddenQuantity;
    side = side;
}

PricingSide PriceStreamOrder::GetSide() const
{
    return side;
}

double PriceStreamOrder::GetPrice() const
{
    return price;
}

long PriceStreamOrder::GetVisibleQuantity() const
{
    return visibleQuantity;
}

long PriceStreamOrder::GetHiddenQuantity() const
{
    return hiddenQuantity;
}


template<typename T>
PriceStream<T>::PriceStream(const T& _product, const PriceStreamOrder& _bidOrder, const PriceStreamOrder& _offerOrder) :
    product(_product), bidOrder(_bidOrder), offerOrder(_offerOrder)
{
}

template<typename T>
const T& PriceStream<T>::GetProduct() const
{
    return product;
}

template<typename T>
const PriceStreamOrder& PriceStream<T>::GetBidOrder() const
{
    return bidOrder;
}

template<typename T>
const PriceStreamOrder& PriceStream<T>::GetOfferOrder() const
{
    return offerOrder;
}


template <typename T>
PriceStream<T>& StreamingService<T>::GetData(string key)
{
    return pricestreams[key];
}

template <typename T>
void StreamingService<T>::OnMessage(PriceStream<T>& data)
{
    pricestreams[data.GetProduct().GetProductId()] = data;
}

template <typename T>
void StreamingService<T>::PublishPrice(PriceStream<T>& priceStream)
{
    Service<string, PriceStream<T> >::Notify(priceStream);
}

#endif
