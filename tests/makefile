TESTFILES = $(wildcard ./*.cpp)
TESTS = $(basename $(TESTFILES))
TESTSO = $(addsuffix .o, $(TESTS))
CFLAGS = -c -Wall -I../eventloop

all: $(TESTS)


$(TESTS) : $(TESTSO) eventloop
	$(CC) -o $@ $< ../eventloop/Eventloop.cpp.o

$(TESTSO) : $(TESTFILES)
	$(CC) $(CFLAGS) $@ $<

eventloop:
	$(MAKE) -C ../eventloop

clean:
	rm ./*.o $(TESTS)
	$(MAKE) -C ../eventloop clean
