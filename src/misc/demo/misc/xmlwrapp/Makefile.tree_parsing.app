# $Id: Makefile.tree_parsing.app 196361 2010-07-01 21:29:50Z satskyse $

SRC = tree_parsing
APP = tree_parsing
LIB = xmlwrapp
LIBS = $(ORIG_LIBS) $(LIBXML_LIBS) $(LIBXSLT_LIBS)

CPPFLAGS= $(ORIG_CPPFLAGS) $(LIBXML_INCLUDE) $(LIBXSLT_INCLUDE)

WATCHERS = satskyse

