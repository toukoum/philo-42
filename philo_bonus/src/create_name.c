/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_name.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiraud <rgiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:51:10 by rgiraud           #+#    #+#             */
/*   Updated: 2024/02/06 21:25:16 by rgiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str && str[len])
		len++;
	return (len);
}

static size_t	nb_digit_rank(size_t num)
{
	size_t	count;

	count = 0;
	if (num == 0)
		return (1);
	while (num != 0)
	{
		count++;
		num /= 10;
	}
	return (count);
}

static char	*ft_itoa(size_t rank)
{
	size_t	nb_digit;
	char	*result;

	nb_digit = nb_digit_rank(rank);
	result = malloc((nb_digit + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[nb_digit] = '\0';
	while (rank)
	{
		result[--nb_digit] = (rank % 10) + '0';
		rank /= 10;
	}
	return (result);
}
/**
 * create an unique name for semaphore
 * concatenate the name and the rank
 * and returning an allocated string
 */
char	*get_name_sem(size_t rank, char *name)
{
	char	*result;
	char	*string_n;
	size_t	i;
	size_t	len_name;
	size_t	len_rank;

	i = 0;
	string_n = ft_itoa(rank);
	if (!string_n)
		return (NULL);
	len_name = ft_strlen(name);
	len_rank = ft_strlen(string_n);
	result = malloc((len_name + len_rank + 1) * sizeof(char));
	if (!result)
		return (free(string_n), NULL);
	result[len_name + len_rank] = '\0';
	while (++i <= len_name)
		result[i - 1] = name[i - 1];
	i = 0;
	while (++i <= len_rank)
		result[len_name + (i - 1)] = string_n[i - 1];
	free(string_n);
	return (result);
}
