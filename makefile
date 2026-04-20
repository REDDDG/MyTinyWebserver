CXX ?= g++
DEBUG ?= 1

# OpenSSL 标志
OPENSSL_FLAGS = -lssl -lcrypto

ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
else
    CXXFLAGS += -O2
endif

# 服务器源文件（包含 AI）
SERVER_SRCS = main.cpp timer/lst_timer.cpp http/http_conn.cpp \
              log/log.cpp CGImysql/sql_connection_pool.cpp \
              webserver.cpp config.cpp Ai/ai.cpp

all: server

# 编译服务器时链接 OpenSSL
server: $(SERVER_SRCS)
	$(CXX) -o $@ $^ $(CXXFLAGS) -lpthread -lmysqlclient $(OPENSSL_FLAGS)

clean:
	rm -rf server

.PHONY: all clean