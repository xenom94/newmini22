/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iabboudi <iabboudi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:12:23 by iabboudi          #+#    #+#             */
/*   Updated: 2024/12/12 19:12:24 by iabboudi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LC_HEADER_H
# define LC_HEADER_H

# include <stdint.h>
# include <stdlib.h>
# include <string.h>

typedef struct List_
{
	size_t			size;
	uintptr_t		ptr;
	struct List_	*next;
}					List;

#endif