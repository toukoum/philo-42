/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:23:12 by rgiraud           #+#    #+#             */
/*   Updated: 2024/01/30 12:54:05 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	msg_log(t_philo *philo, int code)
{
	pthread_mutex_lock(&philo->table->m_log);
	if (code == EAT)
		printf("%s%lld %zu is eating %s\n", GREEN, get_time(), philo->rank, NC);
	else if (code == SLEEP)
		printf("%s%lld %zu is sleeping %s\n", BLUE, get_time(), philo->rank,
			NC);
	else if (code == THINK)
		printf("%s%lld %zu is thinking %s\n", YELLOW, get_time(), philo->rank,
			NC);
	else if (code == DIE)
		printf("%s%lld %zu died %s\n", RED, get_time(), philo->rank, NC);
	else if (code == TAKE_FORK)
		printf("%s%lld %zu has taken a fork %s\n", GREY, get_time(),
			philo->rank, NC);
	else if (code == WAIT_FORK)
		printf("%s%lld %zu is wating for fork %s\n", YELLOW, get_time(),
			philo->rank, NC);
	pthread_mutex_unlock(&philo->table->m_log);
}

int	end_simu(t_table *table)
{
	int	result;

	result = 0;
	pthread_mutex_lock(&table->m_dead);
	result = table->dead;
	pthread_mutex_unlock(&table->m_dead);
	return (result);
}
void	set_end_simu(t_table *table)
{
	pthread_mutex_lock(&table->m_dead);
	table->dead = 1;
	pthread_mutex_unlock(&table->m_dead);
}



void synch_start(long long time_start)
{
	while (get_time() < time_start)
	{
		continue ;
	}
}