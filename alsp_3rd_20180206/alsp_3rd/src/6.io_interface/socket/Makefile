#----------------------------------------------------------------------------
# Destination directory for binaries and class libraries
#----------------------------------------------------------------------------
PREFIX_DIR	= ../../..

OUT		= chk_endian \
			inet_tcp_serv1 \
			inet_udp_rcver \
			inet_udp_snder \
			inet_udp_bcast \
			unix_udp \
			unix_tcp_serv \
			getaddrinfo \
			inet6_tcp_serv1 \
			if_nameindex \
			inet_tcp_serv1_OOB \
			tcp_cli \
			tcp_cli_nonblock \
			tcp_cli_OOB

include $(PREFIX_DIR)/common_opt.mk
#----------------------------------------------------------------------------
# Suffix rules
#----------------------------------------------------------------------------
include $(PREFIX_DIR)/common_rule.mk

#----------------------------------------------------------------------------
# Default target.
#----------------------------------------------------------------------------
.PHONY: all clean dep
all: $(OUT)

#----------------------------------------------------------------------------
# Build binaries
#----------------------------------------------------------------------------
tcp_cli_nonblock: CPPFLAGS+=-DENABLE_O_NONBLOCK
tcp_cli_nonblock: tcp_cli.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

tcp_cli_OOB: CPPFLAGS+=-DENABLE_MSG_OOB
tcp_cli_OOB: tcp_cli.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

#----------------------------------------------------------------------------
# others
#----------------------------------------------------------------------------
include $(PREFIX_DIR)/common_target.mk
