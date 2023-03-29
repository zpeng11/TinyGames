SUBDIRS := tests eventloop map #2028  GreedySnake Tetris 

CXXFLAGS := -g -Wall $(addprefix -I,$(SUBDIRS))

SRCFILES = $(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*.cpp))
SRCFILES_O := $(addsuffix .o, $(basename $(SRCFILES)))

TESTS = $(basename $(wildcard tests/*.cpp))
TESTS_O = $(addsuffix .o, $(TESTS))

GAMESMAIN = $(basename $(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*main.cpp)))
GAMESMAIN_O = $(addsuffix .o, $(GAMESMAIN))

COMMON_LINK = $(filter-out $(GAMESMAIN_O),$(filter-out $(TESTS_O),$(SRCFILES_O)))

all: $(SRCFILES_O) $(TESTS) $(GAMESMAIN)

$(SRCFILES_O): %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

$(TESTS) $(GAMESMAIN): $(SRCFILES_O)
	$(CXX) $(CXXFLAGS) -o $@  $(COMMON_LINK) $@.o

clean:
	$(RM) $(SRCFILES_O) $(TESTS) $(GAMESMAIN)
