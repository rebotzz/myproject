all:test test_cgi

test:test.cc
	g++ -o $@ $^ -std=c++11
test_cgi:test_cgi.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm test test_cgi