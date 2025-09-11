/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbouizem <djihane.bouizem@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 00:08:41 by dbouizem          #+#    #+#             */
/*   Updated: 2025/09/11 16:39:56 by dbouizem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (new_size == 0)
	{
		if (ptr)
			free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	if (old_size == new_size)
		return (ptr);
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (new_size < old_size)
		copy_size = new_size;
	else
		copy_size = old_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}

void	*ft_realloc_double(void *ptr, size_t current_size)
{
	void	*new_ptr;
	size_t	new_size;

	if (!ptr || current_size == 0)
		return (NULL);
	new_size = current_size * 2;
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, current_size);
	free(ptr);
	return (new_ptr);
}
