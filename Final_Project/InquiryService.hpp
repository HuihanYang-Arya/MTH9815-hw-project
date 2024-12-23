#ifndef INQUIRY_SERVICE_HPP
#define INQUIRY_SERVICE_HPP

#include <map>
#include <string>
#include "SOA.hpp"
#include "TradeBookingService.hpp"

 // Various inqyury states
enum InquiryState { RECEIVED, QUOTED, DONE, REJECTED, CUSTOMER_REJECTED };

/**
 * Inquiry object models a customer inquiry from a client.
 * Type T is the product type.
 */
template<typename T>
class Inquiry
{
public:
    // ctor
    Inquiry() = default;
    Inquiry(string _inquiryId, const T& _product, Side _side, long _quantity, 
        double _price, InquiryState _state);

    // Get the inquiry ID
    const string& GetInquiryId() const;

    // Get the product
    const T& GetProduct() const;

    // Get the side on the inquiry
    Side GetSide() const;

    // Get the quantity that the client is inquiring for
    long GetQuantity() const;

    // Get the price that we have responded back with
    double GetPrice() const;

    // Get the current state on the inquiry
    InquiryState GetState() const;

    // Change the state of the inquiry
    void SetState(InquiryState _state);

    // Change the price
    void SetPrice(double _price);
    
private:
    string inquiryId;
    T product;
    Side side;
    long quantity;
    double price;
    InquiryState state;

};


/**
 * Service for customer inquirry objects.
 * Keyed on inquiry identifier (NOTE: this is NOT a product identifier since each inquiry must be unique).
 * Type T is the product type.
 */
template<typename T>
class InquiryService : public Service<string, Inquiry <T> >
{
private:
    map<string, Inquiry<T>> inquiries;

public:
    // Get data on our service given a key
    Inquiry<T>& GetData(string key);

    // The callback that a Connector should invoke for any new or updated data
    void OnMessage(Inquiry<T>& data);

    // Send a quote back to the client
    void SendQuote(const string& inquiryId, double price);

    // Reject an inquiry from the client
    void RejectInquiry(const string& inquiryId);
    
};


/**
 * @brief Constructor for the Inquiry class.
 * 
 * @tparam T The type of the product.
 * @param _inquiryId The ID of the inquiry.
 * @param _product The product associated with the inquiry.
 * @param _side The side of the inquiry (buy or sell).
 * @param _quantity The quantity of the product.
 * @param _price The price of the product.
 * @param _state The state of the inquiry.
 */
template<typename T>
Inquiry<T>::Inquiry(string _inquiryId, const T& _product, Side _side, long _quantity, double _price, InquiryState _state) :
    product(_product)
{
    inquiryId = _inquiryId;
    side = _side;
    quantity = _quantity;
    price = _price;
    state = _state;
}

template<typename T>
const string& Inquiry<T>::GetInquiryId() const
{
    return inquiryId;
}

template<typename T>
const T& Inquiry<T>::GetProduct() const
{
    return product;
}

template<typename T>
Side Inquiry<T>::GetSide() const
{
    return side;
}

template<typename T>
long Inquiry<T>::GetQuantity() const
{
    return quantity;
}

template<typename T>
double Inquiry<T>::GetPrice() const
{
    return price;
}

template<typename T>
InquiryState Inquiry<T>::GetState() const
{
    return state;
}

template <typename T>
void Inquiry<T>::SetState(InquiryState _state) 
{
    state = _state;
}


template <typename T>
void Inquiry<T>::SetPrice(double _price)
{
    price = _price;
}


template <typename T>
Inquiry<T>& InquiryService<T>::GetData(string key)
{
    return inquiries[key];
}

/**
 * @brief Handles incoming messages for inquiries.
 * 
 * This method processes an incoming inquiry message and updates the state of the inquiry
 * based on its current state. If the inquiry state is RECEIVED, it sends a quote and notifies
 * the listeners. If the inquiry state is QUOTED, it updates the state to DONE and notifies
 * the listeners.
 * 
 * @tparam T The type of the product associated with the inquiry.
 * @param data The inquiry data to be processed.
 */
template <typename T>
void InquiryService<T>::OnMessage(Inquiry<T>& data)
{
    inquiries[data.GetInquiryId()] = data;
    if (data.GetState() == RECEIVED)
    {
        string inquiryId = data.GetInquiryId();
        this->SendQuote(inquiryId, 100.0);
        Service<string, Inquiry<T> >::Notify(data);
    }
    else if (data.GetState() == QUOTED)
    {
        data.SetState(DONE);
        Service<string, Inquiry<T> >::Notify(data);
    }
}

/**
 * @brief Sends a quote for a specific inquiry by setting its price.
 * 
 * @tparam T The type of the inquiry.
 * @param inquiryId The ID of the inquiry to which the quote is being sent.
 * @param price The price to be set for the inquiry.
 */
template <typename T>
void InquiryService<T>::SendQuote(const string& inquiryId, double price)
{
    inquiries[inquiryId].SetPrice(price);
}

/**
 * @brief Rejects an inquiry by setting its state to REJECTED.
 * 
 * @tparam T The type of the inquiry.
 * @param inquiryId The ID of the inquiry to be rejected.
 */
template <typename T>
void InquiryService<T>::RejectInquiry(const string& inquiryId)
{
    inquiries[inquiryId].SetState(REJECTED);
}


#endif
