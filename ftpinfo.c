#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

struct ftp_stat{
	unsigned int read_ops;
	unsigned int write_ops;
	unsigned int read_bd;
	unsigned int write_bd;
        unsigned int total_ops;
        unsigned int total_bd;
};

void usage()
{
	char appname[]="ftp-info";
	char **argv = (char **)malloc(2*sizeof(char *));
	argv[0] = appname;
	argv[1] = (char *)malloc(3*sizeof(char));
	memcpy(argv[1],"-h\0",3);
	printf("example [ftp-info show_node_stat] \n");
	exit(0);
}
int main(int argc,char *argv[])
{

	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	int i,byte;
	struct ftp_stat  char_recv;
	//char *char_send = (char *)malloc(sizeof(struct ftp_stat));
	char send_buf[1024];
//	printf("buf size =%d\n",sizeof(send_buf));
	memset(send_buf,0,sizeof(send_buf));
	if ((argc > 1 && (strcmp(argv[1],"-h") == 0)) || argc >2)
	{
		usage();
		return -1;
	}
	if( argc ==2 &&  strcmp(argv[1],"show_node_stat")==0) {
		strcpy(send_buf,argv[1]);
	}else {
        	usage();
		return -1;
        }
	
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	
	address.sin_family=AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port=htons(8899);
	len = sizeof(address);
	
	if((result = connect(sockfd,(struct sockaddr *)&address,len)) == -1)
	{
		perror("connect falied");
		exit(EXIT_FAILURE);
	}
//	printf("enter the context\n");
	//scanf("%c",&char_send);
	if(byte==send(sockfd,send_buf,sizeof(send_buf),0)==-1){
		perror("send");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
        
	if((byte = recv(sockfd,&char_recv,sizeof(struct ftp_stat),0))==-1){
		perror("recv");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
        //memcpy(char_send,&char_recv,sizeof(char_recv));
      //	printf("receive from server write %d, read %d\n",char_recv.write_ops,char_recv.read_ops);
	printf("write_bandwidth\t read_bandwidth\t total_bandwidth \t write_ops\t read_ops\t total_ops \n");
	printf("%10d\t %10d\t %10d\t %17d\t %6d\t %11d\t \n",char_recv.write_bd,char_recv.read_bd,char_recv.total_bd,char_recv.write_ops,char_recv.read_ops,char_recv.total_ops);
	close(sockfd);
	exit(0);
}

