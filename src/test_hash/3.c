/* **************************************************************
 * Copyright(c) 2015, panh. All rights reserverd.
 *     Name : 3.c
 * Describe : 
 *   Author : Pan Hao
 *     Date : 2016-01-05 16:50:04
 *  Version : 1.0
 * **************************************************************/

#include <stdio.h>
#include <unistd.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/socket.h> 

int main(int argc, char const* argv[])
{
    int s = socket(PF_INET,SOCK_STREAM,0); 
    int rcv_size = 0;
    socklen_t optlen = sizeof(rcv_size);
    if (0 == getsockopt(s, SOL_SOCKET, SO_RCVBUF,&rcv_size, &optlen)) {
        printf("recv_buff size is [%d]\n", rcv_size);
    }

    return 0;
}
