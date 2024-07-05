documet=cgi

.PHONY:all
all:$(documet) test_cgi

$(documet):
	mkdir $@;

test_cgi:test_cgi.cc
	g++ -o $@ $^;
	mv $@ $(documet);

.PHONY:clean
clean:
	rm -r $(documet);
