bin=httpserver

$(bin):httpserver.cc
	g++ -o $@ $^ -std=c++17 -lpthread

.PHONY:clean
clean:
	rm $(bin)

