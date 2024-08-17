/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:21:31 by nledent           #+#    #+#             */
/*   Updated: 2024/06/29 23:16:53 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"


int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cout << "Error : at least one argument required" << std::endl;
		exit (1);
	}

	try
	{
		RPN rpn(argc, argv);
		//rpn.print();
		rpn.calculate();
	}
	catch(const RPN::ErrorRPN& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	return (0);
}
