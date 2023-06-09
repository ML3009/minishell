/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmessmer <wmessmer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 08:49:08 by ml                #+#    #+#             */
/*   Updated: 2023/06/12 16:25:13 by wmessmer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void    signal_ctrlc(int sig) 
{
    (void)sig;
    write(2, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay(); 
}