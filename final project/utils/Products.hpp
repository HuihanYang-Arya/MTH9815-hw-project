#ifndef PRODUCTS_HPP
#define PRODUCTS_HPP

#include <iostream>
#include <string>
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace std;
using namespace boost::gregorian;

enum ProductType { IRSWAP, BOND };
enum BondIdType { CUSIP, ISIN };

/**
 * Base class for a product.
 */
class Product
{
public:
    // Constructor for a product
    Product() = default;
    Product(string _productId, ProductType _productType);

    // Get the product identifier
    const string& GetProductId() const;

    // Get the product type
    ProductType GetProductType() const;

private:
    string productId;
    ProductType productType;
};

/**
 * Bond product class
 */
class Bond : public Product
{
public:
    // Constructor for a bond
    Bond() = default;
    Bond(string _productId, BondIdType _bondIdType, string _ticker, double _coupon, date _maturityDate);

    // Get the ticker
    const string& GetTicker() const;

    // Get the coupon
    double GetCoupon() const;

    // Get the maturity date
    const date& GetMaturityDate() const;

    // Get the bond identifier type
    BondIdType GetBondIdType() const;

    // Print the bond
    friend ostream& operator<<(ostream& output, const Bond& bond);

private:
    BondIdType bondIdType;
    string ticker;
    double coupon;
    date maturityDate;
};

// Implementation of Product class methods
Product::Product(string _productId, ProductType _productType)
    : productId(_productId), productType(_productType) {}

const string& Product::GetProductId() const
{
    return productId;
}

ProductType Product::GetProductType() const
{
    return productType;
}

// Implementation of Bond class methods
Bond::Bond(string _productId, BondIdType _bondIdType, string _ticker, double _coupon, date _maturityDate)
    : Product(_productId, BOND), bondIdType(_bondIdType), ticker(_ticker), coupon(_coupon), maturityDate(_maturityDate) {}

const string& Bond::GetTicker() const
{
    return ticker;
}

double Bond::GetCoupon() const
{
    return coupon;
}

const date& Bond::GetMaturityDate() const
{
    return maturityDate;
}

BondIdType Bond::GetBondIdType() const
{
    return bondIdType;
}

ostream& operator<<(ostream& output, const Bond& bond)
{
    output << bond.ticker << " " << bond.coupon << " " << bond.GetMaturityDate();
    return output;
}

#endif // PRODUCTS_HPP
