# PnL Calculator

A C++ implementation of a Profit and Loss (PnL) calculator that supports different accounting methods (FIFO and LIFO) for trade matching.

## Project Structure

```
pnl_calculator/
├── include/
│   ├── container.h      # Container implementations (RingBuffer and Stack)
│   ├── pnl_calculator.h # Main PnL calculation logic
│   └── type_def.h       # Type definitions and enums
├── src/
│   └── main.cpp         # Main program entry point
├── tests/
│   └── test_pnl_calculator.cpp  # Google Test test cases
└── CMakeLists.txt       # CMake build configuration
```

## Features

- Support for both FIFO (First In, First Out) and LIFO (Last In, First Out) accounting methods
- CSV file input/output for trade data and PnL results
- Template-based design for flexible container implementation
- Comprehensive test coverage using Google Test

## Usage

### Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests

```bash
cd build
./test_pnl_calculator
```

### Running the PnL Calculator

```bash
cd build
./pnl_calculator <input_file> <accounting_method>
```

Where:
- `input_file`: Path to the CSV file containing trade data
- `accounting_method`: Either "fifo" or "lifo" to specify the accounting method

Example:
```bash
# Using FIFO accounting
./pnl_calculator trades.csv fifo

# Using LIFO accounting
./pnl_calculator trades.csv lifo
```

The program will process the trades and output the PnL results to `pnl_output.csv` in the current directory.

### Input Format

The input CSV file should have the following format:
```
TIMESTAMP,SYMBOL,BUY_OR_SELL,PRICE,QUANTITY
101,TFS,B,11.00,15.0
102,TFS,S,13.00,20.0
```

### Output Format

The output CSV file (pnl_output.csv) will contain:
```
TIMESTAMP,SYMBOL,PNL
102,TFS,32.50
```

## Design Decisions

### Template Programming in Header Files

The main PnL calculation logic is implemented in header files rather than source files due to the use of template programming. This approach offers several benefits:

1. **Compile-time Type Resolution**: By using templates, we can determine the exact types at compile time, which can lead to better performance as the compiler can optimize the code more effectively.

2. **Flexibility**: The template-based design allows us to easily switch between different container implementations (FIFO vs LIFO) without changing the core logic.

3. **Code Organization**: While this approach might increase compile times, it provides better code organization and makes the relationship between components more explicit.

### Performance Considerations

1. **Immediate Output vs Batch Processing**
   - Current Implementation: PnL results are written to the output file immediately after each match
   - Trade-off: While this approach might be slower due to frequent I/O operations, it prevents memory accumulation in high-throughput scenarios
   - Alternative: We could store all results in memory and write them at the end, which would be faster but could consume significant memory in production environments

2. **Container Choice**
   - Current Implementation: Custom resizable RingBuffer for FIFO and Stack for LIFO
   - For latency-sensitive applications: Consider using [atomic-queue](https://github.com/Jiahao-Fang/atomic-queue), which provides high-performance lock-free queue implementations optimized for concurrent access

3. **Ring Buffer Size Limitation**
   - The current implementation uses a simple ring buffer for FIFO and stack for LIFO
   - The ring buffer size is limited to powers of 2 (e.g., 2, 4, 8, 16, etc.) for efficient modulo operations
   - This limitation is a personal preference and can be modified by changing the container implementation
