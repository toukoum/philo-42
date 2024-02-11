/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 11:23:12 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/11 09:48:08 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

void	msg_log(t_philo *philo, int code)
{
	sem_wait(philo->table->sem_log);
	if (code == EAT)
		printf("%s%09lld %zu is eating%s\n", GREEN, get_time()
			- philo->table->start_time_simu, philo->rank, NC);
	else if (code == SLEEP)
		printf("%s%09lld %zu is sleeping%s\n", BLUE, get_time()
			- philo->table->start_time_simu, philo->rank, NC);
	else if (code == THINK)
		printf("%s%09lld %zu is thinking%s\n", YELLOW, get_time()
			- philo->table->start_time_simu, philo->rank, NC);
	else if (code == DIE)
		printf("%s%09lld %zu died%s\n", RED, get_time()
			- philo->table->start_time_simu, philo->rank, NC);
	else if (code == TAKE_FORK)
		printf("%s%09lld %zu has taken a fork%s\n", CYAN, get_time()
			- philo->table->start_time_simu, philo->rank, NC);
	else if (code == WAIT_FORK)
		printf("%s%09lld %zu is wating for fork%s\n", YELLOW, get_time()
			- philo->table->start_time_simu, philo->rank, NC);
	sem_post(philo->table->sem_log);
}

void	synch_start(long long time_start)
{
	while (get_time() < time_start)
		continue ;
}

void	unlink_my_sem(void)
{
	sem_unlink("/set_end");
	sem_unlink("/forks");
	sem_unlink("/log");
	sem_unlink("/eat_full");
	sem_unlink("/end");
}
