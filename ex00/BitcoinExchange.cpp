/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:03:37 by nledent           #+#    #+#             */
/*   Updated: 2024/07/12 16:24:26 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
std::string BitcoinExchange::_error_message = "";

BitcoinExchange::BitcoinExchange(){}

BitcoinExchange::BitcoinExchange(BitcoinExchange& src)
{
	(void)src;
}

BitcoinExchange& BitcoinExchange::operator=(BitcoinExchange& src)
{
	(void)src;
	return (*this)
;}

BitcoinExchange::~BitcoinExchange()
{
	_wallet_file.close();
}

bool BitcoinExchange::is_wallet_line_valid(const char* str)
{
    regex_t 	regex;
    int 		r;
	bool		r_value = true;

	r = regcomp(&regex, "^([0-9]{4}-[0-9]{2}-[0-9]{2} \\| [-+]?[0-9]+(\\.[0-9]+)?)$", REG_EXTENDED | REG_ICASE);
    if (r)
		throw (ErrorBtcExchange(ERR_SYSTEM));
    r = regexec(&regex, str, 0, NULL, 0);
    if (!r) 
        r_value = true;
	else if (r == REG_NOMATCH)
		r_value = false;
	else 
		throw (ErrorBtcExchange(ERR_SYSTEM));
	regfree(&regex);
	return (r_value);	
}

void BitcoinExchange::ParseWallet(char *argv1)
{
	std::string 	line;
	std::string		date;
	bool			is_empty = 1;

	_wallet_file.open(argv1);
	if (_wallet_file.fail())
		throw(ErrorBtcExchange(ERR_FILENAME));
	while (std::getline(_wallet_file, line))
	{
		is_empty = 0;
		if (line == "date | value")
			continue;
		if (is_wallet_line_valid(line.c_str()) == false)
		{
			std::cout << "Error: bad input" << std::endl;
			continue;
		}
		date = line.substr(0,10);
		if (is_date_valid(date.c_str()) == false)
		{
			std::cout << "Error: invalid date => " << date << std::endl;
			continue;
		}
		double btc_nb = std::atof(line.substr(13).c_str());
		if (btc_nb < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}
		if (std::isinf(btc_nb) || btc_nb > 1000)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}
		std::cout << date << " => " << (getValue(date) * btc_nb) << std::endl;
	}
	if (is_empty)
		throw(std::invalid_argument("Error : input file empty"));
}

bool BitcoinExchange::is_database_line_valid(const char* str)
{
    regex_t 	regex;
    int 		r;
	bool		r_value = true;

	r = regcomp(&regex, "^[0-9]{4}-[0-9]{2}-[0-9]{2},[0-9]+(\\.[0-9]+)?$", REG_EXTENDED | REG_ICASE);
    if (r)
		throw (ErrorBtcExchange(ERR_SYSTEM));
    r = regexec(&regex, str, 0, NULL, 0);
    if (!r) 
        r_value = true;
	else if (r == REG_NOMATCH)
	{
        std::cout << "invalid line : " ;
        std::cout << str << std::endl;
		r_value = false;
	}
	else 
		throw (ErrorBtcExchange(ERR_SYSTEM));
	regfree(&regex);
	return (r_value);
}

bool BitcoinExchange::is_date_valid(std::string date)
{
	int year;
	int month;
	int day;

	year = std::atoi(date.substr(0,4).c_str());
	month = std::atoi(date.substr(5,2).c_str());
	day = std::atoi(date.substr(8,2).c_str());
	if (month > 12 || month < 1 || day == 0)
		return false;
	if (month == 2)
	{
		if (((year % 100 == 0 && year % 400 != 0) || year % 4 != 0) && day > 28)
			return false;
		else if (day > 29)
			return false;
	}
	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		if (day > 31)
			return false;
	}
	else
	{
		if (day > 30)
			return false;
	}
	return true;
}

void BitcoinExchange::loadDatabase()
{
	std::ifstream 	file;
	std::string 	line;
	std::string		date;

	file.open("data.csv");
	if (file.fail())
		throw(ErrorBtcExchange(ERR_NODATABASE));
	if (!std::getline(file, line))
		throw(std::invalid_argument("Error : empty database file"));
	if (line != "date,exchange_rate")
		throw(ErrorBtcExchange(ERR_HEADERDATABASE));
	while (std::getline(file, line))
	{
		if (is_database_line_valid(line.c_str()) == false)
			throw(ErrorBtcExchange(ERR_DATABASEINVALID));
		date = line.substr(0,10);
		if (is_date_valid(line.c_str()) == false)
			throw(ErrorBtcExchange(ERR_DATABASEINVALID));
		_database[date] = std::atof(line.substr(11).c_str());
	}
	file.close();
}

double 	BitcoinExchange::getValue(std::string date)
{
	std::map<std::string,double>::iterator it;

	it = _database.upper_bound(date);
	if (it != _database.begin())
		it--;
	return (it->second);
}

const char* BitcoinExchange::getErrorMessage()
{
	return (_error_message.c_str());
}

void BitcoinExchange::setErrorMessage(std::string message)
{
	_error_message = message;
}

BitcoinExchange::ErrorBtcExchange::ErrorBtcExchange(t_errors error)
{
	std::string str = "Error : ";
	switch (error)
	{
		case ERR_FILENAME:
			str+= "input file open failure";
			break;
		case ERR_NODATABASE:
			str+= "data.csv open failure";
			break;
		case ERR_HEADERDATABASE:
			str+= "data.csv : header corrupted";
			break;
		case ERR_HEADERINPUT:
			str+= "input wallet file : header corrupted";
			break;
		case ERR_DATABASEINVALID:
			str+= "data.csv : invalid data";
			break;
		case ERR_SYSTEM:
			str+= "system error";
			break;	
		default :
			break;
	}
	BitcoinExchange::setErrorMessage(str);
}

const char* BitcoinExchange::ErrorBtcExchange::what() const throw()
{
	return (getErrorMessage());
}