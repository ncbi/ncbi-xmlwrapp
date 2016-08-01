/*  $Id: test_namespace.cpp 435003 2014-05-13 15:08:30Z ucko $
 * ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 *
 * Author:  Sergey Satskiy, NCBI
 *
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


// A helper function to save the stream content into a file
void  saveStreamToFile( CNcbiOstrstream &       stream,
                        const std::string &     fileName )
{
    std::ofstream   f( fileName.c_str() );

    f << string(CNcbiOstrstreamToString(stream));
    f.close();

    return;
}




BOOST_AUTO_TEST_CASE( NamespaceTest01 )
{
    try
    {
        xml::ns     space( "prefix", "uri" );

        BOOST_CHECK( space.get_prefix() == std::string( "prefix" ) );
        BOOST_CHECK( space.get_uri() == std::string( "uri" ) );
        BOOST_CHECK( !space.is_void() );
        BOOST_CHECK( space.is_safe() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing a regular namespace test (01) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest03 )
{
    try
    {
        xml::ns     space( xml::ns::type_void );

        BOOST_CHECK( space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( space.get_uri() == std::string( "" ) );
        BOOST_CHECK( space.is_void() );
        BOOST_CHECK( space.is_safe() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing a void namespace test (03) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest04a )
{
    try
    {
        xml::ns     space1( "p", "u" );
        xml::ns     space2( "p", "u" );

        BOOST_CHECK( space1 == space2 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Comparison of two namespaces test (04a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest04b )
{
    try
    {
        xml::ns     space1( "prefixOne", "u" );
        xml::ns     space2( "prefixTwo", "u" );

        BOOST_CHECK( space1 == space2 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Comparison of two namespaces test (04b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest04c )
{
    try
    {
        xml::ns     space1( "prefix", "URI-One" );
        xml::ns     space2( "prefix", "URI-Two" );

        BOOST_CHECK( space1 != space2 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Comparison of two namespaces test (04c) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest05 )
{
    try
    {
        xml::ns     space1( "p", "u" );
        xml::ns     space2( space1 );

        BOOST_CHECK( space1 == space2 );
        BOOST_CHECK( space2.is_safe() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Copy construction of a namespace test (05) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest06 )
{
    try
    {
        xml::ns     space1( "p", "u" );
        xml::ns     space2( "not supposed", "to be seen" );

        space2 = space1;

        BOOST_CHECK( space2.get_prefix() == std::string( "p" ) );
        BOOST_CHECK( space2.get_uri() == std::string( "u" ) );
        BOOST_CHECK( space2.is_safe() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "A namespace operator= test (06) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest07 )
{
    try
    {
        xml::ns     space1( "prefix", "" );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "An empty namespace uri test (07) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest09 )
{
    try
    {
        xml::document           doc( "data/01.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "NoNamespace", root.begin() );

        xml::ns                 space( i->get_namespace() );

        BOOST_CHECK( space.is_void() );
        BOOST_CHECK( space.is_safe() );

        xml::ns                 unsafe_space( i->get_namespace( xml::ns::type_unsafe_ns ) );

        BOOST_CHECK( unsafe_space.is_void() );
        BOOST_CHECK( !unsafe_space.is_safe() );
        BOOST_CHECK( unsafe_space == space );

        unsafe_space.make_safe();

        BOOST_CHECK( unsafe_space.is_safe() );
        BOOST_CHECK( unsafe_space == space );
    }
    catch ( ... )
    {
        BOOST_FAIL( "No namespace test (09) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest10 )
{
    try
    {
        xml::document           doc( "data/02.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "DefaultNamespace", root.begin() );
        CNcbiOstrstream         ostr;

        xml::ns                 space( i->get_namespace() );

        ostr << "Is void: " << space.is_void() << "\n"
             << "Prefix: '" << space.get_prefix() << "'\n"
             << "URI: '" << space.get_uri() << "'";
        saveStreamToFile( ostr, "data/02.out.real" );

        BOOST_CHECK( space.is_void() == false );
        BOOST_CHECK( space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( space.get_uri() == std::string( "http://The.Default.Namespace" ) );
        BOOST_CHECK( space.is_safe() );

        xml::ns                 unsafe_space( i->get_namespace( xml::ns::type_unsafe_ns ) );

        BOOST_CHECK( unsafe_space.is_void() == false );
        BOOST_CHECK( unsafe_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( unsafe_space.get_uri() == std::string( "http://The.Default.Namespace" ) );
        BOOST_CHECK( !unsafe_space.is_safe() );
        BOOST_CHECK( unsafe_space == space );

        unsafe_space.make_safe();

        BOOST_CHECK( unsafe_space.is_safe() );
        BOOST_CHECK( unsafe_space == space );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/02ex.out.real" );

        BOOST_FAIL( "Default namespace test (10) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/02ex.out.real" );

        BOOST_FAIL( "Default namespace test (10) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest11 )
{
    try
    {
        xml::document           doc( "data/03.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "Namespaced", root.begin() );
        CNcbiOstrstream         ostr;

        xml::ns                 space( i->get_namespace() );

        ostr << "Is void: " << space.is_void() << "\n"
             << "Prefix: '" << space.get_prefix() << "'\n"
             << "URI: '" << space.get_uri() << "'";
        saveStreamToFile( ostr, "data/03.out.real" );

        BOOST_CHECK( space.is_void() == false );
        BOOST_CHECK( space.get_prefix() == std::string( "MyPrefix" ) );
        BOOST_CHECK( space.get_uri() == std::string( "http://The.Namespace" ) );

        xml::ns                 unsafe_space( i->get_namespace( xml::ns::type_unsafe_ns ) );

        BOOST_CHECK( unsafe_space.is_void() == false );
        BOOST_CHECK( unsafe_space.get_prefix() == std::string( "MyPrefix" ) );
        BOOST_CHECK( unsafe_space.get_uri() == std::string( "http://The.Namespace" ) );
        BOOST_CHECK( !unsafe_space.is_safe() );
        BOOST_CHECK( unsafe_space == space );

        unsafe_space.make_safe();

        BOOST_CHECK( unsafe_space.is_safe() );
        BOOST_CHECK( unsafe_space == space );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/03ex.out.real" );

        BOOST_FAIL( "Usual namespace test (11) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/03ex.out.real" );

        BOOST_FAIL( "Usual namespace test (11) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest12 )
{
    try
    {
        xml::document           doc( "data/04.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "NoDefinitions", root.begin() );
        xml::ns_list_type       definitions( i->get_namespace_definitions() );

        BOOST_CHECK( definitions.size() == 0 );

        xml::ns_list_type       unsafe_definitions( i->get_namespace_definitions( xml::ns::type_unsafe_ns ) );

        BOOST_CHECK( unsafe_definitions.size() == 0 );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/04ex.out.real" );

        BOOST_FAIL( "No amespace definitions test (12) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/04ex.out.real" );

        BOOST_FAIL( "No namespace definitions test (12) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest13 )
{
    try
    {
        xml::document           doc( "data/05.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "ThreeDefinitions", root.begin() );
        xml::ns                 space( i->get_namespace() );
        xml::ns_list_type       definitions( i->get_namespace_definitions() );

        BOOST_CHECK( space.is_void() == false );
        BOOST_CHECK( space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( space.get_uri() == std::string( "http://nested.default.ns" ) );

        BOOST_CHECK( definitions.size() == 3 );
        BOOST_CHECK( definitions[0].is_safe() );
        BOOST_CHECK( definitions[0].get_prefix() == std::string( "p1" ) );
        BOOST_CHECK( definitions[0].get_uri() == std::string( "http://the.web.site" ) );
        BOOST_CHECK( definitions[1].is_safe() );
        BOOST_CHECK( definitions[1].get_prefix() == std::string( "p2" ) );
        BOOST_CHECK( definitions[1].get_uri() == std::string( "http://another.web.site" ) );
        BOOST_CHECK( definitions[2].is_safe() );
        BOOST_CHECK( definitions[2].get_prefix() == std::string( "" ) );
        BOOST_CHECK( definitions[2].get_uri() == std::string( "http://nested.default.ns" ) );

        xml::ns_list_type       unsafe_definitions( i->get_namespace_definitions( xml::ns::type_unsafe_ns ) );

        BOOST_CHECK( unsafe_definitions.size() == 3 );
        BOOST_CHECK( !unsafe_definitions[0].is_safe() );
        BOOST_CHECK( unsafe_definitions[0].get_prefix() == std::string( "p1" ) );
        BOOST_CHECK( unsafe_definitions[0].get_uri() == std::string( "http://the.web.site" ) );
        BOOST_CHECK( !unsafe_definitions[1].is_safe() );
        BOOST_CHECK( unsafe_definitions[1].get_prefix() == std::string( "p2" ) );
        BOOST_CHECK( unsafe_definitions[1].get_uri() == std::string( "http://another.web.site" ) );
        BOOST_CHECK( !unsafe_definitions[2].is_safe() );
        BOOST_CHECK( unsafe_definitions[2].get_prefix() == std::string( "" ) );
        BOOST_CHECK( unsafe_definitions[2].get_uri() == std::string( "http://nested.default.ns" ) );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/05ex.out.real" );

        BOOST_FAIL( "Namespace definitions test (13) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/05ex.out.real" );

        BOOST_FAIL( "Namespace definitions test (13) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest14 )
{
    try
    {
        xml::document                   doc( "data/06.xml", NULL );
        xml::node &                     root = doc.get_root_node();
        xml::node::iterator             i = root.find( "node", root.begin() );
        xml::attributes                 attrs = i->get_attributes();

        xml::attributes::const_iterator a1( attrs.find( "attr1" ) );
        xml::ns                         space1( a1->get_namespace() );
        xml::ns                         unsafe_space1( a1->get_namespace(xml::ns::type_unsafe_ns) );

        BOOST_CHECK( space1.get_prefix() == std::string( "" ) );
        BOOST_CHECK( space1.get_uri() == std::string( "" ) );
        BOOST_CHECK( space1.is_safe() );
        BOOST_CHECK( unsafe_space1.get_prefix() == std::string( "" ) );
        BOOST_CHECK( unsafe_space1.get_uri() == std::string( "" ) );
        BOOST_CHECK( !unsafe_space1.is_safe() );
        BOOST_CHECK( space1 == unsafe_space1 );

        unsafe_space1.make_safe();

        BOOST_CHECK( unsafe_space1.is_safe() );
        BOOST_CHECK( space1 == unsafe_space1 );

        xml::attributes::const_iterator a2( attrs.find( "attr2" ) );
        xml::ns                         space2( a2->get_namespace() );
        xml::ns                         unsafe_space2( a2->get_namespace(xml::ns::type_unsafe_ns) );

        BOOST_CHECK( space2.get_prefix() == std::string( "p1" ) );
        BOOST_CHECK( space2.get_uri() == std::string( "http://The.Namespace" ) );
        BOOST_CHECK( space2.is_safe() );
        BOOST_CHECK( unsafe_space2.get_prefix() == std::string( "p1" ) );
        BOOST_CHECK( unsafe_space2.get_uri() == std::string( "http://The.Namespace" ) );
        BOOST_CHECK( !unsafe_space2.is_safe() );
        BOOST_CHECK( space2 == unsafe_space2 );

        unsafe_space2.make_safe();

        BOOST_CHECK( unsafe_space2.is_safe() );
        BOOST_CHECK( space2 == unsafe_space2 );

        // Get attributes by reference
        xml::attributes &               attrsRef = i->get_attributes();
        xml::attributes::const_iterator a3( attrsRef.find( "attr2" ) );
        xml::ns                         space3( a3->get_namespace() );
        xml::ns                         unsafe_space3( a3->get_namespace(xml::ns::type_unsafe_ns) );

        BOOST_CHECK( space3.get_prefix() == std::string( "p1" ) );
        BOOST_CHECK( space3.get_uri() == std::string( "http://The.Namespace" ) );
        BOOST_CHECK( space3.is_safe() );
        BOOST_CHECK( unsafe_space3.get_prefix() == std::string( "p1" ) );
        BOOST_CHECK( unsafe_space3.get_uri() == std::string( "http://The.Namespace" ) );
        BOOST_CHECK( !unsafe_space3.is_safe() );
        BOOST_CHECK( space3 == unsafe_space3 );

        unsafe_space3.make_safe();

        BOOST_CHECK( unsafe_space3.is_safe() );
        BOOST_CHECK( space3 == unsafe_space3 );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/06ex.out.real" );

        BOOST_FAIL( "Attributes' namespace test (14) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/06ex.out.real" );

        BOOST_FAIL( "Attributes' namespace test (14) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest15 )
{
    try
    {
        xml::document           doc( "data/07.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "Namespaced", root.begin() );

        xml::ns                 space( i->get_namespace() );
        xml::ns                 unsafe_space( i->get_namespace(xml::ns::type_unsafe_ns) );

        BOOST_CHECK( space.is_void() == false );
        BOOST_CHECK( space.get_prefix() == std::string( "MyPrefix" ) );
        BOOST_CHECK( space.get_uri() == std::string( "http://The.Namespace" ) );
        BOOST_CHECK( space.is_safe() );
        BOOST_CHECK( unsafe_space.is_void() == false );
        BOOST_CHECK( unsafe_space.get_prefix() == std::string( "MyPrefix" ) );
        BOOST_CHECK( unsafe_space.get_uri() == std::string( "http://The.Namespace" ) );
        BOOST_CHECK( !unsafe_space.is_safe() );
        BOOST_CHECK( space == unsafe_space );

        unsafe_space.make_safe();

        BOOST_CHECK( unsafe_space.is_safe() );
        BOOST_CHECK( space == unsafe_space );

        xml::node               copy( *i );
        xml::ns                 space1( copy.get_namespace() );
        xml::ns                 unsafe_space1( copy.get_namespace(xml::ns::type_unsafe_ns) );

        BOOST_CHECK( space1.is_void() == false );
        BOOST_CHECK( space1.get_prefix() == std::string( "MyPrefix" ) );
        BOOST_CHECK( space1.get_uri() == std::string( "http://The.Namespace" ) );
        BOOST_CHECK( space1.is_safe() );
        BOOST_CHECK( unsafe_space1.is_void() == false );
        BOOST_CHECK( unsafe_space1.get_prefix() == std::string( "MyPrefix" ) );
        BOOST_CHECK( unsafe_space1.get_uri() == std::string( "http://The.Namespace" ) );
        BOOST_CHECK( !unsafe_space1.is_safe() );
        BOOST_CHECK( space1 == unsafe_space1 );

        unsafe_space1.make_safe();

        BOOST_CHECK( unsafe_space1.is_safe() );
        BOOST_CHECK( space1 == unsafe_space1 );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/07ex.out.real" );

        BOOST_FAIL( "Copied node namespace test (15) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/07ex.out.real" );

        BOOST_FAIL( "Copied node namespace test (15) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest16 )
{
    try
    {
        xml::document           doc( "data/08.xml", NULL );
        xml::node &             root = doc.get_root_node();
        CNcbiOstrstream         ostr;

        xml::ns                 space( "prefix", "uri" );


        xml::ns_list_type       definitions( root.get_namespace_definitions() );
        BOOST_CHECK( definitions.size() == 0 );

        root.add_namespace_definition( space, xml::node::type_throw_if_exists );
        definitions = root.get_namespace_definitions();
        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( definitions[0].get_prefix() == std::string( "prefix" ) );
        BOOST_CHECK( definitions[0].get_uri() == std::string( "uri" ) );

        ostr << doc;
        saveStreamToFile( ostr, "data/08.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/08.out" ) );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/08ex.out.real" );

        BOOST_FAIL( "Adding first namespace definition test (16) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/08ex.out.real" );

        BOOST_FAIL( "Adding first namespace definition test (16) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest17 )
{
    try
    {
        xml::document           doc( "data/09.xml", NULL );
        xml::node &             root = doc.get_root_node();
        CNcbiOstrstream         ostr;

        // Default namespace
        xml::ns                 space( "", "uri" );


        xml::ns_list_type       definitions( root.get_namespace_definitions() );
        BOOST_CHECK( definitions.size() == 0 );

        xml::ns                 unsafe_space = root.add_namespace_definition( space, xml::node::type_throw_if_exists );

        definitions = root.get_namespace_definitions();
        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( definitions[0].get_prefix() == std::string( "" ) );
        BOOST_CHECK( definitions[0].get_uri() == std::string( "uri" ) );
        BOOST_CHECK( definitions[0].is_safe() );
        BOOST_CHECK( definitions[0] == space );
        BOOST_CHECK( !unsafe_space.is_safe() );
        BOOST_CHECK( unsafe_space == space );

        ostr << doc;
        saveStreamToFile( ostr, "data/09.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/09.out" ) );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/09ex.out.real" );

        BOOST_FAIL( "Adding first default namespace definition test (17) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/09ex.out.real" );

        BOOST_FAIL( "Adding first default namespace definition test (17) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest17a )
{
    try
    {
        xml::document           doc( "data/09a.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested", root.begin() ) );

        // Before adding default namespace
        xml::ns                 nested_ns( nested->get_namespace() );

        BOOST_CHECK( nested_ns.is_void() );

        // Add default namespace
        root.add_namespace_definition( xml::ns( "", "uri" ), xml::node::type_throw_if_exists );

        // After adding default namespace at one level up
        nested_ns = nested->get_namespace();
        BOOST_CHECK( !nested_ns.is_void() );
        BOOST_CHECK( nested_ns.get_prefix() == std::string( "" ) );
        BOOST_CHECK( nested_ns.get_uri() == std::string( "uri" ) );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/09aex.out.real" );

        BOOST_FAIL( "Adding default namespace definition test (17a) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/09aex.out.real" );

        BOOST_FAIL( "Adding default namespace definition test (17a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest17b )
{
    try
    {
        xml::document           doc( "data/09b.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     foo1( root.find( "foo" ) );

        xml::node::iterator     temp( foo1 );
        xml::node::iterator     foo2( root.find( "foo", ++temp ) );

        xml::node::iterator     bar1( foo1->find( "bar" ) );
        xml::node::iterator     bar2( foo2->find( "bar" ) );
        xml::node::iterator     deep( bar1->find( "deep" ) );

        // Before adding default namespace
        xml::ns                 foo1_ns( foo1->get_namespace() );
        xml::ns                 foo2_ns( foo2->get_namespace() );
        xml::ns                 bar1_ns( bar1->get_namespace() );
        xml::ns                 bar2_ns( bar2->get_namespace() );
        xml::ns                 deep_ns( deep->get_namespace() );

        BOOST_CHECK( foo1_ns.get_prefix() == std::string( "" ) );
        BOOST_CHECK( foo1_ns.get_uri() == std::string( "" ) );
        BOOST_CHECK( foo2_ns.get_prefix() == std::string( "" ) );
        BOOST_CHECK( foo2_ns.get_uri() == std::string( "" ) );
        BOOST_CHECK( bar1_ns.get_prefix() == std::string( "" ) );
        BOOST_CHECK( bar1_ns.get_uri() == std::string( "http://existed.default.ns" ) );
        BOOST_CHECK( bar2_ns.get_prefix() == std::string( "foo_prefix" ) );
        BOOST_CHECK( bar2_ns.get_uri() == std::string( "http://another" ) );
        BOOST_CHECK( deep_ns.get_prefix() == std::string( "" ) );
        BOOST_CHECK( deep_ns.get_uri() == std::string( "http://existed.default.ns" ) );

        // Add default namespace
        root.add_namespace_definition( xml::ns( "", "uri" ), xml::node::type_throw_if_exists );

        // After adding default namespace at one level up
        foo1_ns = foo1->get_namespace();
        foo2_ns = foo2->get_namespace();
        bar1_ns = bar1->get_namespace();
        bar2_ns = bar2->get_namespace();
        deep_ns = deep->get_namespace();

        BOOST_CHECK( foo1_ns.get_prefix() == std::string( "" ) );
        BOOST_CHECK( foo1_ns.get_uri() == std::string( "uri" ) );
        BOOST_CHECK( foo2_ns.get_prefix() == std::string( "" ) );
        BOOST_CHECK( foo2_ns.get_uri() == std::string( "uri" ) );
        BOOST_CHECK( bar1_ns.get_prefix() == std::string( "" ) );
        BOOST_CHECK( bar1_ns.get_uri() == std::string( "http://existed.default.ns" ) );
        BOOST_CHECK( bar2_ns.get_prefix() == std::string( "foo_prefix" ) );
        BOOST_CHECK( bar2_ns.get_uri() == std::string( "http://another" ) );
        BOOST_CHECK( deep_ns.get_prefix() == std::string( "" ) );
        BOOST_CHECK( deep_ns.get_uri() == std::string( "http://existed.default.ns" ) );

    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/09bex.out.real" );

        BOOST_FAIL( "Adding default namespace definition test (17b) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/09bex.out.real" );

        BOOST_FAIL( "Adding default namespace definition test (17b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest18 )
{
    try
    {
        xml::document           doc( "data/10.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns                 space( xml::ns::type_void );

        BOOST_CHECK( space.is_void() );

        root.add_namespace_definition( space, xml::node::type_throw_if_exists );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Adding void namespace definition test (18) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest19 )
{
    try
    {
        xml::document           doc( "data/11.xml", NULL );
        xml::node &             root = doc.get_root_node();
        CNcbiOstrstream         ostr;

        xml::ns                 space( "prefix", "uri" );


        xml::ns_list_type       definitions( root.get_namespace_definitions() );
        BOOST_CHECK( definitions.size() == 1 );

        xml::ns                 unsafe_ns = root.add_namespace_definition( space, xml::node::type_throw_if_exists );

        definitions = root.get_namespace_definitions();
        BOOST_CHECK( definitions.size() == 2 );
        BOOST_CHECK( definitions[1].get_prefix() == std::string( "prefix" ) );
        BOOST_CHECK( definitions[1].get_uri() == std::string( "uri" ) );
        BOOST_CHECK( space == unsafe_ns );

        ostr << doc;
        saveStreamToFile( ostr, "data/11.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/11.out" ) );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/11ex.out.real" );

        BOOST_FAIL( "Adding non first namespace definition test (19) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/11ex.out.real" );

        BOOST_FAIL( "Adding non first namespace definition test (19) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest20 )
{
    try
    {
        xml::document           doc( "data/12.xml", NULL );
        xml::node &             root = doc.get_root_node();
        CNcbiOstrstream         ostr;

        xml::ns                 space( "", "uri" );


        xml::ns_list_type       definitions( root.get_namespace_definitions() );
        BOOST_CHECK( definitions.size() == 1 );

        xml::ns                 unsafe_ns = root.add_namespace_definition( space, xml::node::type_throw_if_exists );

        definitions = root.get_namespace_definitions();
        BOOST_CHECK( definitions.size() == 2 );
        BOOST_CHECK( definitions[1].get_prefix() == std::string( "" ) );
        BOOST_CHECK( definitions[1].get_uri() == std::string( "uri" ) );
        BOOST_CHECK( space == unsafe_ns );

        ostr << doc;
        saveStreamToFile( ostr, "data/12.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/12.out" ) );
    }
    catch ( std::exception &  ex )
    {
        CNcbiOstrstream    ostr;
        ostr << "Exception: " << ex.what() << "\n";
        saveStreamToFile( ostr, "data/12ex.out.real" );

        BOOST_FAIL( "Adding non first default namespace definition test (20) failed" );
    }
    catch ( ... )
    {
        CNcbiOstrstream    ostr;
        ostr << "Unknown exception\n";
        saveStreamToFile( ostr, "data/12ex.out.real" );

        BOOST_FAIL( "Adding non first default namespace definition test (20) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest21 )
{
    try
    {
        xml::document           doc( "data/13.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns                 space( "existed", "http://some/uri" );

        root.add_namespace_definition( space, xml::node::type_throw_if_exists );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Adding existed namespace definition test (21) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest22 )
{
    try
    {
        xml::document           doc( "data/14.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns                 space( "", "http://some/uri" );

        root.add_namespace_definition( space, xml::node::type_throw_if_exists );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Adding default existed namespace definition test (22) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest23 )
{
    try
    {
        xml::document           doc( "data/15.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns                 space( "existed", "http://some.other/uri" );
        CNcbiOstrstream         ostr;

        xml::ns                 unsafe_space = root.add_namespace_definition( space, xml::node::type_replace_if_exists );
        xml::ns_list_type       definitions( root.get_namespace_definitions() );

        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( space == definitions[0] );
        BOOST_CHECK( space == unsafe_space );

        ostr << doc;
        saveStreamToFile( ostr, "data/15.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/15.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Overwriting existed namespace definition test (23) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest24 )
{
    try
    {
        xml::document           doc( "data/16.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns                 space( "", "http://some.other/uri" );
        CNcbiOstrstream         ostr;

        xml::ns                 unsafe_space = root.add_namespace_definition( space, xml::node::type_replace_if_exists );
        xml::ns_list_type       definitions( root.get_namespace_definitions() );

        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( space == definitions[0] );
        BOOST_CHECK( space == unsafe_space );

        ostr << doc;
        saveStreamToFile( ostr, "data/16.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/16.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Overwriting existed default namespace definition test (24) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest25 )
{
    try
    {
        xml::document           doc( "data/17.xml", NULL );
        xml::node &             root = doc.get_root_node();
        CNcbiOstrstream         ostr;
        xml::ns_list_type       definitions;

        definitions.push_back( xml::ns( "", "http://default.name.space" ) );
        definitions.push_back( xml::ns( "prefixOne", "http://first.name.space" ) );
        definitions.push_back( xml::ns( "prefixTwo", "http://second.name.space" ) );

        root.add_namespace_definitions( definitions, xml::node::type_replace_if_exists );

        ostr << doc;
        saveStreamToFile( ostr, "data/17.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/17.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Adding many namespace definitions at a time test (25) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest26 )
{
    try
    {
        xml::document           doc( "data/18.xml", NULL );
        xml::node &             root = doc.get_root_node();
        CNcbiOstrstream         ostr;
        xml::ns_list_type       definitions( root.get_namespace_definitions(xml::ns::type_unsafe_ns) );

        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( !definitions[0].is_safe() );

        xml::node::iterator     i = root.find( "nested", root.begin() );

        BOOST_CHECK( i->get_name() == std::string( "nested" ) );

        i->add_namespace_definition( definitions[0], xml::node::type_throw_if_exists );

        ostr << doc;
        saveStreamToFile( ostr, "data/18.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/18.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Adding first unsafe namespace definition test (26) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest27 )
{
    try
    {
        xml::document           doc( "data/19.xml", NULL );
        xml::node &             root = doc.get_root_node();
        CNcbiOstrstream         ostr;
        xml::ns_list_type       definitions( root.get_namespace_definitions(xml::ns::type_unsafe_ns) );

        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( !definitions[0].is_safe() );

        xml::node::iterator     i = root.find( "nested", root.begin() );

        BOOST_CHECK( i->get_name() == std::string( "nested" ) );

        i->add_namespace_definition( definitions[0], xml::node::type_throw_if_exists );

        ostr << doc;
        saveStreamToFile( ostr, "data/19.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/19.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Adding not first unsafe namespace definition test (27) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest28 )
{
    try
    {
        xml::document           doc( "data/20.xml", NULL );
        xml::node &             root = doc.get_root_node();
        CNcbiOstrstream         ostr;
        xml::node               node( "inserted" );

        node.add_namespace_definition( xml::ns( "prefix", "uri" ), xml::node::type_throw_if_exists );
        root.insert( root.begin(), node );

        ostr << doc;
        saveStreamToFile( ostr, "data/20.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/20.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Adding a namespace definition to a free node test (28) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest29 )
{
    try
    {
        xml::document           doc( "data/21.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "nested1", root.begin() );
        xml::node::iterator     j = i->find( "nested2", i->begin() );

        xml::ns                 same_level_safe( j->lookup_namespace("bar") );
        xml::ns                 same_level_unsafe( j->lookup_namespace("bar", xml::ns::type_unsafe_ns) );

        BOOST_CHECK( same_level_safe.is_safe() );
        BOOST_CHECK( same_level_safe.get_prefix() == std::string( "bar" ) );
        BOOST_CHECK( same_level_safe.get_uri() == std::string( "http://bar" ) );

        BOOST_CHECK( !same_level_unsafe.is_safe() );
        BOOST_CHECK( same_level_unsafe.get_prefix() == std::string( "bar" ) );
        BOOST_CHECK( same_level_unsafe.get_uri() == std::string( "http://bar" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Search namespace definition test (29) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest30 )
{
    try
    {
        xml::document           doc( "data/21.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "nested1", root.begin() );
        xml::node::iterator     j = i->find( "nested2", i->begin() );

        xml::ns                 up_level_safe( j->lookup_namespace("foo") );
        xml::ns                 up_level_unsafe( j->lookup_namespace("foo", xml::ns::type_unsafe_ns) );

        BOOST_CHECK( up_level_safe.is_safe() );
        BOOST_CHECK( up_level_safe.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( up_level_safe.get_uri() == std::string( "http://foo" ) );

        BOOST_CHECK( !up_level_unsafe.is_safe() );
        BOOST_CHECK( up_level_unsafe.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( up_level_unsafe.get_uri() == std::string( "http://foo" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Search namespace definition test (30) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest31 )
{
    try
    {
        xml::document           doc( "data/21.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "nested1", root.begin() );
        xml::node::iterator     j = i->find( "nested2", i->begin() );

        xml::ns                 def_safe( j->lookup_namespace(NULL) );
        xml::ns                 def_unsafe( j->lookup_namespace(NULL, xml::ns::type_unsafe_ns) );

        BOOST_CHECK( def_safe.is_safe() );
        BOOST_CHECK( def_safe.get_prefix() == std::string( "" ) );
        BOOST_CHECK( def_safe.get_uri() == std::string( "http://default.name.space" ) );

        BOOST_CHECK( !def_unsafe.is_safe() );
        BOOST_CHECK( def_unsafe.get_prefix() == std::string( "" ) );
        BOOST_CHECK( def_unsafe.get_uri() == std::string( "http://default.name.space" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Search namespace definition test (31) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest32 )
{
    try
    {
        xml::document           doc( "data/21.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "nested1", root.begin() );
        xml::node::iterator     j = i->find( "nested2", i->begin() );

        xml::ns                 def_safe( j->lookup_namespace("") );
        xml::ns                 def_unsafe( j->lookup_namespace("", xml::ns::type_unsafe_ns) );

        BOOST_CHECK( def_safe.is_safe() );
        BOOST_CHECK( def_safe.get_prefix() == std::string( "" ) );
        BOOST_CHECK( def_safe.get_uri() == std::string( "http://default.name.space" ) );

        BOOST_CHECK( !def_unsafe.is_safe() );
        BOOST_CHECK( def_unsafe.get_prefix() == std::string( "" ) );
        BOOST_CHECK( def_unsafe.get_uri() == std::string( "http://default.name.space" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Search namespace definition test (32) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest33 )
{
    try
    {
        xml::document           doc( "data/21.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     i = root.find( "nested1", root.begin() );
        xml::node::iterator     j = i->find( "nested2", i->begin() );

        xml::ns                 notfound_safe( j->lookup_namespace("does not exist") );
        xml::ns                 notfound_unsafe( j->lookup_namespace("does not exist", xml::ns::type_unsafe_ns) );

        BOOST_CHECK( notfound_safe.is_safe() );
        BOOST_CHECK( notfound_safe.get_prefix() == std::string( "" ) );
        BOOST_CHECK( notfound_safe.get_uri() == std::string( "" ) );
        BOOST_CHECK( notfound_safe.is_void() );

        BOOST_CHECK( !notfound_unsafe.is_safe() );
        BOOST_CHECK( notfound_unsafe.get_prefix() == std::string( "" ) );
        BOOST_CHECK( notfound_unsafe.get_uri() == std::string( "" ) );
        BOOST_CHECK( notfound_unsafe.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Search namespace definition test (33) failed" );
        return;
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest34 )
{
    try
    {
        xml::document           doc( "data/22.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns_list_type       definitions( root.get_namespace_definitions() );


        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( definitions[0].get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( definitions[0].get_uri() == std::string( "http://some.name.space" ) );

        root.erase_namespace_definition( "foo" );

        definitions = root.get_namespace_definitions();
        BOOST_CHECK( definitions.size() == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erase namespace definition test (33) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest35 )
{
    try
    {
        xml::document           doc( "data/23.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns_list_type       definitions( root.get_namespace_definitions() );


        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( definitions[0].get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( definitions[0].get_uri() == std::string( "http://some.name.space" ) );

        root.erase_namespace_definition( "NotExisted" );

        definitions = root.get_namespace_definitions();
        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( definitions[0].get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( definitions[0].get_uri() == std::string( "http://some.name.space" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erase namespace definition test (34) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest36 )
{
    try
    {
        xml::document           doc( "data/24.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns_list_type       definitions( root.get_namespace_definitions() );


        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( definitions[0].get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( definitions[0].get_uri() == std::string( "http://some.name.space" ) );

        // Should generate an exception
        root.erase_namespace_definition( "foo" );
    }
    catch ( ... )
    {
        // exception is expected
        return;
    }
    BOOST_FAIL( "Erase namespace definition test (36) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest37 )
{
    try
    {
        xml::document           doc( "data/25.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns_list_type       definitions( root.get_namespace_definitions() );


        BOOST_CHECK( definitions.size() == 2 );

        root.erase_namespace_definition( "" );

        definitions = root.get_namespace_definitions();
        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( definitions[0].get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( definitions[0].get_uri() == std::string( "http://some.name.space" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erase namespace definition test (37) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest38 )
{
    try
    {
        xml::document           doc( "data/26.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::ns_list_type       definitions( root.get_namespace_definitions() );


        BOOST_CHECK( definitions.size() == 2 );

        root.erase_namespace_definition( NULL );

        definitions = root.get_namespace_definitions();
        BOOST_CHECK( definitions.size() == 1 );
        BOOST_CHECK( definitions[0].get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( definitions[0].get_uri() == std::string( "http://some.name.space" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erase namespace definition test (38) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest39 )
{
    try
    {
        xml::document           doc( "data/27.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node::iterator     deep( nested->find( "deep" ) );
        xml::ns                 name_space( deep->get_namespace() );


        BOOST_CHECK( name_space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "http://some.name.space" ) );

        deep->erase_namespace();
        name_space = deep->get_namespace();

        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erase namespace test (39) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest40 )
{
    try
    {
        xml::document           doc( "data/28.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node::iterator     deep( nested->find( "deep" ) );
        xml::ns                 name_space( deep->get_namespace() );


        BOOST_CHECK( name_space.is_void() );

        deep->erase_namespace();
        name_space = deep->get_namespace();

        BOOST_CHECK( name_space.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erase namespace test (40) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest41 )
{
    try
    {
        xml::document           doc( "data/29.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node::iterator     deep( nested->find( "deep" ) );
        xml::ns                 name_space( deep->get_namespace() );


        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );

        deep->erase_namespace();
        name_space = deep->get_namespace();

        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erase namespace test (41) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest42 )
{
    try
    {
        xml::document           doc( "data/30.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node::iterator     deep( nested->find( "deep" ) );
        xml::ns                 name_space( deep->get_namespace() );


        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );

        deep->set_namespace( "foo" );
        name_space = deep->get_namespace();

        BOOST_CHECK( name_space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "http://some.name.space" ) );

        CNcbiOstrstream         ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/30.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/30.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set namespace test (42) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest43 )
{
    try
    {
        xml::document           doc( "data/31.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node::iterator     deep( nested->find( "deep" ) );
        xml::ns                 name_space( deep->get_namespace() );


        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );

        deep->set_namespace( xml::ns( "foo", "http://some.name.space" ) );
        name_space = deep->get_namespace();

        BOOST_CHECK( name_space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "http://some.name.space" ) );

        CNcbiOstrstream         ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/31.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/31.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set namespace test (43) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest43a )
{
    try
    {
        xml::document           doc( "data/31a.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node::iterator     deep( nested->find( "deep" ) );
        xml::ns                 name_space( deep->get_namespace() );


        BOOST_CHECK( name_space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "http://some.name.space" ) );

        deep->set_namespace( xml::ns( "", "default" ) );
        name_space = deep->get_namespace();

        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );

        CNcbiOstrstream         ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/31a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/31a.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set namespace test (43a) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest44 )
{
    try
    {
        xml::document           doc( "data/32.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node::iterator     deep( nested->find( "deep" ) );
        xml::ns                 name_space( deep->get_namespace() );


        BOOST_CHECK( name_space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "http://some.name.space" ) );

        deep->set_namespace( "" );
        name_space = deep->get_namespace();

        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );

        CNcbiOstrstream         ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/32.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/32.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set namespace test (44) failed" );
        return;
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest45 )
{
    try
    {
        xml::document           doc( "data/33.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node::iterator     deep( nested->find( "deep" ) );
        xml::ns                 name_space( deep->get_namespace() );


        BOOST_CHECK( name_space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "http://some.name.space" ) );

        deep->set_namespace( NULL );
        name_space = deep->get_namespace();

        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );

        CNcbiOstrstream         ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/33.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/33.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set namespace test (45) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest46 )
{
    try
    {
        xml::document           doc( "data/34.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node::iterator     deep( nested->find( "deep" ) );
        xml::ns                 name_space( deep->get_namespace() );


        BOOST_CHECK( name_space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "http://some.name.space" ) );

        deep->set_namespace( xml::ns( xml::ns::type_void )  );
        name_space = deep->get_namespace();

        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );

        CNcbiOstrstream         ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/34.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/34.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set namespace test (46) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest47 )
{
    try
    {
        xml::document           doc( "data/35.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::ns                 name_space( nested->get_namespace() );

        BOOST_CHECK( name_space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( name_space.get_uri() == std::string( "default" ) );

        // Should generate an exception
        nested->set_namespace( "foo" );
    }
    catch ( ... )
    {
        // exception is expected
        return;
    }
    BOOST_FAIL( "Set namespace test (47) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest48 )
{
    try
    {
        xml::document           doc( "data/36.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::ns                 space( nested->get_namespace() );

        BOOST_CHECK( space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( space.get_uri() == std::string( "default" ) );

        // Should generate an exception
        // Because the namespace is defined below but not upper
        nested->set_namespace( xml::ns( "foo", "http://some.name.space" ) );
    }
    catch ( ... )
    {
        // exception is expected
        return;
    }
    BOOST_FAIL( "Set namespace test (48) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest49 )
{
    try
    {
        xml::document           doc( "data/37.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );

        // Should generate an exception
        nested->set_namespace( "" );
    }
    catch ( ... )
    {
        // exception is expected
        return;
    }
    BOOST_FAIL( "Set namespace test (49) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest50 )
{
    try
    {
        xml::document           doc( "data/38.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );

        // Should generate an exception
        nested->set_namespace( NULL );
    }
    catch ( ... )
    {
        // exception is expected
        return;
    }
    BOOST_FAIL( "Set namespace test (50) failed" );
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest51 )
{
    try
    {
        xml::document           doc( "data/39.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );

        // Should generate an exception
        nested->set_namespace( xml::ns( "foo", "wrong.uri" ) );
    }
    catch ( ... )
    {
        // exception is expected
        return;
    }
    BOOST_FAIL( "Set namespace test (51) failed" );
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest52 )
{
    try
    {
        xml::document           doc( "data/40.xml", NULL );
        xml::node &             root = doc.get_root_node();

        // Should generate an exception
        root.erase_namespace_definition( "bar" );
    }
    catch ( ... )
    {
        // exception is expected
        return;
    }
    BOOST_FAIL( "Erase namespace definition test (52) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest53 )
{
    try
    {
        xml::document           doc( "data/41.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::attributes &       attrs = nested->get_attributes();

        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     space( attr->get_namespace() );

        BOOST_CHECK( space.is_void() );

        attr->set_namespace( "foo" );
        space = attr->get_namespace();

        BOOST_CHECK( space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( space.get_uri() == std::string( "foo_ns" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set attribute namespace test (53) failed" );
        return;
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest54 )
{
    try
    {
        xml::document           doc( "data/42.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::attributes &       attrs = nested->get_attributes();

        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     space( attr->get_namespace() );

        BOOST_CHECK( space.is_void() );

        attr->set_namespace( xml::ns( "foo", "foo_ns" ) );
        space = attr->get_namespace();

        BOOST_CHECK( space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( space.get_uri() == std::string( "foo_ns" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set attribute namespace test (54) failed" );
        return;
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest55 )
{
    try
    {
        xml::document           doc( "data/43.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::attributes &       attrs = nested->get_attributes();

        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     space( attr->get_namespace() );

        BOOST_CHECK( space.is_void() );

        attr->set_namespace( xml::ns( "foo", "wrong uri" ) );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Set attribute namespace test (55) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest56 )
{
    try
    {
        xml::document           doc( "data/44.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::attributes &       attrs = nested->get_attributes();

        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     space( attr->get_namespace() );

        BOOST_CHECK( space.is_void() );

        attr->set_namespace( xml::ns( "wrong prefix", "foo_ns" ) );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Set attribute namespace test (56) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest57 )
{
    try
    {
        xml::document           doc( "data/45.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::attributes &       attrs = nested->get_attributes();

        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     space( attr->get_namespace() );

        BOOST_CHECK( space.is_void() );

        attr->set_namespace( "wrong prefix" );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Set attribute namespace test (57) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest58 )
{
    try
    {
        xml::document           doc( "data/46.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::attributes &       attrs = nested->get_attributes();

        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     space( attr->get_namespace() );

        BOOST_CHECK( space.get_prefix() == std::string( "bar" ) );
        BOOST_CHECK( space.get_uri() == std::string( "bar_ns" ) );

        attr->set_namespace( "" );
        space = attr->get_namespace();

        BOOST_CHECK( space.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set attribute namespace test (58) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest59 )
{
    try
    {
        xml::document           doc( "data/47.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::attributes &       attrs = nested->get_attributes();

        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     space( attr->get_namespace() );

        BOOST_CHECK( space.get_prefix() == std::string( "bar" ) );
        BOOST_CHECK( space.get_uri() == std::string( "bar_ns" ) );

        attr->set_namespace( NULL );
        space = attr->get_namespace();

        BOOST_CHECK( space.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set attribute namespace test (59) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( NamespaceTest60 )
{
    try
    {
        xml::document           doc( "data/48.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::attributes &       attrs = nested->get_attributes();

        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     space( attr->get_namespace() );

        BOOST_CHECK( space.get_prefix() == std::string( "bar" ) );
        BOOST_CHECK( space.get_uri() == std::string( "bar_ns" ) );

        attr->set_namespace( xml::ns( "", "http://default.ns" ) );
        space = attr->get_namespace();

        BOOST_CHECK( space.is_void() == false );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set attribute namespace test (60) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest61 )
{
    try
    {
        xml::document           doc( "data/49.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::attributes &       attrs = nested->get_attributes();

        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     space( attr->get_namespace() );

        BOOST_CHECK( space.get_prefix() == std::string( "bar" ) );
        BOOST_CHECK( space.get_uri() == std::string( "bar_ns" ) );

        attr->set_namespace( xml::ns( xml::ns::type_void ) );
        space = attr->get_namespace();

        BOOST_CHECK( space.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set attribute namespace test (61) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest62 )
{
    try
    {
        xml::document           doc( "data/50.xml", NULL );
        xml::node &             root = doc.get_root_node();
        xml::node::iterator     nested( root.find( "nested" ) );
        xml::node               newNode( "inserted" );

        root.insert( nested, newNode );

        xml::node::iterator     inserted( root.find( "inserted" ) );
        xml::ns                 space( inserted->get_namespace() );

        BOOST_CHECK( space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( space.get_uri() == std::string( "default.namespace" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set attribute namespace test (62) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest63 )
{
    try
    {
        xml::document               doc( "data/51.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::node::iterator         nested( root.find( "nested" ) );
        xml::ns                     rootspace( root.get_namespace() );
        xml::ns                     nestedspace( nested->get_namespace() );

        xml::attributes &           attrs( nested->get_attributes() );
        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     attrspace( attr->get_namespace() );

        BOOST_CHECK( rootspace.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( rootspace.get_uri() == std::string( "foo_ns" ) );
        BOOST_CHECK( nestedspace.get_prefix() == std::string( "bar" ) );
        BOOST_CHECK( nestedspace.get_uri() == std::string( "bar_ns" ) );
        BOOST_CHECK( attrspace.get_prefix() == std::string( "bar" ) );
        BOOST_CHECK( attrspace.get_uri() == std::string( "bar_ns" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Set attribute namespace test (63) failed" );
    }
    return;
}


// Note: attributes will have a namespace only if a prefix explicitly
//       used. In other words an attribute will never have a default namespace


BOOST_AUTO_TEST_CASE( NamespaceTest64 )
{
    try
    {
        std::string     doc_str( "<root xmlns=\"http://The.Default.Namespace\">"
                                 "<nested attr=\"one\">value</nested>"
                                 "</root>" );
        xml::document   doc( doc_str.c_str(), doc_str.size(), NULL );
        xml::node &     root( doc.get_root_node() );

        root.erase_namespace_definition( NULL,
                                         xml::node::type_ns_def_erase_enforce );
        xml::ns         rootspace( root.get_namespace() );

        BOOST_CHECK( rootspace.is_void() );

        xml::node::iterator         nested( root.find( "nested" ) );
        xml::ns                     nestedspace( nested->get_namespace() );

        BOOST_CHECK( nestedspace.is_void() );

        xml::attributes &           attrs( nested->get_attributes() );
        xml::attributes::iterator   attr( attrs.find( "attr" ) );
        xml::ns                     attrspace( attr->get_namespace() );

        BOOST_CHECK( attrspace.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Remove default NS definition when "
                    "no other defined test (64) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( NamespaceTest65 )
{
    try
    {
        std::string     doc_str( "<root xmlns=\"http://The.Default.Namespace\">"
                                 "<nested xmlns=\"http://t.d.n\">"
                                     "<nested2 attr=\"one\">value</nested2>"
                                 "</nested>"
                                 "</root>" );
        xml::document   doc( doc_str.c_str(), doc_str.size(), NULL );
        xml::node &     root( doc.get_root_node() );

        BOOST_CHECK( root.get_namespace().get_uri() == std::string( "http://The.Default.Namespace" ) );

        xml::node::iterator         nested( root.find( "nested" ) );

        BOOST_CHECK( nested->get_namespace().get_uri() == std::string( "http://t.d.n" ) );

        xml::node::iterator         nested2( nested->find( "nested2" ) );
        xml::attributes &           attrs( nested2->get_attributes() );
        xml::attributes::iterator   attr( attrs.find( "attr" ) );

        BOOST_CHECK( attr->get_namespace().is_void() );

        nested->erase_namespace_definition( NULL,
                                            xml::node::type_ns_def_erase_enforce );
        xml::ns         nestedspace( nested->get_namespace() );

        BOOST_CHECK( nestedspace.is_void() == false );
        BOOST_CHECK( nestedspace.get_prefix() == std::string( "" ) );
        BOOST_CHECK( nestedspace.get_uri() == std::string( "http://The.Default.Namespace" ) );

        xml::ns         nested2space( nested2->get_namespace() );

        BOOST_CHECK( nested2space.is_void() == false );
        BOOST_CHECK( nested2space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( nested2space.get_uri() == std::string( "http://The.Default.Namespace" ) );

        xml::ns                     attrspace( attr->get_namespace() );

        BOOST_CHECK( attrspace.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Remove default NS definition when "
                    "another defined above; test (65) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NamespaceTest66 )
{
    try
    {
        std::string     doc_str( "<root xmlns=\"http://The.Default.Namespace\">"
                                 "<foo:nested xmlns:foo=\"http://t.nd.n\">"
                                     "<foo:nested2 foo:attr=\"one\">value</foo:nested2>"
                                 "</foo:nested>"
                                 "</root>" );
        xml::document   doc( doc_str.c_str(), doc_str.size(), NULL );
        xml::node &     root( doc.get_root_node() );

        BOOST_CHECK( root.get_namespace().get_uri() == std::string( "http://The.Default.Namespace" ) );

        xml::node::iterator         nested( root.find( "nested" ) );

        BOOST_CHECK( nested->get_namespace().get_uri() == std::string( "http://t.nd.n" ) );

        xml::node::iterator         nested2( nested->find( "nested2" ) );
        xml::attributes &           attrs( nested2->get_attributes() );
        xml::attributes::iterator   attr( attrs.find( "attr" ) );

        BOOST_CHECK( attr->get_namespace().is_void() == false );
        BOOST_CHECK( attr->get_namespace().get_prefix() == std::string( "foo" ) );

        nested->erase_namespace_definition( "foo",
                                            xml::node::type_ns_def_erase_enforce );
        xml::ns         nestedspace( nested->get_namespace() );

        BOOST_CHECK( nestedspace.is_void() == false );
        BOOST_CHECK( nestedspace.get_prefix() == std::string( "" ) );
        BOOST_CHECK( nestedspace.get_uri() == std::string( "http://The.Default.Namespace" ) );

        xml::ns         nested2space( nested2->get_namespace() );

        BOOST_CHECK( nested2space.is_void() == false );
        BOOST_CHECK( nested2space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( nested2space.get_uri() == std::string( "http://The.Default.Namespace" ) );

        xml::ns                     attrspace( attr->get_namespace() );

        BOOST_CHECK( attrspace.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Remove non default NS definition when "
                    "another default NS defined above; test (66) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NamespaceTest67 )
{
    try
    {
        std::string     doc_str( "<root>"
                                 "<foo:nested xmlns=\"http://The.Default.Namespace\" xmlns:foo=\"http://t.nd.n\">"
                                     "<foo:nested2 foo:attr=\"one\">value</foo:nested2>"
                                 "</foo:nested>"
                                 "</root>" );
        xml::document   doc( doc_str.c_str(), doc_str.size(), NULL );
        xml::node &     root( doc.get_root_node() );

        BOOST_CHECK( root.get_namespace().is_void() );

        xml::node::iterator         nested( root.find( "nested" ) );

        BOOST_CHECK( nested->get_namespace().get_uri() == std::string( "http://t.nd.n" ) );

        xml::node::iterator         nested2( nested->find( "nested2" ) );
        xml::attributes &           attrs( nested2->get_attributes() );
        xml::attributes::iterator   attr( attrs.find( "attr" ) );

        BOOST_CHECK( attr->get_namespace().is_void() == false );
        BOOST_CHECK( attr->get_namespace().get_prefix() == std::string( "foo" ) );

        nested->erase_namespace_definition( "foo",
                                            xml::node::type_ns_def_erase_enforce );
        xml::ns         nestedspace( nested->get_namespace() );

        BOOST_CHECK( nestedspace.is_void() == false );
        BOOST_CHECK( nestedspace.get_prefix() == std::string( "" ) );
        BOOST_CHECK( nestedspace.get_uri() == std::string( "http://The.Default.Namespace" ) );

        xml::ns         nested2space( nested2->get_namespace() );

        BOOST_CHECK( nested2space.is_void() == false );
        BOOST_CHECK( nested2space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( nested2space.get_uri() == std::string( "http://The.Default.Namespace" ) );

        xml::ns                     attrspace( attr->get_namespace() );

        BOOST_CHECK( attrspace.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Remove non default NS definition when "
                    "another default NS defined right here; test (67) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NamespaceTest68 )
{
    try
    {
        std::string     doc_str( "<root>"
                                 "<foo:nested xmlns:foo=\"http://t.nd.n\">"
                                     "<foo:nested2 foo:attr=\"one\">value</foo:nested2>"
                                 "</foo:nested>"
                                 "</root>" );
        xml::document   doc( doc_str.c_str(), doc_str.size(), NULL );
        xml::node &     root( doc.get_root_node() );

        BOOST_CHECK( root.get_namespace().is_void() );

        xml::node::iterator         nested( root.find( "nested" ) );

        BOOST_CHECK( nested->get_namespace().get_uri() == std::string( "http://t.nd.n" ) );

        xml::node::iterator         nested2( nested->find( "nested2" ) );
        xml::attributes &           attrs( nested2->get_attributes() );
        xml::attributes::iterator   attr( attrs.find( "attr" ) );

        BOOST_CHECK( attr->get_namespace().is_void() == false );
        BOOST_CHECK( attr->get_namespace().get_prefix() == std::string( "foo" ) );

        nested->erase_namespace_definition( "foo",
                                            xml::node::type_ns_def_erase_enforce );
        xml::ns         nestedspace( nested->get_namespace() );

        BOOST_CHECK( nestedspace.is_void() );

        xml::ns         nested2space( nested2->get_namespace() );

        BOOST_CHECK( nested2space.is_void() );

        xml::ns         attrspace( attr->get_namespace() );

        BOOST_CHECK( attrspace.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Remove non default NS definition when "
                    "there is no default NS defined; test (68) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NamespaceTest69 )
{
    try
    {
        std::string     doc_str( "<root xmlns:foo=\"http://one\">"
                                     "<foo:nested xmlns:foo=\"http://two\">"
                                         "<foo:nested2 xmlns:foo=\"http://three\" foo:attr=\"one\">value</foo:nested2>"
                                     "</foo:nested>"
                                 "</root>" );
        xml::document   doc( doc_str.c_str(), doc_str.size(), NULL );
        xml::node &     root( doc.get_root_node() );

        BOOST_CHECK( root.get_namespace().is_void() );

        xml::node::iterator         nested( root.find( "nested" ) );

        BOOST_CHECK( nested->get_namespace().get_uri() == std::string( "http://two" ) );

        xml::node::iterator         nested2( nested->find( "nested2" ) );
        xml::attributes &           attrs( nested2->get_attributes() );
        xml::attributes::iterator   attr( attrs.find( "attr" ) );

        BOOST_CHECK( attr->get_namespace().is_void() == false );
        BOOST_CHECK( attr->get_namespace().get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( attr->get_namespace().get_uri() == std::string( "http://three" ) );

        nested->erase_namespace_definition( "foo",
                                            xml::node::type_ns_def_erase_enforce );
        xml::ns         nestedspace( nested->get_namespace() );

        BOOST_CHECK( nestedspace.is_void() );

        xml::ns         nested2space( nested2->get_namespace() );

        BOOST_CHECK( nested2space.is_void() == false );
        BOOST_CHECK( nested2space.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( nested2space.get_uri() == std::string( "http://three" ) );

        xml::ns         attrspace( attr->get_namespace() );

        BOOST_CHECK( attrspace.is_void() == false );
        BOOST_CHECK( attrspace.get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( attrspace.get_uri() == std::string( "http://three" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Remove non default NS definition when "
                    "there is same prefix NS above and below; test (69) failed" );
    }
}


BOOST_AUTO_TEST_CASE( NamespaceTest70 )
{
    try
    {
        std::string     doc_str( "<root xmlns=\"http://one\">"
                                     "<nested xmlns=\"http://two\">"
                                         "<nested2 xmlns=\"http://three\" attr=\"one\">value</nested2>"
                                     "</nested>"
                                 "</root>" );
        xml::document   doc( doc_str.c_str(), doc_str.size(), NULL );
        xml::node &     root( doc.get_root_node() );

        BOOST_CHECK( root.get_namespace().is_void() == false );
        BOOST_CHECK( root.get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( root.get_namespace().get_uri() == std::string( "http://one" ) );

        xml::node::iterator         nested( root.find( "nested" ) );

        BOOST_CHECK( nested->get_namespace().get_uri() == std::string( "http://two" ) );

        xml::node::iterator         nested2( nested->find( "nested2" ) );
        xml::attributes &           attrs( nested2->get_attributes() );
        xml::attributes::iterator   attr( attrs.find( "attr" ) );

        BOOST_CHECK( attr->get_namespace().is_void() );

        nested->erase_namespace_definition( "",
                                            xml::node::type_ns_def_erase_enforce );
        xml::ns         nestedspace( nested->get_namespace() );

        BOOST_CHECK( nestedspace.is_void() == false );
        BOOST_CHECK( nestedspace.get_prefix() == std::string( "" ) );
        BOOST_CHECK( nestedspace.get_uri() == std::string( "http://one" ) );

        xml::ns         nested2space( nested2->get_namespace() );

        BOOST_CHECK( nested2space.is_void() == false );
        BOOST_CHECK( nested2space.get_prefix() == std::string( "" ) );
        BOOST_CHECK( nested2space.get_uri() == std::string( "http://three" ) );

        xml::ns         attrspace( attr->get_namespace() );

        BOOST_CHECK( attrspace.is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Remove default NS definition when "
                    "there is default NS above and below; test (70) failed" );
    }
}

