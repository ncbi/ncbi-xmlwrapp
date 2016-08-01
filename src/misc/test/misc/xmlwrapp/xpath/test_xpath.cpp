/*  $Id: test_xpath.cpp 435003 2014-05-13 15:08:30Z ucko $
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


//
// xpath_expression tests
//

BOOST_AUTO_TEST_CASE( XPathTest01 )
{
    try
    {
        xml::xpath_expression       expr( "/root/child" );

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 0 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_no_compile );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xpath_expression failed (01)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest02 )
{
    try
    {
        xml::xpath_expression       expr( "/root/child" );

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 0 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_no_compile );

        expr.compile();
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Compiling xpath_expression failed (02)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest03 )
{
    try
    {
        xml::xpath_expression       expr( "/root/child", xml::xpath_expression::type_compile );

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 0 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );

        expr.compile();
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing compiled xpath_expression failed (03)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest04 )
{
    try
    {
        xml::xpath_expression       expr( "/root/child", xml::ns( "prefix", "uri" ) );

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 1 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_no_compile );

        expr.compile();
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xpath_expression failed (04)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest05 )
{
    try
    {
        xml::xpath_expression       expr( "/root/child", xml::ns( "prefix", "uri" ),
                                          xml::xpath_expression::type_compile );

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 1 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );

        expr.compile();
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing compiled xpath_expression failed (05)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest06 )
{
    try
    {
        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "prefix", "uri" ) );
        ns_list.push_back( xml::ns( "some", "another" ) );

        xml::xpath_expression       expr( "/root/child", ns_list );

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 2 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_no_compile );

        expr.compile();
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xpath_expression failed (06)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest07 )
{
    try
    {
        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "prefix", "uri" ) );
        ns_list.push_back( xml::ns( "some", "another" ) );

        xml::xpath_expression       expr( "/root/child", ns_list,
                                          xml::xpath_expression::type_compile );

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 2 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );

        expr.compile();
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing compiled xpath_expression failed (07)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest08 )
{
    try
    {
        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "prefix", "uri" ) );
        ns_list.push_back( xml::ns( "some", "another" ) );

        xml::xpath_expression       expr( "/root/child", ns_list,
                                          xml::xpath_expression::type_no_compile );
        xml::xpath_expression       expr2( expr );

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 2 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_no_compile );

        BOOST_CHECK( expr2.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr2.get_namespaces().size() == 2 );
        BOOST_CHECK( expr2.get_compile_type() == xml::xpath_expression::type_no_compile );

    }
    catch ( ... )
    {
        BOOST_FAIL( "Copy constructing xpath_expression failed (08)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest09 )
{
    try
    {
        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "prefix", "uri" ) );
        ns_list.push_back( xml::ns( "some", "another" ) );

        xml::xpath_expression       expr( "/root/child", ns_list,
                                          xml::xpath_expression::type_compile );
        xml::xpath_expression       expr2( expr );

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 2 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );

        BOOST_CHECK( expr2.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr2.get_namespaces().size() == 2 );
        BOOST_CHECK( expr2.get_compile_type() == xml::xpath_expression::type_compile );

    }
    catch ( ... )
    {
        BOOST_FAIL( "Copy constructing xpath_expression failed (09)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest10 )
{
    try
    {
        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "prefix", "uri" ) );
        ns_list.push_back( xml::ns( "some", "another" ) );

        xml::xpath_expression       expr( "/root/child", ns_list,
                                          xml::xpath_expression::type_compile );
        xml::xpath_expression       expr2( "/root/child", ns_list,
                                           xml::xpath_expression::type_compile );

        expr2 = expr;

        BOOST_CHECK( expr.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr.get_namespaces().size() == 2 );
        BOOST_CHECK( expr.get_compile_type() == xml::xpath_expression::type_compile );

        BOOST_CHECK( expr2.get_xpath() == std::string( "/root/child" ) );
        BOOST_CHECK( expr2.get_namespaces().size() == 2 );
        BOOST_CHECK( expr2.get_compile_type() == xml::xpath_expression::type_compile );

    }
    catch ( ... )
    {
        BOOST_FAIL( "Assigning xpath_expression failed (10)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest11 )
{
    try
    {
        xml::xpath_expression       expr( "" );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "An empty xpath expression  test (11) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest12 )
{
    try
    {
        xml::xpath_expression       expr( "", xml::ns( "prefix", "uri" ) );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "An empty xpath expression  test (12) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest13 )
{
    try
    {
        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "prefix", "uri" ) );
        ns_list.push_back( xml::ns( "", "another" ) );

        xml::xpath_expression       expr( "", ns_list,
                                          xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "An empty xpath expression  test (13) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest14 )
{
    try
    {
        xml::xpath_expression       expr( NULL );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "An empty xpath expression  test (14) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest15 )
{
    try
    {
        xml::xpath_expression       expr( NULL, xml::ns( "prefix", "uri" ) );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "An empty xpath expression  test (15) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest16 )
{
    try
    {
        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "prefix", "uri" ) );
        ns_list.push_back( xml::ns( "some", "another" ) );

        xml::xpath_expression       expr( NULL, ns_list,
                                          xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "An empty xpath expression  test (16) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest16a )
{
    try
    {
        xml::xpath_expression       expr( "/root/child", xml::ns( "", "uri" ),
                                          xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "A default namespace test (16a) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest16b )
{
    try
    {
        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "prefix", "uri" ) );
        ns_list.push_back( xml::ns( "", "another" ) );

        xml::xpath_expression       expr( "/root/child", ns_list,
                                          xml::xpath_expression::type_compile );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "An empty xpath expression  test (16b) failed" );
    return;
}



//
// node_set tests
//


BOOST_AUTO_TEST_CASE( XPathTest17 )
{
    try
    {
        xml::document               doc( "data/17.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::node_set               nset( root.run_xpath_query( "/root/child" ) );
        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node set creation test failed (17)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest18 )
{
    try
    {
        xml::document               doc( "data/18.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        const xml::node_set         nset( root.run_xpath_query( "/root/child" ) );
        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node set creation test failed (18)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest19 )
{
    try
    {
        xml::document               doc( "data/19.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::node_set               nset( root.run_xpath_query( "/root/nonexistedchild" ) );
        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Empty node set creation test failed (19)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest20 )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        const xml::node_set         nset( root.run_xpath_query( "/root/nonexistedchild" ) );
        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Empty node set creation test failed (20)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest21 )
{
    try
    {
        xml::node_set           nset;
        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Empty node set creation test failed (21)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest22 )
{
    try
    {
        xml::document               doc( "data/22.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/child",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node set creation test failed (22)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest23 )
{
    try
    {
        xml::document               doc( "data/23.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/child",
                                          xml::xpath_expression::type_compile );
        const xml::node_set         nset( root.run_xpath_query( expr ) );
        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Node set creation test failed (23)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest24 )
{
    try
    {
        xml::document               doc( "data/24.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "prefix", "uri" ) );
        ns_list.push_back( xml::ns( "this", "another" ) );

        xml::xpath_expression       expr( "/root/child", ns_list,
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset1( root.run_xpath_query( expr ) );
        xml::node_set               nset2( nset1 );
        xml::node_set               nset3;

        nset3 = nset1;
        nset3 = nset2;
        nset3 = nset3;

        BOOST_CHECK( nset1.size() == 1 );
        BOOST_CHECK( nset1.empty() == false );
        BOOST_CHECK( nset2.size() == 1 );
        BOOST_CHECK( nset2.empty() == false );
        BOOST_CHECK( nset3.size() == 1 );
        BOOST_CHECK( nset3.empty() == false );
    }
    catch ( ... )
    {
        BOOST_FAIL( "node_set creation and copying error (24)" );
    }
    return;
}


//
// node_set:const_iterator tests
//

BOOST_AUTO_TEST_CASE( XPathTest25 )
{
    try
    {
        xml::node_set::const_iterator   k;
        ++k;
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Uninitialised iterator advancing test (25) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest26 )
{
    try
    {
        xml::node_set::const_iterator   k;
        k++;
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Uninitialised iterator advancing test (26) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest27 )
{
    try
    {
        xml::node_set::const_iterator   k;
        xml::node                       n( *k );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Uninitialised iterator dereferencing test (27) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest28 )
{
    try
    {
        xml::node_set::const_iterator   k;
        xml::node                       n( *(k.operator->()) );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Uninitialised iterator dereferencing test (28) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest29 )
{
    try
    {
        xml::document                   doc( "data/29.xml", NULL );
        xml::node &                     root = doc.get_root_node();

        xml::xpath_expression           expr( "/root/nonexistedchild",
                                              xml::xpath_expression::type_compile );
        xml::node_set                   nset( root.run_xpath_query( expr ) );
        xml::node_set::const_iterator   k( nset.begin() );

        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
        BOOST_CHECK( k == nset.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "end() iterator test (29) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest30 )
{
    try
    {
        xml::document                   doc( "data/30.xml", NULL );
        xml::node &                     root = doc.get_root_node();

        xml::xpath_expression           expr( "/root/nonexistedchild",
                                              xml::xpath_expression::type_compile );
        xml::node_set                   nset( root.run_xpath_query( expr ) );
        xml::node_set::const_iterator   k( nset.begin() );

        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
        BOOST_CHECK( k == nset.end() );

        ++k;
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "advancing end() iterator test (30) failed" );
}


BOOST_AUTO_TEST_CASE( XPathTest31 )
{
    try
    {
        xml::document                   doc( "data/31.xml", NULL );
        xml::node &                     root = doc.get_root_node();

        xml::xpath_expression           expr( "/root/nonexistedchild",
                                              xml::xpath_expression::type_compile );
        xml::node_set                   nset( root.run_xpath_query( expr ) );
        xml::node_set::const_iterator   k( nset.begin() );

        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
        BOOST_CHECK( k == nset.end() );

        k++;
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "advancing end() iterator test (31) failed" );
}


BOOST_AUTO_TEST_CASE( XPathTest32 )
{
    try
    {
        xml::document                   doc( "data/32.xml", NULL );
        xml::node &                     root = doc.get_root_node();

        xml::xpath_expression           expr( "/root/child",
                                              xml::xpath_expression::type_compile );
        xml::node_set                   nset( root.run_xpath_query( expr ) );
        xml::node_set::const_iterator   k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
        ++k;
        BOOST_CHECK( k == nset.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "advancing iterator test (32) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest33 )
{
    try
    {
        xml::document                   doc( "data/33.xml", NULL );
        xml::node &                     root = doc.get_root_node();

        xml::xpath_expression           expr( "/root/child",
                                              xml::xpath_expression::type_compile );
        xml::node_set                   nset( root.run_xpath_query( expr ) );
        xml::node_set::const_iterator   k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
        k++;
        BOOST_CHECK( k == nset.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "advancing iterator test (33) failed" );
    }
    return;
}



//
// node_set:iterator tests
//

BOOST_AUTO_TEST_CASE( XPathTest34 )
{
    try
    {
        xml::node_set::iterator     k;
        ++k;
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Uninitialised iterator advancing test (34) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest35 )
{
    try
    {
        xml::node_set::iterator     k;
        k++;
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Uninitialised iterator advancing test (35) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest36 )
{
    try
    {
        xml::node_set::iterator     k;
        xml::node                   n( *k );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Uninitialised iterator dereferencing test (36) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest37 )
{
    try
    {
        xml::node_set::iterator     k;
        xml::node                   n( *(k.operator->()) );
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "Uninitialised iterator dereferencing test (37) failed" );
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest38 )
{
    try
    {
        xml::document               doc( "data/38.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/nonexistedchild",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
        BOOST_CHECK( k == nset.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "end() iterator test (38) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest39 )
{
    try
    {
        xml::document               doc( "data/39.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/nonexistedchild",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
        BOOST_CHECK( k == nset.end() );

        ++k;
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "advancing end() iterator test (39) failed" );
}


BOOST_AUTO_TEST_CASE( XPathTest40 )
{
    try
    {
        xml::document               doc( "data/40.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/nonexistedchild",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
        BOOST_CHECK( k == nset.end() );

        k++;
    }
    catch ( ... )
    {
        // Exception is expected
        return;
    }
    BOOST_FAIL( "advancing end() iterator test (40) failed" );
}


BOOST_AUTO_TEST_CASE( XPathTest41 )
{
    try
    {
        xml::document               doc( "data/41.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/child",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
        ++k;
        BOOST_CHECK( k == nset.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "advancing iterator test (41) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest42 )
{
    try
    {
        xml::document               doc( "data/42.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/child",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
        k++;
        BOOST_CHECK( k == nset.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "advancing iterator test (42) failed" );
    }
    return;
}



//
// node content manipulation tests
//

BOOST_AUTO_TEST_CASE( XPathTest43 )
{
    try
    {
        xml::document               doc( "data/43.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/child",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
        k->set_content( "replaced_value" );

        CNcbiOstrstream             ostr;
        ostr << root << std::endl;

        saveStreamToFile( ostr, "data/43.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/43.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/43.out.real" );
        BOOST_FAIL( "change node value test (43) failed" );
    }
    return;
}



//
// selceting nodes with namespaces
//

BOOST_AUTO_TEST_CASE( XPathTest44 )
{
    try
    {
        xml::document               doc( "data/44.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "a", "http://bla.com" ) );
        xml::xpath_expression       expr( "/root/a:child", ns_list,
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
        k->set_content( "replaced_value" );

        CNcbiOstrstream             ostr;
        ostr << root << std::endl;

        saveStreamToFile( ostr, "data/44.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/44.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/44.out.real" );
        BOOST_FAIL( "change node value test (44) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest44a )
{
    try
    {
        xml::document               doc( "data/44.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        // The effective namespaces are registered implicitly
        xml::node_set               nset( root.run_xpath_query( "/root/a:child" ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
        k->set_content( "replaced_value" );

        CNcbiOstrstream             ostr;
        ostr << root << std::endl;

        saveStreamToFile( ostr, "data/44a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/44.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/44a.out.real" );
        BOOST_FAIL( "change node value test (44a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest45 )
{
    try
    {
        xml::document               doc( "data/45.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "a", "http://bla.com" ) );
        ns_list.push_back( xml::ns( "a", "http://is.not.in.the.document" ) );
        xml::xpath_expression       expr( "/root/a:child", ns_list,
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );

        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "change node value test (45) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest46 )
{
    try
    {
        xml::document               doc( "data/46.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "a", "http://is.not.in.the.document" ) );
        xml::xpath_expression       expr( "/root/a:child", ns_list,
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );

        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "change node value test (46) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest47 )
{
    try
    {
        // The node is not going to be selected because its namespace is not
        // given in the query string
        xml::document               doc( "data/47.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "a", "http://bla.com" ) );
        xml::xpath_expression       expr( "/root/child", ns_list,
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );

        BOOST_CHECK( nset.size() == 0 );
        BOOST_CHECK( nset.empty() == true );
    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "change node value test (47) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest48 )
{
    try
    {
        xml::document               doc( "data/48.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::ns_list_type           ns_list;
        ns_list.push_back( xml::ns( "default", "http://bla.com" ) );
        xml::xpath_expression       expr( "/default:root/default:child", ns_list,
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
        k->set_content( "replaced_value" );

        CNcbiOstrstream             ostr;
        ostr << root << std::endl;

        saveStreamToFile( ostr, "data/48.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/48.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/48.out.real" );
        BOOST_FAIL( "change node value test (48) failed" );
    }
    return;
}


//
// selecting node content only:
// libxml2 returns a node anyway regardless that the text() function is used
//

BOOST_AUTO_TEST_CASE( XPathTest49 )
{
    try
    {
        xml::document               doc( "data/49.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/child/text()",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
        k->set_content( "replaced_value" );

        CNcbiOstrstream             ostr;
        ostr << root << std::endl;

        saveStreamToFile( ostr, "data/49.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/49.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/49.out.real" );
        BOOST_FAIL( "change node value test (49) failed" );
    }
    return;
}


//
// using compiled expression twice
//

BOOST_AUTO_TEST_CASE( XPathTest50 )
{
    try
    {
        xml::document               doc1( "data/50a.xml", NULL );
        xml::node &                 root1 = doc1.get_root_node();
        xml::document               doc2( "data/50b.xml", NULL );
        xml::node &                 root2 = doc2.get_root_node();

        xml::xpath_expression       expr( "/root/child",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset1( root1.run_xpath_query( expr ) );
        xml::node_set::iterator     k1( nset1.begin() );
        xml::node_set               nset2( root2.run_xpath_query( expr ) );
        xml::node_set::iterator     k2( nset2.begin() );

        BOOST_CHECK( nset1.size() == 1 );
        BOOST_CHECK( nset1.empty() == false );
        BOOST_CHECK( nset2.size() == 1 );
        BOOST_CHECK( nset2.empty() == false );

        BOOST_CHECK( k1->get_content() == std::string( "value" ) );
        k1->set_content( "replaced_value" );
        BOOST_CHECK( k2->get_content() == std::string( "value" ) );
        k2->set_content( "replaced_value" );

        CNcbiOstrstream             ostr1;
        ostr1 << root1 << std::endl;
        CNcbiOstrstream             ostr2;
        ostr2 << root2 << std::endl;

        saveStreamToFile( ostr1, "data/50a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr1, "data/50a.out" ) );
        saveStreamToFile( ostr2, "data/50b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr2, "data/50b.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/50.out.real" );
        BOOST_FAIL( "change node value test (50) failed" );
    }
    return;
}



//
// selecting attributes
//

BOOST_AUTO_TEST_CASE( XPathTest51 )
{
    try
    {
        xml::document               doc( "data/51.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/child/@attr1",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 1 );
        BOOST_CHECK( nset.empty() == false );

        k->set_content( "new_value" );

        CNcbiOstrstream             ostr;
        ostr << root << std::endl;

        saveStreamToFile( ostr, "data/51.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/51.out" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/51.out.real" );
        BOOST_FAIL( "attribute selection test (51) failed" );
    }
    return;
}



//
// using the http://jcooney.net/archive/2005/08/09/6517.aspx
//

BOOST_AUTO_TEST_CASE( XPathTest52 )
{
    try
    {
        xml::document               doc( "data/52.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "//*[local-name()='child']",
                                          xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k( nset.begin() );

        BOOST_CHECK( nset.size() == 3 );
        BOOST_CHECK( nset.empty() == false );

        BOOST_CHECK( k->get_content() == std::string( "value" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/52.out.real" );
        BOOST_FAIL( "local-name() selected nodes test (52) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest53 )
{
    try
    {
        xml::document                   doc( "data/53.xml", NULL );
        xml::node &                     root = doc.get_root_node();

        xml::xpath_expression           expr( "/root/child",
                                              xml::xpath_expression::type_compile );
        xml::node_set                   nset( root.run_xpath_query( expr ) );

        /* Checks that the references to the node_set nodes are valid longer
         * than iterators. It was SIGSEGV in the initial implementation */
        const xml::node &               node1 = *( nset.begin() );
        xml::node &                     node2 = *( nset.begin() );

        BOOST_CHECK( node1.get_path() == std::string( "/root/child" ) );
        BOOST_CHECK( node2.get_path() == std::string( "/root/child" ) );

        /* Change the value in the original document */
        node2.set_content( "new content" );
        CNcbiOstrstream ostr;
        ostr << root << std::endl;
        saveStreamToFile( ostr, "data/53.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/53.out" ) );

        /* The node copy should cut the leading path */
        xml::node                       node3 = *( nset.begin() );
        BOOST_CHECK( node3.get_path() == std::string( "/child" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/53.out.real" );
        BOOST_FAIL( "node_set nodes references test failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest54 )
{
    try
    {
        xml::document                   doc( "data/54.xml", NULL );
        xml::node &                     root = doc.get_root_node();

        xml::xpath_expression           expr( "/root/child",
                                              xml::xpath_expression::type_compile );
        xml::node_set                   nset( root.run_xpath_query( expr ) );

        xml::node &                     node1 = *( nset.begin() );
        xml::node &                     node2 = *( ++nset.begin() );

        BOOST_CHECK( node1.get_path() == std::string( "/root/child[1]" ) );
        BOOST_CHECK( node2.get_path() == std::string( "/root/child[2]" ) );
        BOOST_CHECK( node1.get_content() == std::string( "a" ) );
        BOOST_CHECK( node2.get_content() == std::string( "b" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/54.out.real" );
        BOOST_FAIL( "node_set nodes references test failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest55 )
{
    try
    {
        xml::document               doc( "data/55.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        xml::xpath_expression       expr( "/root/child",
                                          xml::xpath_expression::type_compile );
        xml::xpath_expression       nested_expr( "nested",
                                                 xml::xpath_expression::type_compile );
        xml::node_set               nset( root.run_xpath_query( expr ) );
        xml::node_set::iterator     k = nset.begin();

        BOOST_CHECK( nset.size() == 3 );
        BOOST_CHECK( k->get_path() == std::string( "/root/child[1]" ) );

        xml::node_set               nested_set( k->run_xpath_query( nested_expr ) );
        BOOST_CHECK( nested_set.size() == 0 );

        ++k;
        BOOST_CHECK( k->get_path() == std::string( "/root/child[2]" ) );
        nested_set = k->run_xpath_query( nested_expr );
        BOOST_CHECK( nested_set.size() == 2 );

        xml::node_set::iterator     j = nested_set.begin();
        BOOST_CHECK( j->get_path() == std::string( "/root/child[2]/nested[1]" ) );
        ++j;
        BOOST_CHECK( j->get_path() == std::string( "/root/child[2]/nested[2]" ) );

        ++k;
        BOOST_CHECK( k->get_path() == std::string( "/root/child[3]" ) );
        nested_set = k->run_xpath_query( nested_expr );
        BOOST_CHECK( nested_set.size() == 0 );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/55.out.real" );
        BOOST_FAIL( "nested xpath query test (55) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathTest56 )
{
    try
    {
        xml::document               doc( "data/56.xml", NULL );
        xml::node &                 root = doc.get_root_node();

        // This must not generte an exception
        xml::node_set               nset( root.run_xpath_query( "/Workbook" ) );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/56.out.real" );
        BOOST_FAIL( "Default namespace test (56) failed" );
    }
}





xml::node &  find_node(xml::document &  doc )
{
    xml::node &                 root = doc.get_root_node();
    xml::xpath_expression       expr( "/root/child",
                                      xml::xpath_expression::type_compile );
    xml::node_set               nset( root.run_xpath_query( expr ) );
    xml::node_set::iterator     k = nset.begin();

    return *k;
}
BOOST_AUTO_TEST_CASE( XPathTest57 )
{
    try
    {
        xml::document               doc( "data/57.xml", NULL );
        xml::node &                 n = find_node( doc );

        BOOST_CHECK( string( n.get_name() ) == "child" );
        BOOST_CHECK( string( n.get_content() ) == "value1" );
    }
    catch ( const std::exception &  e )
    {
        CNcbiOstrstream ostr;
        ostr << e.what() << std::endl;
        saveStreamToFile( ostr, "data/57.out.real" );
        BOOST_FAIL( "Node set references lifetime test (57) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( XPathTest58 )
{
    try
    {
        std::string     doc_as_string( "<A><B><C/><C/><C/></B></A>" );
        xml::document   doc(doc_as_string.c_str(), doc_as_string.size(), NULL );
        xml::node &     root = doc.get_root_node();
        xml::node_set   nset( root.run_xpath_query( "count(//C)" ) );

        BOOST_CHECK( nset.size() == 1 );

        xml::node_set::iterator     k = nset.begin();
        xml::node &                 result = *k;

        BOOST_CHECK( result.get_name() == string( "xpath_scalar_result" ) );
        BOOST_CHECK( atof(result.get_content()) == 3.0 );

        const xml::attributes &     attrs = result.get_attributes();
        xml::attributes::const_iterator  found = attrs.find( "type" );

        BOOST_CHECK( found != attrs.end() );
        BOOST_CHECK( found->get_value() == string( "number" ) );
    }
    catch ( const std::exception &  e )
    {
        BOOST_FAIL( "XPath scalar return value test (58) failed: " + string( e.what() ) );
    }
    return;
}

