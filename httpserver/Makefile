bin=httpserver
source=main.cc

.PHONY:all
all:$(bin) cgi

$(bin):$(source)
	g++ -o $@ $^ -std=c++17 -lpthread

cgi:
	cd ./cgi;\
	make clean;\
	make;\
	cd -

out:
	rm -rf output;\
	mkdir -p output;\
	cp -r webroot output;\
	cp -r cgi output/webroot;\
	cp $(bin) output;

.PHONY:clean
clean:
	rm $(bin);\
	rm -rf output

