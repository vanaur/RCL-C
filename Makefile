ANAME = RCL
CC = gcc

FLEX = flex
FLEX_OPTS = -pRCL

BISON = bison
BISON_OPTS = -t -pRCL -v

CCFLAGS_WARNS =		-Wno-discarded-qualifiers \
					-Wno-unused-parameter \
					-Wno-unused-value \
					-Wno-return-type \
					-Wno-unused-function \
					-Wno-unused-label \
					-Wno-missing-braces \
					-Wno-strict-aliasing \
					-Wno-parentheses \
					-Wno-unused-variable
CCFLAGS_OPTIMIZE = 	-ftree-loop-distribution \
					-faggressive-loop-optimizations \
					-fno-leading-underscore \
					-floop-parallelize-all \
					-enable-thread-safe \
					-ftree-vectorize \
					-funroll-loops \
					-fstdarg-opt \
					-fcombine-stack-adjustments \
					-fsplit-loops \
					-ftree-loop-if-convert \
					-O2
CCFLAGS_SIMD =		-mavx2
CCFLAGS_MISC =		-g -Wall -static -Iinclude

CCFLAGS = ${CCFLAGS_MISC} ${CCFLAGS_WARNS} ${CCFLAGS_OPTIMIZE} ${CCFLAGS_SIMD}

export CFLAGS = ${CCFLAGS}

.SILENT:

LIBS = -lffi -lgmp -lpthread

CSRC_SUBDIR_PATHS = *.c */*.c */*/*.c */*/*/*.c */*/*/*/*.c */*/*/*/*/*.c */*/*/*/*/*/*.c */*/*/*/*/*/*/*.c

csrc_abstract_paths = $(wildcard $(1) $(CSRC_SUBDIR_PATHS))

VM_CSRC = $(call csrc_abstract_paths, VM\)
VM_OBJS = $(VM_CSRC:.c=.o)

exe_main = source/VM/RCL.o

LIB_CSRC = $(call csrc_abstract_paths, Library\)
LIB_OBJS = $(filter-out $(exe_main), $(LIB_CSRC:.c=.o))

#src/Core/Syntax/Parser.c src/Tools\Syntax\Parser.h:	src/Core/Syntax/RCL.y
#	${BISON} ${BISON_OPTS} src/Core/Syntax/RCL.y

#src/Core/Syntax/Lexer.c: src/Core/Syntax/RCL.l src/Tools\Syntax\Lexer.h
#	${FLEX} ${FLEX_OPTS} src/Core/Syntax/RCL.l

ifeq ($(OS), Windows_NT)
	uname_ext := ${ANAME}.dll
	clean_cmd = @del /s /q *.o > null
else
	uname_ext := ${ANAME}.so
	clean_cmd = @rm -f *.o
endif

PHONY: --todo

--todo:
	@echo  1) If you want to build the virtual machine only (as executable one) ... run `make VM`
	@echo  2) If you want to build the library (as DLL) ........................... run `make DLL`
	@echo  3) If you want to build the library (as .a) ............................ run `make LIB`
	@echo  4) If you want to clean directories from object files .................. run `make clean`
	@echo  5) If you want more options ............................................ run `make help`
	@echo (The option (3) is probably the one you're interested in)

end-msg = @echo Compilation completed

VM: ${VM_OBJS}
	${CC} $^ -o ${ANAME} ${LIBS}
	$(end-msg)

DLL: ${LIB_OBJS}
	${CC} -shared -o $(uname_ext) $^ ${LIBS}
	$(end-msg)

LIB: ${LIB_OBJS}
	ar rcs librcl.a $^
	$(end-msg)

help:
	@echo  1) Build VM (as executable) ............. run `make VM`  --------- produce 'RCL'      (as program)
	@echo  2) Build library (as DLL) ............... run `make LIB` --------- produce 'librcl.a' (for static links)
	@echo  2) Build library (as DLL) ............... run `make DLL` --------- produce 'RCL.dll'  (for dynamic links)
	@echo  4) Clean directories .................... run `make clean`
	@echo  5) Display help menu .................... run `make help`
	@echo  6) Display required tools ............... run `make required`

clean:
	$(clean_cmd)
	@echo Cleaned directories

required:
	@echo   Required compilation tools:
	@echo  -   GCC      (version 5.1.0 was used)
	@echo  -   Makefile (version 4.2.1 was used)
	@echo  (-  Bison    (version 2.4.1 was used))  // pre-generated
	@echo  (-  Flex     (version 2.5.4 was used))  // pre-generated
	@echo  All supposed to be accessible from the shell.
	@echo ---------------------------------------------
	@echo   Required libraries:
	@echo  - libffi     (https://sourceware.org/libffi/)
	@echo  - GMP        (https://gmplib.org/)
	@echo Headers and static libraries simply need to be in the GCC compiler folders.
	@echo ---------------------------------------------
