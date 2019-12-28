#----------------------------------------------------------------------------
# Destination directory for binaries and class libraries
#----------------------------------------------------------------------------
PREFIX_DIR	= ../..

OUT		= cond_var_rts \
		  io_rts \
		  dir_notify \
		  clock_getres \
		  rt_timer \
		  rt_timer_boot \
		  rt_timer_cputime \
		  cputime_process \
		  aio_basic \
		  aio_listio \
		  aio_listio_async \
		  aio_listio_sig \
		  sched \
		  omp_pi_cputime \
		  pthread_pi_cputime \
		  pthread_pi_cputime_sched \
		  sched_timequantum

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
cond_var_rts: LDLIBS+=-lpthread

pthread_pi_cputime pthread_pi_cputime_sched: LDLIBS+=-lpthread

omp_pi_cputime: CFLAGS+=-fopenmp

sig_nomask : sig_basic.c
	$(CC) -DWITH_SA_NOMASK $(CFLAGS) $^ -o $@

aio_listio_async: aio_listio.c
	$(CC) -DASYNCHRONIZED_IO $(CPPFLAGS) $(CFLAGS) $< $(LOADLIBES) $(LDLIBS) -o $@

#----------------------------------------------------------------------------
# others
#----------------------------------------------------------------------------
include $(PREFIX_DIR)/common_target.mk
