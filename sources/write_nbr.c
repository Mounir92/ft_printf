/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_nbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmouhssi <mmouhssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/03 18:45:53 by mmouhssi          #+#    #+#             */
/*   Updated: 2016/05/17 18:35:28 by mmouhssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"


int		no_print(t_format format, char *word)
{
	if (format.precision == NULL)
		return (0);
	if (format.precision[0] == '0' || format.precision[0] == '.')
	{
		if (format.pre == '#' && (format.type == 'o' || format.type == 'O'))
			return (0);
		if (ft_strcmp(word, "0") == 0)
			return (1);
	}
	return (0);
}

static char	*fill_zero(t_format format, char *type, int width)
{
	char	*str;
	char	*str2;
	int	i;

	if (type == NULL || width <= 0)
		return (NULL);
	if (is_dioux(format) == 0 && format.type != 'p')
		return (NULL);
	i = 0;
	str = type;
	//format.pre == '#' ? (width = width - 2) : 0;
	while (i < width)
	{
		str2 = str;
		str = ft_strjoin("0", str2);
		free(str2);
		i++;
	}
	return (str);
}

void	val_width(t_format format , int *width)
{
	format.flags == '0' ? *width = 0 : 0;
	*width < 0 ? *width = 0 : 0;
	/*if (format.pre == '#' && (format.type == 'x' || format.type == 'X'))
		*width = *width - 2;
	if (format.pre == '#' && (format.type == 'o' || format.type == 'O'))
		*width = *width - 1;*/
}

char	*add_width(t_format format, va_list lst, char *type, int *width) // reflechir pour wchat_t // penser condition -
{
	int i;
	int lgt;
	char *str;

	str = NULL;
	if ((format.type == 's' || format.type == 'S') && format.precision != NULL && format.precision[0] != '.' && type != '\0' && ft_atoi(format.precision) < ft_strlen(type))
		lgt = ft_atoi(format.precision);
	else
		lgt = ft_strlen(type);
	if (format.width == NULL)
		return (type);
	else if (format.width[0] == '*')
		*width = va_arg(lst, int) - lgt;
	else if (format.width[0] >= '0' && format.width[0] <= '9')
		*width = ft_atoi(format.width) - lgt;
	else
		return (type);
	/*if (format.flags != '\0')
	{*/
	if (no_print(format, type) == 1)
		(*width)++;
	if (format.pre == '#' && (format.type == 'x' || format.type == 'X') && format.flags != '-')
		*width = *width - 2;
	else if (format.pre == '#' && (format.type == 'o' || format.type == 'O') && format.flags != '-')
		*width = *width - 1;
	format.flags == '0' ? str = fill_zero(format, type, *width) : 0;
	val_width(format, width);
	/*format.flags == '0' ? *width = 0 : 0;
	*width < 0 ? *width = 0 : 0;*/
	//}
	/*ft_putchar('\n');
	ft_putstr("width : ");
	ft_putnbr(*width);
	ft_putchar('\n');*/
	if (str != NULL)
		return (str);
	i = 0;
	while (i < *width && *width > 0)
	{
		ft_putchar(' ');
		i++;
	}
	return (type);
}

static char	*add_precision(t_format format, va_list lst, char *type, int *prcsn) // reflechir pour wchat_t // penser condition -
{
	int i;
	char *str;

	str = NULL;
	if (format.precision == NULL)
		return (type);
	/*else if ((format.precision[0] == '0' || format.precision[0] == '.') && ft_strcmp(type, "0") == 0)
		return ("\0\0");*/
	else if (format.precision[0] == '*')
		*prcsn = va_arg(lst, int) - ft_strlen(type);
	else if (format.precision[0] >= '0' && format.precision[0] <= '9')
		*prcsn = ft_atoi(format.precision) - ft_strlen(type);
	else
		return (type);
	str = fill_zero(format, type, *prcsn);
	if (str != NULL)
		return (str);
	return (type);
}

static char	*add_prenbr(t_format format, char *nbr)
{
	char *pre;
	char *str;

	pre = NULL;
	if (format.pre == '#' && ft_strcmp(nbr, "0") != 0)
	{
		format.type == 'x' ? (pre = "0x") : 0;
		format.type == 'X' ? (pre = "0X") : 0;
	}
	if (format.pre == '#' && (format.type == 'o' || format.type == 'O'))
		nbr[0] != '0' ? (pre = "0") : 0;
	if (format.sign == ' ' && nbr[0] != '-')
		pre = " ";
	else if (format.type == 'd' || format.type == 'D' || format.type == 'i')
	{
		if (format.sign == '+' && nbr[0] != '-')
			pre = "+";
	}
	if (pre != NULL)
	{
		str = nbr;
		nbr = ft_strjoin(pre, str);
		free(str);
	}
	return (nbr);
}

char	*ft_strjoinfree(char *s1, char *s2)
{
	char *str;

	str = s2;
	str = ft_strjoin(s1, s2);
	free(s2);
	return (str);
}

int		write_nbr(t_format format, va_list lst, long long nbr)
{
	char *word;
	int width;
	int b;

	word = NULL;
	width = 0;
	if (format.type == 'x' || format.type == 'p')
		word = ft_lltoah(nbr, 1);
	else if (format.type == 'X')
		word = ft_lltoah(nbr, 2);
	else if (format.type == 'o' || format.type == 'O')
		word = ft_lltoao(nbr);
	else
		word = ft_lltoa(nbr);
	b = no_print(format, word);
	format.type == 'p' ? (word = ft_strjoinfree("10", word)) : 0; // 0x ou 0x10 ?
	word = add_precision(format, lst, word, &width);
	width = 0;
	if (format.flags != '-')
		word = add_width(format, lst, word, &width);
	word = add_prenbr(format, word);
	format.type == 'p' ? (word = ft_strjoinfree("0x", word)) : 0; // 0x ou 0x10 ?
	b == 0 ? ft_putstr(word) : 0;
	if (format.flags == '-')
		add_width(format, lst, word, &width);
	/*if (word == NULL)
		return (0);*/
	/*ft_putstr("width : ");
	ft_putnbr(width);
	ft_putchar('\n');*/
	/*ft_putstr("word : ");
	ft_putnbr(ft_strlen(word));
	ft_putchar('\n');*/
	b == 0 ? (width = width + ft_strlen(word)) : 0;
	if (b == 1 && width == 0)
		width = -1;
	/*ft_putendl("haha nelson");
	ft_putnbr(width);*/
	word == NULL ? width = 0 : 0;
	return (width);
}
