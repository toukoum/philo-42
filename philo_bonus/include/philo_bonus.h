/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:51:47 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 04:53:43 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>    // atoi boost, int_max
# include <pthread.h>   // thread
# include <semaphore.h> // semaphore
# include <signal.h>    // kill
# include <stdio.h>     // printf
# include <stdlib.h>    // malloc, free, EXIT_SUCCESS
# include <string.h>    // semaphore
# include <sys/stat.h>  // O_CREAT flag sem
# include <sys/time.h>  // usleep
# include <unistd.h>    // fork

// ================ Struct ================
typedef struct s_table	t_table;

typedef struct s_philo
{
	size_t				rank;
	long long			last_meal;
	size_t				count_meal;
	sem_t				*sem_last_meal;
	sem_t				*sem_count_meal;
	sem_t				*sem_forks;
	sem_t				*sem_end;
	sem_t				*sem_log;
	sem_t				*sem_full;
	pthread_t			handle_life;
	t_table				*table;
}						t_philo;

struct					s_table
{
	sem_t				*sem_forks;
	sem_t				*sem_log;
	sem_t				*sem_end;
	sem_t				*sem_eat_full;
	pid_t				*child_pids;
	size_t				number_philo;
	size_t				time_to_die;
	size_t				start_time_simu;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				must_eat;
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
# define INIT_ERR 15
// ================ functions ================

// => parse.c
int						ft_parse(int argc, char **argv, t_table *table);
int						init_process_philo(t_philo **philo, t_table *table,
							size_t i);

// => exit.c
void					ft_free(t_table *table);
int						ft_wait_philo(int i, pid_t *child_pids);
void					kill_all_philo(t_table *table);
int						msg_err(int key_error);

// => time
long long				get_time(void);
void					ft_wait(long long time_to_wait);

// => logging
void					msg_log(t_philo *philo, int code);

// => utils.c
void					synch_start(long long time_start);
void					unlink_my_sem(void);

// routine
void					*handle_life(void *table_ptr);
int						routine_philosopher(t_table *table, size_t i);

// check_life process / thread
void					*handle_life(void *philo_ptr);
void					stop_simulation(t_table *table);

#endif