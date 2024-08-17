# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nledent <nledent@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 19:12:30 by nledent           #+#    #+#              #
#    Updated: 2024/07/01 14:55:37 by nledent          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = PmergeMe
CC = c++
FLAGS = -Wextra -Werror -Wall -std=c++98 -g

SRCS = main.cpp \
		
OBJS = $(SRCS:%.cpp=%.o)

%.o : %.cpp
	$(CC) $(FLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) -o $@ $^
	
clean : 
	rm -f $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

debug : fclean
	$(CC) $(FLAGS) $(SRCS) -o $(NAME) -D DEBUG


.PHONY : re clean fclean debug