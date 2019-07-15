CPP      = g++
CXXINCS  = -I"include/" -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 
CXXFLAGS = $(CXXINCS) -std=c++14 -O2
SRCS  	 = $(shell find src -name '*.cpp')
OBJ      = $(addprefix obj/,$(notdir $(SRCS:%.cpp=%.o))) 
BIN      = peste
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

debug: CXXFLAGS = -DDEBUG $(CXXINCS) -std=c++14 -g3 -ggdb3 -Wall -Wextra -D_GLIBCXX_DEBUG -O2 -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -Wno-unused-parameter 
debug: all

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(CXXFLAGS)

obj/%.o: src/%.cpp
	$(CPP) -c $< -o $@ $(CXXFLAGS)
