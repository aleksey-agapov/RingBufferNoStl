# ---------------------------------------------------------------
C_SRCS = RingBufferTest.cpp

OBJS = RingBufferTest.o

EXECUTABLE = RingBufferTest

LIBS = -lpthread -lrt -ldl -lstdc++

.PHONY: all clean build test all

all: build test

build: $(OBJS) 
	@echo 'Building target: $@'
	gcc $(LIBS) -o $(EXECUTABLE) $(OBJS) 

%.o: %.cpp
	@echo 'Building file: $<'
	gcc -c -O3 -o "$@" "$<"

clean:
	rm -rf $(OBJS) $(EXECUTABLE)

test:
	./$(EXECUTABLE)

