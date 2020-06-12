C_SOURCES = $(shell find . -name "*.cpp")
C_OBJECTS = $(patsubst %.cpp, %.o, $(C_SOURCES))

C_FLAGS = -Wall -I/usr/lib/llvm-10/include -std=c++14 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/lib/llvm-10/lib                                                                                     -lLLVM-10
# C_FLAGS = -Wall `llvm-config-10 --cxxflags --ldflags --system-libs --libs core mcjit native orcjit`
# TODO add -Wextra for your own files only, if possible?

all: llvmir

tests: validTest invalidTest

clean:
	rm -rf *.o llvmir

llvmir: $(C_OBJECTS)
	clang++-10 -g $(C_OBJECTS) $(C_FLAGS) -o llvmir

.cpp.o:
	@echo Compiling cpp source code files $< ...
	clang++-10 -Wall -g -c $< $(C_FLAGS) -o $@


validTest: llvmir tests/validCodeTest.c tests/validCode.q
	./llvmir tests/validCode.q > tests/llvmir.ll || echo "TODO FIXME Fix this segfault!"
#	sed -i '/^Segmentation fault/d' llvmir.ll
	
	llvm-as-10 -disable-output tests/llvmir.ll
	
	clang-10 -Wall -Wextra -g -o tests/validCodeTest tests/validCodeTest.c tests/llvmir.ll
	
	./validTest.sh
# 	./validCodeTest > validCodeTest.log
# 	if egrep -v ": 1" validCodeTest.log; then exit 1; else exit 0; fi

invalidTest: llvmir tests/test_*.q
	./invalidTest.sh
