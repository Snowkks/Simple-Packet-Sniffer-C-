#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

int main() {

    int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));


    while(1){

        unsigned char *buffer = ( unsigned char * )malloc(65553);
        int data_size;
        struct sockaddr sddr;
        int sddr_size = sizeof(sddr);

        data_size = recvfrom(sock_raw, buffer, 65553, 0, &sddr, &sddr_size);

        if (data_size == 0) {
            printf("Failed to recieve");
            return 1;
        }

        struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
        unsigned char *src = (unsigned char *)&ip->saddr;
        unsigned char *dst = (unsigned char *)&ip->daddr;
        struct tcphdr *tcp = (struct tcphdr *)(buffer + sizeof(struct ethhdr) + ip->ihl * 4);

        if( ip->protocol == 1 ){
            printf("[Ping] %d.%d.%d.%d -> %d.%d.%d.%d\n",
                   src[0], src[1], src[2], src[3],
                   dst[0], dst[1], dst[2], dst[3]);
        } else if ( ip->protocol == 6 ) {
            printf("[TCP] %d.%d.%d.%d -> %d.%d.%d.%d Port:%d\n",
                   src[0], src[1], src[2], src[3],
                   dst[0], dst[1], dst[2], dst[3],
                   ntohs(tcp->dest));
        } else if ( ip->protocol == 17 ) {
            printf("[UDP] %d.%d.%d.%d -> %d.%d.%d.%d Port:%d\n",
                   src[0], src[1], src[2], src[3],
                   dst[0], dst[1], dst[2], dst[3],
                   ntohs(tcp->dest));
        }
    }

    return 0;
}
