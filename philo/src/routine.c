/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 22:27:48 by rgiraud           #+#    #+#             */
/*   Updated: 2024/01/31 12:54:17 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/**
 * wait little bit then check the life of the philo, then re-wait
 */
static void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_left);
	msg_log(philo, TAKE_FORK);
	pthread_mutex_lock(philo->fork_right);
	msg_log(philo, TAKE_FORK);
	if (end_simu(philo->table))
	{
		pthread_mutex_unlock(philo->fork_right);
		pthread_mutex_unlock(philo->fork_left);
		return ;
	}
	msg_log(philo, EAT);
	pthread_mutex_lock(&philo->table->m_last_meal);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->table->m_last_meal);
	ft_wait(philo->table->time_to_eat, philo->table);
	if (!end_simu(philo->table))
	{
		pthread_mutex_lock(&philo->table->m_count_meal);
		philo->count_meal++;
		pthread_mutex_unlock(&philo->table->m_count_meal);
	}
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
}

static void	ft_sleep(t_philo *philo)
{
	if (end_simu(philo->table))
		return ;
	msg_log(philo, SLEEP);
	ft_wait(philo->table->time_to_sleep, philo->table);
}

/**
 * time to think =
 *
 * 	=> 0 if: time_to_die - (time_since_last_meal + time_to_eat) <= 0
 * 	=> 200 if > 600 ms to think
 */
static void	think(t_philo *philo)
{
	long long	time_to_think;
	long long	temp;

	if (end_simu(philo->table))
		return ;
	pthread_mutex_lock(&philo->table->m_last_meal);
	time_to_think = (philo->table->time_to_die);
	temp = ((get_time() - philo->last_meal) + philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->table->m_last_meal);
	if (temp >= time_to_think)
		time_to_think = 0;
	else
	{
		time_to_think -= temp;
		time_to_think /= 2;
		if (time_to_think > 500)
			time_to_think = 150;
	}
	msg_log(philo, THINK);
	ft_wait(time_to_think, philo->table);
	return ;
}

static void	*alone_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_left);
	msg_log(philo, TAKE_FORK);
	pthread_mutex_unlock(philo->fork_left);
	ft_wait(philo->table->time_to_die, philo->table);
	return (NULL);
}

/**
 * the daily routine of each philosopher
 * 		- eat()
 * 		- sleep()
 * 		- think()
 *
 */
void	*routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	pthread_mutex_lock(&philo->table->m_last_meal);
	philo->last_meal = philo->table->start_time_simu;
	pthread_mutex_unlock(&philo->table->m_last_meal);
	synch_start(philo->table->start_time_simu);
	if (philo->table->number_philo == 1)
		return (alone_philo(philo));
	if (philo->rank % 2)
		ft_wait(philo->table->time_to_eat / 2, philo->table);
	while (!end_simu(philo->table))
	{
		eat(philo);
		ft_sleep(philo);
		think(philo);
	}
	return (NULL);
}
