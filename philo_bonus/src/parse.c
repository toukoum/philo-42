/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:52:02 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 21:51:12 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static long	ft_atoi_boost(const char *str, int *error)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			*error = 1;
	if (!(str[i] >= '0' && str[i] <= '9'))
		*error = 1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
		if ((result > INT_MAX))
			*error = 1;
	}
	return (result);
}

int	ft_init_semaphore(t_table *table)
{
	table->child_pids = malloc(table->number_philo * sizeof(pid_t));
	if (!table->child_pids)
		return (INIT_ERR);
	table->sem_forks = sem_open("/forks", O_CREAT, 0644, table->number_philo);
	if (table->sem_forks == SEM_FAILED)
		return (INIT_ERR);
	table->sem_log = sem_open("/log", O_CREAT, 0644, 1);
	if (table->sem_log == SEM_FAILED)
		return (INIT_ERR);
	table->sem_eat_full = sem_open("/eat_full", O_CREAT, 0644, 0);
	if (table->sem_eat_full == SEM_FAILED)
		return (INIT_ERR);
	table->sem_end = sem_open("/end", O_CREAT, 0644, 0);
	if (table->sem_end == SEM_FAILED)
		return (INIT_ERR);
	return (0);
}

/**
 * Initialise table struct
 */
int	ft_parse(int argc, char **argv, t_table *table)
{
	int	error;

	error = 0;
	if (!(argc == 5 || argc == 6))
		return (INIT_ERR);
	table->number_philo = ft_atoi_boost(argv[1], &error);
	table->time_to_die = ft_atoi_boost(argv[2], &error);
	table->time_to_eat = ft_atoi_boost(argv[3], &error);
	table->time_to_sleep = ft_atoi_boost(argv[4], &error);
	if (argc == 6)
	{
		table->must_eat = ft_atoi_boost(argv[5], &error);
		if (!table->must_eat)
			return (INIT_ERR);
	}
	else
		table->must_eat = 0;
	if (error)
		return (INIT_ERR);
	return (ft_init_semaphore(table));
}

/**
 * assign global semaphore to each philo (child process)
 */
static int	init_personnal_sem(t_philo **philo)
{
	(*philo)->sem_forks = sem_open("/forks", 0);
	if ((*philo)->sem_forks == SEM_FAILED)
		return (INIT_ERR);
	(*philo)->sem_log = sem_open("/log", 0);
	if ((*philo)->sem_log == SEM_FAILED)
		return (INIT_ERR);
	(*philo)->sem_full = sem_open("/eat_full", 0);
	if ((*philo)->sem_full == SEM_FAILED)
		return (INIT_ERR);
	(*philo)->sem_end = sem_open("/end", 0);
	if ((*philo)->sem_end == SEM_FAILED)
		return (INIT_ERR);
	return (0);
}

/**
 * Init child process so philo and assign global semaphore
 * correctly
 */
int	init_process_philo(t_philo **philo, t_table *table, size_t i)
{
	char *sem_name_1;
	char *sem_name_2;

	*philo = malloc(sizeof(t_philo));
	if (!(*philo))
		return (INIT_ERR);
	(*philo)->rank = i + 1;
	(*philo)->table = table;
	(*philo)->count_meal = 0;
	(*philo)->last_meal = table->start_time_simu;
	sem_name_1 = get_name_sem((*philo)->rank, "/count_meal");
	sem_name_2 = get_name_sem((*philo)->rank, "/last_meal");
	if (!sem_name_1 || !sem_name_2)
		return (INIT_ERR);
	(*philo)->sem_count_meal = sem_open(sem_name_1, O_CREAT, 0644, 1);
	if ((*philo)->sem_count_meal == SEM_FAILED)
		return (INIT_ERR);
	(*philo)->sem_last_meal = sem_open(sem_name_2, O_CREAT, 0644, 1);
	if ((*philo)->sem_last_meal == SEM_FAILED)
		return (INIT_ERR);
	free(sem_name_1);
	free(sem_name_2);
	return (init_personnal_sem(philo));
}
