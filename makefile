include = -I./src -I./include 
libs = -lsndfile -llua5.4 -lsfml-audio -lsfml-system
version = -DVERSION=\"beta\"



OS := $(shell uname -s)
ifeq ($(OS),Windows_NT)
endif
ifeq ($(findstring MINGW,$(OS)),MINGW)
endif
ifeq ($(OS),Linux)
endif

all:
	g++ $(include) $(version) ./src/main.cpp -o build/lunarwave $(libs)