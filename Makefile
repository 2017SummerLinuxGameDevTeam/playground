OUT    := playground
CC     := g++
CFLAGS := -std=c++1z -Wall -O3
ODIR   := obj
LIBS   := -lSDL2 -lGL -lGLEW
INCLUDE_DIRS = $(shell find ./ -name 'include') glm
INC    = $(addprefix -I,$(INCLUDE_DIRS))
SDIR   := src
SOURCES = $(shell find ./$(SDIR) -name '*.cpp')
OBJS = $(SOURCES:./%.cpp=%.o)

LIBHUM := hummingbird/lib/libhum.a

all: $(OBJS) $(LIBHUM)
	$(CC) $^ $(LIBS) -o $(OUT) $(CFLAGS)


%.o: %.cpp
	$(CC) $(INC) $< -c -o $@ $(CFLAGS)

$(LIBHUM):
	@$(MAKE) -C hummingbird

run: all
	./playground

.PHONY: clean

clean:
	rm -rf $(OBJS) $(OUT)
