# $Id: Makefile.xml_schema.app 196361 2010-07-01 21:29:50Z satskyse $

SRC = xml_schema
APP = xml_schema
LIB = xmlwrapp
LIBS = $(ORIG_LIBS) $(LIBXML_LIBS) $(LIBXSLT_LIBS)

CPPFLAGS= $(ORIG_CPPFLAGS) $(LIBXML_INCLUDE) $(LIBXSLT_INCLUDE)

WATCHERS = satskyse

