#include "../inc/ft_malcolm.h"

int main(int ac, char **av)
{
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
    printf("%d\n%d\n%d\n%d\n", env->interf, env->tout, env->ver, env->source_mac->binary[1]);
    free_env(env);
}
