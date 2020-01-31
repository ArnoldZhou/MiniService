#include <iostream>
#include <memory>
#include "./config.h"
#include "./game_server/Game.h"
#include "./http_server/Server.h"
#include "./game_server/api/Api.h"
#include "./base/error/Code.h"
#include "./common/nlohmann/json.hpp"
#include "./common/ThreadPool.h"
#include "./common/spdlog/spdlog.h"
#include "./common/spdlog/sinks/basic_file_sink.h"
#include <mysql.h>

// server的static变量安排一下
auto gameApi = api::Api();
auto httpServer = std::shared_ptr<server::Server>(new server::Server);
auto gameServer = std::shared_ptr<server::Game>(new server::Game);
auto errorCode = std::shared_ptr<error::ErrorCode>(new error::ErrorCode);
auto threadPool = std::shared_ptr<ThreadPool>(new ThreadPool(5));

int main(int argc, char *argv[]) {

	spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);
    
    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");
    
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");    
    
    // change log pattern
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    
    // Compile time log levels
    // define SPDLOG_ACTIVE_LEVEL to desired level
    SPDLOG_TRACE("Some trace message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");
    
    // Set the default logger to file logger
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
    spdlog::set_default_logger(file_logger);         

    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL *connection, mysql;
  
    int state;
  
    mysql_init(&mysql);
  
    connection = mysql_real_connect(&mysql,"127.0.0.1","root","zsjss0523","mysql",0,0,0);
  
    // cout << connection;
    if (connection == NULL)
    {
        spdlog::error("mysql error message with arg: {}", mysql_error(&mysql));
        //std::cout << mysql_error(&mysql) << std::endl;
        // return tables;
    }

    state = mysql_query(connection, "SHOW TABLES");
     if (state !=0)
    {
        spdlog::error("mysql connection error message with arg: {}", mysql_error(connection));
        //std::cout << mysql_error(connection) << std::endl;
    }
  
    result = mysql_store_result(connection);
  
    std::cout << "tables: " << mysql_num_rows(result) << std::endl;
    while ( ( row=mysql_fetch_row(result)) != NULL )
    {
        spdlog::info("mysql row {}", row[0]);
        //cout << row[0] << std::endl;
    }
  
    mysql_free_result(result);
    mysql_close(connection);

	std::string path(__CONFIG_PATH__);
	std::ifstream i("../config/default.json");
    
	nlohmann::json defaultConfig;

	i >> defaultConfig;

	std::cout << "加载默认配置" << std::endl;
	std::cout << defaultConfig.dump(4) << std::endl;

	httpServer->Init(std::to_string(defaultConfig["port"].get<int>()));

	httpServer->Start();
	
	return 0;
}