# $Id: Makefile.test_namespace.app 196360 2010-07-01 21:26:41Z satskyse $

SRC = test_namespace
APP = test_namespace
LIB = xmlwrapp test_boost xncbi
LIBS = $(ORIG_LIBS) $(LIBXML_LIBS) $(LIBXSLT_LIBS)


CPPFLAGS = $(ORIG_CPPFLAGS) $(BOOST_INCLUDE)

CHECK_COPY = data

CHECK_CMD = test_namespace

WATCHERS = satskyse

