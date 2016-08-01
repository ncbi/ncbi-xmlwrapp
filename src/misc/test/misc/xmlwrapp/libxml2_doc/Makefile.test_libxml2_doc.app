# $Id: Makefile.test_libxml2_doc.app 338900 2011-09-22 21:09:36Z satskyse $

SRC = test_libxml2_doc
APP = test_libxml2_doc
LIB = xmlwrapp test_boost xncbi
LIBS = $(ORIG_LIBS) $(LIBXML_LIBS) $(LIBXSLT_LIBS)


CPPFLAGS = $(ORIG_CPPFLAGS) $(BOOST_INCLUDE) $(LIBXML_INCLUDE)

CHECK_COPY = data

CHECK_CMD = test_libxml2_doc

WATCHERS = satskyse
