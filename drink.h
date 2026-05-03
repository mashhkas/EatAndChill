#ifndef DRINK_H
#define DRINK_H

#include "product.h"

class Drink : public Product
{
private:
    bool cold;

public:
    Drink(QString n = "", int p = 0, bool c = true);

    QString getInfo() const override;
};

#endif