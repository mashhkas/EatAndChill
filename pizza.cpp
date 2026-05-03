#include "pizza.h"

Pizza::Pizza(QString n, int p, int d)
    : Product(n, p)
{
    diameter = d;
}

QString Pizza::getInfo() const
{
    return name + " | " +
           QString::number(diameter) + " см | " +
           QString::number(price) + " грн";
}