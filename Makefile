OUT    := playground
CC     := g++
CFLAGS := -std=c++11 -Wall -O3
ODIR   := obj
LIBS   := -lSDL2
SDIR   := src
INC    := -Ihummingbird/include/ -Iinclude/

LIBHUM := hummingbird/lib/libhum.a

SOURCES = $(shell find ./$(SDIR) -name '*.cpp')
OBJS = $(patsubst $(SDIR)/%,$(ODIR)/%,$(SOURCES:./%.cpp=%.o))

all: $(OBJS) $(LIBHUM)
	$(CC) $^ $(LIBS) -o $(OUT) $(CFLAGS)


$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(INC) $< -c -o $@ $(CFLAGS)

$(LIBHUM):
	@$(MAKE) -C hummingbird

run: all
	./playground

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o $(OUT)
