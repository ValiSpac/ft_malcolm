#include "../inc/ft_malcolm.h"

int is_valid_ip(char *ip)
{
    struct sockaddr_in sa;
    int result;
    struct addrinfo hints , *res;
    size_t i = 0;
    while (i < ft_strlen(ip) && (ft_isdigit(ip[i]) || ip[i] == '.'))
        i++;
    if (inet_pton(AF_INET, ip, &(sa.sin_addr)) == 1)
            return 1;
    ft_memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    result = getaddrinfo(ip, NULL, &hints, &res);
    if (result != 0 || i == ft_strlen(ip)){
        printf("bad hostname\n");
        return 0;}
    freeaddrinfo(res);
    return 1;
}

int is_valid_mac(char *mac)
{
    if (ft_strlen(mac) != 17)
        return (0);
    char **mac_str = ft_split(mac, ':');
    int i = 0, j = 0;
    char *hex = "0123456789abcdefABCDEF";
    while (mac_str[i])
    {
        j = 0;
        while (mac_str[i][j])
        {
            if (!ft_strchr(hex, mac_str[i][j]))
            {
                ft_split_free(mac_str);
                return (0);
            }
            j++;
        }
        i++;
    }
    ft_split_free(mac_str);
    return (1);
}

char *check_args(int ac, char **av)
{
    int i = 0;
    if (ac > 8 || ac < 5)
        return (ft_strjoin(ARGSERR, ""));
    if (!is_valid_ip(av[++i]))
        return (ft_strjoin(IPADDR, av[i]));
    if (!is_valid_mac(av[++i]))
        return (ft_strjoin(MACADDR, av[i]));
    if (!is_valid_ip(av[++i]))
        return (ft_strjoin(IPADDR, av[i]));
    if (!is_valid_mac(av[++i]))
        return (ft_strjoin(MACADDR, av[i]));
    while (av[++i])
        if (ft_strncmp(av[i], "-i", 2) && ft_strcmp(av[i], "-v") && ft_strncmp(av[i], "-t", 2) && ft_strcmp(av[i], ""))
            return (ft_strjoin(OPTION, av[i]));
    return NULL;
}

t_mac *get_mac(char *mac_str)
{
    t_mac *mac;
    int values[6];
    if ((mac = (t_mac *)malloc(sizeof(t_mac))) == NULL)
        return (NULL);
    ft_memset(mac, 0, sizeof(t_mac));
    ft_strcpy(mac->str, mac_str);
    sscanf(mac_str, "%x:%x:%x:%x:%x:%x",
               &values[0], &values[1], &values[2],&values[3], &values[4], &values[5]);

    for (int i = 0; i < 6; i++) {
        mac->binary[i] = (uint8_t)values[i];
    }
    return (mac);
}

struct sockaddr_in* get_ip(char *ip_str)
{
    struct sockaddr_in *sa;
    struct hostent     *host_entry;
    if ((sa = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in))) == NULL)
        return (NULL);
    ft_memset(sa, 0, sizeof(struct sockaddr_in));
    if (inet_pton(AF_INET, ip_str, &(sa->sin_addr)) == 1)
    {
        sa->sin_family = AF_INET;
        return (sa);
    }
    host_entry = gethostbyname(ip_str);
    if (host_entry == NULL)
    {
        free(sa);
        return (NULL);
    }
    sa->sin_family = AF_INET;
    ft_memcpy(&(sa->sin_addr), host_entry->h_addr_list[0], host_entry->h_length);
    return sa;
}

char*        parse_options(t_env *env, char **av)
{
    int i = 0;
    env->tout = 0;
    env->ver = 0;
    env->sock_fd = 0;
    env->interf = 0;
    env->source_ip = get_ip(av[++i]);
    if (env->source_ip == NULL)
        return (ft_strjoin( PROBIP, av[i]));
    env->source_mac = get_mac(av[++i]);
    if (env->source_mac == NULL)
        return (ft_strjoin( PROBMAC, av[i]));
    env->target_ip = get_ip(av[++i]);
    if (env->target_ip == NULL)
        return (ft_strjoin(PROBIP , av[i]));
    env->target_mac = get_mac(av[++i]);
    if (env->target_mac == NULL)
        return (ft_strjoin(PROBMAC , av[i]));
    while (av[++i])
    {
        if (!strncmp(av[i], "-i", 2))
        {
            char *tmp = av[i] + 2;
            env->interf = if_nametoindex(tmp);
            if (!env->interf)
                return (ft_strjoin("Unable to find interface: ", tmp));
            env->interf_str = strdup(tmp);
        }
        else if (!strncmp(av[i], "-v", 2))
            env->ver = 1;
        else if (!strncmp(av[i], "-t", 2) && ft_strlen(av[i]) > 2)
        {
            char *tmp = av[i] + 2;
            while (*tmp)
            {
                if(!ft_isdigit(*tmp) || ft_strlen(tmp) > 3)
                    return (ft_strjoin("Invalid timeout option: ", av[i] + 2));
                tmp++;
            }
            env->tout = ft_atoi(av[i] + 2);
        }
    }
    if (!env->interf)
        env->interf = if_nametoindex("eth0");
    if (!env->interf)
        return (ft_strjoin("Unable to find default interface: eth0 (use -i option)", ""));
    return NULL;
}

void free_env(t_env *env)
{
    free(env->source_ip);
    free(env->target_ip);
    free(env->source_mac);
    free(env->target_mac);
    if (env->interf_str)
        free(env->interf_str);
    if (env->sock_fd)
        close(env->sock_fd);
    free(env);
}
