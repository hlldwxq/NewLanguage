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



tests: validTest invalidTest dynamicTest



clean:
	rm -rf $(OBJS) $(EXES)


cleandy:
	rm -rf $(DIR_TEST)/dynamicCheck/*.ll $(DIR_TEST)/dynamicCheck/*.log $(DIR_TEST)/dynamicCheck/*.out


Q = $(wildcard $(DIR_TEST)/dynamicCheck/*.q)
IR = $(patsubst %.q, %.ll, $(Q))
C = $(patsubst %.q, %.c, $(Q))
O = $(patsubst %.q, %.out, $(Q))

dynamicTest: llvmir $(O)
	$(DIR_TEST)/dynamicCheck/dynamicCheck.sh

$(O) : $(C) $(IR)
	clang-10 -Wall -Wextra -g -o $@ $(patsubst %.out, %.c, $@) $(patsubst %.out, %.ll, $@)

$(IR) : $(Q)
	./llvmir DyCheck $(patsubst %.ll, %.q, $@) > $@ 2>&1 | true
		



validTest: llvmir $(DIR_TEST)/validTest/validCodeTest.c $(DIR_TEST)/validTest/validCode.q
	./llvmir DyCheck $(DIR_TEST)/validTest/validCode.q > $(DIR_TEST)/validTest/valid.ll || echo "TODO FIXME Fix this segfault!"
	
	llvm-as-10 -disable-output $(DIR_TEST)/validTest/valid.ll
	
	clang-10 -Wall -Wextra -g -o $(DIR_TEST)/validTest/validCodeTest $(DIR_TEST)/validTest/validCodeTest.c $(DIR_TEST)/validTest/valid.ll
	
	$(DIR_TEST)/validTest/validTest.sh
 	#./validCodeTest > validCodeTest.log
 	#if egrep -v ": 1" validCodeTest.log; then exit 1; else exit 0; fi



invalidTest: llvmir $(DIR_TEST)/invalidTest/test_*.q
	$(DIR_TEST)/invalidTest/invalidTest.sh
