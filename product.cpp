#include "product.h"

Product::Product(QString n, int p)
{
    name = n;
    price = p;
}

QString Product::getName() const
{
    return name;
}

int Product::getPrice() const
{
    return price;
}

QString Product::getInfo() const
{
    return name + " - " + QString::number(price) + " грн";
}

// ================= ПЕРЕВАНТАЖЕННЯ ОПЕРАТОРІВ =================
bool Product::operator==(const Product& other) const
{
    return name == other.name &&
           price == other.price;
}
bool Product::operator<(const Product& other) const
{
    return price < other.price;
}

// ================= ПЕРЕВАНТАЖЕННЯ ФУНКЦІЙ =================
void Product::setPrice(int p)
{
    price = p;
}
void Product::setPrice(double p)
{
    price = static_cast<int>(p);
}