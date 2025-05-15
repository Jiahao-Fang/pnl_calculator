#pragma once

#include <string>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>

namespace pnl {

// Trade side enum
enum class Side {
    BUY,
    SELL
};

// Accounting method enum
enum class AccountingMethod {
    FIFO,
    LIFO
};

// Trade structure
struct Trade {
    int timestamp;
    std::string symbol;
    Side side;
    double price;
    double quantity;

    // Read a trade from CSV line
    static Trade fromCSV(const std::string& line) {
        std::istringstream iss(line);
        std::string field;
        
        // Skip header if it's the first line
        if (line.find("TIMESTAMP") != std::string::npos) {
            throw std::runtime_error("Header line should be skipped before calling fromCSV");
        }

        // Parse timestamp
        std::getline(iss, field, ',');
        int timestamp = std::stoi(field);

        // Parse symbol
        std::getline(iss, field, ',');
        std::string symbol = field;

        // Parse buy/sell
        std::getline(iss, field, ',');
        Side side = (field == "B") ? Side::BUY : Side::SELL;

        // Parse price
        std::getline(iss, field, ',');
        double price = std::stod(field);

        // Parse quantity
        std::getline(iss, field, ',');
        double quantity = std::stod(field);

        return Trade{timestamp, symbol, side, price, quantity};
    }
};

// Position structure to track remaining shares
struct Position {
    double price;
    double quantity;
};

// PnL result structure
struct PnLResult {
    int timestamp;
    std::string symbol;
    double pnl;
};

} // namespace pnl 