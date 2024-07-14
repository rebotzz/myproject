bin=httpserver
source=main.cc
cgiPath=cgicode

.PHONY:all
all:$(bin) bin_cgi

$(bin):$(source)
	g++ -o $@ $^ -std=c++17 -lpthread #-D__DEBUG__ 

bin_cgi:
	cd $(cgiPath);\
	make;\
	cd -

out:
	mkdir -p output;\
	cp -r webroot output;\
	cp -r cgicode/cgi/* output/webroot/cgi;\
	cp $(bin) output;

.PHONY:clean
clean:
	rm $(bin);\
	rm -rf output;\
	cd $(cgiPath);\
	make clean;\
	cd -;

