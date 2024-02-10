/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:51:47 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/10 15:29:51 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>     // FLAGS
# include <limits.h>    // atoi boost, int_max
# include <pthread.h>   // thread
# include <semaphore.h> // semaphore
# include <signal.h>    // kill
# include <stdio.h>     // printf
# include <stdlib.h>    // malloc, free, EXIT_SUCCESS
# include <string.h>    // semaphore
# include <sys/stat.h>  // O_CREAT flag sem
# include <sys/time.h>  // usleep
# include <sys/types.h> // O_CREAT
# include <sys/wait.h>  // waitpid
# include <unistd.h>    // fork

// ================ Struct ================
typedef struct s_table	t_table;

typedef struct s_philo
{
	size_t				rank;
	pid_t				pid_philo;
	long long			last_meal;
	size_t				count_meal;
	sem_t				*sem_last_meal;
	sem_t				*sem_count_meal;
	pthread_t			handle_life;
	t_table				*table;
}						t_philo;

struct					s_table
{
	sem_t				*sem_forks;
	sem_t				*sem_log;
	sem_t				*sem_end;
	sem_t				*sem_eat_full;
	sem_t				*sem_set_end;
	size_t				number_philo;
	size_t				time_to_die;
	size_t				start_time_simu;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	size_t				must_eat;
	int					end;
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
int						init_process_philo(t_philo *philo, size_t i,
							t_table *table);

// => exit.c
int						ft_wait_philo(int i, t_philo **philos);
void					kill_all_philo(t_table *table, t_philo **philos);
int						msg_err(int key_error);
void					ft_free(t_table *table, t_philo **philos);

// => time
long long				get_time(void);
void					ft_wait(long long time_to_wait);

// => utils.c
void					msg_log(t_philo *philo, int code);
void					synch_start(long long time_start);
void					unlink_my_sem(void);
char					*get_name_sem(size_t rank, char *name);

// routine
void					*handle_life(void *table_ptr);
int						routine_philosopher(t_philo *philo);

// check_life process / thread
void					*handle_life(void *philo_ptr);
void					stop_simulation(t_table *table, t_philo **philos);

#endif