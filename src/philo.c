/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:54:59 by rgiraud           #+#    #+#             */
/*   Updated: 2024/01/24 22:25:51 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static long	ft_atoi_boost(const char *str, int *error)
{
	int		i;
	int		sign;
	long	result;

	sign = 1;
	i = 0;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	if (!(str[i] >= '0' && str[i] <= '9'))
		*error = 1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
		if ((result > INT_MAX && sign == 1) || (result > (2147483648) && sign
				== -1))
			*error = 1;
	}
	return (result * sign);
}

/**
 * init the mutex of each philosopher
 * malloc the table of philos and the table of mutex forks
 * assign the mutex (fork) of the table left and right to each philosopher
*/
int ft_init_philo(t_table *table)
{
	int i;

	table->philos = malloc(table->number_philo * sizeof(t_philo));
	table->forks = malloc(table->number_philo * sizeof(pthread_mutex_t));
	if (!table->philos || !table->forks)
		return (1);
	pthread_mutex_init(&table->log, NULL);
	i = 0;
	while (i < table->number_philo)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		table->philos[i].fork_left = &table->forks[i];
		table->philos[i].fork_right = &table->forks[(i + 1) % table->number_philo];
		table->philos[i].rank = i;
		table->philos[i].log = &table->log;
		i++;
	}
	return (0);
}

int ft_parse(int argc, char **argv, t_table *table)
{
	int error;
	
	error = 0;
	if (argc == 5 || argc == 6)
	{
		table->number_philo = ft_atoi_boost(argv[1], &error);
		table->time_to_die = ft_atoi_boost(argv[2], &error);
		table->time_to_eat = ft_atoi_boost(argv[3], &error);
		table->time_to_sleep = ft_atoi_boost(argv[4], &error);
		if (argc == 6)
			table->must_eat = ft_atoi_boost(argv[5], &error);
		if (error)
			return (1);
		error = ft_init_philo(table);
	}
	else
		error = 1;
	return (error);
}


void	ft_free(t_table *table)
{
	int i;

	i = 0;
	free(table->philos);
	while (i < table->number_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
	pthread_mutex_destroy(&table->log);
}
/**
 * the daily routine of each philosopher
 * 		- eat()
 * 		- sleep()
 * 		- think()
*/	
void *routine()
{
	printf("all good !");
	return (NULL);
}


/**
 * normalement tout est bien initialise, a tester dans routine avec une fonction simple.
*/
int main(int argc, char *argv[])
{
	t_table table;
	int error;
	
	error = ft_parse(argc, argv, &table);
	if (error)
	{
		printf("error with parameter !\n");
		return (1);
	}
	
	routine();

	ft_free(&table);
	return (0);
}

