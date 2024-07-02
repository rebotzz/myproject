bin=httpserver

$(bin):httpserver.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm $(bin)

