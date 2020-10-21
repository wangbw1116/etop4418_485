#include "rs485.h"

int openSerialPort(char *port)
{
    int fd = open(port, O_RDWR);
    if(fd <= 0){
        printf("open port %s failed\n", port);
        return fd;
    }
    if(fcntl(fd, F_SETFL, 0) < 0)
        printf("fcntl failed!\n");
    else
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
    //测试打开的文件描述符是否引用一个终端设备，以进一步确认串口是否正确打开
    if(isatty(STDIN_FILENO)==0)
        printf("standard input is not a terminal device\n");
    else
        printf("isatty success!\n");
    
    printf("fd-open=%d\n",fd);
    return fd;
}

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    //保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息
    /*
    if ( tcgetattr( fd,&oldtio) != 0){
        perror("SetupSerial 1");
        return -1;
    }
    */
    bzero( &newtio, sizeof( newtio ) );
    //设置字符大小
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
    //设置数据位
    switch( nBits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }
    //设置校验位
    switch( nEvent )
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }
    //设置波特率
    switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    //设置停止位
    if( nStop == 1 )
        newtio.c_cflag &= ~CSTOPB;
    else if ( nStop == 2 )
        newtio.c_cflag |= CSTOPB;
    //设置等待时间和最小接收字符
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    //处理未接收字符
    tcflush(fd,TCIFLUSH);
    //激活新配置
    if((tcsetattr(fd,TCSANOW,&newtio))!=0){
        perror("com set error");//打印com set error及出错原因
        return -1;
    }
    printf("set done!\n");
    return 0;
}

int prepare_to_send(int fd)
{
	int ret;

	ret = ioctl(fd, 1, 0);
	if(ret<0)
	{
		printf("rs485 set ctl to high failed!\r\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

int sendData(int fd, char *data, int len)
{
    int ret = 0;
    ret = prepare_to_send(fd);
    if(ret < 0)
        return ret;
    ret = write(fd, data, len);
    return ret;
}

int prepare_to_recv(int fd)
{
	int ret;

	ret = ioctl(fd, 0, 0);
	if(ret<0)
	{
		printf("rs485 set ctl to low failed!\r\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

int recvData(int fd, char *data, int len)
{
    int ret = 0;
    ret = prepare_to_recv(fd);
    if(ret < 0)
        return ret;
    ret = read(fd, data, len);
    return ret;
}