/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nledent <nledent@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:03:37 by nledent           #+#    #+#             */
/*   Updated: 2024/06/30 13:17:57 by nledent          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOIN_EXCHANGE_HPP
# define BITCOIN_EXCHANGE_HPP

# include <algorithm>
# include <iostream>
# include <fstream>
# include <map>
# include <cmath>
# include <regex.h>

typedef 	enum e_errors
{
	ERR_NB_ARGS,
	ERR_FILENAME,
	ERR_NODATABASE,
	ERR_HEADERDATABASE,
	ERR_DATABASEINVALID,
	ERR_SYSTEM,
	ERR_HEADERINPUT,
	
} t_errors;

class BitcoinExchange
{
	public: 
	BitcoinExchange();
	BitcoinExchange(BitcoinExchange& src);
	BitcoinExchange& operator=(BitcoinExchange& src);
	~BitcoinExchange();

	void ParseWallet(char *argv1);
	void loadDatabase();
	bool is_database_line_valid(const char* str);
	bool is_wallet_line_valid(const char* str);
	bool is_date_valid(std::string date);

	double 		getValue(std::string date);
	static const char* getErrorMessage();
	static void setErrorMessage(std::string message);

	class ErrorBtcExchange : public std::exception
	{
		public :
		ErrorBtcExchange(t_errors error);
		virtual const char* what() const throw();
		
		private :
		ErrorBtcExchange();
	};

	private:
	static std::string 				_error_message;
	std::ifstream 					_wallet_file;
	std::map<std::string, double> 	_database;
};
#endif