
#ifndef PMERGEME_TPP
# define PMERGEME_TPP
# include "PmergeMe.hpp"

template <typename T>
std::string PmergeMe<T>::_error_message = "";

template <typename T>
PmergeMe<T>::PmergeMe()
{
}

template <typename T>
PmergeMe<T>::PmergeMe(int argc, char **argv)
{
	std::string raw_numbers;

	for (int i = 1; i < argc; i++)
	{
		raw_numbers += argv[i];
		raw_numbers += " ";
	}
	_container = split_argv_to(raw_numbers);
	if (_container.size() == 0)
		throw ErrorPmergeMe(ERR_BAD_INPUT);
}

template <typename T>
PmergeMe<T>::PmergeMe(PmergeMe<T>& src)
{
	*this = src;
}

template <typename T>
PmergeMe<T>& PmergeMe<T>::operator=(PmergeMe<T>& src)
{
	_container = src._container;
	_jacobsthal = src._jacobsthal;
	return (*this);
}

template <typename T>
PmergeMe<T>::~PmergeMe()
{
}

template <typename T>
bool PmergeMe<T>::is_number(std::string &str)
{
	std::string	cpy = str;
	std::string::iterator it = cpy.begin();

	if (*it == '+')
		cpy.erase(0,1);
	it = cpy.begin();
	std::string::iterator ite = cpy.end();
	if (cpy.size() == 0 || cpy.size() > 10)
		return false;
	if (cpy.size() == 10 && cpy.compare("2147483647") > 0)
		return false;
	while (it != ite)
	{
		if (*it < '0' || *it > '9')
			return false;
		it++;
	}
	return true;
}

template <typename T>
const char* PmergeMe<T>::getErrorMessage()
{
	return (_error_message.c_str());
}

template <typename T>
void PmergeMe<T>::setErrorMessage(std::string message)
{
	_error_message = message;
}

template <typename T>
PmergeMe<T>::ErrorPmergeMe::ErrorPmergeMe(t_errors error)
{
	std::string str = "Error : ";
	switch (error)
	{
		case ERR_BAD_INPUT:
			str+= "bad input";
			break;				
		default :
			break;
	}
	PmergeMe<T>::setErrorMessage(str);
}

template <typename T>
const char* PmergeMe<T>::ErrorPmergeMe::what() const throw()
{
	return (getErrorMessage());
}

template <typename T>
int PmergeMe<T>::getSize()
{
	return (_container.size());
}

template <typename T>
void PmergeMe<T>::print()
{
	typename T::iterator it;
	typename T::iterator ite;

	#ifdef DEBUG
	std::cout << "In the source sequence : ";
	#endif
	it = _container.begin();
	ite = _container.end();
	while (it != ite)
	{
		std::cout << *it;
		it++;
		if (it != ite)
			std::cout << ", ";
	}
	std::cout << std::endl;
}

template <typename T>
T 		PmergeMe<T>::split_argv_to(std::string &argv)
{
	std::string 			 tmp = "";
	T						 container;
	std::vector<std::string> split;

	for (std::string::iterator it = argv.begin(); it != argv.end(); it++)
	{
		if (*it == ' ')
		{
			if (tmp != "")
			{
				split.push_back(tmp);
				tmp = "";
			}
			continue;
		}
		else
			tmp += *it;
	}

	for (std::vector<std::string>::iterator it = split.begin(); it != split.end(); it++)
	{
		if (is_number(*it) == false)
			throw (ErrorPmergeMe(ERR_BAD_INPUT));
		else
			container.push_back(std::atoi((*it).c_str()));
	}
	return container;
}

template <typename T>
void 	PmergeMe<T>::FJAlgo(int deep_level)
{
	int power = pow(2, deep_level);
	int size = _container.size() / power;
	T	odd;

	if (size % 2 == 1)
	{
		odd = save_odd_el(power);
		erase_odd_el(power);
	}

	swap_min_max(deep_level);

	#ifdef DEBUG
	std::cout << "After Step 1 & 2 : " << std::endl;
	print();
	if (size % 2)
	{
		std::cout << " + ODD nb : " ;
		for (int i = 0; i < power; i++)
			std::cout <<  odd[i] << ",";
	}
			std::cout << std::endl;
	#endif
	
	if (size > 1)
	{
		FJAlgo(deep_level + 1);

		#ifdef DEBUG
		std::cout << "\033[35m-- DEEP "<< deep_level<< " --\033[0m" << std::endl;
		#endif

	
	}
		remainings_insertion(power, odd);
		#ifdef DEBUG
		std::cout << "\033[31mSTEP 5 (binary search insertion with the remaining numbers of this level)\033[0m" << std::endl;
		print();
		std::cout << std::endl;
		#endif	
}

template <typename T>
void	PmergeMe<T>::swap_min_max(int deep_lvl)
{
	int power = pow(2, deep_lvl);
	int gap = 2 * power;
	int size = _container.size();

	#ifdef DEBUG
	std::cout << "\033[35m-- DEEP "<< deep_lvl << " --\033[0m";
	if (deep_lvl > 0)
		std::cout << "\033[31m  STEP 3 (recursively sorting)\033[0m" << std::endl;
	else
		std::cout << std::endl;
	std::cout << "\033[31mSTEP 1 & 2 (pairing then sort these two numbers)\033[0m" << std::endl;
	#endif

	for(int i = power - 1; i + power <= size - 1; i+= gap)
	{
		typename T::iterator it = _container.begin() + i;

		#ifdef DEBUG
		std::cout << "pair number " << i / gap + 1 ;
		std::cout << ": (" << *it;
		std::cout << "," << *(it + power)<< ")" << std::endl;
		std::cout << std::endl;
		#endif

		if (*it > *(it + power))
		{
			for (int i = 0; i < power; i++)
			{
				int tmp = *(it + power - i);
				*(it + power - i) = *(it - i);	
				*(it - i) = tmp;
			}
		}
	}
}

