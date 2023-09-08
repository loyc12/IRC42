#ifndef COLOURS_HPP
# define COLOURS_HPP

# define	COLEN		(int)7

// Dark colours (Becomes light if bold)
# define	DMAGENTA	"\033[0;35m"
# define	DRED		"\033[0;31m"
# define	DYELLOW		"\033[0;33m"
# define	DGREEN		"\033[0;32m"
# define	DCYAN		"\033[0;36m"
# define	DBLUE		"\033[0;34m"

// Light and bold colours
# define	MAGENTA		"\033[1;95m"
# define	RED			"\033[1;91m"
# define	YELLOW		"\033[1;93m"
# define	GREEN		"\033[1;92m"
# define	CYAN		"\033[1;96m"
# define	BLUE		"\033[1;94m"

// Grayscale colours
# define	NOCOLOR		"\033[1;92m" // Setting defcol to bold green
# define	BLACK		"\033[1;30m"
# define	GRAY		"\033[1;90m"
# define	LGRAY		"\033[1;37m"
# define	WHITE		"\033[1;97m"

// Default colour
# define	DEFCOL		"\033[0000m"


#endif //COLOURS_HPP