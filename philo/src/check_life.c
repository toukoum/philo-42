/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 22:24:48 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/01 14:58:49 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**hilo_bonus
 * check if a philo is dead
 * => time_since_last_meal > time_to_die
 */
static int	check_philo_dead(t_philo *philo)
{
	size_t	t_since_last_meal;

	pthread_mutex_lock(&philo->table->m_last_meal);
	t_since_last_meal = get_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->table->m_last_meal);
	if (t_since_last_meal >= philo->table->time_to_die)
	{
		set_end_simu(philo->table);
		usleep(1000);
		msg_log(philo, DIE);
		return (1);
	}
	return (0);
}

/**
 * check if all philo has eaten enough 1 if all philo is ok
 * else 0
 */
static int	reach_end(t_table *table)
{
	size_t	i;
	int		ate_enough;

	ate_enough = 1;
	i = 0;
	while (i < table->number_philo)
	{
		if (check_philo_dead(&table->philos[i]))
			return (1);
		if (table->must_eat)
		{
			pthread_mutex_lock(&table->m_count_meal);
			if (table->philos[i].count_meal < table->must_eat)
				ate_enough = 0;
			pthread_mutex_unlock(&table->m_count_meal);
		}
		i++;
	}
	if (table->must_eat && ate_enough)
		return (set_end_simu(table), 1);
	else
		return (0);
}

/**
 * check every 0.1ms if the philo is dead or if every philo has eaten enough
 * use the mutex last meal, and time_to_die to know
 * use
 */
void	*handle_life(void *table_ptr)
{
	t_table	*table;

	table = (t_table *)table_ptr;
	synch_start(table->start_time_simu);
	while (!end_simu(table))
	{
		if (reach_end(table))
			break ;
		usleep(1000);
	}
	return (NULL);
}
