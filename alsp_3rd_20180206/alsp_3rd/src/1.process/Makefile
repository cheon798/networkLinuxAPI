#----------------------------------------------------------------------------
# Prefix and Output target
#----------------------------------------------------------------------------
PREFIX_DIR	= ../..

OUT		= fork_process \
			fork_omit_switch \
			execl \
			forkexec_parent forkexec_child \
			forkexec_parent_fdcloexec \
			pspawn1 pspawn2 pspawn3 \
			pspawn4 pspawn4_child \
			pspawn5 pspawn5_child_x pspawn5_child_y \

include $(PREFIX_DIR)/common_opt.mk

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

#----------------------------------------------------------------------------
# Build binaries
#----------------------------------------------------------------------------
fork_omit_switch : fork_process.c
	$(CC) $(CFLAGS) -DOMIT_SWITCH $^ -o $@

forkexec_parent_fdcloexec: CPPFLAGS+=-DAPPLY_FD_CLOEXEC
forkexec_parent_fdcloexec: forkexec_parent.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LOADLIBES) $(LDLIBS) 


#----------------------------------------------------------------------------
# others
#----------------------------------------------------------------------------
include $(PREFIX_DIR)/common_target.mk

