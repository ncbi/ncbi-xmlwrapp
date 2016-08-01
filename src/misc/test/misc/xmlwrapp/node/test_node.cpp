/*
   xml::node::
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
 * $Id: test_node.cpp 482375 2015-10-22 16:10:24Z satskyse $
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
#include <string.h>
#include <algorithm>


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
// This test should make sure that we can construct a tree using nodes and
// output the nodes using stream insertion
//

BOOST_AUTO_TEST_CASE( NodeTest01 )
{
    xml::node       root_node( "root" );
    xml::node       one( "count", "one" );
    xml::node       two( "count", "two" );
    xml::node       nested_out( "outter" );
    xml::node       nested_in( "inner" );

    nested_out.push_back( nested_in );
    root_node.push_back( one );
    root_node.push_back( two );
    root_node.push_back( nested_out );

    CNcbiOstrstream ostr;

    ostr << root_node;
    saveStreamToFile( ostr, "data/01.out.real" );
    BOOST_CHECK( compareStreamAndFile( ostr, "data/01.xml" ) );
    return;
}


//
// This test checks xml::node::iterator xml::node::find (const char *name)
//

BOOST_AUTO_TEST_CASE( NodeTest02a )
{
    try
    {
        xml::document       doc( "data/02.xml", NULL );
        xml::node &         root = doc.get_root_node();
        xml::node::iterator i = root.find( "person" );
        CNcbiOstrstream     ostr;

        if ( i != root.end() )
        {
            ostr << *i;
        }
        saveStreamToFile( ostr, "data/02a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/02a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/02a.out.real" );
        BOOST_FAIL( "Node test (02a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest02b )
{
    try
    {
        xml::document               doc( "data/02.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::const_iterator   i = root.find( "person" );
        CNcbiOstrstream             ostr;

        if ( i != root.end() )
        {
            ostr << *i;
        }
        saveStreamToFile( ostr, "data/02b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/02b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/02b.out.real" );
        BOOST_FAIL( "Node test (02b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest02c )
{
    try
    {
        xml::document               doc( "data/02.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::iterator         i = root.find( "person", root.begin() );
        CNcbiOstrstream             ostr;

        while ( i != root.end() )
        {
            ostr << *i;
            i = root.find( "person", ++i );
        }
        saveStreamToFile( ostr, "data/02c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/02c.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/02c.out.real" );
        BOOST_FAIL( "Node test (02c) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest02d )
{
    try
    {
        xml::document               doc( "data/02.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::const_iterator   i = root.find( "person", root.begin() );
        CNcbiOstrstream             ostr;

        while ( i != root.end() )
        {
            ostr << *i;
            i = root.find( "person", ++i );
        }
        saveStreamToFile( ostr, "data/02d.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/02d.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/02d.out.real" );
        BOOST_FAIL( "Node test (02d) failed" );
    }
    return;
}



//
// This test checks xml::node::replace
//

BOOST_AUTO_TEST_CASE( NodeTest03a )
{
    try
    {
        xml::document               doc( "data/03.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::iterator         i = root.find( "to_replace", root.begin() );
        CNcbiOstrstream             ostr;

        if ( i == root.end() )
        {
            throw std::runtime_error( "xml::node::find failed" );
        }

        xml::node                   replacement( "replace" );

        replacement.push_back( xml::node( "replace_child" ) );
        root.replace( i, replacement );
        ostr << root;

        saveStreamToFile( ostr, "data/03a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/03a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/03a.out.real" );
        BOOST_FAIL( "Node test (03a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest03b )
{
    try
    {
        xml::document               doc( "data/03.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::iterator         i = root.find( "to_replace", root.begin() );
        CNcbiOstrstream             ostr;

        if ( i == root.end() )
        {
            throw std::runtime_error( "xml::node::find failed" );
        }

        xml::node                   replacement( "replace" );

        replacement.push_back( xml::node( "replace_child" ) );
        i = root.replace( i, replacement );
        ostr << *i;

        saveStreamToFile( ostr, "data/03b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/03b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/03b.out.real" );
        BOOST_FAIL( "Node test (03b) failed" );
    }
    return;
}


//
// This test checks xml::node::erase
//

BOOST_AUTO_TEST_CASE( NodeTest04a )
{
    try
    {
        xml::document               doc( "data/04.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::iterator         i = root.find( "to_remove", root.begin() );
        CNcbiOstrstream             ostr;

        if ( i == root.end() )
        {
            throw std::runtime_error( "xml::node::find failed" );
        }

        root.erase( i );
        ostr << root;

        saveStreamToFile( ostr, "data/04a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/04a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/04a.out.real" );
        BOOST_FAIL( "Node test (04a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest04b )
{
    try
    {
        xml::document               doc( "data/04.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::iterator         i = root.find( "to_remove", root.begin() );
        CNcbiOstrstream             ostr;

        if ( i == root.end() )
        {
            throw std::runtime_error( "xml::node::find failed" );
        }

        i = root.erase( i );
        ostr << *i;

        saveStreamToFile( ostr, "data/04b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/04b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/04b.out.real" );
        BOOST_FAIL( "Node test (04b) failed" );
    }
    return;
}



//
// This test checks xml::node::insert
//

BOOST_AUTO_TEST_CASE( NodeTest05a )
{
    try
    {
        xml::document               doc( "data/05.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::iterator         i = root.find( "insert_before", root.begin() );
        CNcbiOstrstream             ostr;

        if ( i == root.end() )
        {
            throw std::runtime_error( "xml::node::find failed" );
        }

        root.insert( i, xml::node( "inserted" ) );
        ostr << root;

        saveStreamToFile( ostr, "data/05a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/05a.out.real" );
        BOOST_FAIL( "Node test (05a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest05b )
{
    try
    {
        xml::document               doc( "data/05.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::iterator         i = root.find( "insert_before", root.begin() );
        CNcbiOstrstream             ostr;

        if ( i == root.end() )
        {
            throw std::runtime_error( "xml::node::find failed" );
        }

        i = root.insert( i, xml::node( "inserted" ) );
        ostr << *i;

        saveStreamToFile( ostr, "data/05b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/05b.out.real" );
        BOOST_FAIL( "Node test (05b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest05c )
{
    try
    {
        xml::document               doc( "data/05.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.insert( root.begin(), xml::node( "inserted" ) );
        ostr << root;

        saveStreamToFile( ostr, "data/05c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05c.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/05c.out.real" );
        BOOST_FAIL( "Node test (05c) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest05d )
{
    try
    {
        xml::document               doc( "data/05.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;
        xml::node::iterator         i = root.insert( root.begin(), xml::node( "inserted" ) );

        ostr << *i;

        saveStreamToFile( ostr, "data/05d.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05d.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/05d.out.real" );
        BOOST_FAIL( "Node test (05d) failed" );
    }
    return;
}


//
// This test checks xml::node::get_type()
//

void  dump_node_type( ostream &             stream,
                      const xml::node &     n,
                      int                   indent )
{
    for ( int j = 0; j < indent; ++j )
    {
       stream << " ";
    }

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

    xml::node::const_iterator   i = n.begin(), end = n.end();
    for ( ; i != end; ++i )
    {
        dump_node_type( stream, *i, indent+4 );
    }
}


BOOST_AUTO_TEST_CASE( NodeTest06a )
{
    try
    {
        xml::init::substitute_entities( false );

        xml::document               doc( "data/06a.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        dump_node_type( ostr, root, 0 );
        saveStreamToFile( ostr, "data/06a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/06a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/06a.out.real" );
        BOOST_FAIL( "Node test (06a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest06b )
{
    try
    {
        xml::init::substitute_entities( false );

        xml::document               doc( "data/06b.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        dump_node_type( ostr, root, 0 );
        saveStreamToFile( ostr, "data/06b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/06b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/06b.out.real" );
        BOOST_FAIL( "Node test (06b) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest06c )
{
    try
    {
        xml::init::substitute_entities( false );

        xml::document               doc( "data/06c.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        dump_node_type( ostr, root, 0 );
        saveStreamToFile( ostr, "data/06c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/06c.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/06c.out.real" );
        BOOST_FAIL( "Node test (06c) failed" );
    }
    return;
}

//
// This test checks xml::node::sort
//

BOOST_AUTO_TEST_CASE( NodeTest07a )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/07a.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.sort( "child", "order" );
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/07a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/07a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/07a.out.real" );
        BOOST_FAIL( "Node test (07a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest07b )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/07b.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.sort( "child", "order" );
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/07b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/07b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/07b.out.real" );
        BOOST_FAIL( "Node test (07b) failed" );
    }
    return;
}



//
// This test checks xml::node::sort_fo
//

struct cmp : public std::binary_function< xml::node, xml::node, bool >
{
    bool operator() (const xml::node &lhs, const xml::node &rhs)
    {
        if ( lhs.get_type() != xml::node::type_element ) return true;
        if ( rhs.get_type() != xml::node::type_element ) return false;
        return strcmp( lhs.get_name(), rhs.get_name() ) < 0;
    }
};


BOOST_AUTO_TEST_CASE( NodeTest08a )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/08a.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.sort( cmp() );
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/08a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/08a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/08a.out.real" );
        BOOST_FAIL( "Node test (08a) failed" );
    }
    return;
}


//
// This test checks xml::node::node(cdata)
//

BOOST_AUTO_TEST_CASE( NodeTest09 )
{
    try
    {
        xml::node               n( xml::node::cdata( "This is a CDATA section" ) );
        CNcbiOstrstream         ostr;

        ostr << n;

        saveStreamToFile( ostr, "data/09.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/09.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/09.out.real" );
        BOOST_FAIL( "Node test (09) failed" );
    }
    return;
}


//
// This test checks xml::node::node(comment)
//

BOOST_AUTO_TEST_CASE( NodeTest10 )
{
    try
    {
        xml::node               n( xml::node::comment( "This is an XML comment" ) );
        CNcbiOstrstream         ostr;

        ostr << n;

        saveStreamToFile( ostr, "data/10.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/10.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/10.out.real" );
        BOOST_FAIL( "Node test (10) failed" );
    }
    return;
}


//
// This test checks xml::node::node(pi)
//

BOOST_AUTO_TEST_CASE( NodeTest11 )
{
    try
    {
        xml::node               n( xml::node::pi( "xslt", "stylesheet=\"test.xsl\"" ) );
        CNcbiOstrstream         ostr;

        ostr << n;

        saveStreamToFile( ostr, "data/11.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/11.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/11.out.real" );
        BOOST_FAIL( "Node test (11) failed" );
    }
    return;
}


//
// This test checks xml::node::size()
//

BOOST_AUTO_TEST_CASE( NodeTest12 )
{
    try
    {
        CNcbiOstrstream             ostr;
        xml::node                   n( "root" );

        ostr << n.size() << "\n";

        n.push_back( xml::node( "one" ) );
        ostr << n.size() << "\n";

        n.push_back( xml::node( "two" ) );
        ostr << n.size() << "\n";

        saveStreamToFile( ostr, "data/12.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/12.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/12.out.real" );
        BOOST_FAIL( "Node test (12) failed" );
    }
    return;
}


//
// This test checks xml::node::empty()
//

BOOST_AUTO_TEST_CASE( NodeTest13 )
{
    try
    {
        CNcbiOstrstream         ostr;
        xml::node               n( "root" );

        ostr << n.empty() << "\n";

        n.push_back( xml::node( "one" ) );
        ostr << n.empty() << "\n";

        n.push_back( xml::node( "two" ) );
        ostr << n.empty() << "\n";

        saveStreamToFile( ostr, "data/13.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/13.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/13.out.real" );
        BOOST_FAIL( "Node test (13) failed" );
    }
    return;
}



//
// This test checks xml::node::node(text)
//

BOOST_AUTO_TEST_CASE( NodeTest14 )
{
    try
    {
        CNcbiOstrstream         ostr;
        xml::node               root( "root" );
        xml::node               n( xml::node::text( "some text" ) );

        root.push_back( n );
        ostr << root;

        saveStreamToFile( ostr, "data/14.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/14.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/14.out.real" );
        BOOST_FAIL( "Node test (14) failed" );
    }
    return;
}

//
// Checks duplicate namespace definitions deletion
//

BOOST_AUTO_TEST_CASE( NodeTest15a )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15a.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15a.out.real" );
        BOOST_FAIL( "Node test (15a) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15b )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15b.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15b.out.real" );
        BOOST_FAIL( "Node test (15b) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15c )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15c.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15c.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15c.out.real" );
        BOOST_FAIL( "Node test (15c) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15d )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15d.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15d.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15d.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15d.out.real" );
        BOOST_FAIL( "Node test (15d) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15e )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15e.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15e.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15e.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15e.out.real" );
        BOOST_FAIL( "Node test (15e) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15f )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15f.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15f.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15f.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15f.out.real" );
        BOOST_FAIL( "Node test (15f) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15g )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15g.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15g.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15g.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15g.out.real" );
        BOOST_FAIL( "Node test (15g) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15h )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15h.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15h.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15h.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15h.out.real" );
        BOOST_FAIL( "Node test (15h) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15i )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15i.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15i.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15i.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15i.out.real" );
        BOOST_FAIL( "Node test (15i) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15j )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15j.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15j.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15j.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15j.out.real" );
        BOOST_FAIL( "Node test (15j) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest15k )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/15k.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_duplicate_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/15k.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15k.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/15k.out.real" );
        BOOST_FAIL( "Node test (15k) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest16 )
{
    try
    {
        xml::init::remove_whitespace( true );

        xml::document               doc( "data/16.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        CNcbiOstrstream             ostr;

        root.erase_unused_ns_defs();
        ostr << doc << std::endl;

        saveStreamToFile( ostr, "data/16.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/16.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/16.out.real" );
        BOOST_FAIL( "Node test (16) failed" );
    }
    return;
}


//
// get_path() functionality test
//
BOOST_AUTO_TEST_CASE( NodeTest17 )
{
    try
    {
        xml::document               doc( "data/17.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::node::const_iterator   child = root.find( "child" );
        BOOST_CHECK( child != root.end() );

        xml::node::const_iterator   nested = child->find( "nested" );
        BOOST_CHECK( nested != child->end() );

        xml::node::const_iterator   deep = nested->find( "deep" );
        BOOST_CHECK( deep != nested->end() );

        BOOST_CHECK( deep->get_path() == "/root/child/nested/deep" );
    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "Node test (17) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest18 )
{
    try
    {
        xml::node   root( "root" );

        BOOST_CHECK( root.get_path() == "/root" );
    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "Node test (18) failed" );
    }
    return;
}


//
// STL algorithms and xml::node container
//

struct NodeNamePredicate
{
    std::string     pattern;
    NodeNamePredicate( const std::string &  str ) : pattern( str )
    {}
    bool operator() ( const xml::node &  n ) const
    {
        return pattern == n.get_name();
    }
};

BOOST_AUTO_TEST_CASE( NodeTest19 )
{
    try
    {
        xml::document               doc( "data/19.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        BOOST_CHECK( std::find_if( root.begin(), root.end(),
                                   NodeNamePredicate( "two" ) ) != root.end() );
    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "Node test (19) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NodeTest20 )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::ns                     matched_ns( "bar", "bar_ns" );
        xml::ns                     not_matched_ns( "bar", "non_existed_uri" );
        xml::ns                     void_ns( xml::ns::type_void );


        {
            // const iterator
            xml::node::const_iterator   child = root.find( "nested", &matched_ns );
            BOOST_CHECK( child != root.end() );

            child = root.find( "nested", &not_matched_ns );
            BOOST_CHECK( child == root.end() );

            child = root.find( "nested", &void_ns );
            BOOST_CHECK( child == root.end() );

            child = root.find( "nested", NULL );
            BOOST_CHECK( child != root.end() );
        }


        {
            // non const iterator
            xml::node::iterator   child = root.find( "nested", &matched_ns );
            BOOST_CHECK( child != root.end() );

            child = root.find( "nested", &not_matched_ns );
            BOOST_CHECK( child == root.end() );

            child = root.find( "nested", &void_ns );
            BOOST_CHECK( child == root.end() );

            child = root.find( "nested", NULL );
            BOOST_CHECK( child != root.end() );
        }

    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "Node test (20) failed" );
    }
    return;
}


/*
 * find attribute functionality tests
 */

