#include "drink.h"

Drink::Drink(QString n, int p, bool c)
    : Product(n, p)
{
    cold = c;
}

QString Drink::getInfo() const
{
    QString type;

    if(cold)
        type = "Холодний";
    else
        type = "Гарячий";

    return name + " | " +
           type + " напій | " +
           QString::number(price) + " грн";
}