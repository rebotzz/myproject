all:test test_cgi read

test:test.cc
	g++ -o $@ $^ -std=c++11
test_cgi:test_cgi.cc
	g++ -o $@ $^ -std=c++11
read:read_from_pipe.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm test test_cgi read