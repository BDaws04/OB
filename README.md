# L2 Limit Order Book Simulation

A high performance L2 Limit Order Book that handles Market, IOC, GTC and FOK orders

### Compiling and Building

To compile and build the project, ensure you have a C++17 compatible compiler installed (e.g., `g++` or `clang++`).

### Using g++

Run the following command in your terminal from the project root directory:

```bash
g++ -std=c++17 -pthread -o benchmark benchmark.cpp
```

```CMake
mkdir build
cd build
cmake ..
make
```

### Benchmark:
Computer Specs: 
- CPU: Intel(R) Core(TM) i5-8250U CPU @ 1.60GHz
- RAM: 8GB
- Compiler: g++ version 15.1.0

Performance:
The simulation models realistic order probabilities and distributions:
e.g: GTC = 70%, IOC = 15%, FOK = 5%, Market = 10%
- 1M orders ~0.6s

### Order Book Architecture

This project implements a **high-performance order book** optimized for low-latency trading.

### Core Design

- The order book manages **buy** and **sell** sides separately, each represented by a `Book` instance.
- Each side holds a vector of `PriceLevel` objects, where each price level tracks the **total available volume** at that price and a vector of <order_id, volume> pairs.
- The `Book` keeps track of the **best price index**, which points to the best bid or ask level in the vector.

### How Orders Are Processed

- **Market orders** are filled starting from the current best price level:
  - For **buy market orders**, the book scans price levels *downwards* from the best sell price, removing volume until the order is fully filled or no liquidity remains.
  - For **sell market orders**, the scan goes *upwards* from the best buy price, similarly removing volume.
- Volumes at price levels are updated efficiently with minimal overhead.
- The best price index is updated dynamically as price levels fill or empty.

### Performance Highlights

- Using **indexed vectors** for price levels allows *fast random access* and *cache-friendly iteration*.
- Tracking the best price as an **index** enables *constant-time access* to critical price levels.
- The design avoids complex data structures, focusing on **simplicity and speed**.
- Volume updates happen directly on aggregated price levels, not individual orders, reducing computational overhead.

### Summary

This architecture balances **simplicity** and **performance**, making it suitable for real-time trading systems that require fast order matching and book updates.

