/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:54:59 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 18:39:36 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * launching and creating thread for each philo
 * use the ROUTINE function
 * (error) ? 1 : 0
 * launch also the thread for handling the life of the philosophers
 */
static int	launch_simulation(t_table *table)
{
	size_t	i;

	i = 0;
	while (i < table->number_philo)
	{
		if (pthread_create(&table->philos[i].thread, NULL, &routine,
				&table->philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(&table->philo_life, NULL, &handle_life, table))
		return (1);
	i = 0;
	while (i < table->number_philo)
	{
		if (pthread_join(table->philos[i].thread, NULL))
			return (1);
		i++;
	}
	if (pthread_join(table->philo_life, NULL))
		return (1);
	return (0);
}

/**
 * normalement tout est bien initialise,
	a tester dans routine avec une fonction simple.
 */
int	main(int argc, char *argv[])
{
	t_table	table;

	if (ft_parse(argc, argv, &table))
		return (msg_err(ARG_ERR));
	table.start_time_simu = get_time() + (table.number_philo * 20);
	pthread_mutex_lock(&table.m_log);
	printf("\n=> Start Simulation time:%s %zu%s\n\n", FLASH,
		(size_t)0, NC);
	pthread_mutex_unlock(&table.m_log);
	if (launch_simulation(&table))
		return (msg_err(SIMULATION_ERR));
	ft_free(&table);
	return (0);
}
