/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * $Id: test_xslt.cpp 487956 2015-12-23 14:30:44Z satskyse $
 * NOTE: This file was modified from its original version 0.6.0
 *       to fit the NCBI C++ Toolkit build framework and
 *       API and functionality requirements.
 */


#include <ncbi_pch.hpp>
#include <corelib/test_boost.hpp>
#include <ncbiconf.h>

#include <misc/xmlwrapp/xsltwrapp.hpp>
#include <iostream>
#include <cstdlib>



USING_NCBI_SCOPE;


// A helper function which compares a content of a stream in memory
// with a file content on a disk
bool  compareStreamAndFile( CNcbiOstrstream &       stream,
                            const std::string &     fileName )
{
    ifstream       ifs( fileName.c_str() );
    string         fileContent;

    NcbiStreamToString(&fileContent, ifs);
    ifs.close();

    return string(CNcbiOstrstreamToString(stream)) == fileContent;
}


// A helper function to save the stream content into a file
void  saveStreamToFile( CNcbiOstrstream &       stream,
                        const std::string &     fileName )
{
    std::ofstream   f( fileName.c_str() );

    f << string(CNcbiOstrstreamToString(stream));
    f.close();

    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest01a )
{
    try
    {
        xslt::stylesheet    style( "data/01a.xsl" );

        BOOST_FAIL( "XSL construction test (01a) failed" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/01a.out.real" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest01b )
{
    try
    {
        xslt::stylesheet    style( "data/01b.xsl" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/01b.out.real" );
        BOOST_FAIL( "XSL construction test (01b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest01c )
{
    try
    {
        xslt::stylesheet    style( "data/01c.xsl" );

        BOOST_FAIL( "XSL construction test (01c) failed" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/01c.out.real" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest01d )
{
    try
    {
        std::string         content(
                                "<xsl:stylesheet version=\"1.0\" "
                                "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
                                "<xsl:output method=\"html\" indent=\"yes\" encoding=\"us-ascii\"/>"
                                "</xsl:stylesheet>" );
        xslt::stylesheet    style( content.c_str(), content.size() );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/01d.out.real" );
        BOOST_FAIL( "XSL construction from memory test (01d) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest01e )
{
    try
    {
        std::string         content_with_errors(
                                "<xsl:stylesheet version=\"1.0\" "
                                "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
                                "<xsl:output method=\"html\" indent=\"yes\" encoding=\"us-ascii\"/>"
                                "<!-- the following element is wrong on purpose -->"
                                "<xsl:stylesheet>" );
        xslt::stylesheet    style( content_with_errors.c_str(),
                                   content_with_errors.size() );

        BOOST_FAIL( "XSL construction test (01e) failed" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/01e.out.real" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest01f )
{
    try
    {
        std::strstream         content;

        content << "<xsl:stylesheet version=\"1.0\" "
                   "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
                   "<xsl:output method=\"html\" indent=\"yes\" encoding=\"us-ascii\"/>"
                   "</xsl:stylesheet>";
        xslt::stylesheet    style( content );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/01f.out.real" );
        BOOST_FAIL( "XSL construction from stream test (01f) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest01g )
{
    try
    {
        std::strstream         content_with_errors;

        content_with_errors <<  "<xsl:stylesheet version=\"1.0\" "
                                "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
                                "<xsl:output method=\"html\" indent=\"yes\" encoding=\"us-ascii\"/>"
                                "<!-- the following element is wrong on purpose -->"
                                "<xsl:stylesheet>";
        xslt::stylesheet    style( content_with_errors );

        BOOST_FAIL( "XSL construction test (01g) failed" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/01g.out.real" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest04a )
{
    try
    {
        xslt::stylesheet        style( "data/04a.xsl" );
        xml::document           doc( "data/04a.xml", NULL );
        xml::document           result( style.apply( doc ) );
        CNcbiOstrstream         ostr;

        ostr << result;
        saveStreamToFile( ostr, "data/04a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/04a.out" ) );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/04a.out.real" );
        BOOST_FAIL( "XSL construction test (04a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest05a )
{
    try 
    {
        xslt::stylesheet        style( "data/05a.xsl" );
        xml::document           doc( "data/05a.xml", NULL );

        xslt::stylesheet::param_type    params;
        params[ "foo" ] = "'bar'";

        CNcbiOstrstream         ostr;
        xml::document           result( style.apply( doc, params ) );

        ostr << result;
        saveStreamToFile( ostr, "data/05a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05a.out" ) );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/05a.out.real" );
        BOOST_FAIL( "XSL construction test (05a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest06 )
{
    try
    {
        xslt::stylesheet        style( "data/06.xsl" );
        xml::document           doc( "data/06.xml", NULL );
        xml::document           result( style.apply( doc ) );
        CNcbiOstrstream         ostr;

        ostr << result << "\n";
        saveStreamToFile( ostr, "data/06.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/06.out" ) );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/06ex.out.real" );
        BOOST_FAIL( "XSL construction test (06) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest07 )
{
    try
    {
        xslt::stylesheet        style( "data/07.xsl" );
        xml::document           doc( "data/07.xml", NULL );
        xml::document           result( style.apply( doc ) );
        std::string             s;
        CNcbiOstrstream         ostr;


        result.save_to_string( s, xml::save_op_no_decl );
        ostr << s << "\n";
        saveStreamToFile( ostr, "data/07.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/07.out" ) );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/07ex.out.real" );
        BOOST_FAIL( "XSL output treatment flag test (07) failed" );
    }
    return;
}


#ifdef HAVE_LIBEXSLT
BOOST_AUTO_TEST_CASE( XSLTTest08 )
{
    try
    {
        CNcbiOstrstream         ostr;
        std::string             s;
        std::string             content( "<xsl:stylesheet version=\"1.0\" "
                                         "type=\"text/xsl\" "
                                         "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                         "xmlns:str=\"http://exslt.org/strings\" "
                                         "extension-element-prefixes=\"str\"> "
                                         "<xsl:output method=\"text\"/>"
                                         "<xsl:variable name=\"URL_encoded\" select=\"str:encode-uri('a+a', false())\"/> "
                                         "</xsl:stylesheet>" );
        xslt::stylesheet        style( content.c_str(), content.size() );

        std::string             xmldata( "<root>bla</root>" );
        xml::document           doc( xmldata.c_str(), xmldata.size(), NULL );

        xml::document           result( style.apply( doc ) );

        result.save_to_string( s, xml::save_op_no_decl );
        ostr << s << "\n";
        saveStreamToFile( ostr, "data/08.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/08.out" ) );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/08ex.out.real" );
        BOOST_FAIL( "EXSL test (08) failed" );
    }
    return;
}
#endif


#ifndef NCBI_OS_MSWIN
BOOST_AUTO_TEST_CASE( XSLTTest09 )
{
    try
    {
        std::string         xmldata( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                                     "<root name=\"dad\">"
                                         "<kid name=\"kid a\"/>"
                                         "<kid name=\"kid b\"/>"
                                     "</root>" );
        xml::document       doc( xmldata.c_str(), xmldata.size(), NULL );


        std::string         xsltdata( "<xsl:stylesheet encoding=\"UTF-8\" version=\"1.0\" "
                                                       "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
                                      "<xsl:output method=\"text\"/>"
                                      "<xsl:template match=\"/\"><xsl:text>grrr</xsl:text></xsl:template>"
                                      "</xsl:stylesheet>" );
        xslt::stylesheet    style( xsltdata.c_str(), xsltdata.size() );

        xml::document       result( style.apply( doc ) );
        std::string         s;

        result.save_to_string( s );

        BOOST_CHECK( std::string( "grrr" ) == s );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/09ex.out.real" );
        BOOST_FAIL( "XSLT test (09) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest10 )
{
    try
    {
        std::string         xmldata( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                                     "<root name=\"dad\">"
                                         "<kid name=\"kid a\"/>"
                                         "<kid name=\"kid b\"/>"
                                     "</root>" );
        xml::document       doc( xmldata.c_str(), xmldata.size(), NULL );


        std::string         xsltdata( "<xsl:stylesheet encoding=\"UTF-8\" version=\"1.0\" "
                                                       "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
                                      "<xsl:output method=\"xml\"/>"
                                      "<xsl:template match=\"/\"><root/></xsl:template>"
                                      "</xsl:stylesheet>" );
        xslt::stylesheet    style( xsltdata.c_str(), xsltdata.size() );

        xml::document       result( style.apply( doc ) );
        std::string         s;

        result.save_to_string( s );
        BOOST_CHECK( NStr::Replace( s, "\n", "" ) == "<?xml version=\"1.0\"?><root/>" );

        std::string         s1;
        result.save_to_string( s1, xml::save_op_no_decl );
        BOOST_CHECK( NStr::Replace( s1, "\n", "" ) == "<root/>" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/10ex.out.real" );
        BOOST_FAIL( "XSLT test (10) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XSLTTest11 )
{
    try
    {
        std::string         xmldata( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                                     "<root name=\"dad\">"
                                         "<kid name=\"kid a\"/>"
                                         "<kid name=\"kid b\"/>"
                                     "</root>" );
        xml::document       doc( xmldata.c_str(), xmldata.size(), NULL );


        // Note: output method is not specified.
        //       so the default xml should be taken
        std::string         xsltdata( "<xsl:stylesheet encoding=\"UTF-8\" version=\"1.0\" "
                                                       "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
                                      "<xsl:template match=\"/\"><root/></xsl:template>"
                                      "</xsl:stylesheet>" );
        xslt::stylesheet    style( xsltdata.c_str(), xsltdata.size() );

        xml::document       result( style.apply( doc ) );
        std::string         s;

        result.save_to_string( s );
        BOOST_CHECK( NStr::Replace( s, "\n", "" ) == "<?xml version=\"1.0\"?><root/>" );

        std::string         s1;
        result.save_to_string( s1, xml::save_op_no_decl );
        BOOST_CHECK( NStr::Replace( s1, "\n", "" ) == "<root/>" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/11ex.out.real" );
        BOOST_FAIL( "XSLT test (11) failed" );
    }
    return;
}
#endif


xml::document applyResults( void )
{
    std::string         xsltDoc(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<xsl:stylesheet xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
            "version=\"1.0\">"
            "<xsl:output method=\"html\" indent=\"yes\"/>"
            "<xsl:template match=\"/\">"
                "<html><body>asd</body></html>"
            "</xsl:template>"
            "</xsl:stylesheet>" );

    std::string         xmlDoc( "<root/>" );
    xslt::stylesheet    style( xsltDoc.c_str(), xsltDoc.size() );
    xml::document       doc( xmlDoc.c_str(), xmlDoc.size(), NULL );
    xml::document       result = style.apply( doc );

    return result;
}


BOOST_AUTO_TEST_CASE( XSLTCrashTest12 )
{
    xml::document       result = applyResults();
    std::string         asString;

    result.save_to_string( asString );
    BOOST_CHECK( NStr::Replace( asString, "\n", "" ) == "<html><body>asd</body></html>" );
}



xml::document applyResults2( void )
{
    // The difference is the output method
    std::string         xsltDoc(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<xsl:stylesheet xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
            "version=\"1.0\">"
            "<xsl:output method=\"xml\" indent=\"yes\"/>"
            "<xsl:template match=\"/\">"
                "<html><body>asd</body></html>"
            "</xsl:template>"
            "</xsl:stylesheet>" );

    std::string         xmlDoc( "<root/>" );
    xslt::stylesheet    style( xsltDoc.c_str(), xsltDoc.size() );
    xml::document       doc( xmlDoc.c_str(), xmlDoc.size(), NULL );
    xml::document       result = style.apply( doc );

    return result;
}


BOOST_AUTO_TEST_CASE( XSLTCrashTest13 )
{
    xml::document       result = applyResults2();
    std::string         asString;

    result.save_to_string( asString, xml::save_op_no_decl | xml::save_op_no_format );
    BOOST_CHECK( NStr::Replace( asString, "\n", "" ) == "<html><body>asd</body></html>" );
}




BOOST_AUTO_TEST_CASE( XSLTTest14 )
{
    xslt::stylesheet        style( "data/14.xsl" );
    xml::document           doc( "data/14.xml", NULL );
    xml::error_messages     msgs;

    try
    {
        xml::document       result( style.apply( doc, &msgs ) );

        BOOST_FAIL( "XSL error handling test (14) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std::exception is expected
        BOOST_CHECK( msgs.get_messages().size() >= 1 );

        const xml::error_messages::error_messages_type &
                                            messages = msgs.get_messages();
        xml::error_messages::error_messages_type::const_iterator  k = 
                                            messages.begin();
        BOOST_CHECK( k->get_line() == 7 );
        BOOST_CHECK( k->get_filename() == "data/14.xsl" );

        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/14.out.real" );
    }
    catch ( ... )
    {
        // Any other exception is NOT expected
        CNcbiOstrstream ostr;
        ostr << "Test failed - unknown exception.";
        saveStreamToFile( ostr, "data/14.out.real" );
        BOOST_FAIL( "Unknown exception. Test (14) failed" );
    }
}

