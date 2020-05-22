C_SOURCES = $(shell find . -name "*.cpp")
C_OBJECTS = $(patsubst %.cpp, %.o, $(C_SOURCES))

C_FLAGS = -Wall `llvm-config-10 --cxxflags --ldflags --system-libs --libs core mcjit native orcjit` 


start: $(C_OBJECTS)
	clang++-10 -g $(C_OBJECTS) $(C_FLAGS) -o Parser
.cpp.o:
	@echo Compiling cpp source code files $< ...
	clang++-10 -g -c $< $(C_FLAGS) -o $@

file.ast: Parser file.ast
	./Parser > file.ast 2>&1 || echo "Ignoring Parser failure"