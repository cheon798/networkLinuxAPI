#----------------------------------------------------------------------------
# Prefix and Output target
#----------------------------------------------------------------------------
PREFIX_DIR  = ../../..

OUT		= libsysvipc.a \
			sysv_shm \
			sysv_sem sysv_sem_waitforzero \
			sysv_msg  sysv_msg_regex sysv_msg_info \
			sysv_sem_counting sysv_sem_graph

include $(PREFIX_DIR)/common_opt.mk

LDLIBS 		+= -lsysvipc
LOADLIBES 	+= -L$(PREFIX_DIR)/lib
#----------------------------------------------------------------------------
# Sources
#----------------------------------------------------------------------------
SYSVIPC_FILES = lib_sysv_sem lib_sysv_shm lib_sysv_msg

SYSVIPC_SRC      = $(addsuffix .c,       $(SYSVIPC_FILES))
SYSVIPC_OBJ      = $(addsuffix .o,       $(SYSVIPC_FILES))

#----------------------------------------------------------------------------
# Default target.
#----------------------------------------------------------------------------
.PHONY: all clean dep
all: $(OUT)

#----------------------------------------------------------------------------
# Suffix rules
#----------------------------------------------------------------------------
include $(PREFIX_DIR)/common_rule.mk

#----------------------------------------------------------------------------
# Static pattern rule
#----------------------------------------------------------------------------

# Library Compilation (Position Independent Code)
$(SYSVIPC_OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(CPICFLAGS) $< -o $@

#----------------------------------------------------------------------------
# Build library
#----------------------------------------------------------------------------

libsysvipc.a : $(SYSVIPC_OBJ)
	@echo "++ Building XSI(SysV) IPC Wrapper library... ($@)"
	$(AR) $(ARFLAGS) $@ $(SYSVIPC_OBJ)
	@ranlib $@
	@echo "++ install the archive to $(LIB_DIR)"
	@if [ ! -d $(LIB_DIR) ]; then \
		mkdir $(LIB_DIR); \
	fi
	@mv $@ $(LIB_DIR)
	@rm $^
	@echo ""

libsysvipc.so : $(SYSVIPC_OBJ)
	@echo "++ Building XSI(SysV) IPC Wrapper library... ($@)"
	$(CC) -shared -o $@ $(SYSVIPC_OBJ)
	@echo "++ install the shared object to $(LIB_DIR)"
	@mv $@ $(LIB_DIR)
	@echo ""

#----------------------------------------------------------------------------
# Build sample program
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# others
#----------------------------------------------------------------------------
include $(PREFIX_DIR)/common_target.mk
