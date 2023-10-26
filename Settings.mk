#------------------------------------------------------------------------------#
#                              PROJECT VARIABLES                               #
#------------------------------------------------------------------------------#

# Executable name
NAME		=	ircserv

# Source file names (prefix their subdir if needed)
FILES		=	server_core \
				server_chan \
				server_client \
				server_cmds \
				server_comm \
				server_find \
				main \
				user \
				utils \
				channel \

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