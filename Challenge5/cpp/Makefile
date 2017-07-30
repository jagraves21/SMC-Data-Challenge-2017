# C PreProcessor
#CPPFLAGS = -DNDEBUG

# C Compiler
#CC = 

# C Compiler Flags
CFLAGS = -Wall -ansi -pedantic -std=c99 -O3

# C++ Compiler
#CXX = 

# C++ Compiler Flags
CXXFLAGS = -Wall -ansi -pedantic -std=c++11 -O3

# Extra flags to give to compilers when they are supposed to invoke the linker,
# 'ld', such as -L. Libraries (-lfoo) should be added to the LDLIBS variable
# instead.
LDFLAGS = 

# Library flags or names given to compilers when they are supposed to invoke
# the linker, 'ld'. LOADLIBES is a deprecated (but still supported) alternative
# to LDLIBS. Non-library linker flags, such as -L, should go in the LDFLAGS
# variable.
LDLIBS = 

C_FILES = 
CXX_FILES = create_sparse_matrix.cpp
OBJ_FILES := $(C_FILES:.c=.o)
HDR_FILES = 

PROG = create_sparse_matrix

.PHONY : all
all : $(PROG)

.PHONY : clean
clean:
	$(RM) $(OBJ_FILES)

.PHONY : veryclean
veryclean:
	$(RM) $(PROG) $(OBJ_FILES)

.PHONY : tar
tar:
	tar -czvf src.tar.gz $(HDR_FILES) $(C_FILES) Makefile

