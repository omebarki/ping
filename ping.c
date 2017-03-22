#include<stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define ICMP_MAX_DATA_LENGTH 48
typedef struct __attribute__((packed)){
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	unsigned short identifier;
	unsigned short sequence_number;
}icmp_header;
#define ICMP_HEADER_LENGTH sizeof(icmp_header)

typedef struct __attribute__((packed)) {
	icmp_header header;
	unsigned char data[ICMP_MAX_DATA_LENGTH];
	unsigned short data_length;
} icmp_message;

unsigned int icmp_packet_length(icmp_message* packet){
	return ICMP_HEADER_LENGTH + packet->data_length; 
}

unsigned short compute_icmp_checksum (void * addr, unsigned int length){
	unsigned short *buf = addr;
	int len = length;

	unsigned int sum=0;
	unsigned short result;

	for ( sum = 0; len > 1; len -= 2 )
		sum += *buf++;
	if ( len == 1 )
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

void init_ping_header(icmp_header * ping_header,
			      unsigned short identifier, 
			      unsigned short sequence_number){
	ping_header->type = 8;
	ping_header->code = 0;
	ping_header->checksum = 0;
	ping_header->identifier = htons(identifier);
	ping_header->sequence_number = htons(sequence_number);

}

void init_ping_message(icmp_message * ping_message, 
		       unsigned short identifier,
		       unsigned short sequence_number,
		       unsigned char * data,
		       unsigned short data_length){
	init_ping_header(&(ping_message->header), identifier,sequence_number);
	bzero(ping_message->data, ICMP_MAX_DATA_LENGTH);
	memcpy(ping_message->data, data, data_length);
	ping_message->data_length = data_length;
	unsigned short packet_length = icmp_packet_length(ping_message);
	ping_message->header.checksum = compute_icmp_checksum(ping_message, 
							     packet_length);
}
		       
		
int main(int argc, char *argv[]){
	char * ip_addr = "184.7.70.70";
	char * ping_data = "Hello World!";
	icmp_message ping_packet;
	init_ping_message(&ping_packet,
			  1,
			  1,
			  ping_data,
			  strlen(ping_data) );
	
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = inet_addr(ip_addr);

	int socketfd;
	socketfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if ( sendto(socketfd, &ping_packet, icmp_packet_length(&ping_packet), 0,(struct sockaddr*) &addr, sizeof(addr)) <= 0 )
			perror("sendto");
	sleep(1);
	
	return 0;
}	
