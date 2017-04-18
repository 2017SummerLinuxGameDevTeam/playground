OUT    := playground
CC     := g++
CFLAGS := -std=c++11 -Wall -O3
ODIR   := obj
LIBS   := ../hummingbird/lib/libhum.a
SDIR   := src
INC    := -I../hummingbird/include/ -Iinclude/

SOURCES = $(shell find ./$(SDIR) -name '*.cpp')
OBJS = $(patsubst $(SDIR)/%,$(ODIR)/%,$(SOURCES:./%.cpp=%.o))

all: $(OBJS)
	#$(CC) -o $(OUT) $(OBJS) $(CFLAGS) -I../hummingbird/include/ -Iinclude/ -lhum
	$(CC) $< $(LIBS) -o $(OUT) $(CFLAGS)


$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(INC) $< -c -o $@ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o $(OUT)
