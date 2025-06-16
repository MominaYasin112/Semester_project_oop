#include "ExchangeManager.h"

void ExchangeManager::addExchange(Exchange* exchange) {
    if (!exchange) {
        std::cout << "DEBUG: Tried to add null exchange\n";
        return;
    }
    std::cout << "DEBUG: Adding exchange ID: " << exchange->getExchangeId();
    exchanges.push(exchange);
}
const Dynamic_array<Exchange*>& ExchangeManager::getAllExchanges() const {
    return exchanges;
}

//void ExchangeManager::saveToFile(const char* filename) const {
//    ofstream out(filename, ios::binary);
//    int count = exchanges.size();
//    out.write((char*)&count, sizeof(count));
//    for (int i = 0; i < count; ++i) {
//        exchanges[i]->writeToBinary(out);
//    }
//    out.close();
//}
//
//void ExchangeManager::loadFromFile(const char* filename) {
//    ifstream in(filename, ios::binary);
//    if (!in) return;
//
//    int count;
//    in.read((char*)&count, sizeof(count));
//    for (int i = 0; i < count; ++i) {
//        Exchange* ex = new Exchange();
//        ex->readFromBinary(in);
//        exchanges.push(ex);
//    }
//    in.close();
//}
