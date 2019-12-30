#### Name and directory configuration
PRJ_DIR			?= 
INC_DIR			?=
LIB_DIR 		?=

#### Linux domain configuration
SRCS 			?= inet_tcp_serv1.c
OBJS 			?= $(SRCS:%.c=%.o)
APPS 			?= $(SRCS:%.c=%)

CFLAGS 			?= -W -Wall
CPPFLAGS 		?= -I$(INC_DIR)
LDFLAGS 		?= -L$(LIB_DIR)
LDLIBS 			?= -lpthread -lrt
LDLIBS 			+=

#### General configuration
CC 				= gcc
RM 				= rm

#### Start compling
all :: $(APPS)

$(APPS) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ -g $< $(LDFLAGS) $(LDLIBS)

clean :
	$(RM) -f $(APPS) $(OBJS)

#### Suffix Rule
.c.o :
	$(CC) -c -o $@ $< 