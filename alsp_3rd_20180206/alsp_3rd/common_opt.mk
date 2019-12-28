#----------------------------------------------------------------------------
# Directory for binaries and class libraries
#----------------------------------------------------------------------------
check_defined := \
			  $(foreach 1,$1,$(_check_vars))

_check_vars	= \
			  $(if $(value $1),, \
			  $(error Undefined $1$(if $(value 2), ($(strip $2)))))

$(call check_defined, PREFIX_DIR, prefix directory)
$(call check_defined, OUT, default output target)

LIB_DIR	:= $(PREFIX_DIR)/lib

#----------------------------------------------------------------------------
# Compiler and linker options.
#----------------------------------------------------------------------------
CXX			:= g++
CXXFLAGS	:= -Wall -O2
CC			:= gcc
CFLAGS 		:= -Wall -g -std=gnu99
CPPFLAGS	:= -I$(PREFIX_DIR)/include

CPICFLAGS	:= -fPIC 
LDSOFLAGS	:= -G -dy
ARFLAGS		:= ruv

LOADLIBES	:= 
LDLIBS		:= 

AWK			:= awk
MAKE		:= make
RM			:= rm

#----------------------------------------------------------------------------
# user defined macros
#----------------------------------------------------------------------------
SRCS        = $(wildcard *.c)
DEPENDENCY  = dep.mk
