documet=cgi

.PHONY:all
all:$(documet) test_cgi cal_cgi mysql_cgi

$(documet):
	mkdir $@;
test_cgi:test_cgi.cc
	g++ -o $@ $^;
	mv $@ $(documet);
cal_cgi:cal_cgi.cc
	g++ -o $@ $^;
	mv $@ $(documet);
mysql_cgi:mysql_cgi.cc
	g++ -o $@ $^ -I./mysql/include -L./mysql/lib -lmysqlclient
	mv $@ $(documet);
#export LD_LIBRARY_PATH=lib绝对路径

.PHONY:clean
clean:
	rm -r $(documet);
