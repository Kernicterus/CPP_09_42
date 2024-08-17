/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:03:37 by nledent           #+#    #+#             */
/*   Updated: 2024/07/09 16:12:04 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <algorithm>
# include <iostream>
# include <sstream>
# include <stack>
# include <list>
# include <cmath>

typedef 	enum e_errors
{
	ERR_BAD_INPUT_RPN,
	ERR_MISSING_SPACE,
	ERR_BAD_CHAR,
	ERR_INVALID_EXPR,
} t_errors;

class RPN
{
	public: 
	RPN(int argc, char **argv);
	RPN(RPN& src);
	RPN& operator=(RPN& src);
	~RPN();

	void print(void);
	void calculate(void);
	static const char* getErrorMessage();
	static void setErrorMessage(std::string message);
	
	class ErrorRPN : public std::exception
	{
		public :
		ErrorRPN(t_errors error);
		virtual const char* what() const throw();
		
		private :
		ErrorRPN();
	};

	private:
	RPN();
	static std::string 			_error_message;
	std::list<std::string>		_rpn_elements;

	void 	split_rpn_to_list(std::string raw_rpn);
	bool 	rpnCharCheck(std::string &str);
	bool 	is_operator(std::string &op);
	bool 	is_number(std::string &str);
	double	do_operation(double op1, double op2, std::string &operat);
	std::string to_string(double nb);
};
#endif