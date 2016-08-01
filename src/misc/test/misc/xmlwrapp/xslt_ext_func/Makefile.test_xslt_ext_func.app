# $Id: Makefile.test_xslt_ext_func.app 377251 2012-10-10 15:55:27Z satskyse $

SRC = test_xslt_ext_func
APP = test_xslt_ext_func
LIB = xmlwrapp test_boost xncbi
LIBS = $(ORIG_LIBS) $(LIBXML_LIBS) $(LIBXSLT_LIBS)


CPPFLAGS = $(ORIG_CPPFLAGS) $(BOOST_INCLUDE)

# CHECK_COPY = data

CHECK_CMD = test_xslt_ext_func

WATCHERS = satskyse

