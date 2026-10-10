#!/usr/bin/env bash
set -euo pipefail
cmake -S cpp -B cpp/build -DCMAKE_BUILD_TYPE=Release
cmake --build cpp/build -j
if command -v perf >/dev/null 2>&1; then perf stat -e cycles,instructions,cache-references,cache-misses ./cpp/build/orderbook_tests; else echo "perf not installed; run on Linux with perf available"; fi
if command -v valgrind >/dev/null 2>&1; then valgrind --leak-check=full --error-exitcode=1 ./cpp/build/orderbook_tests; else echo "valgrind not installed; skipping memcheck"; fi
