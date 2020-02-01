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
#include "./common/spdlog/sinks/stdout_color_sinks.h"
#include "./common/mysql/Database.h"

// server的static变量安排一下
auto gameApi = api::Api();
auto gameDb  = std::shared_ptr<Database>(new Database);
auto httpServer = std::shared_ptr<server::Server>(new server::Server);
auto gameServer = std::shared_ptr<server::Game>(new server::Game);
auto errorCode = std::shared_ptr<error::ErrorCode>(new error::ErrorCode);
auto threadPool = std::shared_ptr<ThreadPool>(new ThreadPool(5));

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

	std::string path(__CONFIG_PATH__);
	std::ifstream i("./config/default.json");
    
	nlohmann::json defaultConfig;
	i >> defaultConfig;

	logger.info("加载默认配置");
	logger.info("{0}", defaultConfig.dump(4));
 
    // Initialize the world database
    //"127.0.0.1","root","zsjss0523","mysql"
	logger.info("Initialize the world database");
    if (!gameDb->Initialize("127.0.0.1;3306;root;zsjss0523;mysql"))
    {
		logger.error("GameServer::StartDB - Cannot connect to world database");
        return false;
    }

	if (std::shared_ptr<QueryResult> result = gameDb->Query("show tables;"))
	{
    	do
    	{
        	DbField* pFields = result->fetchCurrentRow();    
        	const std::string name = pFields[0].getCppString();
			logger.info("table name: {0}..", name);
    	}
    	while (result->NextRow());
	}

	httpServer->Init(std::to_string(defaultConfig["port"].get<int>()));

	httpServer->Start();
	
	// Cleanup
	gameDb->Uninitialise();
	return 0;
}