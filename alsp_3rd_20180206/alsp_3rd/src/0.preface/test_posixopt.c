#include <unistd.h>
#include <stdio.h>
int main()
{

#if _POSIX_ASYNCHRONOUS_IO > 0L
	printf("[O] _POSIX_ASYNCHRONOUS_IO\n");
#else
	printf("[X] _POSIX_ASYNCHRONOUS_IO\n");
#endif
#if _POSIX_BARRIERS > 0L
	printf("[O] _POSIX_BARRIERS\n");
#else
	printf("[X] _POSIX_BARRIERS\n");
#endif
#if _POSIX_CLOCK_SELECTION > 0L
	printf("[O] _POSIX_CLOCK_SELECTION\n");
#else
	printf("[X] _POSIX_CLOCK_SELECTION\n");
#endif
#if _POSIX_CPUTIME > 0L
	printf("[O] _POSIX_CPUTIME\n");
#else
	printf("[X] _POSIX_CPUTIME\n");
#endif
#if _POSIX_FILE_LOCKING > 0L
	printf("[O] _POSIX_FILE_LOCKING\n");
#else
	printf("[X] _POSIX_FILE_LOCKING\n");
#endif
#if _POSIX_FSYNC > 0L
	printf("[O] _POSIX_FSYNC\n");
#else
	printf("[X] _POSIX_FSYNC\n");
#endif
#if _POSIX_JOB_CONTROL > 0L
	printf("[O] _POSIX_JOB_CONTROL\n");
#else
	printf("[X] _POSIX_JOB_CONTROL\n");
#endif
#if _POSIX_MAPPED_FILES > 0L
	printf("[O] _POSIX_MAPPED_FILES\n");
#else
	printf("[X] _POSIX_MAPPED_FILES\n");
#endif
#if _POSIX_MEMLOCK > 0L
	printf("[O] _POSIX_MEMLOCK\n");
#else
	printf("[X] _POSIX_MEMLOCK\n");
#endif
#if _POSIX_MEMLOCK_RANGE > 0L
	printf("[O] _POSIX_MEMLOCK_RANGE\n");
#else
	printf("[X] _POSIX_MEMLOCK_RANGE\n");
#endif
#if _POSIX_MEMORY_PROTECTION > 0L
	printf("[O] _POSIX_MEMORY_PROTECTION\n");
#else
	printf("[X] _POSIX_MEMORY_PROTECTION\n");
#endif
#if _POSIX_MESSAGE_PASSING > 0L
	printf("[O] _POSIX_MESSAGE_PASSING\n");
#else
	printf("[X] _POSIX_MESSAGE_PASSING\n");
#endif
#if _POSIX_MONOTONIC_CLOCK > 0L
	printf("[O] _POSIX_MONOTONIC_CLOCK\n");
#else
	printf("[X] _POSIX_MONOTONIC_CLOCK\n");
#endif
#if _POSIX_MULTI_PROCESS > 0L
	printf("[O] _POSIX_MULTI_PROCESS\n");
#else
	printf("[X] _POSIX_MULTI_PROCESS\n");
#endif
#if _POSIX_NO_TRUNC > 0L
	printf("[O] _POSIX_NO_TRUNC\n");
#else
	printf("[X] _POSIX_NO_TRUNC\n");
#endif
#if _POSIX_PRIORITIZED_IO > 0L
	printf("[O] _POSIX_PRIORITIZED_IO\n");
#else
	printf("[X] _POSIX_PRIORITIZED_IO\n");
#endif
#if _POSIX_PRIORITY_SCHEDULING > 0L
	printf("[O] _POSIX_PRIORITY_SCHEDULING\n");
#else
	printf("[X] _POSIX_PRIORITY_SCHEDULING\n");
#endif
#if _POSIX_RAW_SOCKETS > 0L
	printf("[O] _POSIX_RAW_SOCKETS\n");
#else
	printf("[X] _POSIX_RAW_SOCKETS\n");
#endif
#if _POSIX_READER_WRITER_LOCKS > 0L
	printf("[O] _POSIX_READER_WRITER_LOCKS\n");
#else
	printf("[X] _POSIX_READER_WRITER_LOCKS\n");
#endif
#if _POSIX_REALTIME_SIGNALS > 0L
	printf("[O] _POSIX_REALTIME_SIGNALS\n");
#else
	printf("[X] _POSIX_REALTIME_SIGNALS\n");
#endif
#if _POSIX_REGEXP > 0L
	printf("[O] _POSIX_REGEXP\n");
#else
	printf("[X] _POSIX_REGEXP\n");
#endif
#if _POSIX_SAVED_IDS > 0L
	printf("[O] _POSIX_SAVED_IDS\n");
#else
	printf("[X] _POSIX_SAVED_IDS\n");
#endif
#if _POSIX_SEMAPHORES > 0L
	printf("[O] _POSIX_SEMAPHORES\n");
#else
	printf("[X] _POSIX_SEMAPHORES\n");
#endif
#if _POSIX_SHARED_MEMORY_OBJECTS > 0L
	printf("[O] _POSIX_SHARED_MEMORY_OBJECTS\n");
#else
	printf("[X] _POSIX_SHARED_MEMORY_OBJECTS\n");
#endif
#if _POSIX_SHELL > 0L
	printf("[O] _POSIX_SHELL\n");
#else
	printf("[X] _POSIX_SHELL\n");
#endif
#if _POSIX_SPAWN > 0L
	printf("[O] _POSIX_SPAWN\n");
#else
	printf("[X] _POSIX_SPAWN\n");
#endif
#if _POSIX_SPIN_LOCKS > 0L
	printf("[O] _POSIX_SPIN_LOCKS\n");
#else
	printf("[X] _POSIX_SPIN_LOCKS\n");
#endif
#if _POSIX_SPORADIC_SERVER > 0L
	printf("[O] _POSIX_SPORADIC_SERVER\n");
#else
	printf("[X] _POSIX_SPORADIC_SERVER\n");
#endif
#if _POSIX_SYNCHRONIZED_IO > 0L
	printf("[O] _POSIX_SYNCHRONIZED_IO\n");
#else
	printf("[X] _POSIX_SYNCHRONIZED_IO\n");
#endif
#if _POSIX_THREADS > 0L
	printf("[O] _POSIX_THREADS\n");
#else
	printf("[X] _POSIX_THREADS\n");
#endif
#if _POSIX_THREAD_ATTR_STACKADDR > 0L
	printf("[O] _POSIX_THREAD_ATTR_STACKADDR\n");
#else
	printf("[X] _POSIX_THREAD_ATTR_STACKADDR\n");
#endif
#if _POSIX_THREAD_ATTR_STACKSIZE > 0L
	printf("[O] _POSIX_THREAD_ATTR_STACKSIZE\n");
#else
	printf("[X] _POSIX_THREAD_ATTR_STACKSIZE\n");
#endif
#if _POSIX_THREAD_CPUTIME > 0L
	printf("[O] _POSIX_THREAD_CPUTIME\n");
#else
	printf("[X] _POSIX_THREAD_CPUTIME\n");
#endif
#if _POSIX_THREAD_PRIORITY_SCHEDULING > 0L
	printf("[O] _POSIX_THREAD_PRIORITY_SCHEDULING\n");
#else
	printf("[X] _POSIX_THREAD_PRIORITY_SCHEDULING\n");
#endif
#if _POSIX_THREAD_PRIO_INHERIT > 0L
	printf("[O] _POSIX_THREAD_PRIO_INHERIT\n");
#else
	printf("[X] _POSIX_THREAD_PRIO_INHERIT\n");
#endif
#if _POSIX_THREAD_PRIO_PROTECT > 0L
	printf("[O] _POSIX_THREAD_PRIO_PROTECT\n");
#else
	printf("[X] _POSIX_THREAD_PRIO_PROTECT\n");
#endif
#if _POSIX_THREAD_PROCESS_SHARED > 0L
	printf("[O] _POSIX_THREAD_PROCESS_SHARED\n");
#else
	printf("[X] _POSIX_THREAD_PROCESS_SHARED\n");
#endif
#if _POSIX_THREAD_SAFE_FUNCTIONS > 0L
	printf("[O] _POSIX_THREAD_SAFE_FUNCTIONS\n");
#else
	printf("[X] _POSIX_THREAD_SAFE_FUNCTIONS\n");
#endif
#if _POSIX_THREAD_SPORADIC_SERVER > 0L
	printf("[O] _POSIX_THREAD_SPORADIC_SERVER\n");
#else
	printf("[X] _POSIX_THREAD_SPORADIC_SERVER\n");
#endif
#if _POSIX_TIMEOUTS > 0L
	printf("[O] _POSIX_TIMEOUTS\n");
#else
	printf("[X] _POSIX_TIMEOUTS\n");
#endif
#if _POSIX_TIMERS > 0L
	printf("[O] _POSIX_TIMERS\n");
#else
	printf("[X] _POSIX_TIMERS\n");
#endif
#if _POSIX_TRACE > 0L
	printf("[O] _POSIX_TRACE\n");
#else
	printf("[X] _POSIX_TRACE\n");
#endif
#if _POSIX_TRACE_EVENT_FILTER > 0L
	printf("[O] _POSIX_TRACE_EVENT_FILTER\n");
#else
	printf("[X] _POSIX_TRACE_EVENT_FILTER\n");
#endif
#if _POSIX_TRACE_INHERIT > 0L
	printf("[O] _POSIX_TRACE_INHERIT\n");
#else
	printf("[X] _POSIX_TRACE_INHERIT\n");
#endif
#if _POSIX_TRACE_LOG > 0L
	printf("[O] _POSIX_TRACE_LOG\n");
#else
	printf("[X] _POSIX_TRACE_LOG\n");
#endif
#if _POSIX_TYPED_MEMORY_OBJECTS > 0L
	printf("[O] _POSIX_TYPED_MEMORY_OBJECTS\n");
#else
	printf("[X] _POSIX_TYPED_MEMORY_OBJECTS\n");
#endif
#if _POSIX_VDISABLE > 0L
	printf("[O] _POSIX_VDISABLE\n");
#else
	printf("[X] _POSIX_VDISABLE\n");
#endif
	return 0;
}
