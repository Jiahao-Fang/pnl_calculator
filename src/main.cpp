#include "pnl_calculator.h"
#include <iostream>
#include <stdexcept>

void printUsage() {
    std::cerr << "Usage: pnl_calculator <input_file> <accounting_method>" << std::endl;
    std::cerr << "  accounting_method: fifo or lifo" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage();
        return 1;
    }

    std::string inputFile = argv[1];
    std::string method = argv[2];

    try {
        if (method == "fifo") {
            pnl::FIFOPnLCalculator calculator;
            calculator.processTrades(inputFile);
        } else if (method == "lifo") {
            pnl::LIFOPnLCalculator calculator;
            calculator.processTrades(inputFile);
        } else {
            std::cerr << "Error: accounting method must be either 'fifo' or 'lifo'" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 