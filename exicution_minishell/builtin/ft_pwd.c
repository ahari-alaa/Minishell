/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maskour <maskour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:51:33 by maskour           #+#    #+#             */
/*   Updated: 2025/06/25 15:04:15 by maskour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_pwd() t_env *env_list)
{
    char cwd[PATH_MAX];//the path max is in inlude <limits.h>
    t_env *env;
    env = env_list;
    if (getcwd(cwd, sizeof(cwd)))// getcwd in include<unistd.h>
    {
            printf("%s\n",cwd);
            
    }
    else
    {
      while (env)
    {
        if (strncmp(env->data_env, "PWD=", 4) == 0)
        {
            printf("%s\n", env->data_env + 4);
            
            return ;
        }
        env = env->next;
    }
    
    perror("PWD not found in environment\n");  
    }
}   
