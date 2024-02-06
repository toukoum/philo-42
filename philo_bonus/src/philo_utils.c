/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:23:12 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 04:53:19 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

void	msg_log(t_philo *philo, int code)
{
	sem_wait(philo->sem_log);
	if (code == EAT)
		printf("%s%lld %zu is eating%s\n", GREEN, get_time() - philo->table->start_time_simu, philo->rank, NC);
	else if (code == SLEEP)
		printf("%s%lld %zu is sleeping%s\n", BLUE, get_time() - philo->table->start_time_simu, philo->rank, NC);
	else if (code == THINK)
		printf("%s%lld %zu is thinking%s\n", YELLOW, get_time() - philo->table->start_time_simu, philo->rank,
			NC);
	else if (code == DIE)
		printf("%s%lld %zu died%s\n", RED, get_time() - philo->table->start_time_simu, philo->rank, NC);
	else if (code == TAKE_FORK)
		printf("%s%lld %zu has taken a fork%s\n", CYAN, get_time() - philo->table->start_time_simu, philo->rank,
			NC);
	else if (code == WAIT_FORK)
		printf("%s%lld %zu is wating for fork%s\n", YELLOW, get_time() - philo->table->start_time_simu,
			philo->rank, NC);
	sem_post(philo->sem_log);
}

void	synch_start(long long time_start)
{
	while (get_time() < time_start)
	{
		continue ;
	}
}

void unlink_my_sem()
{
	sem_unlink("/count_meal");
	sem_unlink("/last_meal");
	sem_unlink("/forks");
	sem_unlink("/log");
	sem_unlink("/eat_full");
	sem_unlink("/end");
}