import pandas as pd
import pytest
from orderbook import quote_metrics, kyle_lambda, order_arrival_rate

def test_quote_metrics():
 q=pd.DataFrame({"timestamp":["2025-01-01T00:00:00Z","2025-01-01T00:00:01Z"],"bid":[100,101],"ask":[102,103],"bid_size":[10,5],"ask_size":[5,5]})
 out=quote_metrics(q); assert list(out.spread)==[2,2]; assert out.queue_imbalance.iloc[0]==pytest.approx(1/3)
def test_crossed_quote_rejected():
 q=pd.DataFrame({"timestamp":["2025-01-01"],"bid":[102],"ask":[101],"bid_size":[1],"ask_size":[1]})
 with pytest.raises(ValueError): quote_metrics(q)
def test_kyle_lambda():
 t=pd.DataFrame({"signed_volume":[-1,0,1],"price_change":[-2,0,2]}); assert kyle_lambda(t)==pytest.approx(2)
def test_arrival_rate():
 e=pd.DataFrame({"timestamp":["2025-01-01T00:00:00Z","2025-01-01T00:00:02Z","2025-01-01T00:00:04Z"]}); assert order_arrival_rate(e)==pytest.approx(.5)
