# EyeService

#### 介绍
C++服务系统，标准C++11，ThreadPool，httpService，websocket，log，mysql，json基本库，小麻雀五脏俱全

#### 软件架构
软件架构说明


#### 安装教程
Apple LLVM version 10.0.1  (clang-1001.0.46.4) gxx 4.2.1 标准c++17
brew install mysql

编译
```
rm server.out
make
./server.out
```

#### example

```
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

connection = mysql_real_connect(&mysql,"127.0.0.1","root","","mysql",0,0,0);

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

Database GameDb;    

// Initialize the world database
if (!GameDb.Initialize("host;port;user;pw;dbname"))
{
    printf("GameServer::StartDB - Cannot connect to world database");
    return false;
}

// Example blocking query
if (std::shared_ptr<QueryResult> result = GameDb.Query("SELECT entry, name FROM table"))
{
    do
    {
        DbField* pFields = result->fetchCurrentRow();    
        const unsigned int entry = pFields[0].getUInt32();
        const std::string name = pFields[1].getCppString();
    }
    while (result->NextRow());
}

// If you wan't to issue a non blocking query without concern for the result, you queue it as such.
// The queue is executed in the order it's given queries, but is asynchronous to executions outside of that queue.
GameDb.QueueExecuteQuery("UPDATE table SET name = ''");

// Executes a blocking query without concern for the result.
GameDb.ExecuteQueryInstant("UPDATE table SET );

// If you want to set-up adding many queries to the queue at once with the option to cancel before you've finished adding them all in.
GameDb.BeginManyQueries();

// Would have many calls of 'QueueExecuteQuery' inside SaveAllPlayers().
if (Game::SaveAllPlayers())
{
    GameDb.CommitManyQueries();
}
else
{
    printf("Failed to save all players!");
    GameDb.CancelTransaction();
}
    
// If you want to get data without blocking, you queue up what I called a "Callback" by providing an ID and a query string.
// Then, later, check and process any results.
// GameDb.queueCallbackQuery(GET_PLAYER_DATA_QUERY, "SELECT * FROM players WHERE name = ''");
// GameDb.GrabAndClearCallbackQueries(uoPlaceToPutResults);
// ProcessResults(uoPlaceToPutResults);

// Cleanup
GameDb.Uninitialise();
```
