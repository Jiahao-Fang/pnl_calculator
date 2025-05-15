#include <gtest/gtest.h>
#include "pnl_calculator.h"
#include "type_def.h"
#include <sstream>
#include <fstream>
#include <filesystem>

namespace pnl {
namespace test {

// Helper class to read CSV output
class CSVReader {
public:
    struct PnLRecord {
        int timestamp;
        std::string symbol;
        double pnl;
    };

    static std::vector<PnLRecord> readCSV(const std::string& filename) {
        std::vector<PnLRecord> records;
        std::ifstream file(filename);
        std::string line;

        // Skip header
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string field;
            
            // Parse timestamp
            std::getline(iss, field, ',');
            int timestamp = std::stoi(field);

            // Parse symbol
            std::getline(iss, field, ',');
            std::string symbol = field;

            // Parse PnL
            std::getline(iss, field, ',');
            double pnl = std::stod(field);

            records.push_back({timestamp, symbol, pnl});
        }

        return records;
    }
};

// Test fixture for PnL calculator tests
class PnLCalculatorTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {
        // Clean up output file after each test
        std::filesystem::remove("pnl_output.csv");
    }
};

TEST(TradeTest, FromCSV) {
    std::string line = "101,TFS,B,11.00,15.0";
    Trade trade = Trade::fromCSV(line);
    
    EXPECT_EQ(trade.timestamp, 101);
    EXPECT_EQ(trade.symbol, "TFS");
    EXPECT_EQ(trade.side, Side::BUY);
    EXPECT_DOUBLE_EQ(trade.price, 11.00);
    EXPECT_DOUBLE_EQ(trade.quantity, 15.0);
}

TEST_F(PnLCalculatorTest, FIFOExample) {
    // Create test input file
    std::ofstream input_file("test_input.csv");
    input_file << "TIMESTAMP,SYMBOL,BUY_OR_SELL,PRICE,QUANTITY" << std::endl;
    input_file << "101,TFS,B,11.00,15.0" << std::endl;
    input_file << "102,TFS,B,12.50,15.0" << std::endl;
    input_file << "103,TFS,S,13.00,20.0" << std::endl;
    input_file << "104,TFS,S,12.75,10.0" << std::endl;
    input_file.close();

    // Process trades
    FIFOPnLCalculator calculator;
    calculator.processTrades("test_input.csv");
    
    // Read and verify results
    auto results = CSVReader::readCSV("pnl_output.csv");
    ASSERT_EQ(results.size(), 2);
    
    EXPECT_EQ(results[0].timestamp, 103);
    EXPECT_EQ(results[0].symbol, "TFS");
    EXPECT_DOUBLE_EQ(results[0].pnl, 32.50);
    
    EXPECT_EQ(results[1].timestamp, 104);
    EXPECT_EQ(results[1].symbol, "TFS");
    EXPECT_DOUBLE_EQ(results[1].pnl, 2.50);

    // Clean up test input file
    std::filesystem::remove("test_input.csv");
}

TEST_F(PnLCalculatorTest, LIFOExample) {
    // Create test input file
    std::ofstream input_file("test_input.csv");
    input_file << "TIMESTAMP,SYMBOL,BUY_OR_SELL,PRICE,QUANTITY" << std::endl;
    input_file << "101,TFS,B,11.00,15.0" << std::endl;
    input_file << "102,TFS,B,12.50,15.0" << std::endl;
    input_file << "103,TFS,S,13.00,20.0" << std::endl;
    input_file << "104,TFS,S,12.75,10.0" << std::endl;
    input_file.close();

    // Process trades
    LIFOPnLCalculator calculator;
    calculator.processTrades("test_input.csv");
    
    // Read and verify results
    auto results = CSVReader::readCSV("pnl_output.csv");
    ASSERT_EQ(results.size(), 2);
    
    EXPECT_EQ(results[0].timestamp, 103);
    EXPECT_EQ(results[0].symbol, "TFS");
    EXPECT_DOUBLE_EQ(results[0].pnl, 17.50);
    
    EXPECT_EQ(results[1].timestamp, 104);
    EXPECT_EQ(results[1].symbol, "TFS");
    EXPECT_DOUBLE_EQ(results[1].pnl, 17.50);

    // Clean up test input file
    std::filesystem::remove("test_input.csv");
}

TEST_F(PnLCalculatorTest, ZeroPnLMatch) {
    // Create test input file with matching prices
    std::ofstream input_file("test_input.csv");
    input_file << "TIMESTAMP,SYMBOL,BUY_OR_SELL,PRICE,QUANTITY" << std::endl;
    input_file << "101,TFS,B,10.00,10.0" << std::endl;
    input_file << "102,TFS,S,10.00,10.0" << std::endl;
    input_file.close();

    // Process trades
    FIFOPnLCalculator calculator;
    calculator.processTrades("test_input.csv");
    
    // Read and verify results
    auto results = CSVReader::readCSV("pnl_output.csv");
    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].timestamp, 102);
    EXPECT_EQ(results[0].symbol, "TFS");
    EXPECT_DOUBLE_EQ(results[0].pnl, 0.00);

    // Clean up test input file
    std::filesystem::remove("test_input.csv");
}

TEST_F(PnLCalculatorTest, MultipleSymbols) {
    // Create test input file with multiple symbols
    std::ofstream input_file("test_input.csv");
    input_file << "TIMESTAMP,SYMBOL,BUY_OR_SELL,PRICE,QUANTITY" << std::endl;
    input_file << "101,TFS,B,10.00,10.0" << std::endl;
    input_file << "102,ABC,B,20.00,5.0" << std::endl;
    input_file << "103,TFS,S,11.00,10.0" << std::endl;
    input_file << "104,ABC,S,21.00,5.0" << std::endl;
    input_file.close();

    // Process trades
    FIFOPnLCalculator calculator;
    calculator.processTrades("test_input.csv");
    
    // Read and verify results
    auto results = CSVReader::readCSV("pnl_output.csv");
    ASSERT_EQ(results.size(), 2);
    
    EXPECT_EQ(results[0].timestamp, 103);
    EXPECT_EQ(results[0].symbol, "TFS");
    EXPECT_DOUBLE_EQ(results[0].pnl, 10.00);
    
    EXPECT_EQ(results[1].timestamp, 104);
    EXPECT_EQ(results[1].symbol, "ABC");
    EXPECT_DOUBLE_EQ(results[1].pnl, 5.00);

    // Clean up test input file
    std::filesystem::remove("test_input.csv");
}

} // namespace test
} // namespace pnl 