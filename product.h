#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product
{
protected:
    QString name;
    int price;

public:
    Product(QString n = "", int p = 0);

    QString getName() const;
    int getPrice() const;
    void setPrice(int p);
    void setPrice(double p);

    virtual QString getInfo() const;
    bool operator==(const Product& other) const;
    bool operator<(const Product& other) const;
};

#endif