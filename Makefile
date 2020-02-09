ANAME = RCL
CC = gcc

FLEX = flex
FLEX_OPTS = -pRCL

BISON = bison
BISON_OPTS = -t -pRCL -v

CCFLAGS =	-g \
			-O1 \
			-s \
			-Wall \
			-Wno-unused-label \
			-Wno-missing-braces \
			-Wno-uninitialized \
			-Wno-maybe-uninitialized \
			-Wno-sign-compare \
			-Wno-unused-parameter \
			-Wno-unused-variable \
			-Wno-cast-function-type \
			-Wno-unused-value \
			-Wno-return-type \
			-Wno-format-extra-args \
			-Wno-format-zero-length \
			-Wno-unused-but-set-variable \
			-Wno-parentheses \
			-Wno-unused-function \
			-Wno-discarded-qualifiers \
			-ftree-loop-distribution \
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
			-static \
			-Iinclude

export CFLAGS = ${CCFLAGS}

.SILENT:

LIBS = -lffi -lgmp -lpthread

CSRC_SUBDIR_PATHS = *.c */*.c */*/*.c */*/*/*.c */*/*/*/*.c */*/*/*/*/*.c */*/*/*/*/*/*.c */*/*/*/*/*/*/*.c

csrc_abstract_paths = $(wildcard $(1) $(CSRC_SUBDIR_PATHS))

VM_CSRC = $(call csrc_abstract_paths, VM\)
VM_OBJS = $(VM_CSRC:.c=.o)

LIB_CSRC = $(call csrc_abstract_paths, Library\)
LIB_OBJS = $(LIB_CSRC:.c=.o)

#src/Core/Syntax/Parser.c src/Tools\Syntax\Parser.h:	src/Core/Syntax/RCL.y
#	${BISON} ${BISON_OPTS} src/Core/Syntax/RCL.y

#src/Core/Syntax/Lexer.c: src/Core/Syntax/RCL.l src/Tools\Syntax\Lexer.h
#	${FLEX} ${FLEX_OPTS} src/Core/Syntax/RCL.l

PHONY: --todo

--todo:
	@echo  1) If you want to build the virtual machine only (as executable one) ... run `make VM`
	@echo  2) If you want to build the library (as DLL) ........................... run `make DLL`
	@echo  3) If you want to build the library (as .a) ............................ run `make LIB`
	@echo  4) If you want to clean directories from object files .................. run `make clean`
	@echo  5) If you want more options ............................................ run `make help`
	@echo (The option (2) is probably the one you're interested in)

end-msg = @echo Compilation completed

VM: ${VM_OBJS}
	${CC} $^ -o ${ANAME} ${LIBS}
	$(end-msg)

DLL: ${LIB_OBJS}
	${CC} -shared -o ${ANAME}.dll $^ ${LIBS}
	$(end-msg)

LIB: ${LIB_OBJS}
	ar rcs librcl.a $^
	$(end-msg)

ALL: ${LIB_OBJS}
	@mkdir All
	${CC} $^ -o All\${ANAME} ${LIBS}
	${CC} -shared -o All\${ANAME}.dll $^ ${LIBS}
	ar rcs All\librcl.a $^
	CScript Assets\Windows\zip.vbs All "All RCL.zip" > null
	@rmdir /s /q All
	$(end-msg)

help:
	@echo  1) Build VM (as executable) ............. run `make VM`  --------- produce 'RCL.exe'  (as program)
	@echo  2) Build library (as DLL) ............... run `make LIB` --------- produce 'librcl.a' (for static links)
	@echo  2) Build library (as DLL) ............... run `make DLL` --------- produce 'RCL.dll'  (for dynamic links)
	@echo  3) Build all (1 and 2) .................. run `make ALL` --------- produce 'RCL.exe', 'librcl.a' and 'RCL.dll' in 'All.zip'
	@echo  4) Clean directories .................... run `make clean`
	@echo  5) Display help menu .................... run `make help`
	@echo  6) Display required tools ............... run `make required`

clean:
	del /s /q *.o > null
	@echo Cleaned directories

required:
	@echo   Required tools:
	@echo  -  GCC       (version 5.1.0 was used)
	@echo  -  Makefile  (version 4.2.1 was used)
	@echo  -  Bison     (version 2.4.1 was used)
	@echo  -  Flex      (version 2.5.4 was used)
	@echo  All supposed to be accessible from the shell
	@echo ---------------------------------------------
