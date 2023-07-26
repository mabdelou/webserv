# .DEFAULT_GOAL	= run

NAME 			= webserve

CFLAGS			= -std=c++98 -Wall -Wextra -Werror

CFILES			= main.cpp

# Mabdelou
CFILES			+= parssing/parssing/Parssing.cpp parssing/parssing/Utilities.cpp \
					parssing/request_parssing/parssing_files/Request.cpp \
					parssing/request_parssing/parssing_files/Request_utilitis.cpp

# Ahmaidi
CFILES			+= Response/auto_index.cpp Response/fetch_index_files.cpp\
					Response/Response.cpp Response/check_request_well_fromed.cpp \
					Response/err.cpp Response/cgi.cpp

# Mboukhal
CFILES			+= socket/socketInit.cpp socket/socketRW.cpp \
					socket/socketListen.cpp socket/socketUtils.cpp \
					socket/getIter.cpp socket/sendIt.cpp  \
					shared/shared.cpp 


OBJS			= $(CFILES:.cpp=.o)

all : $(NAME)
	@ rm -rf .vscode

%.o: %.cpp
	c++ $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@ c++ $(OBJS) -o $(NAME) 

clean:
	@ rm -rf $(OBJS)

fclean: clean
	@ rm -rf $(NAME) 
	
re: fclean all

run: all
	@ ./$(NAME) "file.conf"

	
.PHONY: fclean clean re all run 