template <typename T>
void PmergeMe<T>::sort()
{
	jacobsthal_sequ_generation(_container.size());
	FJAlgo(0);
}

template <typename T>
void	PmergeMe<T>::remainings_insertion(int power, T &odd)
{
	int						gap 		= power * 2;
	int 					size_full 	= _container.size();
	int						i;
	T 						tmp;
	typename T::iterator 	it 			= _container.begin() + power - 1;
	typename T::iterator 	target;

	i = 0;
	
	//store
	for (i = 0 ; i  < size_full - power + 1; i += gap)
	{
		for (int j = 0; j < power; j++)
			tmp.push_back(*(it + i + j - power + 1)) ;
	}
	// add odd if exist
	if (odd.size() != 0)
		tmp.insert(tmp.end(), odd.begin(), odd.end());
	//erase
	int end;
	for (int j = 0 ; j < i / gap ; j++)
	{
		end = power * j;
		it = _container.begin() + power - 1 + end;
		_container.erase((it - power + 1), (it + 1));
	}

	#ifdef DEBUG
	std::cout << "After storing remaining y and erasing them from the main chain :" << std::endl;
	print();
	std::cout << std::endl;
	#endif

	//insert
	jacobsthal_insertion_order(power, tmp);
}

template <typename T>
void	PmergeMe<T>::jacobsthal_insertion_order(int power, T &tmp)
{
	typename T::iterator 	it;
	typename T::iterator 	target;
	int						id_to_insert;
	int						stack;
	
/* 	it = tmp.begin() + (power - 1);
	target = _container.begin();
	_container.insert(target, it - power + 1, it + 1);
	tmp.erase(tmp.begin(), it + 1);
	stack = tmp.size() / power; */

	it = tmp.begin();
	target = _container.begin();
	_container.insert(target, it, it + power);
	tmp.erase(tmp.begin(), it + power);
	stack = tmp.size() / power;

	#ifdef DEBUG
	std::cout << "\033[31mSTEP 4 (add pair of the min nb in the beginning)\033[0m" << std::endl;
	print();
	std::cout << std::endl;
	#endif

	for (int j = 0; j < stack ; j++)
	{
		id_to_insert = jacobsthal_id_transformation(j, tmp.size() / power);
		id_to_insert = (power - 1 + j * power);
		it = tmp.begin() + id_to_insert;

		target = binary_search(tmp[id_to_insert], 0, (_container.size() / (power)) - 1, power);
		_container.insert(target, it - power + 1, it + 1);
	}	
}

template <typename T>
int		PmergeMe<T>::jacobsthal_id_transformation(int old_id, int size_container)
{
	int new_id 				= 0;
	int	bloc_nb 			= 0;
	int	total_nb_in_blocs	= 2;
	int	decr				= 0;

	typename T::iterator jacobs = _jacobsthal.begin();

	while (old_id + 1 > total_nb_in_blocs)
	{
		bloc_nb++;
		total_nb_in_blocs += *(jacobs + bloc_nb);
	}
	new_id = total_nb_in_blocs + 2;
	new_id -= 3; 
	if (new_id >= size_container)
		new_id = size_container - 1;
	decr = *(jacobs + bloc_nb) + old_id - total_nb_in_blocs;
	new_id -= decr;
	return (new_id);
}

template <typename T>
typename T::iterator	PmergeMe<T>::binary_search(int nb, int lower_limit, int upper_limit, int power)
{
	if (upper_limit - lower_limit < 2)
	{
		typename T::iterator lower = _container.begin() + power * lower_limit + (power - 1);
		typename T::iterator upper = _container.begin() + power * upper_limit + (power - 1);

		if (nb < *lower)
			return (lower + 1 - power);
		else if (nb > *upper)
			return (_container.end());
		else
			return (upper + 1 - power);
	}
	int target = (upper_limit - lower_limit) / 2 + lower_limit;

	typename T::iterator it_target = _container.begin() + power * target + (power - 1);

	if (nb == *it_target)
		return (it_target +1 - power);
	else if (nb < *it_target)
		return (binary_search(nb, lower_limit, target, power));
	else 
		return (binary_search(nb, target, upper_limit, power));
}

template <typename T>
T		PmergeMe<T>::save_odd_el(int power)
{
	T tmp;
	typename T::iterator it = _container.end();

	it = it - power;
	for (int i = 0; i < power; i++)
		tmp.push_back(*(it + i));
	return (tmp);
}

template <typename T>
void	PmergeMe<T>::erase_odd_el(int power)
{
	typename T::iterator it = _container.end();

	_container.erase((it - power), it);
}

template <typename T>
void	PmergeMe<T>::jacobsthal_sequ_generation(int size)
{
    int 					total_nb_storable;
	int 					previous_sum;
	int 					next_power;
	double					log2= std::log(2);
    typename T::iterator 	last;

	_jacobsthal.push_back(2);
    _jacobsthal.push_back(2);

    for (total_nb_storable = 4; total_nb_storable < size; total_nb_storable++) 
	{
		last = _jacobsthal.end() - 1;
        previous_sum = *(last) + *(last - 1);
        next_power = std::pow(2, 1 + (std::log(previous_sum)/ log2));
        _jacobsthal.push_back(next_power - *(last));
		total_nb_storable += _jacobsthal.back();
    }
}

#endif