#========================================#
#=============== VARIABLES ==============#
#========================================#

#~~~~ Output ~~~~#

NAME		=	ircserv

#~~~~ Paths ~~~~#

VPATH		=	src/
PATH_INC	=	inc/
PATH_OBJ	=	obj/

#~~~~ Files ~~~~#
 
SRC			=	main.cpp \
				Channel.cpp \
				Client.cpp
OBJ			=	$(addprefix $(PATH_OBJ), $(SRC:.cpp=.o))
INC			=	$(addprefix $(PATH_INC), irc.hpp) \
				$(addprefix $(PATH_INC), Channel.hpp) \
				$(addprefix $(PATH_INC), Client.hpp)


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

.PHONY:		all clean fclean re eugene
