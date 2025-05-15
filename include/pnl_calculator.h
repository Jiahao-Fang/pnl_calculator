#pragma once

#include "type_def.h"
#include "container.h"
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <fstream>
#include <iomanip>

namespace pnl {
template<typename Container>
class PnLCalculator {
public:
    PnLCalculator() {
        // Open output file and write header
        output_file_.open("pnl_output.csv");
        output_file_ << "TIMESTAMP,SYMBOL,PNL" << std::endl;
    }

    ~PnLCalculator() {
        if (output_file_.is_open()) {
            output_file_.close();
        }
    }

    // Process multiple trades from a file
    void processTrades(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        // Skip header
        std::getline(file, line);

        while (std::getline(file, line)) {
            Trade trade = Trade::fromCSV(line);
            processTrade(trade);
        }
    }

private:
    // Output PnL to CSV file
    void outputPnL(const PnLResult& result) {
        output_file_ << result.timestamp << ","
                    << result.symbol << ","
                    << std::fixed << std::setprecision(2) << result.pnl
                    << std::endl;
    }

    // Process a single trade
    void processTrade(const Trade& trade) {
        if (trade.side == Side::BUY) {
            processBuy(trade);
        } else {
            processSell(trade);
        }
    }

    // Process a buy trade
    void processBuy(const Trade& trade) {
        auto& storage = positions_[trade.symbol];
        
        if (storage.empty()) {
            // No existing positions, just add the buy
            storage.push({trade.price, trade.quantity});
            return; // No match, return
        }

        // Check if existing position is also a buy
        auto& front = storage.front();
        if (front.quantity > 0) {
            // Same direction, just add to position
            storage.push({trade.price, trade.quantity});
            return; // No match, return
        }

        // Different direction, need to match
        double remainingQuantity = trade.quantity;
        double totalPnL = 0.0;
        bool hasMatch = false;

        while (remainingQuantity > 0 && !storage.empty()) {
            auto& position = storage.front();
            double matchedQuantity = std::min(remainingQuantity, std::abs(position.quantity));
            
            double pnl = matchedQuantity * (position.price - trade.price);
            totalPnL += pnl;
            hasMatch = true;

            position.quantity += matchedQuantity;
            remainingQuantity -= matchedQuantity;

            if (position.quantity == 0) {
                storage.pop();
            }
        }

        if (hasMatch) {
            outputPnL({trade.timestamp, trade.symbol, totalPnL});
            if (remainingQuantity > 0) {
                storage.push({trade.price, remainingQuantity});
            }
            return;
        }

        if (remainingQuantity > 0) {
            storage.push({trade.price, remainingQuantity});
        }
    }

    // Process a sell trade
    void processSell(const Trade& trade) {
        auto& storage = positions_[trade.symbol];
        
        if (storage.empty()) {
            // No existing positions, just add the sell
            storage.push({trade.price, -trade.quantity});
            return; // No match, return
        }

        // Check if existing position is also a sell
        auto& front = storage.front();
        if (front.quantity < 0) {
            // Same direction, just add to position
            storage.push({trade.price, -trade.quantity});
            return; // No match, return
        }

        // Different direction, need to match
        double remainingQuantity = trade.quantity;
        double totalPnL = 0.0;
        bool hasMatch = false;

        while (remainingQuantity > 0 && !storage.empty()) {
            auto& position = storage.front();
            double matchedQuantity = std::min(remainingQuantity, position.quantity);
            
            double pnl = matchedQuantity * (trade.price - position.price);
            totalPnL += pnl;
            hasMatch = true;

            position.quantity -= matchedQuantity;
            remainingQuantity -= matchedQuantity;

            if (position.quantity == 0) {
                storage.pop();
            }
        }

        if (hasMatch) {
            outputPnL({trade.timestamp, trade.symbol, totalPnL});
            if (remainingQuantity > 0) {
                storage.push({trade.price, -remainingQuantity});
            }
            return;
        }

        if (remainingQuantity > 0) {
            storage.push({trade.price, -remainingQuantity});
        }
    }

    std::unordered_map<std::string, Container> positions_;
    std::ofstream output_file_;
};

// Type aliases for specific implementations
using FIFOPnLCalculator = PnLCalculator<RingBuffer>;
using LIFOPnLCalculator = PnLCalculator<Stack>;

} // namespace pnl 