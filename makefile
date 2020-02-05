ACTIVE = .
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR := $(dir $(MKFILE_PATH))
INCLUDE_PATH := $(MKFILE_DIR)common
CONFIG_PATH := $(dir $(MKFILE_DIR)../config/)

OPTS = -D __CONFIG_PATH__=\"$(CONFIG_PATH)\" -std=c++11 -I${INCLUDE_PATH} 

OBJECTS += \
$(ACTIVE)/main.cpp \
$(ACTIVE)/common/mongoose.c\
$(ACTIVE)/common/mysql/DbField.cpp \
$(ACTIVE)/common/mysql/QueryObject.cpp \
$(ACTIVE)/common/mysql/QueryResult.cpp \
$(ACTIVE)/common/mysql/Database.cpp \
$(ACTIVE)/http_server/Server.cpp\
$(ACTIVE)/service/session/SessionService.cpp\

server.out: $(OBJECTS)
	rm -rf server.out
	clang++ $(OPTS) -o $@ -I/usr/local/Cellar/mysql/8.0.19/include -I/usr/local/Cellar/mysql/8.0.19/include/mysql -W -lmysqlclient -L/usr/local/Cellar/mysql/8.0.19/lib $(OBJECTS)