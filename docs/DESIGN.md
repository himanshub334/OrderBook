# Design notes

- Price levels are held in ordered maps. The best price is available from the first level in each map (bids use descending order, asks ascending order).
- Each price level stores a FIFO `std::list<Order>` to preserve price-time priority. An unordered locator maps order IDs to list iterators, so cancellation is average O(1) after lookup and level removal is O(log L) when a level becomes empty. Adding at a new price is O(log L); adding to an existing level is average O(1).
- Matching walks the opposing best levels until the incoming order is filled or the best price no longer crosses. A marketable order can generate multiple trades.
- The SPSC queue is only safe for exactly one producer and one consumer. It uses acquire/release publication and cache-line separation of head/tail; its usable capacity is `Capacity - 1`.
- Python analytics calculate quote spread, mid-price, queue imbalance, simple mid returns, order arrival rate, and a basic OLS Kyle's-lambda estimate.

## Not claimed as completed

The default build does not integrate the optional pybind11 module, and the order book currently uses standard allocator-backed `std::list` rather than a custom pool allocator. The included benchmark measures Python analytics only; it does not validate C++ throughput, L1 miss rates, or latency claims. Run Linux `perf stat` and Valgrind locally to obtain machine-specific measurements before publishing benchmark figures.
