/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abinois <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 19:10:13 by abinois           #+#    #+#             */
/*   Updated: 2019/04/12 11:59:19 by abinois          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_base(long n, int base)
{
    char *base_tab;

    base_tab = "0123456789abcdef";
	if (n == -2147483648)
	{
		ft_putstr("-2");
		ft_putnbr_base(147483648, base);
	}
	else if (n < 0)
	{
		ft_putchar('-');
		ft_putnbr_base(-n, base);
	}
	else if (n >= base)
	{
		ft_putnbr_base(n / base, base);
		ft_putchar(base_tab[n % base]);
	}
	else
		ft_putchar(base_tab[n]);
}
