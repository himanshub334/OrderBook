#include "orderbook/order_book.hpp"
#include "orderbook/spsc_queue.hpp"
#include <iostream>
int main(){ ob::OrderBook b; b.add({1,ob::Side::Buy,10000,10}); b.add({2,ob::Side::Sell,10100,5}); auto r=b.add({3,ob::Side::Buy,10100,7}); std::cout<<"trades="<<r.trades.size()<<" remaining="<<r.remaining<<" best_bid="<<*b.best_bid()<<"\n"; return 0; }
