/* vim: set ts=4 sw=4: */
/* Filename    : struct_xdr.c
 * Description : XDR (eXternal Data Representation)
 * Author      : Sun-young Kim
 * Notes       : 
 */
#include <stdio.h>
#include <stddef.h>
#define print_strt_offset(strt_name_suffix,strt_member)    \
    printf("sizeof(STRT_TRANS_"#strt_name_suffix") = %zd ,", \
			sizeof(STRT_TRANS_##strt_name_suffix)); \
			printf("offsetof(...,"#strt_member") = %zd\n", offsetof(STRT_TRANS_##strt_name_suffix,strt_member))
typedef struct {
    char    soh[1];     /* 1 byte   */
    int     len;        /* 4 bytes  */
    char    cname[5];   /* 5 bytes  */
    short   svc;        /* 2 bytes  */
    short   ret;        /* 2 bytes  */
    char    rsvd[20];
} STRT_TRANS_PAD;       /* total 40 bytes */

typedef struct {
    char    soh[1];
    int     len;
    char    cname[5];
    short   svc;
    short   ret;
    char    rsvd[20];
} __attribute__((packed)) STRT_TRANS_PACK;

int main()
{
	print_strt_offset(PAD,len);
	print_strt_offset(PAD,cname);
	print_strt_offset(PAD,svc);
	print_strt_offset(PAD,ret);

	print_strt_offset(PACK,len);
	print_strt_offset(PACK,cname);
	print_strt_offset(PACK,svc);
	print_strt_offset(PACK,ret);
	return 0;
}
