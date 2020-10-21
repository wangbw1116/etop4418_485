#include "rs485.h"

int main()
{
    int fd = openSerialPort("/dev/ttyAMA1");
    printf("fd: %d\n", fd);
    set_opt(fd, 9600, 8, 'N', 1);
    char sendBuf[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    char recvBuf[100] = {0};
    int sendBytes = 0, recvBytes = 0, i, total=0;
    while(1){
        recvBytes = recvData(fd, recvBuf+total, 100-total);
        printf("recvBytes: %d\n", recvBytes);
        total += recvBytes;
        if(total > 0){
            printf("recvBuf: ");
            for(i = 0; i < total; i++)
                printf("%d ", recvBuf[i]);
            printf("\n");
        }
        sendBytes = sendData(fd, sendBuf, 8);
        printf("sendBytes: %d\n", sendBytes);
        sleep(1);
    }
    return 0;
}