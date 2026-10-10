# OrderBook — High-Performance Order Book & Microstructure Analytics

C++17 limit order book and Python market-microstructure analytics library for educational strategy research. It is a simulator/library, not an exchange connectivity product and not financial advice.

## Features

- Price-time priority matching with partial fills
- Ordered bid/ask price levels and FIFO queues
- Best bid/ask and depth snapshots
- Order cancellation using an order-ID locator
- Cache-line-separated SPSC notification queue template
- Python/pandas quote metrics: spread, mid-price, queue imbalance, mid returns
- Kyle's lambda OLS estimator and order arrival-rate helper
- CMake tests, pytest suite, CI workflow, Linux profiling helper
- Optional pybind11 binding scaffold

## Build C++

```bash
cmake -S cpp -B cpp/build -DCMAKE_BUILD_TYPE=Release
cmake --build cpp/build -j
ctest --test-dir cpp/build --output-on-failure
./cpp/build/orderbook_demo
```

## Python

```bash
python -m venv .venv
# Linux/macOS: source .venv/bin/activate
# Windows PowerShell: .venv\Scripts\Activate.ps1
pip install -r requirements.txt
PYTHONPATH=python pytest -q
```

## Profiling on Linux

```bash
./scripts/profile_linux.sh
```

Run Valgrind and `perf stat` on a Linux host where the tools and permissions are available. Results depend on CPU, compiler, kernel, workload, and build flags.

## Optional pybind11

`bindings/module.cpp` is a binding scaffold and is not wired into the default build/package. Install pybind11 and add a module target to CMake before importing it from Python.

## Complexity notes

Let `L` be the number of distinct price levels and `K` the number of orders matched by an incoming order. New price level insertion is O(log L); appending to an existing level is average O(1); cancellation lookup/removal is average O(1), plus O(log L) if the emptied level is removed; best bid/ask lookup is O(1); matching cost depends on the number of fills/levels traversed.

## Benchmark honesty

No fixed latency, throughput, or L1 cache-miss improvement is claimed by this repository. The reported figures in any resume description (19%→4.2% L1 miss rate, 420 ns→85 ns, 500k orders/sec, zero leaks) must be reproduced on a documented Linux machine before being presented as measured results. The current C++ implementation uses `std::list` and does not yet implement the described custom pool allocator.
# OrderBook
