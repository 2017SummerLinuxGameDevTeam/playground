OUT    := playground
CC     := g++
CFLAGS := -std=c++1z -Wall -O3
ODIR   := obj
LIBS   := -lSDL2 -lGLEW
INCLUDE_DIRS = $(shell find ./ -name 'include') glm
INC    = $(addprefix -I,$(INCLUDE_DIRS))
SDIR   := src
SOURCES = $(shell find ./$(SDIR) -name '*.cpp')
OBJS = $(SOURCES:./%.cpp=%.o)

# Link OpenGL right depending on the OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LIBS := -framework OpenGL  $(LIBS)
else
	LIBS := -lGL  $(LIBS)
endif

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
