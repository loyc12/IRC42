#------------------------------------------------------------------------------#
#                              PROJECT VARIABLES                               #
#------------------------------------------------------------------------------#

# Executable name
NAME		=	ircserv

# Source file names (prefix their subdir if needed)
FILES		=	server \
				main \
				user \
				utils \
				channel \
#				message \

# Directory names
SRCDIR		=	src/
OBJDIR		=	obj/
INCDIR		=	includes/
SUBDIRS		=

# Libraries (.a files) to include for compilation
LIBS		=
LIBX		=

# Arguments to call the command with
ARGS		= 6666 1234