.SUFFIXES:
.DELETE_ON_ERROR:

SRCS := $(wildcard BWAPI/Source/BW/*.cpp) $(wildcard Util/Source/Util/*.cpp) $(wildcard BWAPI/Source/BWAPI/*.cpp) $(wildcard BWAPILIB/Source/*.cpp) BWAPI/Source/DLLMain.cpp

CXX := g++
CFLAGS := -O2 -std=gnu++0x -IBWAPI/Source -IUtil/Source -Iinclude -w -I.

.PHONY: all clean

all: bin/BWAPI.dll

#-include *.P

bin/BWAPI.dll: $(patsubst %.cpp,%.o,$(SRCS))
	@-mkdir $(dir $@)
	$(CXX) -shared -o $@ $^

# $(CXX) $(CFLAGS) -MMD -MP -MF $(subst /,__,$(patsubst %.o,%.P,$@)) -c -o $@ $^
%.o: %.cpp
	@-mkdir $(dir $@)
	$(CXX) $(CFLAGS) -c -o $@ $^

clean:
	-rm -r bin $(patsubst %.cpp,%.o,$(SRCS))
