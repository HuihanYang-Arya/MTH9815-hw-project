#include "DataGenerator.hpp"
#include "AlgoExecutionService.hpp"
#include "AlgoStreamingService.hpp"
#include "Connectors.hpp"
#include "ExecutionService.hpp"
#include "GUIService.hpp"
#include "HistoricalDataService.hpp"
#include "InquiryService.hpp"
#include "Listeners.hpp"
#include "MarketDataService.hpp"
#include "PositionService.hpp"
#include "PricingService.hpp"
#include "Products.hpp"
#include "RiskService.hpp"
#include "SOA.hpp"
#include "StreamingService.hpp"
#include "TradeBookingService.hpp"

using namespace std;

/**
 * @brief Generates various types of financial data and saves them to files.
 * 
 * This function calls several other functions to generate different types of 
 * financial data including trades, prices, market data, and inquiries. The 
 * generated data is saved to corresponding text files in the "data_generated" 
 * directory.
 * 
 * The following files are generated:
 * - trades.txt: Contains generated trade data.
 * - prices.txt: Contains generated price data.
 * - marketdata.txt: Contains generated market data.
 * - inquiries.txt: Contains generated inquiry data.
 */
void Generate_Data()
{
    Generate_Trades("data_generated/trades.txt");

    Generate_Prices("data_generated/prices.txt");

    Generate_Mktdata("data_generated/marketdata.txt");

    Generate_Inquiry("data_generated/inquiries.txt");
}


/**
 * @file main.cpp
 * @brief Main entry point for the bond trading system.
 *
 * This program sets up various services and listeners to process bond trading data,
 * including trade booking, position, risk, pricing, market data, execution, and inquiry services.
 * It links these services together using listeners and connectors to process data from input files
 * and generate output files.
 *
 * The main steps include:
 * 1. Generating initial data.
 * 2. Setting up services and linking them via listeners.
 * 3. Processing price data to generate GUI and streaming outputs.
 * 4. Processing order book data to generate execution, position, and risk outputs.
 * 5. Processing inquiry data to generate an inquiry output.
 * 6. Subscribing connectors to input files to fire up the system.
 *
 * Input files:
 * - data_generated/trades.txt
 * - data_generated/prices.txt
 * - data_generated/marketdata.txt
 * - data_generated/inquiries.txt
 *
 * Output files:
 * - output/gui.txt
 * - output/streaming.txt
 * - output/executions.txt
 * - output/positions.txt
 * - output/risk.txt
 * - output/all_inquiries.txt
 */
int main()
{
    Generate_Data();       

    TradeBookingService<Bond> trade_booking_service;
    PositionService<Bond> position_service;
    PositionServiceListener<Bond> position_listener(&position_service);
    // The trade booking service should be linked to a position service via listener
    trade_booking_service.AddListener(&position_listener);

    RiskService<Bond> risk_service;
    RiskServiceListener<Bond> risk_service_listener(&risk_service);
    // The position service should be linked to a risk service via listener
    position_service.AddListener(&risk_service_listener);

    HistoricalPositionService<Bond> historical_position_service;
    HistoricalPositionListener<Bond> historical_position_listener(&historical_position_service);
    // Link the position service to the historical position listener
    position_service.AddListener(&historical_position_listener);

    HistoricalRiskService<Bond> historical_risk_service;
    HistoricalRiskListener<Bond> historical_risk_listener(&historical_risk_service);
    // Link the risk service to the historical risk listener
    risk_service.AddListener(&historical_risk_listener);

    /*
    * Process price data from data_generated/prices.txt
    * 
    * prices.txt -> pricing service -> GUI service -> output/gui.txt
    * prices.txt -> pricing service -> algo streaming service -> streaming service -> historical streaming service 
       -> output/streaming.txt
    */

    GUIService<Bond> gui_service;
    GUIServiceListener<Bond> gui_listener(&gui_service);
    PricingService<Bond> pricing_service;
    // Link the pricing service to the gui listener
    pricing_service.AddListener(&gui_listener);

    AlgoStreamingService<Bond> algo_streaming_service;
    AlgoStreamingServiceListener<Bond> algo_streaming_listener(&algo_streaming_service);
    // Link the pricing service to the algo streaming listener
    pricing_service.AddListener(&algo_streaming_listener);

    StreamingService<Bond> streaming_service;
    StreamingServiceListener<Bond> streaming_listener(&streaming_service);
    // Link the algo streaming service to the streaming listener
    algo_streaming_service.AddListener(&streaming_listener);

    HistoricalStreamingService<Bond> historical_streaming_service;
    HistoricalStreamingListener<Bond> historical_streaming_listener(&historical_streaming_service);
    // Link the streaming service to the historcial streaming listener
    streaming_service.AddListener(&historical_streaming_listener);

    /*
    * Process order book data from data_generated/marketdata.txt
    * Generate one file: output/executions.txt
    * Update two files: output/positions.txt and output/risk.txt
    * 
    * marketdata.txt -> market data service -> algo execution service -> execution service -> historical execution
        service -> executions.txt
    * marketdata.txt -> market data service -> algo execution service -> execution service -> trade booking service
        -> same as part (a)
    */

    MarketDataService<Bond> market_data_service;
    AlgoExecutionService<Bond> algo_execution_service;
    AlgoExecutionServiceListener<Bond> algo_execution_listener(&algo_execution_service);
    // Link the market data service to the algo execution listener
    market_data_service.AddListener(&algo_execution_listener);

    ExecutionService<Bond> execution_service;
    ExecutionServiceListener<Bond> execution_listener(&execution_service);
    // Link the algo execution service to the execution listener
    algo_execution_service.AddListener(&execution_listener);

    TradeBookingServiceListener<Bond> trade_booking_listener(&trade_booking_service);
    // Link the execution service to the trade booking listener
    execution_service.AddListener(&trade_booking_listener);

    HistoricalExecutionService<Bond> historical_execution_service;
    HistoricalExecutionListener<Bond> historical_execution_listener(&historical_execution_service);
    // Link the execution service to the historical execution listener
    execution_service.AddListener(&historical_execution_listener);

    /*
    *Process inquiry data from data_generated/inquires.txt
    * Generate one file: output/all_inquiries.txt
    * 
    * inquiries.txt -> inquiry service -> historical inquiry service -> all_inquiries.txt
    */

    InquiryService<Bond> inquiry_service;
    HistoricalInquiryService<Bond> historical_inquiry_service;
    HistoricalInquiryListener<Bond> historical_inquiry_listner(&historical_inquiry_service);
    inquiry_service.AddListener(&historical_inquiry_listner);


    // Run the system
    TradeBookingConnector<Bond> trade_connector(&trade_booking_service);
    PricingConnector<Bond> pricing_connector(&pricing_service);
    MarketDataConnector<Bond> market_data_connector(&market_data_service);
    InquiryConnector<Bond> inquiry_connector(&inquiry_service);

    trade_connector.Subscribe("data_generated/trades.txt");
    pricing_connector.Subscribe("data_generated/prices.txt");
    market_data_connector.Subscribe("data_generated/marketdata.txt");
    inquiry_connector.Subscribe("data_generated/inquiries.txt");

    return 0;
}
