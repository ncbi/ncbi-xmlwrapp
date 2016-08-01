# $Id: Makefile.find_attribute.app 208584 2010-10-19 16:06:55Z satskyse $

SRC = find_attribute
APP = find_attribute
LIB = xmlwrapp
LIBS = $(ORIG_LIBS) $(LIBXML_LIBS) $(LIBXSLT_LIBS)

CPPFLAGS= $(ORIG_CPPFLAGS) $(LIBXML_INCLUDE) $(LIBXSLT_INCLUDE)

WATCHERS = satskyse

