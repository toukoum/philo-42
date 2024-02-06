/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 05:25:20 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 18:03:18 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	free_philo(t_table *table)
{
	size_t	i;

	i = 0;
	free(table->philos);
	while (i < table->number_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
}

void	ft_free(t_table *table)
{
	size_t	i;

	i = 0;
	free(table->philos);
	while (i < table->number_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
	pthread_mutex_destroy(&table->m_dead);
	pthread_mutex_destroy(&table->m_log);
	pthread_mutex_destroy(&table->m_last_meal);
	pthread_mutex_destroy(&table->m_count_meal);
}

int	msg_err(int key_error)
{
	if (key_error == ARG_ERR)
		printf("\033[1;31mError, please enter valid arguments.\033[0m\n\n"
			"  => \033[1;34m./philo <time_to_die> <time_to_eat> <time_to_sleep>"
			"[number_of_time_must_eat]\033[0m\n\n");
	else if (key_error == SIMULATION_ERR)
		printf("\033[1;31mError with simulation !\033[0m\n\n");
	return (EXIT_FAILURE);
}
