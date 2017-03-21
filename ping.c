#include<stdio.h>
#include <string.h>
#include <arpa/inet.h>

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

unsigned short compute_icmp_checksum (void * addr, int length){
	register long sum = 0;

	int count = length;
	while( count > 1 )  {
           /*  This is the inner loop */
               sum += * (unsigned short *) addr++;
               count -= 2;
       }

           /*  Add left-over byte, if any */
       if( count > 0 )
               sum += * (unsigned char *) addr;

           /*  Fold 32-bit sum to 16 bits */
       while (sum>>16)
           sum = (sum & 0xffff) + (sum >> 16);

       unsigned short checksum = ~sum;

	
	return checksum;
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
	ping_message->header.checksum = compute_icmp_checksum(&ping_message, 
							     ICMP_HEADER_LENGTH + data_length);
}
		       
			
