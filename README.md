# Limit Order Book (LOB) Simulator

> A high-performance, multithreaded Limit Order Book simulation engine written in C++.  
> Designed for modeling order flow, market dynamics, and benchmarking trading strategies.

---

## 🚀 Features

- **Multithreaded Participants:** Each simulated market participant runs in its own thread, placing thousands of orders concurrently.
- **Order Types Supported:** GTC (Good-Til-Cancel), IOC (Immediate-Or-Cancel), FOK (Fill-Or-Kill).
- **Configurable Market Parameters:** Peg price, price levels, and participant count.
- **Real-time Market Simulation:** Continuously matches and processes orders.
- **Extensible Design:** Modular architecture for easy customization and extension.

---

## 📦 Installation & Setup

### Prerequisites

- C++17 compatible compiler (GCC, Clang, or MSVC)
- CMake (optional but recommended)

### Build Instructions

```bash
git clone https://github.com/BDaws04/OB.git
cd OB

# Using g++
g++ -std=c++17 -o benchmark benchmark.cpp -pthread

# Or using CMake
make

