include = -I./src -I./include 
libs = -lsndfile -lsfml-audio -lsfml-system
version = -DVERSION=\"1.0\"
output_file = build/lunarwave


OS := $(shell uname -s)
ifeq ($(OS),Windows_NT)
	libs +=  -llua
	output_file +=.exe
endif
ifeq ($(findstring MINGW,$(OS)),MINGW)
	libs +=  -llua
endif
ifeq ($(OS),Linux)
	libs +=  -llua5.4
endif

all:
	g++ $(include) $(version) ./src/main.cpp -o build/lunarwave $(libs)