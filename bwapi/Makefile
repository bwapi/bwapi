.SUFFIXES:
.DELETE_ON_ERROR:

SRCS := $(wildcard BWAPI/Source/BW/*.cpp) $(wildcard Util/Source/Util/*.cpp) $(wildcard BWAPI/Source/BWAPI/*.cpp) $(wildcard BWAPILIB/Source/*.cpp) BWAPI/Source/DLLMain.cpp

CXX := g++
CFLAGS := -O2 -std=gnu++0x -IBWAPI/Source -IUtil/Source -Iinclude -w

.PHONY: all clean

all: bin/BWAPI.dll

#-include obj/*.P

bin/BWAPI.dll: $(patsubst %.cpp,obj/%.o,$(SRCS))
	@-mkdir $(dir $@)
	$(CXX) -shared -o $@ $^

# $(CXX) $(CFLAGS) -MMD -MP -MF obj/$(subst /,__,$(patsubst obj/%.o,%.P,$@)) -c -o $@ $^
obj/%.o: %.cpp
	@-mkdir $(dir $@)
	$(CXX) $(CFLAGS) -c -o $@ $^

clean:
	-rmdir /s /q obj bin
