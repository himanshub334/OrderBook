#pragma once
#include <cstdint>
#include <list>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ob {
enum class Side { Buy, Sell };
struct Order { std::uint64_t id{}; Side side{}; std::int64_t price{}; std::uint64_t quantity{}; };
struct Trade { std::uint64_t maker_id{}, taker_id{}; std::int64_t price{}; std::uint64_t quantity{}; };
struct Level { std::int64_t price{}; std::uint64_t quantity{}; std::size_t order_count{}; };
struct AddResult { std::vector<Trade> trades; std::uint64_t remaining{}; };
class OrderBook {
 using Queue = std::list<Order>;
 using Iterator = Queue::iterator;
 std::map<std::int64_t, Queue, std::greater<std::int64_t>> bids_;
 std::map<std::int64_t, Queue> asks_;
 struct Location { Side side; std::int64_t price; Iterator it; };
 std::unordered_map<std::uint64_t, Location> locations_;
 static std::uint64_t level_qty(const Queue& q);
public:
 AddResult add(Order order);
 bool cancel(std::uint64_t id);
 std::optional<std::int64_t> best_bid() const;
 std::optional<std::int64_t> best_ask() const;
 std::vector<Level> depth(Side side, std::size_t limit=10) const;
 std::size_t order_count() const { return locations_.size(); }
};
}
