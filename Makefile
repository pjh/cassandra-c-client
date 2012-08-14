# ......

# gen-c_glib is the directory created when this command is run inside of
# the cassandra directory:
#   thrift --gen c_glib interface/cassandra.thrift
# The entire directory can then be copied here, or this path can be modified.
THRIFT_GEN_DIR	= gen-c_glib

CC 	= gcc
#INCL = -I/usr/local/include/thrift/ -I/usr/include/boost/
THRIFT_CFLAGS	:= $(shell pkg-config --cflags thrift_c_glib)
INCL	= -I$(THRIFT_GEN_DIR) $(THRIFT_CFLAGS)

GLIBCFLAGS1 := $(shell pkg-config --cflags glib-2.0)
GLIBCFLAGS2 := $(shell pkg-config --cflags gthread-2.0)
GLIBCFLAGS3 := $(shell pkg-config --cflags gobject-2.0)
CFLAGS	= $(INCL) -D_GNU_SOURCE -g -Wall
#CFLAGS 	= $(INCL) -D_GNU_SOURCE $(GLIBCFLAGS1) $(GLIBCFLAGS2) $(GLIBCFLAGS3) -g -Wall

THRIFT_LIBS	:= $(shell pkg-config --libs thrift_c_glib)
GLIBCLIBS1 := $(shell pkg-config --libs glib-2.0)
GLIBCLIBS2 := $(shell pkg-config --libs gthread-2.0)
GLIBCLIBS3 := $(shell pkg-config --libs gobject-2.0)
LIBS	= $(THRIFT_LIBS)
#LIBS = -L$(HOME)/lib/ -lthrift -lthrift_c_glib $(GLIBCLIBS1) $(GLIBCLIBS2) $(GLIBCLIBS3)
#LIBS = -L$(HOME)/lib/ -lthrift -lthrift_c_glib -lthriftnb -lthriftz $(GLIBCLIBS1) $(GLIBCLIBS2) $(GLIBCLIBS3)

#/home/pjh/lib/libthrift.a  /home/pjh/lib/libthrift_c_glib.a  /home/pjh/lib/libthriftnb.a  /home/pjh/lib/libthriftz.a
#   looks like libthriftnb needs -levent as well
#  Glib flags: http://developer.gnome.org/glib/2.28/glib-compiling.html
#    Note that if you want to override the system-default lib directories
#    (i.e. on a systems lab machine) and use your own, LD_RUN_PATH must
#    be set (in addition to using the pkg-config command below)
#    Example (before "make"): export LD_RUN_PATH=$HOME/lib
#  Also: http://stackoverflow.com/questions/1435861/computing-makefile-variable-on-assignment
#    http://linuxmafia.com/faq/Admin/ld-lib-path.html
#    http://gcc.gnu.org/faq.html

default: test_cassandra_client

cassandra_types.o: $(THRIFT_GEN_DIR)/cassandra_types.c $(THRIFT_GEN_DIR)/cassandra_types.h
	$(CC) -c -o $@ $< $(CFLAGS)

cassandra.o: $(THRIFT_GEN_DIR)/cassandra.c $(THRIFT_GEN_DIR)/cassandra.h
	$(CC) -c -o $@ $< $(CFLAGS)

test_cassandra_client: test_cassandra_client.c cassandra_types.o cassandra.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#test-cassandra: test-cassandra.c
#	$(CC) -c -o $@.o $@.c $(CFLAGS)
#
#TEST_EXEC = test-cassandra
#test: cassandra_types.o cassandra.o test-cassandra.o
#	$(CC) -o $(TEST_EXEC) test-cassandra.o cassandra.o cassandra_types.o $(CFLAGS) $(LIBS)

all: test_cassandra_client

clean:
	rm -f *.o test_cassandra_client

