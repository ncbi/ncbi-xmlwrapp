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
 * $Id: test_libxml2_doc.cpp 435003 2014-05-13 15:08:30Z ucko $
 */


#include <ncbi_pch.hpp>
#include <corelib/test_boost.hpp>
#include <ncbiconf.h>

#include <misc/xmlwrapp/libxml2_xmlwrapp.hpp>
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


// A helper function which compares a content of two files
bool  compareFiles( const std::string &     fileName1,
                    const std::string &     fileName2 )
{
    ifstream       ifs1( fileName1.c_str() ), ifs2( fileName2.c_str() );

    return NcbiStreamCompare(ifs1, ifs2);
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


//
// This test checks xml::libxml2_document iteration
//

void dump_node_type( ostream &          stream,
                     const xml::node &  n )
{
    switch ( n.get_type() )
    {
        case xml::node::type_element:
            stream << "type_element\n";
            break;

        case xml::node::type_text:
            stream << "type_text\n";
            break;

        case xml::node::type_cdata:
            stream << "type_cdata\n";
            break;

        case xml::node::type_pi:
            stream << "type_pi\n";
            break;

        case xml::node::type_comment:
            stream << "type_comment\n";
            break;

        case xml::node::type_entity:
            stream << "type_entity\n";
            break;

        case xml::node::type_entity_ref:
            stream << "type_entity_ref\n";
            break;

        case xml::node::type_xinclude:
            stream << "type_xinclude\n";
            break;

        case xml::node::type_document:
            stream << "type_document\n";
            break;

        case xml::node::type_document_type:
            stream << "type_document_type\n";
            break;

        case xml::node::type_document_frag:
            stream << "type_document_frag\n";
            break;

        case xml::node::type_notation:
            stream << "type_notation\n";
            break;

        case xml::node::type_dtd:
            stream << "type_dtd\n";
            break;

        case xml::node::type_dtd_element:
            stream << "type_dtd_element\n";
            break;

        case xml::node::type_dtd_attribute:
            stream << "type_dtd_attribute\n";
            break;

        case xml::node::type_dtd_entity:
            stream << "type_dtd_entity\n";
            break;

        case xml::node::type_dtd_namespace:
            stream << "type_dtd_namespace\n";
            break;
    }
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest0 )
{
    string      content = "<?xml version=\"1.0\"?><root/>";
    xmlDoc *    raw_doc = xmlParseMemory( content.c_str(), content.size() );

    if (raw_doc == NULL) {
        BOOST_FAIL( "Libxml2 document test (0) failed - could not create raw document." );
        return;
    }

    try
    {
        xml::libxml2_document       doc( raw_doc, xml::type_not_own );

        BOOST_CHECK( doc.get_raw_doc() == raw_doc );
        BOOST_CHECK( doc.get_root_node().get_name() == string("root") );
        BOOST_CHECK( doc.get_ownership() == xml::type_not_own );
    }
    catch ( std::exception &  Exception )
    {
        BOOST_FAIL( "Libxml2 document test (0) failed. Exception: " + string(Exception.what()) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Libxml2 document test (0) failed - unknown exception." );
    }

    xmlFreeDoc( raw_doc );
    return;
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest01 )
{
    try
    {
        xml::init::substitute_entities( false );

        xml::libxml2_document   doc( "data/01.xml", NULL );
        CNcbiOstrstream         ostr;
        xml::node::iterator     i = doc.begin(), end=doc.end();

        for ( ; i != end; ++i )
        {
            dump_node_type( ostr, *i );
        }
        saveStreamToFile( ostr, "data/01.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/01.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/01.out.real" );
        BOOST_FAIL( "Libxml2 document test (01) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest02 )
{
    try
    {
        xml::libxml2_document   doc;
        CNcbiOstrstream         ostr;

        ostr << doc;
        saveStreamToFile( ostr, "data/02.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/02.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Libxml2 document test (02) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest03 )
{
    try
    {
        xml::libxml2_document   doc( "root" );
        CNcbiOstrstream         ostr;

        ostr << doc;
        saveStreamToFile( ostr, "data/03.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/03.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Libxml2 document test (03) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest04 )
{
    try
    {
        xml::node               n( "root", "pcdata" );
        xml::libxml2_document   doc( n );
        CNcbiOstrstream         ostr;

        ostr << doc;
        saveStreamToFile( ostr, "data/04.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/04.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Libxml2 document test (04) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest05 )
{
    try
    {
        // Malformed document
        std::string                 xmldata( "<a:book> xmlns:a=\"a\""
                                             "<title>title</title>"
                                             "</a:book>" );
        xml::error_messages         msgs;
        xml::libxml2_document       doc( xmldata.c_str(), xmldata.size(), &msgs );
        BOOST_FAIL( "Libxml2 document test (05) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/05.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/05.out.real" );
        BOOST_FAIL( "Libxml2 document test (05) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest06 )
{
    strstream   str;

    str << "<?xml version=\"1.0\"?><root/>";

    try
    {
        xml::error_messages     msgs;
        xml::libxml2_document   doc(str, &msgs,
                                    xml::type_warnings_not_errors );

        BOOST_CHECK( doc.get_version() == string( "1.0" ) );
        BOOST_CHECK( doc.get_root_node().get_name() == string( "root" ) );
        BOOST_CHECK( doc.get_root_node().size() == 0 );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/06.out.real" );
        BOOST_FAIL( "Libxml2 document test (06) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Libxml2 document test (06) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest07 )
{
    try
    {
        xml::libxml2_document       doc( NULL, xml::type_not_own );
        BOOST_FAIL( "Libxml2 document test (07) failed. Exception is expected." );
    }
    catch ( std::exception &  Exception )
    {
        // OK, exception is expected
        return;
    }
    catch (...)
    {
        BOOST_FAIL( "Libxml2 document test (07) failed. "
                    "Standard exception is expected." );
    }
    return;
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest08 )
{
    string      content = "<?xml version=\"1.0\"?><root/>";
    xmlDoc *    raw_doc = xmlParseMemory( content.c_str(), content.size() );

    if (raw_doc == NULL) {
        BOOST_FAIL( "Libxml2 document test (08) failed - "
                    "could not create raw document." );
        return;
    }

    try
    {
        xml::libxml2_document       doc( raw_doc, xml::type_own );

        doc.set_raw_doc( NULL, xml::type_own );
        BOOST_FAIL( "Libxml2 document test (08) failed. Exception is expected." );
    }
    catch ( std::exception &  Exception )
    {
        // OK, exception is expected
        return;
    }
    catch (...)
    {
        BOOST_FAIL( "Libxml2 document test (08) failed. "
                    "Standard exception is expected." );
    }
    return;
}


BOOST_AUTO_TEST_CASE( Libxml2DocumentTest09 )
{
    string      content = "<?xml version=\"1.0\"?><root/>";
    xmlDoc *    raw_doc_1 = xmlParseMemory( content.c_str(), content.size() );
    xmlDoc *    raw_doc_2 = xmlParseMemory( content.c_str(), content.size() );

    if (raw_doc_1 == NULL || raw_doc_2 == NULL) {
        BOOST_FAIL( "Libxml2 document test (09) failed - "
                    "could not create raw documents." );
        return;
    }

    try
    {
        xml::libxml2_document       doc( raw_doc_1, xml::type_own );

        doc.set_raw_doc( raw_doc_2, xml::type_own );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/09.out.real" );
        BOOST_FAIL( "Libxml2 document test (09) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Libxml2 document test (09) failed" );
    }
    return;
}

