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
 * $Id: test_document.cpp 487761 2015-12-21 19:43:34Z satskyse $
 * NOTE: This file was modified from its original version 0.6.0
 *       to fit the NCBI C++ Toolkit build framework and
 *       API and functionality requirements.
 */


#include <ncbi_pch.hpp>
#include <corelib/test_boost.hpp>
#include <ncbiconf.h>

#include <misc/xmlwrapp/xmlwrapp.hpp>
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

int space_counter = 0;
void  dump_node( ostream &       stream,
                 const xml::node &  n )
{
    if ( n.is_text() )
    {
        return;
    }

    for ( int space=0; space < space_counter; ++space )
    {
        stream << " ";
    }
    stream << n.get_name() << std::endl;

    xml::node::const_iterator   i( n.begin() ), end( n.end() );
    for ( ; i != end; ++i )
    {
        space_counter += 4;
        dump_node( stream, *i );
        space_counter -= 4;
    }
}


//
// This test checks xml::document iteration
//

void dump_node_type( ostream &       stream,
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


//
// This test checks xml::document iteration
//

BOOST_AUTO_TEST_CASE( DocumentTest01 )
{
    try
    {
        xml::init::substitute_entities( false );

        xml::document       doc( "data/01.xml", NULL );
        CNcbiOstrstream     ostr;
        xml::node::iterator i = doc.begin(), end=doc.end();

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
        BOOST_FAIL( "Document test (01) failed" );
    }
}


//
// This test checks xml::document default constructor
//

BOOST_AUTO_TEST_CASE( DocumentTest02 )
{
    try
    {
        xml::document       doc;
        CNcbiOstrstream     ostr;

        ostr << doc;
        saveStreamToFile( ostr, "data/02.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/02.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (02) failed" );
    }
}



//
// This test checks xml::document constructor that takes the name of the root
// node.
//

BOOST_AUTO_TEST_CASE( DocumentTest03 )
{
    try
    {
        xml::document       doc( "root" );
        CNcbiOstrstream     ostr;

        ostr << doc;
        saveStreamToFile( ostr, "data/03.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/03.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (03) failed" );
    }
}


//
// This test checks xml::document constructor that takes a node
//

BOOST_AUTO_TEST_CASE( DocumentTest04 )
{
    try
    {
        xml::node           n( "root", "pcdata" );
        xml::document       doc( n );
        CNcbiOstrstream     ostr;

        ostr << doc;
        saveStreamToFile( ostr, "data/04.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/04.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (04) failed" );
    }
}


//
// This test checks xml::document copy constructor
//

BOOST_AUTO_TEST_CASE( DocumentTest05 )
{
    try
    {
        xml::node           n( "root", "pcdata" );
        xml::document       doc( n );
        xml::document       doc_copy( doc );
        CNcbiOstrstream     ostr;

        ostr << doc_copy;
        saveStreamToFile( ostr, "data/05.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (05) failed" );
    }
}


//
// This test checks xml::docment assignment operator
//

BOOST_AUTO_TEST_CASE( DocumentTest06 )
{
    try
    {
        xml::node           n( "root", "pcdata" );
        xml::document       doc( n );
        xml::document       doc_copy;
        CNcbiOstrstream     ostr;

        doc_copy = doc;
        ostr << doc_copy;
        saveStreamToFile( ostr, "data/06.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/06.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (06) failed" );
    }
}


//
// This test checks xml::docment::get_root_node
//

BOOST_AUTO_TEST_CASE( DocumentTest07 )
{
    try
    {
        xml::node           n( "root", "pcdata" );
        xml::document       doc( n );
        CNcbiOstrstream     ostr;

        ostr << doc.get_root_node();
        saveStreamToFile( ostr, "data/07.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/07.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (07) failed" );
    }
}


//
// This test checks xml::docment::set_root_node()
//

BOOST_AUTO_TEST_CASE( DocumentTest08 )
{
    try
    {
        xml::node           n( "root", "pcdata" );
        xml::document       doc;
        CNcbiOstrstream     ostr;

        ostr << doc;
        doc.set_root_node( n );
        ostr << doc;

        saveStreamToFile( ostr, "data/08.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/08.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (08) failed" );
    }
}


//
// This test checks xml::docment::get_version()
//

BOOST_AUTO_TEST_CASE( DocumentTest09 )
{
    try
    {
        xml::document       doc( "data/09.xml", NULL );
        CNcbiOstrstream     ostr;

        ostr << doc.get_version() << "\n";

        saveStreamToFile( ostr, "data/09.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/09.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/09.out.real" );
        BOOST_FAIL( "Document test (09) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (09) failed" );
    }
}


//
// This test checks xml::docment::set_version()
//

BOOST_AUTO_TEST_CASE( DocumentTest10 )
{
    try
    {
        xml::document       doc( "data/10.xml", NULL );
        CNcbiOstrstream     ostr;

        doc.set_version( "1.1" );
        ostr << doc;

        saveStreamToFile( ostr, "data/10.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/10.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/10.out.real" );
        BOOST_FAIL( "Document test (10) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (10) failed" );
    }
}


//
// This test checks xml::docment::get_encoding()
//

BOOST_AUTO_TEST_CASE( DocumentTest11 )
{
    try
    {
        xml::document       doc( "data/11.xml", NULL );
        CNcbiOstrstream     ostr;

        ostr << doc.get_encoding() << "\n";

        saveStreamToFile( ostr, "data/11.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/11.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/11.out.real" );
        BOOST_FAIL( "Document test (11) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (11) failed" );
    }
}


//
// This test checks xml::docment::set_version()
//

BOOST_AUTO_TEST_CASE( DocumentTest12 )
{
    try
    {
        xml::document       doc( "data/12.xml", NULL );
        CNcbiOstrstream     ostr;

        doc.set_encoding( "UTF-8" );
        ostr << doc;

        saveStreamToFile( ostr, "data/12.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/12.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (12) failed" );
    }
}


//
// This test checks xml::docment::get_is_standalone() and set_is_standalone()
//

BOOST_AUTO_TEST_CASE( DocumentTest13a )
{
    try
    {
        xml::document       doc( "data/13a.xml", NULL );
        CNcbiOstrstream     ostr;
        bool                sa = doc.get_is_standalone();

        ostr << sa << "\n";

        doc.set_is_standalone( !sa );
        ostr << doc;

        saveStreamToFile( ostr, "data/13a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/13a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/13a.out.real" );
        BOOST_FAIL( "Document test (13a) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (13a) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest13b )
{
    try
    {
        xml::document       doc( "data/13b.xml", NULL );
        CNcbiOstrstream     ostr;
        bool                sa = doc.get_is_standalone();

        ostr << sa << "\n";

        doc.set_is_standalone( !sa );
        ostr << doc;

        saveStreamToFile( ostr, "data/13b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/13b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/13b.out.real" );
        BOOST_FAIL( "Document test (13b) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (13b) failed" );
    }
}


//
// This test checks xml::docment::process_xinclude()
//

BOOST_AUTO_TEST_CASE( DocumentTest14 )
{
    try
    {
        xml::document       doc( "data/14.xml", NULL );
        CNcbiOstrstream     ostr;

        ostr << doc.process_xinclude() << "\n";
        ostr << doc;

        saveStreamToFile( ostr, "data/14.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/14.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/14.out.real" );
        BOOST_FAIL( "Document test (14) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (14) failed" );
    }
}


//
// This test checks xml::docment::save_to_file()
//

BOOST_AUTO_TEST_CASE( DocumentTest15 )
{
    try
    {
        xml::document       doc( "root" );

        doc.get_root_node().push_back( xml::node( "child" ) );
        doc.save_to_file( "data/15.out.real", 0 );

        BOOST_CHECK( compareFiles( "data/15.out.real", "data/15.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15.out.real" );
        BOOST_FAIL( "Document test (15) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (15) failed" );
    }
}


//
// This test checks xml::docment::size()
//

BOOST_AUTO_TEST_CASE( DocumentTest16 )
{
    try
    {
        CNcbiOstrstream     ostr;
        xml::document       doc_01( "root" );

        ostr << doc_01.size() << "\n";
        doc_01.push_back( xml::node( xml::node::comment( "This is a comment" ) ) );
        ostr << doc_01.size() << "\n";

        xml::document       doc_02( doc_01 );
        ostr << doc_02.size() << "\n";

        xml::document       doc_03;
        ostr << doc_03.size() << "\n";

        xml::node           n( "root" );
        xml::document       doc_04( n );
        ostr << doc_04.size() << "\n";

        BOOST_CHECK( compareStreamAndFile( ostr, "data/16.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/16.out.real" );
        BOOST_FAIL( "Document test (16) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (16) failed" );
    }
}


//
// This test checks xml::docment::push_back and insert
//

BOOST_AUTO_TEST_CASE( DocumentTest17 )
{
    try
    {
        xml::document       doc( "root" );

        doc.push_back( xml::node( xml::node::comment( " Comment From push_back " ) ) );

        xml::node::iterator n( doc.insert( xml::node( xml::node::comment( "This Will Be Changed" ) ) ) );
        n->set_content(" Comment From insert ");

        n = doc.insert( doc.begin(), xml::node( xml::node::pi( "test" ) ) );
        n->set_content( "one=\"1\"" );

        CNcbiOstrstream ostr;

        ostr << doc;

        saveStreamToFile( ostr, "data/17.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/17.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/17.out.real" );
        BOOST_FAIL( "Document test (17) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (17) failed" );
    }
}


//
// This test checks xml::docment::push_back and insert to make sure they throw
// exceptions
//

BOOST_AUTO_TEST_CASE( DocumentTest18a )
{
    try
    {
        xml::document       doc( "root" );

        doc.push_back( xml::node( "noway" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/18a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/18a.out" ) );
        return;
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (18a) failed" );
    }
    BOOST_FAIL( "Document test (18a) failed" );
}


BOOST_AUTO_TEST_CASE( DocumentTest18b )
{
    try
    {
        xml::document       doc( "root" );

        doc.insert( xml::node( "noway" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/18b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/18b.out" ) );
        return;
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (18b) failed" );
    }
    BOOST_FAIL( "Document test (18b) failed" );
}


BOOST_AUTO_TEST_CASE( DocumentTest18c )
{
    try
    {
        xml::document       doc( "root" );

        doc.insert( doc.end(), xml::node( "noway" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/18c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/18c.out" ) );
        return;
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (18c) failed" );
    }
    BOOST_FAIL( "Document test (18c) failed" );
}


//
// This test checks xml::docment::replace()
//

BOOST_AUTO_TEST_CASE( DocumentTest19 )
{
    try
    {
        xml::document       doc( "root" );
        CNcbiOstrstream     ostr;
        xml::node::iterator n( doc.insert( xml::node( xml::node::comment( " To Be Replaced " ) ) ) );

        doc.replace( n, xml::node( xml::node::comment( " This is the replacement comment " ) ) );

        ostr << doc;

        saveStreamToFile( ostr, "data/19.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/19.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/19.out.real" );
        BOOST_FAIL( "Document test (19) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (19) failed" );
    }
}



//
// This test checks xml::docment::replace() to make sure it throws an
// exception.
//

BOOST_AUTO_TEST_CASE( DocumentTest20a )
{
    try
    {
        xml::document       doc( "root" );
        xml::node::iterator n( doc.insert( xml::node( xml::node::comment( " To Be Replaced " ) ) ) );

        doc.replace( n, xml::node( "noway" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/20a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/20.out" ) );
        return;
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (20a) failed" );
    }
    BOOST_FAIL( "Document test (20a) failed" );
}


BOOST_AUTO_TEST_CASE( DocumentTest20b )
{
    try
    {
        xml::document       doc( "root" );
        xml::node::iterator n( doc.insert( xml::node( xml::node::comment( " To Be Replaced " ) ) ) );

        doc.replace( doc.begin(), xml::node( xml::node::comment( " no way " ) ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/20b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/20.out" ) );
        return;
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (20b) failed" );
    }
    BOOST_FAIL( "Document test (20b) failed" );
}



//
// This test checks xml::docment::erase()
//

BOOST_AUTO_TEST_CASE( DocumentTest21 )
{
    try
    {
        xml::document       doc( "root" );
        CNcbiOstrstream     ostr;

        doc.push_back( xml::node( xml::node::comment( " Comment from push_back " ) ) );

        xml::node::iterator n( doc.insert( xml::node( xml::node::comment( " You should not see me " ) ) ) );
        doc.erase( n );

        ostr << doc;

        saveStreamToFile( ostr, "data/21.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/21.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/21.out.real" );
        BOOST_FAIL( "Document test (21) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (21) failed" );
    }
}



//
// This test checks xml::docment::erase() to make sure it throws an exception
//

BOOST_AUTO_TEST_CASE( DocumentTest22 )
{
    try
    {
        xml::document       doc( "root" );

        doc.push_back( xml::node( xml::node::comment( " Comment from push_back " ) ) );
        doc.erase( doc.begin(), doc.end() );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/22.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/22.out" ) );
        return;
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (22) failed" );
    }
    BOOST_FAIL( "Document test (22) failed" );
}



//
// Document output tests
//

BOOST_AUTO_TEST_CASE( DocumentTest23 )
{
    std::string         s;
    try
    {
        xml::document       doc( "root" );
        std::ofstream       out( "data/23.out.real" );

        // string
        doc.get_root_node().push_back( xml::node( "child" ) );
        doc.save_to_string( s, xml::save_op_default );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/23.out.real", "data/23.out" ) );

        // stream
        std::ofstream       out2( "data/23a.out.real" );
        doc.save_to_stream( out2, xml::save_op_default );
        out2.close();

        BOOST_CHECK( compareFiles( "data/23a.out.real", "data/23.out" ) );

        // file
        doc.save_to_file( "data/23b.out.real", xml::save_op_default );
        BOOST_CHECK( compareFiles( "data/23b.out.real", "data/23.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/23.out.real" );
        BOOST_FAIL( "Document test (23) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (23) failed" );
    }
}



BOOST_AUTO_TEST_CASE( DocumentTest24 )
{
    std::string         s;
    try
    {
        xml::document       doc( "root" );
        std::ofstream       out( "data/24.out.real" );

        // string
        doc.get_root_node().push_back( xml::node( "child" ) );
        doc.save_to_string( s, xml::save_op_no_decl );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/24.out.real", "data/24.out" ) );

        // stream
        std::ofstream       out2( "data/24a.out.real" );
        doc.save_to_stream( out2, xml::save_op_no_decl );
        out2.close();

        BOOST_CHECK( compareFiles( "data/24a.out.real", "data/24.out" ) );

        // file
        doc.save_to_file( "data/24b.out.real", xml::save_op_no_decl );
        BOOST_CHECK( compareFiles( "data/24b.out.real", "data/24.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/24.out.real" );
        BOOST_FAIL( "Document test (24) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (24) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest25 )
{
    std::string         s;
    try
    {
        xml::document       doc( "root" );
        std::ofstream       out( "data/25.out.real" );

        // string
        doc.get_root_node().push_back( xml::node( "child" ) );
        doc.save_to_string( s, xml::save_op_no_format );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/25.out.real", "data/25.out" ) );

        // stream
        std::ofstream       out2( "data/25a.out.real" );
        doc.save_to_stream( out2, xml::save_op_no_format );
        out2.close();

        BOOST_CHECK( compareFiles( "data/25a.out.real", "data/25.out" ) );

        // file
        doc.save_to_file( "data/25b.out.real", xml::save_op_no_format );
        BOOST_CHECK( compareFiles( "data/25b.out.real", "data/25.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/25.out.real" );
        BOOST_FAIL( "Document test (25) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (25) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest26 )
{
    try
    {
        xml::error_messages msgs;
        xml::document       doc( "data/26.xml", &msgs );
        CNcbiOstrstream     ostr;

        dump_node( ostr, doc.get_root_node() );
        saveStreamToFile( ostr, "data/26.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/26.out" ) );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/26.out.real" );
        BOOST_FAIL( "Document test (26) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest27 )
{
    try
    {
        xml::document       doc( "data/27.xml",
                                 (xml::error_messages*) NULL );
        CNcbiOstrstream     ostr;

        dump_node( ostr, doc.get_root_node() );
        saveStreamToFile( ostr, "data/27.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/27.out" ) );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/27.out.real" );
        BOOST_FAIL( "Document test (27) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest28 )
{
    try
    {
        xml::document       doc( "data/28.xml", NULL );
        BOOST_FAIL( "Document test (28) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/28.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/28.out.real" );
        BOOST_FAIL( "Document test (28) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest29 )
{
    try
    {
        xml::error_messages     msgs;
        xml::document           doc( "data/29.xml", &msgs );
        BOOST_FAIL( "Document test (29) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/29.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/29.out.real" );
        BOOST_FAIL( "Document test (29) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest30 )
{
    try
    {
        xml::document       doc( "data/30.xml", (xml::error_messages*) NULL );
        BOOST_FAIL( "Document test (30) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/30.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/30.out.real" );
        BOOST_FAIL( "Document test (30) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest31 )
{
    try
    {
        std::string         xmldata( "<root><a><b><c><d><e><f><g/></f></e></d></c></b></a></root>" );
        xml::document       doc( xmldata.c_str(), xmldata.size(), NULL );
        CNcbiOstrstream     ostr;

        dump_node( ostr, doc.get_root_node() );
        saveStreamToFile( ostr, "data/31.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/31.out" ) );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/31.out.real" );
        BOOST_FAIL( "Document test (31) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest32 )
{
    try
    {
        std::string         xmldata( "<root><a><b><c><d><e><f><g/></f></e></d></c></b></a></root>" );
        xml::error_messages msgs;
        xml::document       doc( xmldata.c_str(), xmldata.size(), &msgs );
        CNcbiOstrstream     ostr;

        dump_node( ostr, doc.get_root_node() );
        saveStreamToFile( ostr, "data/32.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/32.out" ) );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/32.out.real" );
        BOOST_FAIL( "Document test (32) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest33 )
{
    try
    {
        std::string         xmldata( "<root><a><b></c></d></root>" );
        xml::document       doc( xmldata.c_str(), xmldata.size(), NULL );
        BOOST_FAIL( "Tree parser test (33) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/33.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/33.out.real" );
        BOOST_FAIL( "Document test (33) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest34 )
{
    try
    {
        std::string         xmldata( "<root><a><b></c></d></root>" );
        xml::error_messages msgs;
        xml::document       doc( xmldata.c_str(), xmldata.size(), &msgs );
        BOOST_FAIL( "Document test (34) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/34.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/34.out.real" );
        BOOST_FAIL( "Document test (34) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest35 )
{
    try
    {
        // Malformed document
        std::string         xmldata( "<a:book> xmlns:a=\"a\""
                                     "<title>title</title>"
                                     "</a:book>" );
        xml::document       doc( xmldata.c_str(), xmldata.size(), NULL );
        BOOST_FAIL( "Document test (35) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/35.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/35.out.real" );
        BOOST_FAIL( "Document test (35) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest36 )
{
    try
    {
        // Malformed document
        std::string         xmldata( "<a:book> xmlns:a=\"a\""
                                     "<title>title</title>"
                                     "</a:book>" );
        xml::error_messages msgs;
        xml::document       doc( xmldata.c_str(), xmldata.size(), &msgs );
        BOOST_FAIL( "Document test (36) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/36.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/36.out.real" );
        BOOST_FAIL( "Document test (36) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest37 )
{
    try
    {
        // new constructor
        xml::error_messages  msgs;
        xml::document        doc( "data/37.xml", &msgs,
                                  xml::type_warnings_not_errors );
        xml::document        doc2( "data/37.xml",
                                   (xml::error_messages*) NULL,
                                   xml::type_warnings_not_errors );

        BOOST_CHECK( msgs.get_messages().size() == 1 );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/37.out.real" );
        BOOST_FAIL( "Document test (37) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected.\n";
        saveStreamToFile( ostr, "data/37.out.real" );
        BOOST_FAIL( "Document test (37) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest38 )
{
    try
    {
        // new constructor
        xml::error_messages  msgs;
        xml::document        doc( "data/38.xml", &msgs,
                                  xml::type_warnings_are_errors );
        BOOST_FAIL( "Document test (38) failed. Exception is expected." );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/38.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/38.out.real" );
        BOOST_FAIL( "Document test (38) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest39 )
{
    try
    {
        // new constructor
        xml::document       doc( "data/39.xml",
                                 (xml::error_messages*)(NULL),
                                 xml::type_warnings_are_errors );
        BOOST_FAIL( "Document test (39) failed. Exception is expected." );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/39.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/39.out.real" );
        BOOST_FAIL( "Document test (39) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest40 )
{
    try
    {
        // document with warnings
        std::string         xmldata( "<!DOCTYPE root SYSTEM \"13.dtd\" ["
                                     "<!ENTITY myname \"Peter Jones\">"
                                     "]>"
                                     "<root>&myname;</root>" );
        xml::document       doc( xmldata.c_str(), xmldata.size(), NULL,
                                 xml::type_warnings_are_errors );
        BOOST_FAIL( "Document test (40) failed" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/40.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/40.out.real" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest41 )
{
    try
    {
        // document with warnings
        std::string         xmldata( "<!DOCTYPE root SYSTEM \"13.dtd\" ["
                                     "<!ENTITY myname \"Peter Jones\">"
                                     "]>"
                                     "<root>&myname;</root>" );
        xml::error_messages msgs;
        xml::document       doc( xmldata.c_str(), xmldata.size(),
                                 &msgs, xml::type_warnings_are_errors );
        BOOST_FAIL( "Document test (41) failed" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/41.out.real" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/41.out.real" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest42 )
{
    try
    {
        // document with warnings
        std::string         xmldata( "<!DOCTYPE root SYSTEM \"14.dtd\" ["
                                     "<!ENTITY myname \"Peter Jones\">"
                                     "]>"
                                     "<root>&myname;</root>" );
        xml::document       doc( xmldata.c_str(), xmldata.size(), NULL,
                                 xml::type_warnings_not_errors );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/42.out.real" );
        BOOST_FAIL( "Document test (42) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is not expected.\n";
        saveStreamToFile( ostr, "data/42.out.real" );
        BOOST_FAIL( "Document test (42) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest43 )
{
    try
    {
        // document with warnings
        std::string         xmldata( "<!DOCTYPE root SYSTEM \"14.dtd\" ["
                                     "<!ENTITY myname \"Peter Jones\">"
                                     "]>"
                                     "<root>&myname;</root>" );
        xml::error_messages msgs;
        xml::document       doc( xmldata.c_str(), xmldata.size(),
                                 &msgs, xml::type_warnings_not_errors );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/43.out.real" );
        BOOST_FAIL( "Document test (43) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is not expected.\n";
        saveStreamToFile( ostr, "data/43.out.real" );
        BOOST_FAIL( "Document test (43) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest44 )
{
    strstream   str;

    str << "<?xml version=\"1.0\"?><root/>";

    try
    {
        xml::error_messages     msgs;
        xml::document           doc(str, &msgs,
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
        saveStreamToFile( ostr, "data/44.out.real" );
        BOOST_FAIL( "Document test (44) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (44) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest44a )
{
    strstream   str;

    str << "<?xml version=\"1.1\"?><root/>";

    try
    {
        xml::error_messages     msgs;
        xml::document           doc(str, &msgs,
                                    xml::type_warnings_not_errors );

        BOOST_CHECK( msgs.get_messages().size() == 1 );
        BOOST_CHECK( msgs.get_messages().begin()->get_message() == "Unsupported version '1.1'" );
    }
    catch ( std::exception &  Exception )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - exception is not expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/44a.out.real" );
        BOOST_FAIL( "Document test (44a) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (44a) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest44b )
{
    strstream   str;

    str << "<?xml version=\"1.1\"?><root/>";
    xml::error_messages     msgs;

    try
    {
        xml::document           doc(str, &msgs,
                                    xml::type_warnings_are_errors );

        BOOST_FAIL( "Document test (44b) failed. Exception is expected." );
    }
    catch ( std::exception &  Exception )
    {
        BOOST_CHECK( msgs.get_messages().size() == 1 );
        BOOST_CHECK( msgs.get_messages().begin()->get_message() == "Unsupported version '1.1'" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (44b) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest44c )
{
    strstream   str;

    str << "<?xml version=\"1.0\"?><root><error>";
    xml::error_messages     msgs;

    try
    {
        xml::document           doc(str, &msgs,
                                    xml::type_warnings_not_errors );

        BOOST_FAIL( "Document test (44c) failed. Exception is expected." );
    }
    catch ( std::exception &  Exception )
    {
        BOOST_CHECK( msgs.get_messages().size() == 1 );
        BOOST_CHECK( msgs.get_messages().begin()->get_message() == "Extra content at the end of the document" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (44c) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest45 )
{
    xml::error_messages     msgs;
    try
    {
        xml::document           doc( "Non-existing-file", &msgs,
                                     xml::type_warnings_not_errors );

        BOOST_FAIL( "Document test (45) failed. Exception is expected." );
    }
    catch ( std::exception &  Exception )
    {
        BOOST_CHECK( msgs.get_messages().size() == 1 );
        BOOST_CHECK( msgs.get_messages().begin()->get_message() == "Cannot open file" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (45) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest46 )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/46.xml", NULL );
        std::ofstream       out( "data/46.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::c14n_1_0,
                                         xml::keep_comments,
                                         xml::without_formatting,
                                         xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/46.out.real", "data/46.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/46.out.real" );
        BOOST_FAIL( "Document test (46 - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (46 - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest46a )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/46.xml", NULL );
        std::ofstream       out( "data/46a.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::c14n_1_0,
                                         xml::keep_comments,
                                         xml::without_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/46a.out.real", "data/46a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/46a.out.real" );
        BOOST_FAIL( "Document test (46a - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (46a - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest46b )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/46.xml", NULL );
        std::ofstream       out( "data/46b.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::c14n_1_0,
                                         xml::keep_comments,
                                         xml::with_formatting,
                                         xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/46b.out.real", "data/46b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/46b.out.real" );
        BOOST_FAIL( "Document test (46b - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (46b - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest46c )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/46.xml", NULL );
        std::ofstream       out( "data/46c.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::c14n_1_0,
                                         xml::keep_comments,
                                         xml::with_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/46c.out.real", "data/46c.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/46c.out.real" );
        BOOST_FAIL( "Document test (46c - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (46c - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest46d )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/46.xml", NULL );
        std::ofstream       out( "data/46d.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::c14n_1_0,
                                         xml::strip_comments,
                                         xml::with_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/46d.out.real", "data/46d.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/46d.out.real" );
        BOOST_FAIL( "Document test (46d - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (46d - save_to_string_canonical) failed" );
    }
}


// This is not really a document test. It checks the xml::init class however
// it seems unreasonable to create another subdir for a single test
BOOST_AUTO_TEST_CASE( DocumentTest47 )
{
    // The default value must be false
    BOOST_CHECK( xml::init::get_remove_whitespace() == false );

    xml::init::remove_whitespace( true );
    BOOST_CHECK( xml::init::get_remove_whitespace() == true );

    xml::init::remove_whitespace( false );
    BOOST_CHECK( xml::init::get_remove_whitespace() == false );
}



BOOST_AUTO_TEST_CASE( DocumentTest48 )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns,
                                         xml::keep_comments,
                                         xml::without_formatting,
                                         xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48.out.real", "data/48.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48.out.real" );
        BOOST_FAIL( "Document test (48 - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48 - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48a )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48a.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns_no_decl,
                                         xml::keep_comments,
                                         xml::without_formatting,
                                         xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48a.out.real", "data/48a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48a.out.real" );
        BOOST_FAIL( "Document test (48a - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48a - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48b )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48b.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns,
                                         xml::keep_comments,
                                         xml::without_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48b.out.real", "data/48b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48b.out.real" );
        BOOST_FAIL( "Document test (48b - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48b - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48c )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48c.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns_no_decl,
                                         xml::keep_comments,
                                         xml::without_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48c.out.real", "data/48c.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48c.out.real" );
        BOOST_FAIL( "Document test (48c - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48c - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48d )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48d.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns,
                                         xml::keep_comments,
                                         xml::with_formatting,
                                         xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48d.out.real", "data/48d.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48d.out.real" );
        BOOST_FAIL( "Document test (48d - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48d - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48e )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48e.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns_no_decl,
                                         xml::keep_comments,
                                         xml::with_formatting,
                                         xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48e.out.real", "data/48e.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48e.out.real" );
        BOOST_FAIL( "Document test (48e - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48e - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48f )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48f.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns,
                                         xml::keep_comments,
                                         xml::with_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48f.out.real", "data/48f.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48f.out.real" );
        BOOST_FAIL( "Document test (48f - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48f - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48g )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48g.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns_no_decl,
                                         xml::keep_comments,
                                         xml::with_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48g.out.real", "data/48g.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48g.out.real" );
        BOOST_FAIL( "Document test (48g - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48g - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48h )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48h.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns,
                                         xml::strip_comments,
                                         xml::with_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48h.out.real", "data/48h.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48h.out.real" );
        BOOST_FAIL( "Document test (48h - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48h - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48i )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48.xml", NULL );
        std::ofstream       out( "data/48i.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns_no_decl,
                                         xml::strip_comments,
                                         xml::with_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48i.out.real", "data/48i.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48i.out.real" );
        BOOST_FAIL( "Document test (48i - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48i - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest48j )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/48j.xml", NULL );
        std::ofstream       out( "data/48j.out.real" );

        std::string         s;
        doc.save_to_string_canonical( s, xml::sort_attr_and_ns_no_decl,
                                         xml::strip_comments,
                                         xml::with_formatting,
                                         xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/48j.out.real", "data/48j.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48j.out.real" );
        BOOST_FAIL( "Document test (48j - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test (48j - save_to_string_canonical) failed" );
    }
}


std::string generateDocumentAsString( size_t  siblings,
                                      size_t  nesting )
{
    std::string     doc = "<?xml version=\"1.0\"?>";
    doc += "<!DOCTYPE eSummaryResult PUBLIC \"-//NLM//DTD esummary seqannot 20130530//EN\" \"http://eutils.ncbi.nlm.nih.gov/eutils/dtd/20130530/esummary_seqannot.dtd\">";
    doc += "<root>";

    for ( size_t  k = 0; k < siblings; ++k )
    {
        char            buf[ 32 ];

        sprintf( buf, "%ld", k );
        std::string     sibling = "<sibling uid" + std::string( buf ) + "=\"" + std::string( buf ) + "\">";

        std::string     nested;
        for ( size_t  j = 1; j < nesting; ++j )
        {
            nested = "<nested>" + nested + "</nested>";
        }
        sibling += nested;

        sibling += "<one>some data" + std::string( buf ) + "</one>";
        sibling += "<two>other data" + std::string( buf ) + "</two>";
        sibling += "<three><![CDATA[ generated cdata " + std::string( buf ) + "]]></three>";
        sibling += "<four>2015/10/23</four>";
        sibling += "<five>2015/12/15</five>";
        sibling += "</sibling>";
        doc += sibling;
    }

    doc += "</root>";
    return doc;
}

BOOST_AUTO_TEST_CASE( DocumentTest49 )
{
    std::string         s = generateDocumentAsString( 300000, 2 );
    try
    {
        xml::document       doc( s.c_str(), s.size(), NULL );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/49.out.real" );
        BOOST_FAIL( "Document test "
                    "(49 - extra large number of siblings) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test "
                    "(49 - extra large number of siblings) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest50 )
{
    std::string         s = generateDocumentAsString( 2, 500 );
    try
    {
        xml::document       doc( s.c_str(), s.size(), NULL );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/50.out.real" );
        BOOST_FAIL( "Document test "
                    "(50 - extra large nesting) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Document test "
                    "(50 - extra large nesting) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest51 )
{
    xml::error_messages     msgs;
    try
    {
        xml::document       doc( "data/28.xml", &msgs );
        BOOST_FAIL( "Document test (51) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/51.out.real" );

        BOOST_CHECK( msgs.get_messages().size() >= 1 );

        const xml::error_messages::error_messages_type &
                                            messages = msgs.get_messages();
        const xml::error_message            m = * messages.begin();

        BOOST_CHECK( m.get_line() == 1 );
        BOOST_CHECK( m.get_filename() == "data/28.xml" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/51.out.real" );
        BOOST_FAIL( "Document test (51) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest52 )
{
    xml::error_messages     msgs;
    std::string             s = "<?xml version=\"1.0\"?>\n"
                                "<root>\n"
                                "</notaroot>";

    try
    {
        xml::document       doc( s.c_str(), s.size(), &msgs );
        BOOST_FAIL( "Document test (52) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/52.out.real" );

        BOOST_CHECK( msgs.get_messages().size() >= 1 );
        const xml::error_messages::error_messages_type &
                                            messages = msgs.get_messages();
        const xml::error_message            m = * messages.begin();

        BOOST_CHECK( m.get_line() == 3 );
        BOOST_CHECK( m.get_filename() == "" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/52.out.real" );
        BOOST_FAIL( "Document test (52) failed" );
    }
}


BOOST_AUTO_TEST_CASE( DocumentTest53 )
{
    xml::error_messages     msgs;
    std::string             s = "<?xml version=\"1.0\"?>\n"
                                "<root a=\">\n"
                                "</root>";

    try
    {
        xml::document       doc( s.c_str(), s.size(), &msgs );
        BOOST_FAIL( "Document test (53) failed" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        CNcbiOstrstream ostr;
        ostr << "Test passed - exception is expected. \n"
             << "Message: " << Exception.what() << "\n";
        saveStreamToFile( ostr, "data/53.out.real" );

        BOOST_CHECK( msgs.get_messages().size() >= 1 );
        const xml::error_messages::error_messages_type &
                                            messages = msgs.get_messages();
        const xml::error_message            m = * messages.begin();

        BOOST_CHECK( m.get_line() == 3 );
        BOOST_CHECK( m.get_filename() == "" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Test failed - std::exception is expected.\n";
        saveStreamToFile( ostr, "data/53.out.real" );
        BOOST_FAIL( "Document test (53) failed" );
    }
}


