CFLAGS=-Wall -g
BIN1=./bin/pagelib_main
BIN2=./bin/split_main
BIN3=./bin/query_main
OBJS1=./src/pagelib.o ./src/test_pagelib.o
OBJS2=./src/split.o ./src/test_split.o
OBJS3=./src/EchoServer.o ./src/test_query.o ./src/query.cpp
STARD=-std=c++0x
LINKS=-lpthread -L./lib/ -lecho
INCLUDE=-I./include
all:./bin/pagelib_main ./bin/split_main ./bin/query_main
.PHONY:all
$(BIN1):$(OBJS1)
	g++ -o $@ $(CFLAGS) $^ $(LINKS) $(INCLUDE)
%.o:%.cpp
	g++ $(CFLAGS) -c $< -o $@ $(STARD) $(INCLUDE)
$(BIN2):$(OBJS2)
	g++ -o $@ $(CFLAGS) $^ $(LINKS) $(INCLUDE)
%.o:%.cpp
	g++ $(CFLAGS) -c $< -o $@ $(STARD) $(INCLUDE)
$(BIN3):$(OBJS3)
	g++ -o $@ $(CFLAGS) $^ $(LINKS) $(INCLUDE)
%.o:%.cpp
	g++ $(CFLAGS) -c $< -o $@ $(STARD) $(INCLUDE)
.PHONY:clean
cleanobj:
	rm -f ./src/*.o
