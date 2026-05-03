#ifndef PIZZA_H
#define PIZZA_H

#include "product.h"

class Pizza : public Product
{
private:
    int diameter;

public:
    Pizza(QString n = "", int p = 0, int d = 30);

    QString getInfo() const override;
};

#endif