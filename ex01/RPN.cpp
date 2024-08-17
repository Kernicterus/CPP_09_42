/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:03:37 by nledent           #+#    #+#             */
/*   Updated: 2024/06/29 12:47:41 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
std::string RPN::_error_message = "";

RPN::RPN()
{
}

RPN::RPN(int argc, char **argv)
{
	std::string raw_rpn;

	for (int i = 1; i < argc; i++)
	{
		raw_rpn += argv[i];
		raw_rpn += " ";
	}
	if (rpnCharCheck(raw_rpn) == false)
		throw (RPN::ErrorRPN(ERR_BAD_CHAR));	
	split_rpn_to_list(raw_rpn);
}

RPN::RPN(RPN& src)
{
	_rpn_elements = src._rpn_elements;
}

RPN& RPN::operator=(RPN& src)
{
	_rpn_elements = src._rpn_elements;
	return (*this);
}

RPN::~RPN()
{
}

void RPN::split_rpn_to_list(std::string raw_rpn)
{
	std::string 	tmp = "";
	for (std::string::iterator it = raw_rpn.begin(); it != raw_rpn.end(); it++)
	{
		if (*it == ' ')
		{
			if (tmp != "")
			{
				_rpn_elements.push_back(tmp);
				tmp = "";
			}
			continue;
		}
		else
			tmp += *it;
	}
}

bool RPN::is_operator(std::string &op)
{
	if (op == "+" || op == "-" || op == "/" || op == "*")
		return true;
	return false;
}

void RPN::calculate()
{
	double				 				result_tmp;
	double								op1;
	double								op2;
	std::stack<std::string> 			stack;
	std::list<std::string>::iterator 	it_list = _rpn_elements.begin();
	std::list<std::string>::iterator 	it_list_end = _rpn_elements.end();

	for (it_list=_rpn_elements.begin(); it_list != it_list_end; it_list++)
	{
		if (is_number(*it_list) == false && is_operator(*it_list) == false)
			throw (ErrorRPN(ERR_INVALID_EXPR));
	}

	it_list = _rpn_elements.begin();
	while (it_list != it_list_end)
	{
		while (it_list != it_list_end && !is_operator(*it_list))
		{
			stack.push(*it_list);
			it_list++;
		}
		if (it_list == it_list_end)
			throw (RPN::ErrorRPN(ERR_INVALID_EXPR));
		else if (it_list != it_list_end)
		{
			if (stack.size() < 2)
				throw (RPN::ErrorRPN(ERR_INVALID_EXPR));
			op2 = std::atof(stack.top().c_str());
			stack.pop();
			op1 = std::atof(stack.top().c_str());
			stack.pop();
			result_tmp = do_operation(op1, op2, *it_list);
			stack.push(to_string(result_tmp));
			it_list++;
		}
		else
			break;
	}
	if (stack.size() != 1)
		throw (RPN::ErrorRPN(ERR_INVALID_EXPR));
	std::cout << result_tmp << std::endl;
}

double	RPN::do_operation(double op1, double op2, std::string &operat)
{
	if (operat == "+")
			return (op1+op2);
	else if (operat == "-")
			return (op1-op2);
	else if (operat == "/")
	{
		if (op2 == 0)
			throw (RPN::ErrorRPN(ERR_INVALID_EXPR));
		return (op1/op2);
	}
	else if (operat == "*")
		return (op1*op2);
	else
		throw (ErrorRPN(ERR_INVALID_EXPR));			
}

bool RPN::rpnCharCheck(std::string &str)
{
	std::string::iterator it = str.begin();
	std::string::iterator ite = str.end();
	while (it != ite)
	{
		if (*it >= '0' && *it <= '9')
			it++;
		else if (*it == '+' || *it == '-' || *it == '/' || *it == '*')
			it++;
		else if (*it == ' ')
			it++;
		else 
			return (false);
	}
	return true;
}

bool RPN::is_number(std::string &str)
{
	std::string::iterator it = str.begin();
	std::string::iterator ite = str.end();

	if (*it == '+' || *it == '-')
		*it++;
	if (it == ite)
		return false;
	while (it != ite)
	{
		if (*it < '0' || *it > '9')
			return false;
		it++;
	}
	return true;
}

std::string RPN::to_string(double nb)
{
	std::ostringstream oss;
	oss << nb;
	std::string str = oss.str();
	return (str);
}

void RPN::print(void)
{
	std::cout << "In the container : ";
	std::list<std::string>::iterator it = _rpn_elements.begin();
	std::list<std::string>::iterator ite = _rpn_elements.end();
	while (it != ite)
	{
		std::cout << *it;
		it++;
		if (it != ite)
			std::cout << ", ";
	}
	std::cout << std::endl;
}

const char* RPN::getErrorMessage()
{
	return (_error_message.c_str());
}

void RPN::setErrorMessage(std::string message)
{
	_error_message = message;
}

RPN::ErrorRPN::ErrorRPN(t_errors error)
{
	std::string str = "Error : ";
	switch (error)
	{
		case ERR_BAD_INPUT_RPN:
			str+= "bad input in rpn expression";
			break;
		case ERR_MISSING_SPACE:
			str+= "missing space between characters";
			break;
		case ERR_INVALID_EXPR:
			str+= "invalid expression";
			break;	
		case ERR_BAD_CHAR:
			str+= "bad character";
			break;					
		default :
			break;
	}
	RPN::setErrorMessage(str);
}

const char* RPN::ErrorRPN::what() const throw()
{
	return (getErrorMessage());
}