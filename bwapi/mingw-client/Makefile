INCLUDEPATH = ../include/
LIBPATH     = ../BWAPILIB/Source/
CLIENTPATH  = ../BWAPIClient/Source/
UTILPATH    = ../Util/Source/
SHAREDPATH  = ../Shared/
OBJECTPATH  = out/

CXX         = g++
CXXFLAGS    = -Wall -Wextra -O2 --std=c++0x -I$(INCLUDEPATH)BWAPI/Client -I$(INCLUDEPATH) -I$(UTILPATH) -I$(SHAREDPATH)
CXXLIBS     = -L. -lBWAPI

CSOURCES    = $(wildcard $(CLIENTPATH)*.cpp)
LSOURCES    = $(wildcard $(LIBPATH)*.cpp)
SSOURCES    = $(wildcard $(SHAREDPATH)*.cpp)
USOURCES    = $(wildcard $(UTILPATH)Util/*.cpp)
SOURCES     = $(CSOURCES) $(LSOURCES) $(SSOURCES) $(USOURCES) $(LIBPATH)../UnitCommand.cpp
OBJECTS     = $(addprefix $(OBJECTPATH), $(notdir $(SOURCES:.cpp=.o)))

all: libBWAPI.a ExampleAIClient.exe

ExampleAIClient.exe: ExampleAIClient.cpp
	$(CXX) $(CXXFLAGS) $< $(CXXLIBS) -o $@

libBWAPI.a: $(OBJECTS)
	$(CXX) -shared $(OBJECTS) -o $@

echo:
	@echo Sources:
	@echo $(SOURCES)
	@echo Objects:
	@echo $(OBJECTS)

$(OBJECTPATH)%.o: $(CLIENTPATH)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTPATH)%.o: $(LIBPATH)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTPATH)%.o: $(SHAREDPATH)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTPATH)%.o: $(UTILPATH)Util/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTPATH)UnitCommand.o: $(LIBPATH)../UnitCommand.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJECTS): | $(OBJECTPATH)

$(OBJECTPATH):
	mkdir $(OBJECTPATH)

clean:
	rm -rf $(OBJECTPATH) *.exe libBWAPI.a
