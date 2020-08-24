DIR_SRCS = ./source
DIR_OBJS = ./object
DIR_TEST = tests

SRCS = $(wildcard $(DIR_SRCS)/*.cpp)
OBJS = $(patsubst %.cpp, $(DIR_OBJS)/%.o, $(notdir $(SRCS)))
EXES = llvmir

C_FLAGS = -Wall -I/usr/lib/llvm-10/include -std=c++14 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/lib/llvm-10/lib -lLLVM-10
# C_FLAGS = -Wall `llvm-config-10 --cxxflags --ldflags --system-libs --libs core mcjit native orcjit`

all: $(EXES)

$(EXES) : $(OBJS)
	clang++-10 -g $(OBJS) $(C_FLAGS) -o $(EXES)

object/%.o : source/%.cpp
	@echo Compiling cpp source code files $< ...
	clang++-10 -g -c $< $(C_FLAGS) -o $@



tests: validTest invalidTest dynamicTest meanfulTest



clean:
	rm -rf $(OBJS) $(EXES)


cleanTest:
	rm -rf $(DIR_TEST)/validTest/out/*
	rm -rf $(DIR_TEST)/MeanfulTest/out/*
	rm -rf $(DIR_TEST)/dynamicCheck/out/*
	rm -rf $(DIR_TEST)/benchmark/Rust/out/*
	rm -rf $(DIR_TEST)/invalidTest/other/out/*.log
	rm -rf $(DIR_TEST)/invalidTest/typeConvert/out/*.log

cleanBenchmark:
	rm -rf $(DIR_TEST)/benchmark/Java/out/*
	rm -rf $(DIR_TEST)/benchmark/Cpp/out/*

RandGenCPP = tests/benchmark/Java/RandomGen.cpp
RandGenOUT = tests/benchmark/Java/out/RandomGen.out

BinSearchCount = 10000000
BinSearchRep = 10000000

DisjointCount = 10000
DisjointRep = 10

ReverseCount = 10000000
ReverseRep = 11

RotateCount = 1000000
RotateRep = 101

SortCount = 100000
SortRep = 200

randomBinSearch: $(RandGenOUT)
	$(RandGenOUT) binarySearch $(BinSearchCount) $(BinSearchRep)

randomDisjoint: $(RandGenOUT)
	$(RandGenOUT) disjoint $(DisjointCount) $(DisjointRep)

randomReverse: $(RandGenOUT)
	$(RandGenOUT) reverse $(ReverseCount) $(ReverseRep)

randomRotate: $(RandGenOUT)
	$(RandGenOUT) rotate $(RotateCount) $(RotateRep)

randomSort: $(RandGenOUT)
	$(RandGenOUT) sort $(SortCount) $(SortRep)



randomGen: $(RandGenCPP) $(RandGenOUT)
	$(RandGenOUT) binarySearch $(BinSearchCount) $(BinSearchRep)
	$(RandGenOUT) disjoint $(DisjointCount) $(DisjointRep)
	$(RandGenOUT) reverse $(ReverseCount) $(ReverseRep)
	$(RandGenOUT) rotate $(RotateCount) $(RotateRep)
	$(RandGenOUT) sort $(SortCount) $(SortRep)


$(RandGenOUT): $(RandGenCPP)
	clang++-10 $(RandGenCPP) -o $(RandGenOUT)


B_CPPQ = $(wildcard $(DIR_TEST)/betchmark/CPP/*.q)
B_CPP = $(patsubst %.q, %.cpp, $(B_CPPQ))
B_JAVAQ = $(wildcard $(DIR_TEST)/betchmark/Java/*.q)
B_JAVA = $(patsubst %.q, %.java, $(B_JAVAQ))
benchmark: llvmir $(RandGenOUT) $(B_CPPQ) $(B_CPP) $(B_JAVAQ) $(B_JAVA) $(DIR_TEST)/benchmark/CPP/CPP.sh $(DIR_TEST)/benchmark/Java/Java.sh
	$(DIR_TEST)/benchmark/CPP/CPP.sh
	$(DIR_TEST)/benchmark/Java/Java.sh
	python diagramData.py



B_CPPQ = $(wildcard $(DIR_TEST)/betchmark/CPP/*.q)
B_CPP = $(patsubst %.q, %.cpp, $(B_CPPQ))
cppbenchmark: llvmir $(RandGenOUT) $(B_CPPQ) $(B_CPP) $(DIR_TEST)/benchmark/CPP/CPP.sh 
	$(DIR_TEST)/benchmark/CPP/CPP.sh
	python diagramData.py



B_JAVAQ = $(wildcard $(DIR_TEST)/betchmark/Java/*.q)
B_JAVA = $(patsubst %.q, %.java, $(B_JAVAQ))
javabenchmark: llvmir $(RandGenOUT) $(B_JavaQ) $(B_JAVA) $(DIR_TEST)/benchmark/Java/Java.sh
	$(DIR_TEST)/benchmark/Java/Java.sh
	python diagramData.py




Q = $(wildcard $(DIR_TEST)/dynamicCheck/*.q)
C = $(patsubst %.q, %.c, $(Q))
dynamicTest: llvmir $(Q) $(C) $(DIR_TEST)/dynamicCheck/dynamicCheck.sh
	$(DIR_TEST)/dynamicCheck/dynamicCheck.sh



VT_Q = $(wildcard $(DIR_TEST)/validTest/*.q)
VT_C = $(patsubst %.q, %.c, $(VT_Q))
validTest: llvmir $(VT_C) $(VT_Q) $(DIR_TEST)/validTest/validTest.sh
	$(DIR_TEST)/validTest/validTest.sh



invalidTest: llvmir $(wildcard $(DIR_TEST)/invalidTest/*.q)
	$(DIR_TEST)/invalidTest/invalidTest.sh



M_Q = $(wildcard $(DIR_TEST)/MeanfulTest/*.q)
M_C = $(patsubst %.q, %.c, $(M_Q))
meanfulTest: llvmir $(M_Q) $(M_C) $(DIR_TEST)/MeanfulTest/meanfulTest.sh
	$(DIR_TEST)/MeanfulTest/meanfulTest.sh

