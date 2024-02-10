/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:54:59 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/10 15:34:05 by rgiraud          ###   ########.fr       */
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
static int	launch_simulation(t_philo **philos, t_table *table)
{
	size_t	i;

	i = 0;
	while (i < table->number_philo)
	{
		((*philos) + i)->pid_philo = fork();
		if (((*philos) + i)->pid_philo == -1)
			return (ft_wait_philo(i, philos));
		else if (((*philos) + i)->pid_philo == 0)
			return (routine_philosopher((*philos) + i));
		i++;
	}
	return (0);
}

int	init_philo(t_philo **philos, t_table *table)
{
	size_t	i;

	i = 0;
	*philos = malloc((table->number_philo) * sizeof(t_philo));
	if (!philos)
		return (INIT_ERR);
	while (i < table->number_philo)
	{
		if (init_process_philo((*philos) + i, i, table))
			return (INIT_ERR);
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_table	table;
	t_philo	*philos;

	if (ft_parse(argc, argv, &table))
		return (msg_err(ARG_ERR));
	table.start_time_simu = get_time() + (table.number_philo * 42);
	if (init_philo(&philos, &table))
		return (EXIT_FAILURE);
	sem_wait(table.sem_log);
	printf("\n=> Start Simulation time:%s %zu%s\n\n", FLASH, (size_t)0, NC);
	sem_post(table.sem_log);
	if (launch_simulation(&philos, &table))
		return (msg_err(SIMULATION_ERR));
	stop_simulation(&table, &philos);
	ft_free(&table, &philos);
	return (0);
}
