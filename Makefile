# Makefile for Cassandra C Client
#
# Written in 2012 by Peter Hornyack <pjh@cs.washington.edu>
#
# To the extent possible under law, the author(s) have dedicated all
# copyright and related and neighboring rights to this software to the public
# domain worldwide. This software is distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along
# with this software. If not, see
# <http://creativecommons.org/publicdomain/zero/1.0/>.

# Sorry that the style of this Makefile is so awful.
#
# To use your own client code that builds on the functions in
# cassandra_client.h, replace TEST_CLIENT with the name of your own file,
# or tweak the Makefile below to use your own object files, etc.
TEST_CLIENT			= test_cassandra_client
TEST_CLIENT_FILE	= $(TEST_CLIENT).c

### Build parameters: ###

# gen-c_glib is the directory created when this command is run inside of
# the cassandra directory:
#   thrift --gen c_glib interface/cassandra.thrift
# The entire directory can then be copied here, or this path can be modified.
THRIFT_GEN_DIR	= gen-c_glib
THRIFT_CFLAGS	:= $(shell pkg-config --cflags thrift_c_glib)
THRIFT_LIBS		:= $(shell pkg-config --libs thrift_c_glib)
INCL			= -I$(THRIFT_GEN_DIR) $(THRIFT_CFLAGS)
#INCL = -I/usr/local/include/thrift/ -I/usr/include/boost/

CC 		= gcc
OBJ_DIR	= obj
CFLAGS	= $(INCL) -D_GNU_SOURCE -g -Wall
LIBS	= $(THRIFT_LIBS)

### Build targets: ###
default: $(TEST_CLIENT)

cassandra_types.o: $(THRIFT_GEN_DIR)/cassandra_types.c $(THRIFT_GEN_DIR)/cassandra_types.h
	$(CC) -c -o $(OBJ_DIR)/$@ $< $(CFLAGS)

cassandra.o: $(THRIFT_GEN_DIR)/cassandra.c $(THRIFT_GEN_DIR)/cassandra.h
	$(CC) -c -o $(OBJ_DIR)/$@ $< $(CFLAGS)

cassandra_client.o: cassandra_client.c cassandra_client.h ccc_macros.h
	$(CC) -c -o $(OBJ_DIR)/$@ $< $(CFLAGS)

_OBJ	= cassandra_types.o cassandra.o cassandra_client.o
OBJ     = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))
$(TEST_CLIENT): $(TEST_CLIENT_FILE) $(_OBJ) ccc_macros.h
	$(CC) -o $@ $(TEST_CLIENT_FILE) $(OBJ) $(CFLAGS) $(LIBS)

all: $(TEST_CLIENT)

clean:
	rm -f $(OBJ_DIR)/*.o $(TEST_CLIENT)

