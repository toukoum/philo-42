/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 22:33:19 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 22:41:23 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/**
 * for wait child philo to finish
 * when fork() goes wrong
 */
int	ft_wait_philo(int i, pid_t *child_pids)
{
	while (i > 0)
	{
		waitpid(child_pids[i - 1], NULL, 0);
		exit(EXIT_FAILURE);
		i--;
	}
	return (INIT_ERR);
}

void	ft_free(t_table *table)
{
	free(table->child_pids);
	sem_close(table->sem_forks);
	sem_unlink("/forks");
	sem_close(table->sem_log);
	sem_unlink("/log");
	sem_close(table->sem_eat_full);
	sem_unlink("/eat_full");
	sem_close(table->sem_end);
	sem_unlink("/end");
}

int	msg_err(int key_error)
{
	if (key_error == ARG_ERR)
		printf("\033[1;31mError, please enter valid arguments.\033[0m\n\n"
			" => \033[1;34m./philo <number_of_philo>"
			" <time_to_die> <time_to_eat>"
			" <time_to_sleep> [number_of_time_must_eat]\033[0m\n\n");
	else if (key_error == SIMULATION_ERR)
		printf("\033[1;31mError with simulation !\033[0m\n\n");
	else if (key_error == INIT_ERR)
		printf("Error with memory !");
	return (EXIT_FAILURE);
}

/**
 * kill all the philo by their pid
 */
void	kill_all_philo(t_table *table)
{
	size_t	i;
	int		status;

	i = 0;
	while (i < table->number_philo)
	{
		kill(table->child_pids[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < table->number_philo)
	{
		waitpid(table->child_pids[i], &status, 0);
		i++;
	}
}
