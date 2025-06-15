#ifndef EXCHANGE_MANAGER_H
#define EXCHANGE_MANAGER_H

#include "Exchange.h"
#include "Dynamic_array.h"
#include <fstream>

class ExchangeManager {
private:
    Dynamic_array<Exchange*> exchanges;

public:
    void addExchange(Exchange* exchange);
    const Dynamic_array<Exchange*>& getAllExchanges() const;

    void saveToFile(const char* filename) const;
    void loadFromFile(const char* filename);
};

#endif