BOOST_AUTO_TEST_CASE( NodeTest21 )
{
    try
    {
        xml::document               doc( "data/21.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        BOOST_CHECK( root.find_attribute( NULL ) == root.get_attributes().end() );
    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "Node test (21) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest22 )
{
    try
    {
        xml::document               doc( "data/22.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        // Found attributes
        xml::attributes::iterator   found = root.find_attribute( "attr2" );
        BOOST_CHECK( found != attrs.end() );
        BOOST_CHECK( found->get_value() == std::string( "value2" ) );
        BOOST_CHECK( found->get_name() == std::string( "attr2" ) );

        found = root.find_attribute( "attr3", NULL );
        BOOST_CHECK( found != attrs.end() );
        BOOST_CHECK( found->get_value() == std::string( "value3" ) );
        BOOST_CHECK( found->get_name() == std::string( "attr3" ) );

        xml::attributes::const_iterator     const_found = root.find_attribute( "attr1" );
        BOOST_CHECK( const_found != attrs.end() );
        BOOST_CHECK( const_found->get_value() == std::string( "value1" ) );
        BOOST_CHECK( const_found->get_name() == std::string( "attr1" ) );

        const_found = root.find_attribute( "attr3", NULL );
        BOOST_CHECK( const_found != attrs.end() );
        BOOST_CHECK( const_found->get_value() == std::string( "value3" ) );
        BOOST_CHECK( const_found->get_name() == std::string( "attr3" ) );

        // Not found attributes
        xml::attributes::iterator   not_found = root.find_attribute( "non-existent" );
        BOOST_CHECK( not_found == attrs.end() );

        not_found = root.find_attribute( "non-existent", NULL );
        BOOST_CHECK( not_found == attrs.end() );

        xml::attributes::const_iterator     const_not_found = root.find_attribute( "non-existent" );
        BOOST_CHECK( const_not_found == attrs.end() );

        const_not_found = root.find_attribute( "non-existent", NULL );
        BOOST_CHECK( const_not_found == attrs.end() );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/22.out.real" );
        BOOST_FAIL( "Node test (22) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Unexpected exception" << std::endl;
        saveStreamToFile( ostr, "data/22.out.real" );
        BOOST_FAIL( "Node test (22) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest23 )
{
    try
    {
        xml::document               doc( "data/23.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     matched_ns( "bar", "bar_ns" );

        // Found attributes
        xml::attributes::iterator   found = root.find_attribute( "attr3", &matched_ns );
        BOOST_CHECK( found != attrs.end() );
        BOOST_CHECK( found->get_value() == std::string( "value3" ) );
        BOOST_CHECK( found->get_name() == std::string( "attr3" ) );
        BOOST_CHECK( found->get_namespace() == matched_ns );

        found = root.find_attribute( "attr3" );
        BOOST_CHECK( found != attrs.end() );
        BOOST_CHECK( found->get_value() == std::string( "value3" ) );
        BOOST_CHECK( found->get_name() == std::string( "attr3" ) );
        BOOST_CHECK( found->get_namespace() == matched_ns );

        xml::attributes::const_iterator   const_found = root.find_attribute( "attr3", &matched_ns );
        BOOST_CHECK( const_found != attrs.end() );
        BOOST_CHECK( const_found->get_value() == std::string( "value3" ) );
        BOOST_CHECK( const_found->get_name() == std::string( "attr3" ) );
        BOOST_CHECK( const_found->get_namespace() == matched_ns );

        const_found = root.find_attribute( "attr3" );
        BOOST_CHECK( const_found != attrs.end() );
        BOOST_CHECK( const_found->get_value() == std::string( "value3" ) );
        BOOST_CHECK( const_found->get_name() == std::string( "attr3" ) );
        BOOST_CHECK( const_found->get_namespace() == matched_ns );

        // Not found attributes
        xml::ns                     not_matched_ns( "bar", "non_existed_uri" );

        xml::attributes::iterator   not_found = root.find_attribute( "attr3", &not_matched_ns );
        BOOST_CHECK( not_found == attrs.end() );

        not_found = root.find_attribute( "non-existent", &not_matched_ns );
        BOOST_CHECK( not_found == attrs.end() );

        not_found = root.find_attribute( "attr1", &not_matched_ns );
        BOOST_CHECK( not_found == attrs.end() );

        xml::attributes::const_iterator     const_not_found = root.find_attribute( "attr3", &not_matched_ns );
        BOOST_CHECK( const_not_found == attrs.end() );

        const_not_found = root.find_attribute( "non-existent", &not_matched_ns );
        BOOST_CHECK( const_not_found == attrs.end() );

        const_not_found = root.find_attribute( "attr1", &not_matched_ns );
        BOOST_CHECK( const_not_found == attrs.end() );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/23.out.real" );
        BOOST_FAIL( "Node test (23) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream ostr;
        ostr << "Unexpected exception" << std::endl;
        saveStreamToFile( ostr, "data/23.out.real" );
        BOOST_FAIL( "Node test (23) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NodeTest24 )
{
    try
    {
        std::string         xmldata( "<root><a><b><c><d><e><f><g/></f></e></d></c></b></a></root>" );
        xml::document       doc( xmldata.c_str(), xmldata.size(), NULL );
        xml::node &         root( doc.get_root_node() );

        xml::node::iterator   child = root.find( "a" );
        xml::node             implicit_copy = *child;
        xml::node *           explicit_copy = child->detached_copy();

        BOOST_CHECK( child->get_path() == std::string( "/root/a" ) );
        BOOST_CHECK( implicit_copy.get_path() == std::string( "/a" ) );
        BOOST_CHECK( explicit_copy->get_path() == std::string( "/a" ) );

        delete explicit_copy;
    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "Node test (24) failed" );
    }
    return;
}


//
// Output tests
//

BOOST_AUTO_TEST_CASE( NodeTest25 )
{
    try
    {
        xml::node           n( "root" );
        n.push_back( xml::node( "child" ) );

        // string
        std::string         s;
        std::ofstream       out( "data/25.out.real" );
        n.append_to_string( s, xml::save_op_default );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/25.out.real", "data/25.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/25.out.real" );
        BOOST_FAIL( "Node test (25) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (25) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest26 )
{
    try
    {
        xml::node           n( "root" );
        n.push_back( xml::node( "child" ) );

        // string
        std::string         s = "rubbish";
        std::ofstream       out( "data/26.out.real" );
        n.save_to_string( s, xml::save_op_no_decl );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/26.out.real", "data/26.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/26.out.real" );
        BOOST_FAIL( "Node test (26) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (26) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest27 )
{
    try
    {
        xml::node           n( "root" );
        n.push_back( xml::node( "child" ) );

        // string
        std::string         s;
        std::ofstream       out( "data/27.out.real" );
        n.save_to_string( s, xml::save_op_no_format );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/27.out.real", "data/27.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/27.out.real" );
        BOOST_FAIL( "Node test (27) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (27) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest28 )
{
    try
    {
        xml::document               doc( "data/28.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        string                      s;
        root.save_to_string( s, xml::save_op_no_format | xml::save_op_no_decl );

        std::ofstream               out( "data/28.out.real" );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/28.out.real", "data/28.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/28.out.real" );
        BOOST_FAIL( "Node test (28) failed (node content encoding)" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (28) failed (node content encoding)" );
    }
    return;
}


// Test 29 must not core dump
xml::node &  insert_new_node( xml::document &  doc )
{
    xml::node &             root = doc.get_root_node();
    xml::node::iterator     new_node = root.insert( root.begin(),
                                                    xml::node( "inserted" ) );
    new_node->set_content( "bla" );
    return *new_node;
}
BOOST_AUTO_TEST_CASE( NodeTest29 )
{
    std::string         xmldata( "<root><a><b></b></a></root>" );
    xml::document       doc( xmldata.c_str(), xmldata.size(), NULL );
    xml::node &         new_node = insert_new_node( doc );

    BOOST_CHECK( string( new_node.get_name() ) == string( "inserted" ) );
    BOOST_CHECK( string( new_node.get_content() ) == string( "bla" ) );
    return;
}


// Test special characters escaping
BOOST_AUTO_TEST_CASE( NodeTest30 )
{
    std::string     content = "bla <bla> bla";

    xml::node       node1( "test", content.c_str() );
    BOOST_CHECK( string( node1.get_content() ) == content );

    xml::node       node2( "test" );
    node2.set_content( content.c_str() );
    BOOST_CHECK( string( node2.get_content() ) == content );
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest31 )
{
    std::string     content = "bla & bla";

    xml::node       node1( "test", content.c_str() );
    BOOST_CHECK( string( node1.get_content() ) == content );

    xml::node       node2( "test" );
    node2.set_content( content.c_str() );
    BOOST_CHECK( string( node2.get_content() ) == content );
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest32 )
{
    std::string     doc_content( "<!DOCTYPE root ["
                                 "<!ENTITY myname \"Peter Jones\"> ]>"
                                 "<root>&myname;</root>" );
    xml::document   doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &     root = doc.get_root_node();

    BOOST_CHECK( string( root.get_content() ) == "Peter Jones" );

    root.set_content( "&myname;" );
    BOOST_CHECK( string( root.get_content() ) == "&myname;" );

    root.set_raw_content( "&myname;" );
    BOOST_CHECK( string( root.get_content() ) == "Peter Jones" );

    return;
}

BOOST_AUTO_TEST_CASE( NodeTest33 )
{
    std::string     doc_content( "<root><child>ttt</child></root>" );
    xml::document   doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &     root = doc.get_root_node();

    BOOST_CHECK( root.size() != 0 );
    string          s;
    root.append_to_string( s, xml::save_op_no_decl );
    BOOST_CHECK( s == string( "<root>\n  <child>ttt</child>\n</root>\n" ) );

    root.clear();
    BOOST_CHECK( root.size() == 0 );
    root.save_to_string( s, xml::save_op_no_decl );
    BOOST_CHECK( s == string( "<root/>\n" ) );

    return;
}

BOOST_AUTO_TEST_CASE( NodeTest34 )
{
    xml::node   n( "name", "content" );

    BOOST_CHECK( n.is_root() == true );
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest35 )
{
    std::string     doc_content( "<root><child>ttt</child></root>" );
    xml::document   doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &     root = doc.get_root_node();

    BOOST_CHECK( root.is_root() == true );

    xml::node::const_iterator  it = root.find( "child" );
    BOOST_CHECK( it->is_root() == false );

    xml::node   n( "name", "content" );
    BOOST_CHECK( n.is_root() == true );

    it = root.insert( n );
    BOOST_CHECK( it->is_root() == false );
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest36 )
{
    std::string     doc_content( "<root><child></child></root>" );
    xml::document   doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &     root = doc.get_root_node();

    xml::node::iterator         i = root.find( "child" );
    xml::ns_list_type           ns_list;

    ns_list = i->get_effective_namespaces(xml::node::type_ns_all);
    BOOST_CHECK( ns_list.size() == 0 );

    ns_list = i->get_effective_namespaces(xml::node::type_ns_only_default);
    BOOST_CHECK( ns_list.size() == 0 );

    ns_list = i->get_effective_namespaces(xml::node::type_ns_only_non_default);
    BOOST_CHECK( ns_list.size() == 0 );
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest37 )
{
    std::string     doc_content( "<root xmlns=\"top\"><child xmlns=\"nested\"></child></root>" );
    xml::document   doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &     root = doc.get_root_node();

    xml::node::iterator         i = root.find( "child" );
    xml::ns_list_type           ns_list;

    ns_list = i->get_effective_namespaces(xml::node::type_ns_all);
    BOOST_CHECK( ns_list.size() == 1 );

    ns_list = i->get_effective_namespaces(xml::node::type_ns_only_default);
    BOOST_CHECK( std::string( ns_list[0].get_uri() ) == "nested" );
    BOOST_CHECK( ns_list.size() == 1 );

    ns_list = i->get_effective_namespaces(xml::node::type_ns_only_non_default);
    BOOST_CHECK( ns_list.size() == 0 );
    return;
}

BOOST_AUTO_TEST_CASE( NodeTest38 )
{
    std::string     doc_content( "<root xmlns=\"top\" xmlns:p1=\"uri1\"><child xmlns=\"nested\"></child></root>" );
    xml::document   doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &     root = doc.get_root_node();

    xml::node::iterator         i = root.find( "child" );
    xml::ns_list_type           ns_list;

    ns_list = i->get_effective_namespaces(xml::node::type_ns_all);
    BOOST_CHECK( ns_list.size() == 2 );

    ns_list = i->get_effective_namespaces(xml::node::type_ns_only_default);
    BOOST_CHECK( std::string( ns_list[0].get_uri() ) == "nested" );
    BOOST_CHECK( ns_list.size() == 1 );

    ns_list = i->get_effective_namespaces(xml::node::type_ns_only_non_default);
    BOOST_CHECK( ns_list.size() == 1 );
    BOOST_CHECK( std::string( ns_list[0].get_uri() ) == "uri1" );
    BOOST_CHECK( std::string( ns_list[0].get_prefix() ) == "p1" );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest39 )
{
    xml::node       n1( "node1", "content1" );
    xml::node       n2( "node2", "content2" );

    n1.swap( n2 );
    BOOST_CHECK( n1.get_name() == std::string( "node2" ) );
    BOOST_CHECK( n2.get_name() == std::string( "node1" ) );
    BOOST_CHECK( n1.get_content() == std::string( "content2" ) );
    BOOST_CHECK( n2.get_content() == std::string( "content1" ) );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest40 )
{
    std::string         doc_content( "<root><child>content</child></root>" );
    xml::document       doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &         root = doc.get_root_node();
    xml::node::iterator it = root.find( "child" );

    xml::node       n( "replace", "new_content" );

    (*it).swap( n );
    BOOST_CHECK( n.get_name() == std::string( "child" ) );
    BOOST_CHECK( n.get_content() == std::string( "content" ) );

    it = root.find( "child" );
    BOOST_CHECK( it == root.end() );

    it = root.find( "replace" );
    BOOST_CHECK( it != root.end() );
    if ( it != root.end() )
    {
        BOOST_CHECK( (*it).get_name() == std::string( "replace" ) );
        BOOST_CHECK( (*it).get_content() == std::string( "new_content" ) );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest41 )
{
    std::string         doc_content( "<root><child>content</child></root>" );
    xml::document       doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &         root = doc.get_root_node();
    xml::node::iterator it = root.find( "child" );

    xml::node       n( "replace", "new_content" );

    n.swap( *it );
    BOOST_CHECK( n.get_name() == std::string( "child" ) );
    BOOST_CHECK( n.get_content() == std::string( "content" ) );

    it = root.find( "child" );
    BOOST_CHECK( it == root.end() );

    it = root.find( "replace" );
    BOOST_CHECK( it != root.end() );
    if ( it != root.end() )
    {
        BOOST_CHECK( (*it).get_name() == std::string( "replace" ) );
        BOOST_CHECK( (*it).get_content() == std::string( "new_content" ) );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest42 )
{
    std::string         doc_content( "<root><child>content</child></root>" );
    xml::document       doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &         root = doc.get_root_node();

    xml::node       n( "new_root", "new_content" );

    root.swap( n );
    BOOST_CHECK( n.get_name() == std::string( "root" ) );

    xml::node &     new_root = doc.get_root_node();
    BOOST_CHECK( new_root.get_name() == std::string( "new_root" ) );
    BOOST_CHECK( new_root.get_content() == std::string( "new_content" ) );

    xml::node::iterator     it = n.find( "child" );
    BOOST_CHECK( it != n.end() );
    if ( it != n.end() )
    {
        BOOST_CHECK( (*it).get_name() == std::string( "child" ) );
        BOOST_CHECK( (*it).get_content() == std::string( "content" ) );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest43 )
{
    std::string         doc_content( "<root><child>content</child></root>" );
    xml::document       doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &         root = doc.get_root_node();

    xml::node       n( "new_root", "new_content" );

    n.swap( root );
    BOOST_CHECK( n.get_name() == std::string( "root" ) );

    xml::node &     new_root = doc.get_root_node();
    BOOST_CHECK( new_root.get_name() == std::string( "new_root" ) );
    BOOST_CHECK( new_root.get_content() == std::string( "new_content" ) );

    xml::node::iterator     it = n.find( "child" );
    BOOST_CHECK( it != n.end() );
    if ( it != n.end() )
    {
        BOOST_CHECK( (*it).get_name() == std::string( "child" ) );
        BOOST_CHECK( (*it).get_content() == std::string( "content" ) );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest44 )
{
    std::string         doc_content( "<root><child1><subchild1>content1</subchild1></child1>"
                                           "<child2>content2</child2>"
                                     "</root>" );
    xml::document       doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &         root = doc.get_root_node();
    xml::node::iterator it = root.find( "child1" );
    xml::node::iterator it1 = it->find( "subchild1" );
    xml::node::iterator it2 = root.find( "child2" );

    (*it1).swap( *it2 );

    it1 = it->find( "subchild1" );
    BOOST_CHECK( it1 == it->end() );
    it1 = it->find( "child2" );
    BOOST_CHECK( it1 != it->end() );

    xml::node &     new_root = doc.get_root_node();
    it2 = new_root.find( "child2" );
    BOOST_CHECK( it2 == new_root.end() );
    it2 = new_root.find( "subchild1" );
    BOOST_CHECK( it2 != new_root.end() );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest45 )
{
    std::string         doc_content1( "<root><child1>content1</child1></root>" );
    std::string         doc_content2( "<root><child2>content2</child2></root>" );

    xml::document       doc1( doc_content1.c_str(), doc_content1.size(), NULL );
    xml::document       doc2( doc_content2.c_str(), doc_content2.size(), NULL );

    xml::node &         root1 = doc1.get_root_node();
    xml::node &         root2 = doc2.get_root_node();

    xml::node::iterator it1 = root1.find( "child1" );
    xml::node::iterator it2 = root2.find( "child2" );

    (*it1).swap( *it2 );

    it1 = root1.find( "child1" );
    BOOST_CHECK( it1 == root1.end() );
    it1 = root1.find( "child2" );
    BOOST_CHECK( it1 != root1.end() );

    it2 = root2.find( "child2" );
    BOOST_CHECK( it2 == root2.end() );
    it2 = root2.find( "child1" );
    BOOST_CHECK( it2 != root2.end() );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest46 )
{
    std::string         doc_content1( "<root><child1>content1</child1></root>" );
    std::string         doc_content2( "<root><child2>content2</child2></root>" );

    xml::document       doc1( doc_content1.c_str(), doc_content1.size(), NULL );
    xml::document       doc2( doc_content2.c_str(), doc_content2.size(), NULL );

    xml::node &         root1 = doc1.get_root_node();
    xml::node &         root2 = doc2.get_root_node();


    root1.swap( root2 );


    xml::node &         new_root1 = doc1.get_root_node();
    xml::node &         new_root2 = doc2.get_root_node();

    BOOST_CHECK( new_root1.find( "child1" ) == new_root1.end() );
    BOOST_CHECK( new_root1.find( "child2" ) != new_root1.end() );

    BOOST_CHECK( new_root2.find( "child2" ) == new_root2.end() );
    BOOST_CHECK( new_root2.find( "child1" ) != new_root2.end() );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest47 )
{
    std::string         doc_content( "<root><child1><subchild1>content1</subchild1></child1>"
                                     "</root>" );
    xml::document       doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &         root = doc.get_root_node();
    xml::node::iterator it1= root.find( "child1" );
    xml::node::iterator it2 = it1->find( "subchild1" );

    try {
        (*it1).swap( *it2 );
    } catch (const xml::exception &  ex) {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Node test (47) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest48 )
{
    std::string         doc_content( "<root><child1><subchild1>content1</subchild1></child1>"
                                     "</root>" );
    xml::document       doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &         root = doc.get_root_node();
    xml::node::iterator it1= root.find( "child1" );
    xml::node::iterator it2 = it1->find( "subchild1" );

    try {
        (*it2).swap( *it1 );
    } catch (const xml::exception &  ex) {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Node test (48) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest49 )
{
    std::string         doc_content( "<root><child1><subchild1>content1</subchild1></child1>"
                                     "</root>" );
    xml::document       doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &         root = doc.get_root_node();
    xml::node::iterator it1= root.find( "child1" );

    try {
        root.swap( *it1 );
    } catch (const xml::exception &  ex) {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Node test (49) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest50 )
{
    std::string         doc_content( "<root><child1><subchild1>content1</subchild1></child1>"
                                     "</root>" );
    xml::document       doc( doc_content.c_str(), doc_content.size(), NULL );
    xml::node &         root = doc.get_root_node();
    xml::node::iterator it1= root.find( "child1" );

    try {
        (*it1).swap( root );
    } catch (const xml::exception &  ex) {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Node test (50) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest51 )
{
    std::string         doc_content1( "<root><child1 attr1=\"val1\">content1</child1></root>" );
    std::string         doc_content2( "<root><child2 attr2=\"val2\">content2</child2></root>" );

    xml::document       doc1( doc_content1.c_str(), doc_content1.size(), NULL );
    xml::document       doc2( doc_content2.c_str(), doc_content2.size(), NULL );

    xml::node &         root1 = doc1.get_root_node();
    xml::node &         root2 = doc2.get_root_node();

    xml::node::iterator it1 = root1.find( "child1" );
    xml::node::iterator it2 = root2.find( "child2" );

    (*it1).swap( *it2 );

    it1 = root1.find( "child1" );
    BOOST_CHECK( it1 == root1.end() );
    it1 = root1.find( "child2" );
    BOOST_CHECK( it1 != root1.end() );

    xml::attributes &           attrs1 = it1->get_attributes();
    xml::attributes::iterator   found = it1->find_attribute( "attr1" );
    BOOST_CHECK( found == attrs1.end() );
    found = it1->find_attribute( "attr2" );
    BOOST_CHECK( found != attrs1.end() );
    BOOST_CHECK( found->get_value() == std::string( "val2" ) );

    it2 = root2.find( "child2" );
    BOOST_CHECK( it2 == root2.end() );
    it2 = root2.find( "child1" );
    BOOST_CHECK( it2 != root2.end() );
    xml::attributes &           attrs2 = it2->get_attributes();
    found = it2->find_attribute( "attr2" );
    BOOST_CHECK( found == attrs2.end() );
    found = it2->find_attribute( "attr1" );
    BOOST_CHECK( found != attrs2.end() );
    BOOST_CHECK( found->get_value() == std::string( "val1" ) );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest52 )
{
    std::string         doc_content1( "<root><child1 xmlns:a=\"http://a.org\" a:attr1=\"val1\"><a:subchild1 a:attr1=\"val1\">content1</a:subchild1></child1></root>" );
    std::string         doc_content2( "<root><child2 xmlns:b=\"http://b.org\" b:attr2=\"val2\"><b:subchild2 b:attr2=\"val2\">content2</b:subchild2></child2></root>" );

    xml::document       doc1( doc_content1.c_str(), doc_content1.size(), NULL );
    xml::document       doc2( doc_content2.c_str(), doc_content2.size(), NULL );
    xml::ns             matched_nsa( "a", "http://a.org" );
    xml::ns             matched_nsb( "b", "http://b.org" );


    xml::node &         root1 = doc1.get_root_node();
    xml::node &         root2 = doc2.get_root_node();

    xml::node::iterator it1 = root1.find( "child1" );
    it1 = it1->find( "subchild1" );
    xml::node::iterator it2 = root2.find( "child2" );
    it2 = it2->find( "subchild2" );

    (*it1).swap( *it2 );

    it1 = root1.find( "child1" );
    BOOST_CHECK( it1 != root1.end() );
    it1 = it1->find( "subchild2" );

    xml::attributes &           attrs1 = it1->get_attributes();
    xml::attributes::iterator   found = it1->find_attribute( "a:attr1" );
    BOOST_CHECK( found == attrs1.end() );
    found = it1->find_attribute( "b:attr2" );
    BOOST_CHECK( found != attrs1.end() );
    BOOST_CHECK( found->get_value() == std::string( "val2" ) );
    BOOST_CHECK( found->get_namespace() == matched_nsb );

    xml::ns_list_type   namespaces = it1->get_namespace_definitions();
    BOOST_CHECK( namespaces.size() == 1 );
    BOOST_CHECK( namespaces[ 0 ] == matched_nsb );

    it2 = root2.find( "child2" );
    BOOST_CHECK( it2 != root2.end() );
    it2 = it2->find( "subchild1" );

    xml::attributes &           attrs2 = it2->get_attributes();
    xml::attributes::iterator   found2 = it2->find_attribute( "b:attr2" );
    BOOST_CHECK( found2 == attrs2.end() );
    found2 = it2->find_attribute( "a:attr1" );
    BOOST_CHECK( found2 != attrs2.end() );
    BOOST_CHECK( found2->get_value() == std::string( "val1" ) );
    BOOST_CHECK( found2->get_namespace() == matched_nsa );

    namespaces = it2->get_namespace_definitions();
    BOOST_CHECK( namespaces.size() == 1 );
    BOOST_CHECK( namespaces[ 0 ] == matched_nsa );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest52a )
{
    xml::document       doc1( "data/52a1.xml", NULL );
    xml::document       doc2( "data/52a2.xml", NULL );
    xml::ns             matched_nsa( "a", "http://a.org" );
    xml::ns             matched_nsb( "b", "http://b.org" );


    xml::node &         root1 = doc1.get_root_node();
    xml::node &         root2 = doc2.get_root_node();

    xml::node::iterator it1 = root1.find( "child1" );
    it1 = it1->find( "subchild1" );
    xml::node::iterator it2 = root2.find( "child2" );
    it2 = it2->find( "subchild2" );

    (*it1).swap( *it2 );

    it1 = root1.find( "child1" );
    BOOST_CHECK( it1 != root1.end() );
    it1 = it1->find( "subchild2" );

    xml::attributes &           attrs1 = it1->get_attributes();
    xml::attributes::iterator   found = it1->find_attribute( "a:attr1" );
    BOOST_CHECK( found == attrs1.end() );
    found = it1->find_attribute( "b:attr2" );
    BOOST_CHECK( found != attrs1.end() );
    BOOST_CHECK( found->get_value() == std::string( "val2" ) );
    BOOST_CHECK( found->get_namespace() == matched_nsb );

    xml::ns_list_type   namespaces = it1->get_namespace_definitions();
    BOOST_CHECK( namespaces.size() == 1 );
    BOOST_CHECK( namespaces[ 0 ] == matched_nsb );

    it2 = root2.find( "child2" );
    BOOST_CHECK( it2 != root2.end() );
    it2 = it2->find( "subchild1" );

    xml::attributes &           attrs2 = it2->get_attributes();
    xml::attributes::iterator   found2 = it2->find_attribute( "b:attr2" );
    BOOST_CHECK( found2 == attrs2.end() );
    found2 = it2->find_attribute( "a:attr1" );
    BOOST_CHECK( found2 != attrs2.end() );
    BOOST_CHECK( found2->get_value() == std::string( "val1" ) );
    BOOST_CHECK( found2->get_namespace() == matched_nsa );

    namespaces = it2->get_namespace_definitions();
    BOOST_CHECK( namespaces.size() == 1 );
    BOOST_CHECK( namespaces[ 0 ] == matched_nsa );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest53 )
{
    std::string         doc_content1( "<root xmlns=\"http://d1.org\"><child1><subchild1 attr1=\"val1\">content1</subchild1></child1></root>" );
    std::string         doc_content2( "<root xmlns=\"http://d2.org\"><child2><subchild2 attr2=\"val2\">content2</subchild2></child2></root>" );

    xml::document       doc1( doc_content1.c_str(), doc_content1.size(), NULL );
    xml::document       doc2( doc_content2.c_str(), doc_content2.size(), NULL );
    xml::ns             matched_nsd1( "", "http://d1.org" );
    xml::ns             matched_nsd2( "", "http://d2.org" );


    xml::node &         root1 = doc1.get_root_node();
    xml::node &         root2 = doc2.get_root_node();

    xml::node::iterator it1 = root1.find( "child1" );
    it1 = it1->find( "subchild1" );
    xml::node::iterator it2 = root2.find( "child2" );
    it2 = it2->find( "subchild2" );

    (*it1).swap( *it2 );

    it1 = root1.find( "child1" );
    BOOST_CHECK( it1 != root1.end() );
    it1 = it1->find( "subchild2" );

    xml::attributes &           attrs1 = it1->get_attributes();
    xml::attributes::iterator   found = it1->find_attribute( "attr1" );
    BOOST_CHECK( found == attrs1.end() );
    found = it1->find_attribute( "attr2" );
    BOOST_CHECK( found != attrs1.end() );
    BOOST_CHECK( found->get_value() == std::string( "val2" ) );

    xml::ns_list_type   namespaces = it1->get_namespace_definitions();
    BOOST_CHECK( namespaces.size() == 1 );
    BOOST_CHECK( namespaces[ 0 ] == matched_nsd2 );
    BOOST_CHECK( it1->get_namespace() == matched_nsd2 );

    it2 = root2.find( "child2" );
    BOOST_CHECK( it2 != root2.end() );
    it2 = it2->find( "subchild1" );

    xml::attributes &           attrs2 = it2->get_attributes();
    xml::attributes::iterator   found2 = it2->find_attribute( "attr2" );
    BOOST_CHECK( found2 == attrs2.end() );
    found2 = it2->find_attribute( "attr1" );
    BOOST_CHECK( found2 != attrs2.end() );
    BOOST_CHECK( found2->get_value() == std::string( "val1" ) );

    namespaces = it2->get_namespace_definitions();
    BOOST_CHECK( namespaces.size() == 1 );
    BOOST_CHECK( namespaces[ 0 ] == matched_nsd1 );
    BOOST_CHECK( it2->get_namespace() == matched_nsd1 );
    return;
}



BOOST_AUTO_TEST_CASE( NodeTest54 )
{
    try
    {
        std::string     doc_content = "<?xml version=\"1.0\"?>"
                                      "<!DOCTYPE root PUBLIC \"something\" \"12.dtd\" ["
                                          "<!ATTLIST child defaultAttr CDATA \"defaultVal\">"
                                      "]>"
                                      "<root explicitAttr=\"explicitVal\">"
                                          "<child attr1=\"a\">value1</child>"
                                      "</root>";


        xml::document               doc1( doc_content.c_str(), doc_content.size(), NULL );
        xml::node &                 root1 = doc1.get_root_node();
        xml::node::iterator         it1 = root1.find( "child" );
        xml::attributes &           attrs = it1->get_attributes();
        xml::attributes::iterator   k = attrs.find( "defaultAttr" );

        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == true );
        BOOST_CHECK( k->get_value() == std::string( "defaultVal" ) );


        std::string             doc_content2( "<root2><child2>content2</child2></root2>" );
        xml::document           doc2( doc_content2.c_str(), doc_content2.size(), NULL );
        xml::node &             root2 = doc2.get_root_node();
        xml::node::iterator     it2 = root2.find( "child2" );

        it1->swap( *it2 );

        // It's forbidden to access defailt attributes via existing iterators
        // after a node swap between documents
        k->get_value();
        BOOST_FAIL( "NodeTest54 failed (acessing default attributes after swap)" );
    }
    catch ( const xml::exception &  ex )
    {
        // xml::exception is expected
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest55 )
{
    xml::node               n1( "node1", "content1" );
    xml::node               n2( "node2", "content2" );
    xml::node::iterator     it1 = n1.self();
    xml::node::iterator     it2 = n2.self();

    (*it1).swap( *it2 );
    BOOST_CHECK( n1.get_name() == std::string( "node2" ) );
    BOOST_CHECK( n2.get_name() == std::string( "node1" ) );
    BOOST_CHECK( n1.get_content() == std::string( "content2" ) );
    BOOST_CHECK( n2.get_content() == std::string( "content1" ) );
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest56 )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/56.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/56.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::c14n_1_0,
                                          xml::keep_comments,
                                          xml::without_formatting,
                                          xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/56.out.real", "data/56.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/56.out.real" );
        BOOST_FAIL( "Node test (56 - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (56 - save_to_string_canonical) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest56a )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/56.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/56a.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::c14n_1_0,
                                          xml::keep_comments,
                                          xml::without_formatting,
                                          xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/56a.out.real", "data/56a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/56a.out.real" );
        BOOST_FAIL( "Node test (56a - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (56a - save_to_string_canonical) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest56b )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/56.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/56b.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::c14n_1_0,
                                          xml::keep_comments,
                                          xml::with_formatting,
                                          xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/56b.out.real", "data/56b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/56b.out.real" );
        BOOST_FAIL( "Node test (56b - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (56b - save_to_string_canonical) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest56c )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/56.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/56c.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::c14n_1_0,
                                          xml::keep_comments,
                                          xml::with_formatting,
                                          xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/56c.out.real", "data/56c.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/56c.out.real" );
        BOOST_FAIL( "Node test (56c - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (56c - save_to_string_canonical) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NodeTest56d )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/56.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/56d.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::c14n_1_0,
                                          xml::strip_comments,
                                          xml::with_formatting,
                                          xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/56d.out.real", "data/56d.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/56d.out.real" );
        BOOST_FAIL( "Node test (56d - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (56d - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NodeTest57 )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/57.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/57.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::sort_attr_and_ns,
                                          xml::keep_comments,
                                          xml::without_formatting,
                                          xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/57.out.real", "data/57.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/57.out.real" );
        BOOST_FAIL( "Node test (57 - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (57 - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NodeTest57a )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/57.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/57a.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::sort_attr_and_ns,
                                          xml::keep_comments,
                                          xml::without_formatting,
                                          xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/57a.out.real", "data/57a.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/57a.out.real" );
        BOOST_FAIL( "Node test (57a - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (57a - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NodeTest57b )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/57.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/57b.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::sort_attr_and_ns_no_decl,
                                          xml::keep_comments,
                                          xml::with_formatting,
                                          xml::without_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/57b.out.real", "data/57b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/57b.out.real" );
        BOOST_FAIL( "Node test (57b - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (57b - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NodeTest57c )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/57.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/57c.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::sort_attr_and_ns_no_decl,
                                          xml::keep_comments,
                                          xml::with_formatting,
                                          xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/57c.out.real", "data/57c.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/57c.out.real" );
        BOOST_FAIL( "Node test (57c - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (57c - save_to_string_canonical) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NodeTest57d )
{
    std::string         s;
    try
    {
        xml::document       doc( "data/57.xml", NULL );
        xml::node &         root = doc.get_root_node();
        std::ofstream       out( "data/57d.out.real" );

        std::string         s;
        root.save_to_string_canonical( s, xml::sort_attr_and_ns_no_decl,
                                          xml::strip_comments,
                                          xml::with_formatting,
                                          xml::with_node_sorting );
        out << s;
        out.close();

        BOOST_CHECK( compareFiles( "data/57d.out.real", "data/57d.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/57d.out.real" );
        BOOST_FAIL( "Node test (57d - save_to_string_canonical) failed" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node test (57d - save_to_string_canonical) failed" );
    }
}

