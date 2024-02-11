/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:51:47 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/11 09:33:33 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

// ================ Struct ================
typedef struct s_table	t_table;

typedef struct s_philo
{
	size_t				rank;
	pthread_t			thread;
	pthread_mutex_t		*fork_left;
	pthread_mutex_t		*fork_right;
	long long			last_meal;
	t_table				*table;
	size_t				count_meal;
}						t_philo;

// struct for contain all the philosophers
struct					s_table
{
	t_philo				*philos;
	pthread_t			philo_life;
	pthread_mutex_t		*forks;
	pthread_mutex_t		m_dead;
	pthread_mutex_t		m_count_meal;
	pthread_mutex_t		m_log;
	pthread_mutex_t		m_last_meal;
	size_t				number_philo;
	size_t				time_to_die;
	size_t				start_time_simu;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				must_eat;
	int					dead;
};

enum
{
	EAT = 1,
	SLEEP = 2,
	THINK = 3,
	DIE = 4,
	TAKE_FORK = 5,
	WAIT_FORK = 6
};

# define GREEN "\033[0;32m"
# define BLUE "\033[0;36m"
# define YELLOW "\033[0;33m"
# define RED "\033[1;31m"
# define CYAN "\033[0;35m"
# define FLASH "\033[1;93m"
# define NC "\033[0m"

# define ARG_ERR 13
# define SIMULATION_ERR 14
// ================ functions ================

// => parse.c
int						ft_parse(int argc, char **argv, t_table *table);

// => exit.c
void					ft_free(t_table *table);
void					free_philo(t_table *table);
int						msg_err(int key_error);

// => time
long long				get_time(void);
void					ft_wait(long long time_to_wait, t_table *table);

// => logging
void					msg_log(t_philo *philo, int code);

// => utils.c
void					set_end_simu(t_table *table);
int						end_simu(t_table *table);
void					synch_start(long long time_start);

// routine
void					*handle_life(void *table_ptr);
void					*routine(void *philo_ptr);

#endif