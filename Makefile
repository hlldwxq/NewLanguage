C_FLAGS = `llvm-config-10 --cxxflags --ldflags --system-libs --libs core mcjit native orcjit`

start: 
	
	clang++-10 -g -c ParserExpression.cpp $(C_FLAGS) -o expression.o
	clang++-10 -g -c ParserCommand.cpp $(C_FLAGS) -o command.o

