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
	rm -rf $(DIR_TEST)/dynamicCheck/*.ll $(DIR_TEST)/dynamicCheck/*.log $(DIR_TEST)/dynamicCheck/*.out
	rm -rf $(DIR_TEST)/invalidTest/other/*.log
	rm -rf $(DIR_TEST)/invalidTest/typeConvert/*.log
	rm -rf $(DIR_TEST)/MeanfulTest/*.ll $(DIR_TEST)/MeanfulTest/*.log $(DIR_TEST)/MeanfulTest/*.out
	rm -rf $(DIR_TEST)/validTest/*.ll $(DIR_TEST)/validTest/*.log $(DIR_TEST)/validTest/*.out
	

meetTest: test.ll test.c
	clang-10 -O2 -Wall -Wextra --rtlib=compiler-rt -g test.c test.ll -o test.out

test.ll: test.q llvmir
	./llvmir DyCheck test.q>test.ll


callTest: testCall.ll testCall.c
	clang-10 -O2 -Wall -Wextra --rtlib=compiler-rt -g testCall.c testCall.ll -o testCall.out

testCall.ll: testCall.q llvmir
	./llvmir DyCheck testCall.q>testCall.ll



Q = $(wildcard $(DIR_TEST)/dynamicCheck/*.q)
IR = $(patsubst %.q, %.ll, $(Q))
C = $(patsubst %.q, %.c, $(Q))
O = $(patsubst %.q, %.out, $(Q))

dynamicTest: llvmir $(O)
	$(DIR_TEST)/dynamicCheck/dynamicCheck.sh

$(O) : $(C) $(IR)
	llvm-as-10 -disable-output $(patsubst %.out, %.ll, $@)
	clang-10 -O2 -Wall -Wextra --rtlib=compiler-rt -g -o $@ $(patsubst %.out, %.c, $@) $(patsubst %.out, %.ll, $@)

$(IR) : $(Q) llvmir
	./llvmir DyCheck $(patsubst %.ll, %.q, $@) > $@ || ( rm $@; exit 1 )




VT_Q = $(wildcard $(DIR_TEST)/validTest/*.q)
VT_IR = $(patsubst %.q, %.ll, $(VT_Q))
VT_C = $(patsubst %.q, %.c, $(VT_Q))
VT_O = $(patsubst %.q, %.out, $(VT_Q))

validTest: llvmir $(VT_O)
	$(DIR_TEST)/validTest/validTest.sh

$(VT_O) : $(VT_C) $(VT_IR)
	llvm-as-10 -disable-output $(patsubst %.out, %.ll, $@)
	clang-10 -O2 -Wall -Wextra --rtlib=compiler-rt -g -o $@ $(patsubst %.out, %.c, $@) $(patsubst %.out, %.ll, $@)

$(VT_IR) : $(VT_Q) llvmir
	./llvmir DyCheck $(patsubst %.ll, %.q, $@) > $@ || ( rm $@; exit 1 )




invalidTest: llvmir $(wildcard $(DIR_TEST)/invalidTest/*.q)
	$(DIR_TEST)/invalidTest/invalidTest.sh




M_Q = $(wildcard $(DIR_TEST)/MeanfulTest/*.q)
M_IR = $(patsubst %.q, %.ll, $(M_Q))
M_C = $(patsubst %.q, %.c, $(M_Q))
M_O = $(patsubst %.q, %.out, $(M_Q))

meanfulTest: llvmir $(M_O)
	$(DIR_TEST)/MeanfulTest/meanfulTest.sh

$(M_O) : $(M_C) $(M_IR)
	llvm-as-10 -disable-output $(patsubst %.out, %.ll, $@)
	clang-10 -O2 -Wall -Wextra --rtlib=compiler-rt -g -o $@ $(patsubst %.out, %.c, $@) $(patsubst %.out, %.ll, $@)

$(M_IR) : $(M_Q) llvmir
	./llvmir DyCheck $(patsubst %.ll, %.q, $@) > $@ || ( rm $@; exit 1 )