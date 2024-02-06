/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:54:59 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 04:52:38 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/**
 * launching and creating child process for each philo
 * use fork() for duplicate process
 * use the ROUTINE function
 * (error) ? 1 : 0
 * launch also the child process for handling the life of the philo
 */
static int	launch_simulation(t_table *table)
{
	size_t i;
	pid_t pid_fork;
	
	i = 0;
	while (i < table->number_philo)
	{
		pid_fork = fork();
		if (pid_fork == -1)
			return (ft_wait_philo(i, table->child_pids));
		else if (pid_fork == 0)
		{
			if (routine_philosopher(table, i))
				exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		else
			table->child_pids[i] = pid_fork;
		i++;
	}
	return (0);
}



int	main(int argc, char *argv[])
{
	t_table	table;
	
	unlink_my_sem();
	if (ft_parse(argc, argv, &table))
		return (msg_err(ARG_ERR));
	table.start_time_simu = get_time() + (table.number_philo * 42);
	sem_wait(table.sem_log);
	printf("\n=> Start Simulation time:%s %zu%s\n\n", FLASH,
		(size_t)0, NC);
	sem_post(table.sem_log);
	if (launch_simulation(&table))
		return (msg_err(SIMULATION_ERR));
	stop_simulation(&table);
	ft_free(&table);
	return (0);
}
