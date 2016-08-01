# $Id: Makefile.xpath_run.app 208337 2010-10-15 18:30:57Z satskyse $

SRC = xpath_run
APP = xpath_run
LIB = xmlwrapp
LIBS = $(ORIG_LIBS) $(LIBXML_LIBS) $(LIBXSLT_LIBS)

CPPFLAGS= $(ORIG_CPPFLAGS) $(LIBXML_INCLUDE) $(LIBXSLT_INCLUDE)

WATCHERS = satskyse

