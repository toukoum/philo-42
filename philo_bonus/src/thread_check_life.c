/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_check_life.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 22:24:48 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/10 17:23:47 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/**
 * check if a philo is dead
 * => time_since_last_meal >= time_to_die
 */
static int	check_philo_dead(t_philo *philo)
{
	size_t	t_since_last_meal;

	sem_wait(philo->sem_last_meal);
	t_since_last_meal = get_time() - philo->last_meal;
	sem_post(philo->sem_last_meal);
	return (t_since_last_meal > philo->table->time_to_die);
}

/**
 * check every 0.1ms if the philo is dead or if every philo has eaten enough
 * use the mutex last meal, and time_to_die to know
 * increment sem_eat_full if philo has eat enough
 */
void	*handle_life(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	sem_wait(philo->table->sem_eat_full);
	sem_wait(philo->table->sem_end);
	synch_start(philo->table->start_time_simu);
	while (1)
	{
		if (check_philo_dead(philo))
		{
			sem_post(philo->table->sem_end);
			msg_log(philo, DIE);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
