"""Python analytics microbenchmark; does not measure the C++ matching engine."""
import time
import pandas as pd
from orderbook import quote_metrics
n=500_000
q=pd.DataFrame({"timestamp":pd.date_range("2025-01-01",periods=n,freq="ms",tz="UTC"),"bid":100.0,"ask":100.02,"bid_size":10,"ask_size":8})
t0=time.perf_counter(); out=quote_metrics(q); elapsed=time.perf_counter()-t0
print(f"rows={len(out)} elapsed_seconds={elapsed:.4f} rows_per_second={len(out)/elapsed:.0f}")
