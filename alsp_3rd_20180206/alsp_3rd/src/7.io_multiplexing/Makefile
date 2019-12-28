#----------------------------------------------------------------------------
# Destination directory for binaries and class libraries
#----------------------------------------------------------------------------
PREFIX_DIR	= ../..

OUT		= io_select io_select2 \
		  io_poll io_poll_OOB io_poll_nb \
		  io_poll_LT \
		  io_epoll io_epoll_OOB io_epoll_LT io_epoll_ET
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
OBJ_OUT_DERIVED1 = $(addsuffix .o, $(filter %_OOB, $(OUT)))
$(OBJ_OUT_DERIVED1): %_OOB.o: %.c
	$(CC) -c -DENABLE_MSG_OOB $(CFLAGS) $(CPPFLAGS) $< -o $@

#----------------------------------------------------------------------------
# Build binaries
#----------------------------------------------------------------------------

io_epoll_LT : io_epoll_trigger.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

io_epoll_ET : CPPFLAGS+=-DENABLE_EPOLLET
io_epoll_ET : io_epoll_trigger.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

#----------------------------------------------------------------------------
# others
#----------------------------------------------------------------------------
include $(PREFIX_DIR)/common_target.mk
