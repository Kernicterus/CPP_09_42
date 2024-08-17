/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 17:21:31 by nledent           #+#    #+#             */
/*   Updated: 2024/07/12 12:45:51 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include "PmergeMe.tpp"

int main(int argc, char **argv)
{
	#ifdef DEBUG
	std::cout << "--- DEBUG MODE ---"<< std::endl;
	#endif
	
	if (argc < 2)
	{
		std::cout << "Error : at least one argument required" << std::endl;
		exit (1);
	}

	try
	{
		PmergeMe< std::vector<int> > pmergeme_vector(argc, argv);
		#ifndef DEBUG
		std::cout << "Before: ";
		#endif		
		pmergeme_vector.print();
		clock_t start = clock();
		pmergeme_vector.sort();
		clock_t end = clock();

		double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000000;

		#ifndef DEBUG
		std::cout << "After: ";
		#endif			
		pmergeme_vector.print();

		std::cout << "Time to process a range of "<< pmergeme_vector.getSize() <<" elements with std::vector<int> : " ;
		std::cout << std::fixed << std::setprecision(5) << duration << " us" << std::endl;

		PmergeMe< std::deque<int> > pmergeme_deque(argc, argv);
		start = clock();
		pmergeme_deque.sort();
		end = clock();
		duration = (double)(end - start) / CLOCKS_PER_SEC * 1000000;

		std::cout << "Time to process a range of "<< pmergeme_deque.getSize() <<" elements with std::deque<int>  : " ;
		std::cout << std::fixed << std::setprecision(5) << duration << " us" << std::endl;

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	return (0);
}
