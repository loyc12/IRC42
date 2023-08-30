include Settings.mk

#------------------------------------------------------------------------------#
#                                   COLOURS                                    #
#------------------------------------------------------------------------------#

# Dark colours (Becomes light if bold)
DMAGENTA	=\033[0;35m
DRED		=\033[0;31m
DYELLOW		=\033[0;33m
DGREEN		=\033[0;32m
DCYAN		=\033[0;36m
DBLUE		=\033[0;34m

# Light and bold colours
MAGENTA		=\033[1;95m
RED			=\033[1;91m
YELLOW		=\033[1;93m
GREEN		=\033[1;92m
CYAN		=\033[1;96m
BLUE		=\033[1;94m

# Grayscale colours
DEFCOL		=\033[0m
BLACK		=\033[1;30m
GRAY		=\033[1;90m
LGRAY		=\033[1;37m
WHITE		=\033[1;97m

# RED		: Deletion done (major)
# MAGENTA	: Deletion done (minor)

# YELLOW	: Task started
# GREEN		: Task done

# CYAN		: Creation done
# BLUE		: Installation done

#------------------------------------------------------------------------------#
#                                   GENERICS                                   #
#------------------------------------------------------------------------------#

# Special variables
DEFAULT_GOAL: all
.DELETE_ON_ERROR: $(NAME)
.PHONY: all long \
		clean clear \
		fclean fclear \
		re run rerun \
		leaks releaks \
		vleaks revleaks \
		norm libft \
		brew cmake \
		grind \

#------------------------------------------------------------------------------#
#                                    FLAGS                                     #
#------------------------------------------------------------------------------#

# Flags for gcc, valgrind and leaks
CFLAGS	=	-Wall -Werror -Wextra -std=c++98 $(XFLAGS)
LFLAGS	=	-atExit
VFLAGS	=	--leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes
# add "--suppressions=include/supp" to use the supression file

# Comment the line bellow to have verbose cmds:
HIDE	=	@

# Extra flags
# Use "export XFLAGS= {flags} " to add extra compilation flags
# Potential flags to use :
# -g					for debug mode
# -fsanitize=thread		to see race conditions

#------------------------------------------------------------------------------#
#                                  VARIABLES                                   #
#------------------------------------------------------------------------------#

# Compiler, flags and shortcuts
CC		=	c++
RM		=	rm -rf
CPY		=	cp -f
MKDR	=	mkdir -p
INCLUDE =	-I INCDIR
VPATH	=	SRCDIR \
			INCDIR \

# Creates file paths
SRCS	=	$(addprefix $(SRCDIR), $(addsuffix .cpp, $(FILES)))
OBJS	=	$(addprefix $(OBJDIR), $(addsuffix .o, $(FILES)))

# Command to call when using make run or make leaks
CMD			=	./$(NAME) $(ARGS)

#------------------------------------------------------------------------------#
#                                 BASE TARGETS                                 #
#------------------------------------------------------------------------------#

# For full install (except brew)
long: cmake glfw $(NAME)

# For standard compilation
all: mkobj $(NAME)

mkobj:
	$(HIDE) mkdir -p obj

# Compiles all files into an executable
$(NAME): $(OBJS)
	@echo "$(GREEN)Files compiled with flags : $(CFLAGS)$(DEFCOL)"
	@echo "$(DEFCOL)"
	$(HIDE) $(CC) $(MODE) $(CFLAGS) -o $@ $^ $(INCLUDE) $(LIBS) $(LIBX)
	@echo "$(CYAN)Executable created! $(DEFCOL)"
	@echo "$(DEFCOL)"

# Compiles each source file into a .o file
$(OBJS): $(OBJDIR)%.o : $(SRCDIR)%.cpp
	@echo "$(YELLOW)Compiling file : $< $(DEFCOL)"
	$(HIDE) $(CC) $(MODE) $(CFLAGS) -c $< -o $@

#------------------------------------------------------------------------------#
#                               CLEANING TARGETS                               #
#------------------------------------------------------------------------------#

# Removes objects
clear: clean
clean:
	$(HIDE) $(RM) $(OBJS)
	$(HIDE) $(RM) $(NAME).dSYM
	@echo "$(MAGENTA)Deleted object files $(DEFCOL)"
	@echo "$(DEFCOL)"

# Removes object dir and executable
fclear: fclean
fclean: clean
	$(HIDE) $(RM) $(OBJDIR)
	@echo "$(RED)Deleted object directory $(DEFCOL)"
	$(HIDE) $(RM) $(NAME)
	@echo "$(RED)Deleted executable $(DEFCOL)"
	@echo "$(DEFCOL)"

# Removes object dir and executable and remakes everything
re: fclean all
	@echo "$(CYAN)Rebuilt everything! $(DEFCOL)"
	@echo "$(DEFCOL)"

#------------------------------------------------------------------------------#
#                               SHORTCUT TARGETS                               #
#------------------------------------------------------------------------------#

# Runs the program
rerun: re run
run: all
	@echo "$(YELLOW)Launching command : $(CMD) $(DEFCOL)"
	@echo "$(GREEN)"
	$(HIDE) $(CMD) || true
	@echo "$(GREEN)Exited normally! $(DEFCOL)"
	@echo "$(DEFCOL)"

# Runs the program with leaks
releaks: re leaks
leaks: all
	@echo "$(RED)Launching command : leaks $(LFLAGS) -- $(CMD) $(DEFCOL)"
	@echo "$(YELLOW)"
	$(HIDE) leaks $(LFLAGS) -- $(CMD) || true
	@echo "$(GREEN)Exited normally! $(DEFCOL)"
	@echo "$(DEFCOL)"

# Runs the program with valgrind
revleaks: re vleaks
vleaks: all
	@echo "$(RED)Launching command : valgrind $(VFLAGS) $(CMD) $(DEFCOL)"
	@echo "$(YELLOW)"
	$(HIDE) valgrind $(VFLAGS) $(CMD) || true
	@echo "$(GREEN)Exited normally! $(DEFCOL)"
	@echo "$(DEFCOL)"

# Runs the norminette
norm:
	@echo "$(DEFCOL)"
	@echo "$(YELLOW)Norminetting .cpp files $(RED)"
	@norminette $(SRCS) | grep Error || true
	@echo "$(DEFCOL)"
	@echo "$(YELLOW)Norminetting .hpp files $(RED)"
	@norminette include | grep Error || true
	@echo "$(DEFCOL)"

# Updates the libft module (requires push after)
libft:
	@echo "$(YELLOW)Updating libft to latest commit $(WHITE)"
	$(HIDE) cd Libft42 && git pull origin main
	@echo "$(GREEN)Libft updated! $(DEFCOL)"
	@echo "$(DEFCOL)"

#------------------------------------------------------------------------------#
#                                 BREW TARGETS                                 #
#------------------------------------------------------------------------------#

# Installs/Updates homebrew (called manually cause slow af)
brew:
	$(HIDE) bash include/brew_install.sh
	@echo "$(BLUE)Brew installed $(DEFCOL)"
	@echo "$(RED)CLOSE AND REOPEN TERMINAL IF IT DOESN'T WORK$(DEFCOL)"
	@echo "$(GREEN)Exited normally! $(DEFCOL)"
	@echo "$(DEFCOL)"

# Installs/Updates cmake
cmake:
	@echo "$(YELLOW)Installing Cmake $(DEFCOL)"
	$(HIDE) brew install cmake
	@echo "$(BLUE)Cmake installed $(DEFCOL)"
	@echo "$(DEFCOL)"

# Installs/Updates valgrind
grind:
	@echo "$(YELLOW)Installing Valgrind $(DEFCOL)"
	$(HIDE) brew tap LouisBrunner/valgrind
	$(HIDE) brew install --HEAD LouisBrunner/valgrind/valgrind
	@echo "$(BLUE)Valgrind installed $(DEFCOL)"
	@echo "$(DEFCOL)"