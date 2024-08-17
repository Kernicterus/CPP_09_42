/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:21:31 by nledent           #+#    #+#             */
/*   Updated: 2024/07/12 16:19:55 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Error : bad number of arguments" << std::endl;
		exit (1);
	}

	try
	{
		BitcoinExchange bitcoin_exchange;
		bitcoin_exchange.loadDatabase();
		bitcoin_exchange.ParseWallet(argv[1]);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	return (0);
}
