/* vim: set ts=4 sw=4: */
/* Filename    : sig_waitid.c
 * Description : 
 * Author      : SunYoung Kim <sunyzero@gmail.com>
 * Notes       : 
 */
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void sa_handler_chld(int signum);

int main()
{
	int		ret;
	struct sigaction sa_chld;

	memset(&sa_chld, 0, sizeof(struct sigaction));
	sa_chld.sa_handler = sa_handler_chld;
	sigfillset(&sa_chld.sa_mask);	/* fill signal block mask. */

	sigaction(SIGCHLD, &sa_chld, NULL);
	printf("[MAIN] SIGNAL Handler installed, pid(%d)\n", getpid());
	switch((ret = fork())) {
		case 0:
			pause();
			exit(EXIT_SUCCESS);
		case -1:
			break;
		default:
			printf("- Child pid = %d\n", ret);
			break;
	}
		
	while (1) {
		pause();
		printf("[MAIN] Recv SIGNAL...\n");
	}
	return EXIT_SUCCESS;
}

void sa_handler_chld(int signum)
{
    printf("[SIGNAL] RECV SIGCHLD signal\n");
    int     optflags = WNOHANG|WEXITED|WSTOPPED|WCONTINUED;
    siginfo_t wsiginfo = {.si_pid = 0};
    char    *str_status;
    while (1) {
        if (waitid(P_ALL, 0, &wsiginfo, optflags) == 0 && wsiginfo.si_pid != 0) {
            switch (wsiginfo.si_code) {
                case CLD_EXITED:
                    str_status = "Exited";
                    break;
                case CLD_KILLED:
                    str_status = "Killed";
                    break;
                case CLD_DUMPED:
                    str_status = "Dumped";
                    break;
                case CLD_STOPPED:
                    str_status = "Stopped";
                    break;
                case CLD_CONTINUED:
                    str_status = "Continued";
                    break;
                default:
                    str_status = "si_code";
                    break;
            }
            printf("child pid(%d) %s(%d)\n", 
                    wsiginfo.si_pid, str_status, wsiginfo.si_status);
        } else {
            break;
        }
    }
}
