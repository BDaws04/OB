Participant:
- individual who interacts with the market
- places orders, either GTC, IOC or FOK
placeOrder(market&, OrderRequest&);

OrderRequest:
- struct containing Price, Volume, flags

Market:
- Queue for processing orders
- Buy and Sell book
- Statistics for orders processed

105
104
103
...

105
104