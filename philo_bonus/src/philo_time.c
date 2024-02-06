/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 22:28:48 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 21:43:45 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

void	ft_wait(long long time_to_wait)
{
	long long	time;

	time = get_time() + time_to_wait;
	while (get_time() < time)
		usleep(100);
}

/**
 * for getting current time in ms
 */
long long	get_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((long long)(current_time.tv_sec * 1000LL) + (current_time.tv_usec
			/ 1000));
}
