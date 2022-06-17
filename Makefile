#========================================#
#=============== VARIABLES ==============#
#========================================#

#~~~~ Output ~~~~#

NAME		=	ircserv

#~~~~ Paths ~~~~#

VPATH		=	src/:inc/:src/command/
PATH_INC	=	inc/
PATH_OBJ	=	obj/

#~~~~ Files ~~~~#
 
SRC			=	main.cpp						\
				Server.cpp						\
				Client.cpp						\
				Channel.cpp						\
				channel_operations.cpp			\
				connection_registration.cpp		\
				miscellaneous_messages.cpp		\
				sending_messages.cpp			\
				server_queries_and_commands.cpp	\
				user_based_queries.cpp

OBJ			=	$(addprefix $(PATH_OBJ), $(SRC:.cpp=.o))

INC			=	ircserv.hpp		\
				Server.hpp		\
				Client.hpp		\
				Channel.hpp		\
				replies.hpp		\
				errors.hpp

#~~~~ Macros ~~~~#

CC			=	c++
CFLAG		=	-Wall -Wextra -Werror -std=c++98
RM			=	rm -rf

#========================================#
#=============== TARGETS ================#
#========================================#

#~~~~ Main Rules ~~~~#

all :			$(NAME)

$(NAME) :		$(OBJ)
				$(CC) $(CFLAG) $(OBJ) -o $(NAME) -I $(PATH_INC)

re :			fclean all

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJ)%.o :	%.cpp $(INC)
					@mkdir -p $(PATH_OBJ);
					$(CC) $(CFLAG) -c $< -o $@ -I $(PATH_INC)

#~~~~ Cleaning Rules ~~~~#

clean :
			$(RM) $(PATH_OBJ)

fclean :
			$(RM) $(PATH_OBJ) $(NAME)

#~~~~ Eugene ~~~~#

eugene :	
			@ echo "               _,........__"
			@ echo "            ,-'            \"\`-."
			@ echo "          ,'                   \`-."
			@ echo "        ,'                        \\"
			@ echo "      ,'                           ."
			@ echo "      .'\\               ,\"\".       \`"
			@ echo "     ._.'|             / |  \`       \\"
			@ echo "     |   |            \`-.'  ||       \`."
			@ echo "     |   |            '-._,'||       | \\"
			@ echo "     .\`.,'             \`..,'.'       , |\`-."
			@ echo "     l                       .'\`.  _/  |   \`."
			@ echo "     \`-.._'-   ,          _ _'   -\" \\  .     \`"
			@ echo "\`.\"\"\"\"\"'-.\`-...,---------','         \`. \`....__."
			@ echo ".'        \`\"-..___      __,'\\          \\  \\     \\"
			@ echo "\\_ .          |   \`\"\"\"\"'    \`.           . \\     \\"
			@ echo "  \`.          |              \`.          |  .     L"
			@ echo "    \`.        |\`--...________.'.        j   |     |"
			@ echo "      \`._    .'      |          \`.     .|   ,     |"
			@ echo "         \`--,\\       .            \`7\"\"' |  ,      |"
			@ echo "            \` \`      \`            /     |  |      |    _,-'\"\"\"\`-."
			@ echo "             \\ \`.     .          /      |  '      |  ,'          \`."
			@ echo "              \\  v.__  .        '       .   \\    /| /              \\"
			@ echo "               \\/    \`\"\"\\\"\"\"\"\"\"\"\`.       \\   \\  /.''                |"
			@ echo "                \`        .        \`._ ___,j.  \`/ .-       ,---.     |"
			@ echo "                ,\`-.      \\         .\"     \`.  |/        j     \`    |"
			@ echo "               /    \`.     \\       /         \\ /         |     /    j"
			@ echo "              |       \`-.   7-.._ .          |\"          '         /"
			@ echo "              |          \`./_    \`|          |            .     _,'"
			@ echo "              \`.           / \`----|          |-............\`---'"
			@ echo "                \\          \\      |          |"
			@ echo "               ,'          )     \`.         |"
			@ echo "                7____,,..--'      /          |"
			@ echo "                                  \`---.__,--.'"

.PHONY:		all clean fclean re eugene camion
