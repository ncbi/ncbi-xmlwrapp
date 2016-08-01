# $Id: Makefile.test_xslt_ext_elem.app 377654 2012-10-15 14:11:19Z satskyse $

SRC = test_xslt_ext_elem
APP = test_xslt_ext_elem
LIB = xmlwrapp test_boost xncbi
LIBS = $(ORIG_LIBS) $(LIBXML_LIBS) $(LIBXSLT_LIBS)


CPPFLAGS = $(ORIG_CPPFLAGS) $(BOOST_INCLUDE)

# CHECK_COPY = data

CHECK_CMD = test_xslt_ext_elem

WATCHERS = satskyse

