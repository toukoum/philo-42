/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_check_life.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 21:34:44 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 05:06:54 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/**
 * fork the monitoring process for checking if
 * the global semaphore end is set to one
 * kill all philo if it's the case.
 */
int	check_dead_philo(t_table *table)
{
	pid_t	pid_fork;
	int		status;

	pid_fork = fork();
	if (pid_fork == -1)
		return (kill_all_philo(table), 1);
	if (pid_fork == 0)
	{
		synch_start(table->start_time_simu);
		sem_wait(table->sem_end);
		kill_all_philo(table);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid_fork, &status, 0);
	return (pid_fork);
}

/**
 * fork the monitoring process for checking if
 * all philo has enough eat
 */
int	check_eat_enough(t_table *table)
{
	pid_t	pid_fork;
	size_t	count;

	pid_fork = fork();
	if (pid_fork == -1)
		return (kill_all_philo(table), 1);
	if (pid_fork == 0)
	{
		count = 0;
		synch_start(table->start_time_simu);
		while (count < table->number_philo)
		{
			sem_wait(table->sem_eat_full);
			count++;
		}
		sem_post(table->sem_end);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

void	stop_simulation(t_table *table)
{
	if (table->must_eat)
		check_eat_enough(table);
	check_dead_philo(table);
}