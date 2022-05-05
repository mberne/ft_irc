#========================================#
#=============== VARIABLES ==============#
#========================================#

#~~~~ Output ~~~~#

SERV		=	ircserv
CLIENT		=	ircclient

#~~~~ Paths ~~~~#

VPATH		=	src/
PATH_INC	=	inc/
PATH_OBJ	=	obj/
PATH_CLIENT =	irc_client/

#~~~~ Files ~~~~#
 
SRC			=	main.cpp
OBJ			=	$(addprefix $(PATH_OBJ), $(SRC:.cpp=.o))
INC			=	$(addprefix $(PATH_INC), irc.hpp)

#~~~~ Macros ~~~~#

CC			=	c++
CFLAG		=	-Wall -Wextra -Werror -std=c++98
RM			=	rm -rf

#========================================#
#=============== TARGETS ================#
#========================================#

#~~~~ Main Rules ~~~~#

all :			$(SERV) $(CLIENT)

$(SERV) :		$(OBJ)
				$(CC) $(CFLAG) $(OBJ) -o $(SERV) -I $(PATH_INC)

$(CLIENT) :
				$(MAKE) -C $(PATH_CLIENT)
				mv $(PATH_CLIENT)bircd $(CLIENT)

re :			fclean all

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJ)%.o :	%.cpp $(INC)
					@mkdir -p $(PATH_OBJ);
					$(CC) $(CFLAG) -c $< -o $@ -I $(PATH_INC)

#~~~~ Cleaning Rules ~~~~#

clean :
			$(RM) $(PATH_OBJ)
			$(MAKE) clean -C $(PATH_CLIENT)

fclean :
			$(RM) $(PATH_OBJ) $(SERV) $(CLIENT)
			$(MAKE) fclean -C $(PATH_CLIENT)

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
			@ echo "               ,'           )     \`.         |"
			@ echo "                7____,,..--'      /          |"
			@ echo "                                  \`---.__,--.'"

.PHONY:		all clean fclean re eugene
