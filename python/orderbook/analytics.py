"""Small pandas-based market microstructure analytics helpers."""
from __future__ import annotations
import numpy as np
import pandas as pd

def quote_metrics(quotes: pd.DataFrame) -> pd.DataFrame:
    """Input columns: timestamp, bid, ask, bid_size, ask_size."""
    required={"timestamp","bid","ask","bid_size","ask_size"}
    missing=required-set(quotes.columns)
    if missing: raise ValueError(f"missing columns: {sorted(missing)}")
    q=quotes.copy().sort_values("timestamp")
    if (q["ask"] < q["bid"]).any(): raise ValueError("crossed quote: ask < bid")
    q["mid"]=(q["bid"]+q["ask"])/2.0
    q["spread"]=q["ask"]-q["bid"]
    denom=q["bid_size"]+q["ask_size"]
    q["queue_imbalance"]=np.where(denom>0,(q["bid_size"]-q["ask_size"])/denom,np.nan)
    q["mid_return"]=q["mid"].pct_change()
    return q

def kyle_lambda(trades: pd.DataFrame) -> float:
    """OLS slope of price change on signed volume; expects signed_volume, price_change."""
    required={"signed_volume","price_change"}
    if not required.issubset(trades.columns): raise ValueError(f"expected columns: {sorted(required)}")
    x=trades["signed_volume"].to_numpy(dtype=float); y=trades["price_change"].to_numpy(dtype=float)
    mask=np.isfinite(x)&np.isfinite(y)
    x=x[mask]; y=y[mask]
    if len(x)<2 or np.var(x)==0: return float("nan")
    return float(np.cov(x,y,ddof=0)[0,1]/np.var(x))

def order_arrival_rate(events: pd.DataFrame, timestamp_col: str="timestamp") -> float:
    """Events per second over observed timestamp span; timestamps must be parseable."""
    if timestamp_col not in events: raise ValueError(f"missing {timestamp_col}")
    t=pd.to_datetime(events[timestamp_col],utc=True).dropna().sort_values()
    if len(t)<2: return 0.0
    seconds=(t.iloc[-1]-t.iloc[0]).total_seconds()
    return float((len(t)-1)/seconds) if seconds>0 else float("inf")
