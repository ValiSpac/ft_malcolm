#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include "../libft/libft.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>

#define USAGE "usage: sudo ./ft_malcolm <source ip> <source mac> <target ip> <target mac> + options: -i(interface) -t(timeout) -v(verbose)"
#define IPADDR "Invalid ip address/hostname: "
#define MACADDR "Invalid mac address: "
#define OPTION "Invalid option: "
#define ARGSERR "Wrong number of arguments arguments!"
#define PERM "Insufficent permission!"
#define PROBIP "Problem parsing the IP address: "
#define PROBMAC "Problem parsing the MAC address: "

typedef struct s_mac{
    uint8_t binary[6];
    char   str[18];
}   t_mac;

typedef struct s_env{
    struct sockaddr_in  *source_ip;
    struct sockaddr_in  *target_ip;
    t_mac               *source_mac;
    t_mac               *target_mac;
    int                 ver;
    int                 tout;
    unsigned int        interf;
}t_env;

char*        parse_options(t_env *env, char **av);
char*        check_args(int ac, char **av);
void         free_env(t_env *env);


#endif
