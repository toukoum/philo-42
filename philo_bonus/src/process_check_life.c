/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_check_life.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 21:34:44 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/11 09:58:26 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/**
 * thread the monitoring process for checking if
 * the global semaphore end is set to one
 * kill all philo if it's the case.
 */
void	*check_dead_philo(void *philos_ptr)
{
	t_philo	**philos;
	t_table	*table;

	philos = (t_philo **)philos_ptr;
	table = philos[0]->table;
	synch_start(table->start_time_simu);
	sem_wait(table->sem_end);
	sem_wait(table->sem_set_end);
	table->end = 1;
	sem_post(table->sem_set_end);
	kill_all_philo(table, philos);
	return (NULL);
}

int	is_sim_end(t_table *table)
{
	int	bool_end;

	bool_end = 0;
	sem_wait(table->sem_set_end);
	bool_end = table->end;
	sem_post(table->sem_set_end);
	return (bool_end);
}

/**
 * thread the monitoring process for checking if
 * all philo has enough eat
 */
void	*check_eat_enough(void *philos_ptr)
{
	t_philo	**philos;
	t_table	*table;
	size_t	count_meal;

	count_meal = 0;
	philos = (t_philo **)philos_ptr;
	table = philos[0]->table;
	synch_start(table->start_time_simu);
	while (count_meal < table->number_philo)
	{
		if (is_sim_end(table))
			return (NULL);
		sem_wait(table->sem_eat_full);
		if (!is_sim_end(table))
			count_meal++;
		else
			return (NULL);
	}
	sem_post(table->sem_end);
	return (NULL);
}

void	stop_simulation(t_table *table, t_philo **philos)
{
	pthread_t	thread_death;
	pthread_t	thread_meal;

	if (pthread_create(&thread_death, NULL, &check_dead_philo, philos))
		kill_all_philo(table, philos);
	if (table->must_eat)
	{
		if (pthread_create(&thread_meal, NULL, &check_eat_enough, philos))
			kill_all_philo(table, philos);
		if (pthread_join(thread_meal, NULL))
			kill_all_philo(table, philos);
	}
	if (pthread_join(thread_death, NULL))
		kill_all_philo(table, philos);
}
