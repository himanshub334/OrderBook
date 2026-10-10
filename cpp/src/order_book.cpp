#include "orderbook/order_book.hpp"
#include <algorithm>
#include <stdexcept>
namespace ob {
std::uint64_t OrderBook::level_qty(const Queue& q) { std::uint64_t n=0; for (const auto& o:q) n+=o.quantity; return n; }
AddResult OrderBook::add(Order order) {
 if (!order.id || !order.quantity || locations_.count(order.id)) throw std::invalid_argument("order id must be unique and quantity positive");
 AddResult out; auto remaining=order.quantity;
 if (order.side==Side::Buy) {
  while (remaining && !asks_.empty() && asks_.begin()->first<=order.price) {
   auto level=asks_.begin(); auto it=level->second.begin(); auto fill=std::min(remaining,it->quantity);
   out.trades.push_back({it->id,order.id,it->price,fill}); remaining-=fill; it->quantity-=fill;
   if (!it->quantity) { locations_.erase(it->id); level->second.erase(it); }
   if (level->second.empty()) asks_.erase(level);
  }
  if (remaining) { order.quantity=remaining; auto& q=bids_[order.price]; q.push_back(order); auto it=std::prev(q.end()); locations_.emplace(order.id,Location{Side::Buy,order.price,it}); }
 } else {
  while (remaining && !bids_.empty() && bids_.begin()->first>=order.price) {
   auto level=bids_.begin(); auto it=level->second.begin(); auto fill=std::min(remaining,it->quantity);
   out.trades.push_back({it->id,order.id,it->price,fill}); remaining-=fill; it->quantity-=fill;
   if (!it->quantity) { locations_.erase(it->id); level->second.erase(it); }
   if (level->second.empty()) bids_.erase(level);
  }
  if (remaining) { order.quantity=remaining; auto& q=asks_[order.price]; q.push_back(order); auto it=std::prev(q.end()); locations_.emplace(order.id,Location{Side::Sell,order.price,it}); }
 }
 out.remaining=remaining; return out;
}
bool OrderBook::cancel(std::uint64_t id) {
 auto found=locations_.find(id); if(found==locations_.end()) return false; auto loc=found->second;
 if(loc.side==Side::Buy) { auto level=bids_.find(loc.price); level->second.erase(loc.it); if(level->second.empty()) bids_.erase(level); }
 else { auto level=asks_.find(loc.price); level->second.erase(loc.it); if(level->second.empty()) asks_.erase(level); }
 locations_.erase(found); return true;
}
std::optional<std::int64_t> OrderBook::best_bid() const { if(bids_.empty()) return {}; return bids_.begin()->first; }
std::optional<std::int64_t> OrderBook::best_ask() const { if(asks_.empty()) return {}; return asks_.begin()->first; }
std::vector<Level> OrderBook::depth(Side side,std::size_t limit) const {
 std::vector<Level> out; if(side==Side::Buy) { for(auto& [p,q]:bids_) { if(out.size()>=limit) break; out.push_back({p,level_qty(q),q.size()}); } }
 else { for(auto& [p,q]:asks_) { if(out.size()>=limit) break; out.push_back({p,level_qty(q),q.size()}); } } return out;
}
}
