#ifndef EXCHANGE_MANAGER_H
#define EXCHANGE_MANAGER_H

#include "Exchange.h"
#include "Dynamic_array.h"
#include <fstream>

class ExchangeManager {
private:
    Dynamic_array<Exchange*> exchanges;
    int nextExchangeId = 1;

public:
    void addExchange(Exchange* exchange);
    const Dynamic_array<Exchange*>& getAllExchanges() const;
    int getNextExchangeId() { return nextExchangeId++; }

    /*void saveToFile(const char* filename) const;
    void loadFromFile(const char* filename);*/
};

#endif