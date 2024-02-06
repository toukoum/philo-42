/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 22:27:48 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 05:19:42 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/**
 * take 2 fork, then eat (wait time_to_eat)
 * incremente count_meal
 * update last_meal
 **/
static void	eat(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	msg_log(philo, TAKE_FORK);
	sem_wait(philo->sem_forks);
	msg_log(philo, TAKE_FORK);
	msg_log(philo, EAT);
	sem_wait(philo->sem_last_meal);
	philo->last_meal = get_time();
	sem_post(philo->sem_last_meal);
	ft_wait(philo->table->time_to_eat);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	sem_wait(philo->sem_count_meal);
	philo->count_meal++;
	sem_post(philo->sem_count_meal);
}

static void	ft_sleep(t_philo *philo)
{
	msg_log(philo, SLEEP);
	ft_wait(philo->table->time_to_sleep);
}

///**
// * time to think =
// *
// * 	=> 0 if: time_to_die - (time_since_last_meal + time_to_eat) <= 0
// * 	=> 200 if > 600 ms to think
// */
static void	think(t_philo *philo)
{
	long long	time_to_think;
	long long	temp;

	sem_wait(philo->sem_last_meal);
	time_to_think = (philo->table->time_to_die);
	temp = ((get_time() - philo->last_meal) + philo->table->time_to_eat);
	sem_post(philo->sem_last_meal);
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
	ft_wait(time_to_think);
	return ;
}

static int	alone_philo(t_philo *philo)
{
	synch_start(philo->table->start_time_simu);
	sem_wait(philo->sem_forks);
	msg_log(philo, TAKE_FORK);
	sem_post(philo->sem_forks);
	ft_wait(philo->table->time_to_die);
	sem_post(philo->sem_end);
	msg_log(philo, DIE);
	return (0);
}

/**
 * the daily routine of each philosopher
 * 		- eat()
 * 		- sleep()
 * 		- think()
 *	from here, we are in child process <==> in philosopher body
 *
 */
int	routine_philosopher(t_table *table, size_t i)
{
	t_philo	*philo;

	if (init_process_philo(&philo, table, i))
		return (INIT_ERR);
	if (philo->table->number_philo == 1)
		return (alone_philo(philo));
	if (pthread_create(&philo->handle_life, NULL, &handle_life, philo))
		return (INIT_ERR);
	synch_start(table->start_time_simu);
	if (philo->rank % 2)
		ft_wait(philo->table->time_to_eat);
	while (1)
	{
		eat(philo);
		ft_sleep(philo);
		think(philo);
	}
	if (pthread_join(philo->handle_life, NULL))
		return (SIMULATION_ERR);
	return (0);
}
