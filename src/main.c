#include "../inc/ft_malcolm.h"

int setup_socket(t_env *env)
{
    struct timeval rcv_timeout;
    rcv_timeout.tv_sec = env->tout;
    rcv_timeout.tv_usec = 0;
    env->sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (env->sock_fd < 0)
        return -1;
    if (setsockopt(env->sock_fd, SOL_SOCKET, SO_RCVTIMEO,  (const char*)&rcv_timeout, sizeof(rcv_timeout)) < 0)
    {
        dprintf(2, "Setsockopt failed!\n");
        close(env->sock_fd);
        return -1;
    }
    if (setsockopt(env->sock_fd, SOL_SOCKET, SO_BINDTODEVICE, env->interf_str, ft_strlen(env->interf_str)) < 0)
    {
        dprintf(2, "Bind to interface failed!\n");
        close(env->sock_fd);
        return -1;
    }
    return 0;
}

int main(int ac, char **av)
{
    int tmp;
    if (getuid() != 0)
        return (printf("%s\n%s\n", PERM, USAGE), -1);
    char *args = check_args(ac, av);
    if (args)
        return (printf("%s\n%s\n ", args, USAGE), free(args), -1);
    t_env *env;
    if ((env = (t_env *)malloc(sizeof(t_env))) == NULL)
        return (-1);
    char *opt = parse_options(env, av);
    if (opt)
        return (free_env(env),printf("%s\n", opt), free(opt), -1);
    if ((tmp = setup_socket(env)) == -1)
        return (free_env(env), -1);
    printf("%d\n%d\n%d\n%d\n", env->interf, env->tout, env->ver, env->source_mac->binary[1]);
    free_env(env);
}
