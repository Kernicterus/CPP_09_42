/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:03:37 by nledent           #+#    #+#             */
/*   Updated: 2024/07/12 12:45:41 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PmergeMe_HPP
# define PmergeMe_HPP

# include <algorithm>
# include <iostream>
# include <iomanip>
# include <cmath>
# include <vector>
# include <deque>
# include <time.h>

typedef 	enum e_errors
{
	ERR_BAD_INPUT,
} t_errors;

template <typename T>
class PmergeMe
{
	public: 
	PmergeMe(int argc, char **argv);
	PmergeMe(PmergeMe& src);
	PmergeMe& operator=(PmergeMe& src);
	~PmergeMe();

	void 					print();

	static const char* 		getErrorMessage();
	static void 			setErrorMessage(std::string message);

	int						getSize();
	void 					sort();	
	
	class ErrorPmergeMe : public std::exception
	{
		public :
		ErrorPmergeMe(t_errors error);
		virtual const char* what() const throw();
		
		private :
		ErrorPmergeMe();
	};

	private:
	PmergeMe();
	
	static std::string 		_error_message;
	T						_container;
	T						_jacobsthal;

	void					jacobsthal_sequ_generation(int size);
	void 					FJAlgo(int deep_level);	
	T 						split_argv_to(std::string &argv);
	void					remainings_insertion(int power, T &odd);
	typename T::iterator	binary_search(int nb, int lower_limit, int upper_limit, int power);
	void					swap_min_max(int deep_lvl);
	void					insertion_level(typename T::iterator target, T container, int deep_lvl, typename T::iterator it);
	bool 					is_number(std::string &str);
	T						save_odd_el(int power);
	void					erase_odd_el(int power);
	void					jacobsthal_insertion_order(int power, T &tmp);
	int						jacobsthal_id_transformation(int old_id, int size_container);
};
#endif