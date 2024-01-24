/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 16:51:47 by rgiraud           #+#    #+#             */
/*   Updated: 2024/01/24 21:51:42 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <limits.h>

// struct for philosopher
typedef struct s_philo
{
	int rank;
	pthread_t thread;
	pthread_mutex_t *fork_left;
	pthread_mutex_t *fork_right;
	pthread_mutex_t *log;
} t_philo;


// struct for contain all the philosophers
typedef struct s_table
{
	t_philo *philos;
	pthread_mutex_t *forks;
	pthread_mutex_t log;
	int number_philo;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int must_eat;
} t_table;

#endif