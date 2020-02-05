#include <iostream>
#include <fstream>
#include <memory>
#include "./config.h"
#include "./http_server/Server.h"
#include "./common/nlohmann/json.hpp"
#include "./common/ThreadPool.h"
#include "./common/spdlog/spdlog.h"
#include "./common/spdlog/sinks/basic_file_sink.h"
#include "./common/spdlog/sinks/stdout_color_sinks.h"
#include "./common/mysql/Database.h"

// server的static变量安排一下
auto dbServer  = std::shared_ptr<Database>(new Database);
auto httpServer = std::shared_ptr<server::Server>(new server::Server);

int main(int argc, char *argv[]) {
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::warn);
    console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/basic.txt", true);
    file_sink->set_level(spdlog::level::trace);

    spdlog::logger logger("multi_sink", {console_sink, file_sink});
    logger.set_level(spdlog::level::debug);
    logger.warn("this should appear in both console and file");
    logger.info("this message should not appear in the console, only in the file");

	std::ifstream i("./config/default.json");
	nlohmann::json defaultConfig;
	i >> defaultConfig;

	logger.info("加载默认配置");
	logger.info("{0}", defaultConfig.dump(4));
 
	logger.info("Initialize the database");
    if (!dbServer->Initialize("127.0.0.1;3306;root;;mysql"))
    {
		logger.error("StartDB - Cannot connect to database");
        return -1;
    }

	httpServer->Init(std::to_string(defaultConfig["port"].get<int>()));

	httpServer->Start();
	
	// Cleanup
	dbServer->Uninitialise();
	return 0;
}