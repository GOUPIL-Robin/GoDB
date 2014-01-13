##
## Easymake
##
## Made by robin goupil
## Login   <goupil_r@epitech.net>
##
## Started on  Wed May 15 07:58:45 2013 robin goupil
## Last update Mon Sep  2 13:42:45 2013 robin goupil
##

MAKE.NAME	=	GoDB

CXX		=	g++
LNK		=	g++
LD		=	ar rcs

DIR.I		=	./include
DIR.S		=	./sources
DIR.O		=	./.objects
DIR.CUSTOM	=	./doc ./bin

#DEPS		=

FLAGS.I		=	-I$(DIR.I)
FLAGS.CXX	=	-W -Wall -Werror -Wextra
FLAGS.DEBUG	=	-g3 -DDEBUG
FLAGS.RELEASE	=	-O2 -DRELEASE

FLAGS.LNK	=
FLAGS.LD	=

# $(BIN) || $(STATIC) || $(SHARED)
TARGET.TYPE	=	$(STATIC)
TARGET.NAME	=	bin/libGoDB.a
# $(DEBUG) || $(RELEASE)
TARGET.MOD	=	$(DEBUG)

SRC		=	Database.cpp			\
			DataStorer.cpp			\
			AResource.cpp			\
			SizableResource.cpp		\


include ./.easymake

test:		deps present dirs $(TARGET.NAME)
	$(LNK) $(FLAGS.DEBUG) -Iinclude sources/main.cpp -L./bin -lGoDB

doc:		deps present dirs
	$(RM) -fr doc/html
	doxygen