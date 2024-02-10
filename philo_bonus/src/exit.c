/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 22:33:19 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/10 17:53:52 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/**
 * for wait child philo to finish
 * when fork() goes wrong
 */
int	ft_wait_philo(int i, t_philo **philos)
{
	while (i > 0)
	{
		waitpid(((*philos) + i)->pid_philo, NULL, 0);
		exit(EXIT_FAILURE);
		i--;
	}
	return (INIT_ERR);
}

void	ft_free(t_table *table, t_philo **philos)
{
	sem_close(table->sem_forks);
	sem_close(table->sem_log);
	sem_close(table->sem_eat_full);
	sem_close(table->sem_end);
	sem_close(table->sem_set_end);
	free(*philos);
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
void	kill_all_philo(t_table *table, t_philo **philos)
{
	size_t	i;
	int		status;

	i = 0;
	while (i < table->number_philo)
	{
		sem_close(((*philos) + i)->sem_count_meal);
		sem_close(((*philos) + i)->sem_last_meal);
		kill(((*philos) + i)->pid_philo, SIGKILL);
		i++;
	}
	i = 0;
	while (i < table->number_philo)
	{
		waitpid(((*philos) + i)->pid_philo, &status, 0);
		i++;
	}
}
