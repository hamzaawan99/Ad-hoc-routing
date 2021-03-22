#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

using namespace std;

pthread_t tid; // thread id

struct sockaddr_in server_address, client_address;
int connection_socket;
socklen_t address_size;
char send_msg[2];
char receive_msg[10];

// Thread which will always be listening and receiving from other node
void* always_receiving(void* arg) {
	bool cond = true;
	sleep(3);
	while(1) {
		recvfrom(connection_socket, receive_msg, sizeof(receive_msg), 0, (struct sockaddr *) &client_address, (socklen_t *) &client_address);
		if (cond) {
			printf("received: %s\n", receive_msg);
			cond = false;
		}
	}
	//pthread_exit(NULL);
}
class Node {

};

int main(int argc, char** argv) {
	//struct sockaddr_in server_address, client_address;
	//int connection_socket;
	//socklen_t address_size;
	int err;
	send_msg[0] = argv[2][0];
	connection_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if(connection_socket < 0) {
		printf("Socket not Created!!!\n");
		return 0;
	}

	pthread_create(&tid, NULL, always_receiving, NULL);
	
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[1]));
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(server_address.sin_zero,'\0',8);

	err = bind(connection_socket, (struct sockaddr *) &server_address, sizeof(struct sockaddr));
	if(err < 0) {
		printf("Binding Failed!!!\n");
		return 0;
	}

	bool cond1 = true;
	while(1) {
		sendto(connection_socket, send_msg, strlen(send_msg), 0, (struct sockaddr *) &server_address, sizeof(server_address));
		if (cond1) {
			printf("sent: %s\n", send_msg);
			cond1 = false;
		}
	}

	//pthread_create(&tid[0], NULL, always_receiving, NULL);
	return 0;
}
