#----------------------------------------------------------------------------
# Destination directory for binaries and class libraries
#----------------------------------------------------------------------------
PREFIX_DIR	= ../..

OUT		= io_timerfd \
		  io_eventfd \
		  io_signalfd \
		  io_signalfd_chld \
		  epoll_timerfd

include $(PREFIX_DIR)/common_opt.mk
LDLIBS	+= -lrt
#----------------------------------------------------------------------------
# Default target.
#----------------------------------------------------------------------------
.PHONY: all clean dep

all: dep $(OUT)

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

#----------------------------------------------------------------------------
# others
#----------------------------------------------------------------------------
include $(PREFIX_DIR)/common_target.mk
