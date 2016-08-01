# $Id: Makefile.dtd_validation.app 201169 2010-08-16 21:41:41Z satskyse $

SRC = dtd_validation
APP = dtd_validation
LIB = xmlwrapp
LIBS = $(ORIG_LIBS) $(LIBXML_LIBS) $(LIBXSLT_LIBS)

CPPFLAGS= $(ORIG_CPPFLAGS) $(LIBXML_INCLUDE) $(LIBXSLT_INCLUDE)

WATCHERS = satskyse

