documet=cgi

.PHONY:all
all:$(documet) test_cgi cal_cgi littleComment #mysql_cgi read

$(documet):
	mkdir $@;
test_cgi:test_cgi.cc
	g++ -o $@ $^;
	mv $@ $(documet);
cal_cgi:cal_cgi.cc
	g++ -o $@ $^;
	mv $@ $(documet);
littleComment:littleComment.cc
	g++ -o $@ $^
	mv $@ $(documet);
#mysql_cgi:mysql_cgi.cc
#	g++ -o $@ $^ -I./mysql/include -L./mysql/lib -lmysqlclient
#	mv $@ $(documet);
##export LD_LIBRARY_PATH=lib绝对路径
#read:read_from_pipe.cc
#	g++ -o $@ $^
#	mv $@ $(documet);

.PHONY:clean
clean:
	rm -r $(documet);
