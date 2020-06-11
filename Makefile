C_SOURCES = $(shell find . -name "*.cpp")
C_OBJECTS = $(patsubst %.cpp, %.o, $(C_SOURCES))

C_FLAGS = -Wall `llvm-config-10 --cxxflags --ldflags --system-libs --libs core mcjit native orcjit`
# TODO add -Wextra for your own files only, if possible?


start: $(C_OBJECTS)
	clang++-10 -g $(C_OBJECTS) $(C_FLAGS) -o llvmir
.cpp.o:
	@echo Compiling cpp source code files $< ...
	clang++-10 -g -c $< $(C_FLAGS) -o $@


validTest: llvmir validCodeTest.c
	./llvmir > llvmir.ll 2>&1 || echo "Ignoring Parser failure"
	sed -i '/^Segmentation fault/d' llvmir.ll
	
	llvm-as-10 -disable-output llvmir.ll 
	
	clang-10 -Wall -Wextra -g -o validCodeTest validCodeTest.c llvmir.ll
	
	./validCodeTest


invalidTest: llvmir
	./llvmir 
