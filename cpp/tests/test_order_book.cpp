#include "orderbook/order_book.hpp"
#include "orderbook/spsc_queue.hpp"
#include <cassert>
#include <iostream>
int main(){
 ob::OrderBook b; b.add({1,ob::Side::Buy,100,10}); b.add({2,ob::Side::Buy,101,4}); b.add({3,ob::Side::Sell,105,8});
 assert(b.best_bid().value()==101 && b.best_ask().value()==105);
 auto r=b.add({4,ob::Side::Sell,100,6}); assert(r.trades.size()==2); assert(r.trades[0].maker_id==2 && r.trades[0].quantity==4); assert(r.trades[1].maker_id==1 && r.trades[1].quantity==2); assert(b.order_count()==1);
 assert(b.cancel(3)); assert(!b.cancel(3));
 ob::SPSCQueue<int,8> q; assert(q.push(42)); int v=0; assert(q.pop(v)&&v==42); assert(!q.pop(v));
 std::cout<<"OrderBook tests passed\n";
}
