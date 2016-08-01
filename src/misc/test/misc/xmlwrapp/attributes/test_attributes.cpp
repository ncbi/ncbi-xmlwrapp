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
 * $Id: test_attributes.cpp 478660 2015-09-11 14:34:37Z satskyse $
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


// A helper function to save the stream content into a file
void  saveStreamToFile( CNcbiOstrstream &       stream,
                        const std::string &     fileName )
{
    std::ofstream   f( fileName.c_str() );

    f << string(CNcbiOstrstreamToString(stream));
    f.close();

    return;
}



BOOST_AUTO_TEST_CASE( AttributeTest01a )
{
    try
    {
        xml::document                       doc( "data/01a.xml", NULL );
        CNcbiOstrstream                     ostr;
        const xml::attributes &             attrs = doc.get_root_node().get_attributes();
        xml::attributes::const_iterator     i=attrs.begin(), end=attrs.end();

        for (; i!=end; ++i)
        {
            ostr << i->get_name() << "=" << i->get_value() << "\n";
        }
        saveStreamToFile( ostr, "data/01a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/01a.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Reading attribute test (01a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest01b )
{
    try
    {
        xml::document                       doc( "data/01b.xml", NULL );
        CNcbiOstrstream                     ostr;
        const xml::attributes &             attrs = doc.get_root_node().get_attributes();
        xml::attributes::const_iterator     i=attrs.begin(), end=attrs.end();

        for (; i!=end; ++i)
        {
            ostr << i->get_name() << "=" << i->get_value() << "\n";
        }
        saveStreamToFile( ostr, "data/01b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/01b.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Reading attribute test (01b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest01c )
{
    try
    {
        xml::document                       doc( "data/01c.xml", NULL );
        CNcbiOstrstream                     ostr;
        const xml::attributes &             attrs = doc.get_root_node().get_attributes();
        xml::attributes::const_iterator     i=attrs.begin(), end=attrs.end();

        for (; i!=end; ++i)
        {
            ostr << i->get_name() << "=" << i->get_value() << "\n";
        }
        saveStreamToFile( ostr, "data/01c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/01c.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Reading attribute test (01c) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest02 )
{
    try
    {
        xml::document           doc( "data/02.xml", NULL );
        CNcbiOstrstream         ostr;
        xml::attributes &       attrs = doc.get_root_node().get_attributes();

        attrs.insert("b", "b");

        ostr << doc;
        saveStreamToFile( ostr, "data/02.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/02.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Insert attribute test (02) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest03 )
{
    try
    {
        xml::document                       doc( "data/03.xml", NULL );
        const xml::attributes &             attrs = doc.get_root_node().get_attributes();
        xml::attributes::const_iterator     i;

        i = attrs.find( "one" );
        BOOST_CHECK( i != attrs.end() );

        i = attrs.find( "two" );
        BOOST_CHECK( i != attrs.end() );

        i = attrs.find( "three" );
        BOOST_CHECK( i != attrs.end() );

        i = attrs.find( "missing" );
        BOOST_CHECK( i == attrs.end() );

        i = attrs.find( "also_missing" );
        BOOST_CHECK( i == attrs.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Find attribute test (03) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest04a )
{
    try
    {
        xml::document               doc( "data/04.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();
        xml::attributes::iterator   i = attrs.find( "attr_one" );

        BOOST_CHECK( i != attrs.end() );
        attrs.erase(i);

        CNcbiOstrstream ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/04a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/04a.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Removing attribute test (04a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest04b )
{
    try
    {
        xml::document               doc( "data/04.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();
        xml::attributes::iterator   i = attrs.find( "attr_two" );

        BOOST_CHECK( i != attrs.end() );
        attrs.erase(i);

        CNcbiOstrstream ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/04b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/04b.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Removing attribute test (04b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest04c )
{
    try
    {
        xml::document               doc( "data/04.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();
        xml::attributes::iterator   i = attrs.find( "attr_three" );

        BOOST_CHECK( i != attrs.end() );
        attrs.erase(i);

        CNcbiOstrstream ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/04c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/04c.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Removing attribute test (04c) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest04d )
{
    try
    {
        xml::document               doc( "data/04.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();
        xml::attributes::iterator   i = attrs.find( "attr_four" );

        BOOST_CHECK( i != attrs.end() );
        attrs.erase(i);

        CNcbiOstrstream ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/04d.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/04d.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Removing attribute test (04d) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest05a )
{
    try
    {
        xml::document               doc( "data/05.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();

        attrs.erase( "attr_one" );

        CNcbiOstrstream ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/05a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05a.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Removing attribute test (05a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest05b )
{
    try
    {
        xml::document               doc( "data/05.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();

        attrs.erase( "attr_two" );

        CNcbiOstrstream ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/05b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05b.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Removing attribute test (05b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest05c )
{
    try
    {
        xml::document               doc( "data/05.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();

        attrs.erase( "attr_three" );

        CNcbiOstrstream ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/05c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05c.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Removing attribute test (05c) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest05d )
{
    try
    {
        xml::document               doc( "data/05.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();

        attrs.erase( "attr_four" );

        CNcbiOstrstream ostr;
        ostr << doc;
        saveStreamToFile( ostr, "data/05d.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/05d.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Removing attribute test (05d) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest06a )
{
    try
    {
        xml::document               doc( "data/06a.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();


        CNcbiOstrstream ostr;
        ostr << attrs.empty() << "\n";
        saveStreamToFile( ostr, "data/06a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/06a.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Empty attribute test (06a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest06b )
{
    try
    {
        xml::document               doc( "data/06b.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();


        CNcbiOstrstream ostr;
        ostr << attrs.empty() << "\n";
        saveStreamToFile( ostr, "data/06b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/06b.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Empty attribute test (06b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest07a )
{
    try
    {
        xml::document               doc( "data/07a.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();


        CNcbiOstrstream ostr;
        ostr << attrs.size() << "\n";
        saveStreamToFile( ostr, "data/07a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/07a.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Size of attributes test (07a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest07b )
{
    try
    {
        xml::document               doc( "data/07b.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();


        CNcbiOstrstream ostr;
        ostr << attrs.size() << "\n";
        saveStreamToFile( ostr, "data/07b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/07b.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Size of attributes test (07b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest07c )
{
    try
    {
        xml::document               doc( "data/07c.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();


        CNcbiOstrstream ostr;
        ostr << attrs.size() << "\n";
        saveStreamToFile( ostr, "data/07c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/07c.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Size of attributes test (07c) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest07d )
{
    try
    {
        xml::document               doc( "data/07d.xml", NULL );
        xml::attributes &           attrs = doc.get_root_node().get_attributes();


        CNcbiOstrstream ostr;
        ostr << attrs.size() << "\n";
        saveStreamToFile( ostr, "data/07d.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/07d.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Size of attributes test (07d) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest08 )
{
    try
    {
        xml::document                       doc( "data/08.xml", NULL );

        /* MAKE A COPY! */
        xml::attributes &                   attrs = doc.get_root_node().get_attributes();
        xml::attributes::const_iterator     i = attrs.begin(), end=attrs.end();

        CNcbiOstrstream ostr;
        for (; i!=end; ++i)
        {
            ostr << i->get_name() << "=" << i->get_value() << "\n";
        }

        saveStreamToFile( ostr, "data/08.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/08.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Attributes copy constructor test (08) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest09a )
{
    try
    {
        xml::document               doc( "data/09.xml", NULL );

        if ( doc.has_internal_subset() && !doc.validate() )
        {
            BOOST_FAIL( "DTD attributes test (09a) failed" );
        }

        CNcbiOstrstream                     ostr;
        xml::attributes &                   attrs = doc.get_root_node().get_attributes();
        xml::attributes::const_iterator     i = attrs.find( "one" );

        ostr << i->get_value() << "\n";
        ostr << (i == attrs.end() ? "end" : "notend") << "\n";

        saveStreamToFile( ostr, "data/09a.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/09a.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "DTD attributes test (09a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest09b )
{
    try
    {
        xml::document                       doc( "data/09.xml", NULL );

        if ( doc.has_internal_subset() && !doc.validate() )
        {
            BOOST_FAIL( "DTD attributes test (09b) failed" );
        }

        CNcbiOstrstream                     ostr;
        xml::attributes &                   attrs = doc.get_root_node().get_attributes();
        xml::attributes::const_iterator     i = attrs.find( "two" );

        ostr << i->get_value() << "\n";
        ostr << (i == attrs.end() ? "end" : "notend") << "\n";

        saveStreamToFile( ostr, "data/09b.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/09b.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "DTD attributes test (09b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest09c )
{
    try
    {
        xml::document                       doc( "data/09.xml", NULL );

        if ( doc.has_internal_subset() && !doc.validate() )
        {
            BOOST_FAIL( "DTD attributes test (09c) failed" );
        }

        CNcbiOstrstream                     ostr;
        xml::attributes &                   attrs = doc.get_root_node().get_attributes();
        xml::attributes::const_iterator     i = attrs.find( "three" );

        ostr << i->get_value() << "\n";
        ostr << (i == attrs.end() ? "end" : "notend") << "\n";

        saveStreamToFile( ostr, "data/09c.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/09c.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "DTD attributes test (09c) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest10 )
{
    try
    {
        xml::document                       doc( "data/10.xml", NULL );

        if ( doc.has_internal_subset() && !doc.validate() ) 
        {
            BOOST_FAIL( "DTD attributes test (10) failed" );
        }

        CNcbiOstrstream                     ostr;
        xml::attributes &                   attrs = doc.get_root_node().get_attributes();
        xml::attributes::const_iterator     i = attrs.find( "optional" );

        if ( i == attrs.end() )
        {
            ostr << "****END****\n";
        }
        else
        {
            ostr << i->get_value() << "\n";
        }

        saveStreamToFile( ostr, "data/10.out.real" );
        BOOST_CHECK( compareStreamAndFile( ostr, "data/10.out" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "DTD attributes test (10) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest11 )
{
    try
    {
        xml::document               doc( "data/11.xml", NULL );
        xml::node                   root = doc.get_root_node();
        xml::node                   nested_node( *root.find( "nested" ) );
        xml::attributes &           attrs = nested_node.get_attributes();

        xml::ns                     matched_ns( "bar", "bar_ns" );
        xml::ns                     not_matched_ns( "bar", "non_existed_uri" );
        xml::ns                     void_ns( xml::ns::type_void );

        {
            // const iterator
            xml::attributes::const_iterator     i = attrs.find( "attr", &matched_ns );
            BOOST_CHECK( i != attrs.end() );

            i = attrs.find( "attr", &not_matched_ns );
            BOOST_CHECK( i == attrs.end() );

            i = attrs.find( "attr", &void_ns );
            BOOST_CHECK( i == attrs.end() );

            i = attrs.find( "attr", NULL );
            BOOST_CHECK( i != attrs.end() );
        }

        {
            // non-const iterator
            xml::attributes::iterator     i = attrs.find( "attr", &matched_ns );
            BOOST_CHECK( i != attrs.end() );

            i = attrs.find( "attr", &not_matched_ns );
            BOOST_CHECK( i == attrs.end() );

            i = attrs.find( "attr", &void_ns );
            BOOST_CHECK( i == attrs.end() );

            i = attrs.find( "attr", NULL );
            BOOST_CHECK( i != attrs.end() );
        }
    }
    catch ( ... )
    {
        BOOST_FAIL( "attributes search with namespaces test (11) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest12 )
{
    try
    {
        xml::document               doc( "data/12.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        xml::attributes::const_iterator     i = attrs.find( "explicitAttr" );
        BOOST_CHECK( i != attrs.end() );
        BOOST_CHECK( i->is_default() == false );
        BOOST_CHECK( i->get_value() == std::string( "explicitVal" ) );

        i = attrs.find( "defaultAttr" );
        BOOST_CHECK( i != attrs.end() );
        BOOST_CHECK( i->is_default() == true );
        BOOST_CHECK( i->get_value() == std::string( "defaultVal" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "default attributes search test (12) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest13 )
{
    try
    {
        // xml::document               doc( "data/13.xml", NULL );
        // xml::node &                 root = doc.get_root_node();
        // xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k;

        ++k;

        BOOST_FAIL( "Advancing non initialised attributes iterator test (13) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest13a )
{
    try
    {
        xml::document               doc( "data/13.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.end();

        ++k;

        BOOST_FAIL( "Advancing end() attributes iterator test (13a) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest13b )
{
    try
    {
        xml::document               doc( "data/13.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.end();
        xml::attributes::iterator   j;

        BOOST_CHECK( k != j );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Comparing non-initialized and end() attributes iterators test (13b) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest13c )
{
    try
    {
        xml::document               doc( "data/13.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.end();

        xml::document               doc1( "data/13.xml", NULL );
        xml::node &                 root1 = doc1.get_root_node();
        xml::attributes &           attrs1 = root1.get_attributes();
        xml::attributes::iterator   k1 = attrs1.end();

        BOOST_CHECK( k != k1 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Comparing end() attributes iterators from different nodes test (13c) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest14 )
{
    try
    {
        xml::document               doc( "data/14.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.find( "explicitAttr" );

        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == false );
        BOOST_CHECK( k->get_value() == std::string( "explicitVal" ) );

        // It's forbidden to advance find iterators
        k++;
        BOOST_FAIL( "Advancing find(...) iterators test (14) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest14a )
{
    try
    {
        xml::document               doc( "data/14.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.find( "defaultAttr" );

        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == true );
        BOOST_CHECK( k->get_value() == std::string( "defaultVal" ) );

        // It's forbidden to advance find iterators
        ++k;
        BOOST_FAIL( "Advancing find(...) iterators test (14) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest15 )
{
    try
    {
        xml::document               doc( "data/15.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.find( "explicitAttr" );

        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == false );
        BOOST_CHECK( k->get_value() == std::string( "explicitVal" ) );

        k->set_value( "newVal" );
        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == false );
        BOOST_CHECK( k->get_value() == std::string( "newVal" ) );

        xml::attributes::iterator   j = attrs.find( "explicitAttr" );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->is_default() == false );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j == k );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Setting an attribute value test (15) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest15a )
{
    try
    {
        xml::document               doc( "data/15.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.find( "defaultAttr" );

        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == true );
        BOOST_CHECK( k->get_value() == std::string( "defaultVal" ) );

        k->set_value( "newVal" );
        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == false );
        BOOST_CHECK( k->get_value() == std::string( "newVal" ) );

        xml::attributes::iterator   j = attrs.find( "defaultAttr" );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->is_default() == false );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j == k );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Setting an attribute value test (15a) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest15b )
{
    try
    {
        xml::document               doc( "data/15.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.find( "defaultAttr" );
        xml::attributes::iterator   j = attrs.find( "defaultAttr" );

        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == true );
        BOOST_CHECK( k->get_value() == std::string( "defaultVal" ) );
        BOOST_CHECK( k == j );

        k->set_value( "newVal" );
        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == false );
        BOOST_CHECK( k->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->is_default() == false );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j == k );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Setting an attribute value test (15b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest16 )
{
    try
    {
        xml::document               doc( "data/16.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.find( "defaultAttr" );

        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == true );
        BOOST_CHECK( k->get_value() == std::string( "defaultVal" ) );
        BOOST_CHECK( k->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( k->get_namespace().get_uri() == std::string( "http://some.thing" ) );

        k->set_namespace( "foo" );
        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == false );
        BOOST_CHECK( k->get_value() == std::string( "defaultVal" ) );
        BOOST_CHECK( k->get_namespace().get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( k->get_namespace().get_uri() == std::string( "http://foo.com" ) );

        xml::attributes::iterator   j = attrs.find( "defaultAttr" );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->is_default() == false );
        BOOST_CHECK( j->get_value() == std::string( "defaultVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://foo.com" ) );
        BOOST_CHECK( j == k );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Setting an attribute namespace test (16) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest17 )
{
    try
    {
        xml::document               doc( "data/17.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   k = attrs.find( "foo:defaultAttr" );

        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == true );
        BOOST_CHECK( k->get_value() == std::string( "defaultVal" ) );
        BOOST_CHECK( k->get_namespace().get_prefix() == std::string( "foo" ) );
        BOOST_CHECK( k->get_namespace().get_uri() == std::string( "http://foo.com" ) );

        k->erase_namespace();
        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == false );
        BOOST_CHECK( k->get_value() == std::string( "defaultVal" ) );
        BOOST_CHECK( k->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( k->get_namespace().get_uri() == std::string( "" ) );
        BOOST_CHECK( k->get_namespace().is_void() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute namespace test (17) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest18 )
{
    try
    {
        xml::document               doc( "data/18.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   end = attrs.end();
        xml::attributes::iterator   k = attrs.find( "defaultAttr" );
        xml::attributes::iterator   j;

        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->is_default() == true );

        j = attrs.erase( k );
        BOOST_CHECK( j == attrs.end() );
        BOOST_CHECK( k->get_name() == std::string( "defaultAttr" ) );
        BOOST_CHECK( k->get_value() == std::string( "defaultVal" ) );

        j = attrs.find( "defaultAttr" );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->is_default() == true );
        BOOST_CHECK( j->get_name() == std::string( "defaultAttr" ) );
        BOOST_CHECK( j->get_value() == std::string( "defaultVal" ) );

        j = attrs.erase( end );
        BOOST_CHECK( j == attrs.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (18) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest18a )
{
    try
    {
        xml::document               doc( "data/18.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   j;

        attrs.erase( j );
        BOOST_FAIL( "Erasing an attribute test - non-initialized iterator (18a) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest18b )
{
    try
    {
        xml::document               doc( "data/18.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        xml::document               doc1( "data/18.xml", NULL );
        xml::node &                 root1 = doc1.get_root_node();
        xml::attributes &           attrs1 = root1.get_attributes();
        xml::attributes::iterator   j = attrs1.find( "defaultAttr" );

        BOOST_CHECK( j != attrs1.end() );

        attrs.erase( j );
        BOOST_FAIL( "Erasing an attribute test - foreign iterator (18b) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest19 )
{
    try
    {
        xml::document               doc( "data/19.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   j = attrs.find( "foo" );
        xml::attributes::iterator   k;

        BOOST_CHECK( j != attrs.end() );

        k = attrs.erase( j );
        BOOST_CHECK( k != attrs.end() );
        BOOST_CHECK( k->get_name() == std::string( "bar" ) );
        BOOST_CHECK( k->get_value() == std::string( "barVal" ) );

        k = attrs.find( "foo" );
        BOOST_CHECK( k == attrs.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test - foreign iterator (19) failed" );
    }
    return;
}



BOOST_AUTO_TEST_CASE( AttributeTest20 )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        attrs.insert( NULL, "newVal", NULL );
        BOOST_FAIL( "Inserting an attribute test - NULL name (20) failed" );
    }
    catch ( ... )
    {
        // exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest20a )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        attrs.insert( "foo", NULL, NULL );
        BOOST_FAIL( "Inserting an attribute test - NULL value (20a) failed" );
    }
    catch ( ... )
    {
        // exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest20b )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        attrs.insert( ":foo", "newVal", NULL );
        BOOST_FAIL( "Inserting an attribute test - bad attribute name (20b) failed" );
    }
    catch ( ... )
    {
        // exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest20c )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "prefix", "http://uri" );

        attrs.insert( "some:foo", "newVal", &nspace );
        BOOST_FAIL( "Inserting an attribute test [both qualified name and namespace] (20c) failed" );
    }
    catch ( ... )
    {
        // exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest20d )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "", "http://uri" );

        attrs.insert( "foo", "newVal", &nspace );
        BOOST_FAIL( "Inserting an attribute test [default namespace] (20d) failed" );
    }
    catch ( ... )
    {
        // exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest20e )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     void_ns( xml::ns::type_void );
        xml::attributes::iterator   j = attrs.find( "foo" );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "foo" ) );
        BOOST_CHECK( j->get_value() == std::string( "fooVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "" ) );

        attrs.insert( "foo", "newVal", & void_ns );

        j = attrs.find( "foo" );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "foo" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (20e) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest20f )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     void_ns( xml::ns::type_void );
        xml::attributes::iterator   j = attrs.find( "fred" );

        BOOST_CHECK( j == attrs.end() );

        attrs.insert( "fred", "newVal", & void_ns );

        j = attrs.find( "fred" );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "fred" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (20f) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest20g )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   j = attrs.find( "foo" );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "foo" ) );
        BOOST_CHECK( j->get_value() == std::string( "fooVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "" ) );

        attrs.insert( "foo", "newVal", NULL );

        j = attrs.find( "foo" );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "foo" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (20g) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest20h )
{
    try
    {
        xml::document               doc( "data/20.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::attributes::iterator   j = attrs.find( "fred" );

        BOOST_CHECK( j == attrs.end() );

        attrs.insert( "fred", "newVal", NULL );

        j = attrs.find( "fred" );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "fred" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (20h) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest21 )
{
    try
    {
        xml::document               doc( "data/21.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "some_ns", "http://the.com" );
        xml::attributes::iterator   j = attrs.find( "foo", & nspace );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "foo" ) );
        BOOST_CHECK( j->get_value() == std::string( "fooVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );

        attrs.insert( "some_ns:foo", "newVal", NULL );

        j = attrs.find( "foo", & nspace );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "foo" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (21) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest21a )
{
    try
    {
        xml::document               doc( "data/21.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "some_ns", "http://the.com" );
        xml::attributes::iterator   j = attrs.find( "fred", & nspace );

        BOOST_CHECK( j == attrs.end() );

        attrs.insert( "some_ns:fred", "newVal", NULL );

        j = attrs.find( "fred", & nspace );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "fred" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (21a) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest22 )
{
    try
    {
        xml::document               doc( "data/22.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns_list_type           ns_list = root.get_namespace_definitions( xml::ns::type_unsafe_ns );

        BOOST_CHECK( ns_list.size() == 1 );
        attrs.insert( "newName", "newVal", & ns_list[ 0 ] );

        BOOST_FAIL( "Inserting an attribute test (unsafe default namespace) (22) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest23 )
{
    try
    {
        xml::document               doc( "data/23.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns_list_type           ns_list = root.get_namespace_definitions( xml::ns::type_unsafe_ns );

        BOOST_CHECK( ns_list.size() == 1 );

        xml::attributes::iterator   j = attrs.find( "fred", & ns_list[ 0 ] );

        BOOST_CHECK( j == attrs.end() );

        attrs.insert( "fred", "newVal", & ns_list[ 0 ] );
        j = attrs.find( "fred", & ns_list[ 0 ] );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "fred" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (unsafe namespace) (23) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest23a )
{
    try
    {
        xml::document               doc( "data/23.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns_list_type           ns_list = root.get_namespace_definitions( xml::ns::type_unsafe_ns );

        BOOST_CHECK( ns_list.size() == 1 );

        xml::attributes::iterator   j = attrs.find( "foo", & ns_list[ 0 ] );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "foo" ) );
        BOOST_CHECK( j->get_value() == std::string( "fooVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );

        attrs.insert( "foo", "newVal", & ns_list[ 0 ] );
        j = attrs.find( "foo", & ns_list[ 0 ] );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "foo" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (unsafe namespace) (23a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest24 )
{
    try
    {
        xml::document               doc( "data/24.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "some_ns", "http://the.com" );

        xml::attributes::iterator   j = attrs.find( "foo", & nspace );

        BOOST_CHECK( j == attrs.end() );

        attrs.insert( "foo", "newVal", & nspace );
        j = attrs.find( "foo", & nspace );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "foo" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (24) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest24a )
{
    try
    {
        xml::document               doc( "data/24.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "some_ns", "http://the.com" );

        xml::attributes::iterator   j = attrs.find( "bar", & nspace );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "bar" ) );
        BOOST_CHECK( j->get_value() == std::string( "barVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );

        attrs.insert( "bar", "newVal", & nspace );
        j = attrs.find( "bar", & nspace );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "bar" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (24a) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest24b )
{
    try
    {
        xml::document               doc( "data/24.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "not_existing", "http://safe.namespace" );

        attrs.insert( "bar", "newVal", & nspace );

        BOOST_FAIL( "Inserting an attribute test (24b) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest24c )
{
    try
    {
        xml::document               doc( "data/24.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "some_ns", "http://the.com" );
        xml::ns                     void_ns( xml::ns::type_void );

        xml::attributes::iterator   j = attrs.find( "bar", & nspace );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "bar" ) );
        BOOST_CHECK( j->get_value() == std::string( "barVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );

        attrs.insert( "bar", "newVal", & void_ns );
        j = attrs.find( "bar", & nspace );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "bar" ) );
        BOOST_CHECK( j->get_value() == std::string( "barVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );

        j = attrs.find( "bar", & void_ns );

        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_name() == std::string( "bar" ) );
        BOOST_CHECK( j->get_value() == std::string( "newVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Inserting an attribute test (24c) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest24d )
{
    try
    {
        xml::document               doc( "data/24.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        attrs.insert( "not_existing:newName", "newVal" );

        BOOST_FAIL( "Inserting an attribute test (24d) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}



BOOST_AUTO_TEST_CASE( AttributeTest25 )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        BOOST_CHECK( attrs.erase( NULL, NULL ) == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25a )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        BOOST_CHECK( attrs.erase( "non_existing", NULL ) == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25a) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25b )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        BOOST_CHECK( attrs.erase( "bar", NULL ) == 2 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25b) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25c )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     default_ns( "", "http://the.com.not.matched" );

        BOOST_CHECK( attrs.erase( "bar", &default_ns ) == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25c) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25d )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     default_ns( "", "http://the.com" );

        // Matched basing on URI, so some_ns:bar is deleted
        BOOST_CHECK( attrs.erase( "bar", &default_ns ) == 1 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25d) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25e )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "non_existing", "http://the.com.space" );

        BOOST_CHECK( attrs.erase( "bar", & nspace ) == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25e) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25f )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     void_ns( xml::ns::type_void );

        BOOST_CHECK( attrs.erase( "bar", & void_ns ) == 1 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25f) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25g )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     void_ns( xml::ns::type_void );

        BOOST_CHECK( attrs.erase( "kika", & void_ns ) == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25g) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25h )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns_list_type           ns_list = root.get_namespace_definitions( xml::ns::type_unsafe_ns );

        BOOST_CHECK( ns_list.size() == 1 );

        BOOST_CHECK( attrs.erase( "bar", & ns_list[ 0 ] ) == 1 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25h) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25i )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns_list_type           ns_list = root.get_namespace_definitions( xml::ns::type_unsafe_ns );

        BOOST_CHECK( ns_list.size() == 1 );

        BOOST_CHECK( attrs.erase( "foo", & ns_list[ 0 ] ) == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25i) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25j )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        BOOST_CHECK( attrs.erase( ":foo", NULL ) == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25j) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest25k )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "some_ns", "http://the.com" );

        BOOST_CHECK( attrs.erase( "whatever:foo", &nspace ) == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25k) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25l )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        BOOST_CHECK( attrs.erase( "some_ns:bar", NULL ) == 1 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25l) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest25m )
{
    try
    {
        xml::document               doc( "data/25.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        BOOST_CHECK( attrs.erase( "some_ns:foo", NULL ) == 0 );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Erasing an attribute test (25m) failed" );
    }
    return;
}




BOOST_AUTO_TEST_CASE( AttributeTest26 )
{
    try
    {
        xml::document               doc( "data/26.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        xml::attributes::iterator   j = attrs.find( NULL, NULL );
        BOOST_CHECK( j == attrs.end() );

        j = attrs.find( NULL );
        BOOST_CHECK( j == attrs.end() );

        j = attrs.find( ":foo", NULL );
        BOOST_CHECK( j == attrs.end() );
        j = attrs.find( ":foo" );
        BOOST_CHECK( j == attrs.end() );

        j = attrs.find( "foo:", NULL );
        BOOST_CHECK( j == attrs.end() );
        j = attrs.find( "foo:" );
        BOOST_CHECK( j == attrs.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Searching an attribute test (26) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest26a )
{
    try
    {
        xml::document               doc( "data/26.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        xml::attributes::iterator   j = attrs.find( "bar", NULL );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_value() == std::string( "barVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );

        xml::ns                     void_ns( xml::ns::type_void );
        j = attrs.find( "bar", & void_ns );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_value() == std::string( "otherBar" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Searching an attribute test (26a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( AttributeTest26b )
{
    try
    {
        xml::document               doc( "data/26.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();

        xml::attributes::iterator   j = attrs.find( "some_ns:kika", NULL );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_value() == std::string( "zz" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Searching an attribute test (26b) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest26c )
{
    try
    {
        xml::document               doc( "data/26.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "some_ns", "http://the.com" );

        xml::attributes::iterator   j = attrs.find( "bar", & nspace );
        BOOST_CHECK( j != attrs.end() );
        BOOST_CHECK( j->get_value() == std::string( "barVal" ) );
        BOOST_CHECK( j->get_namespace().get_prefix() == std::string( "some_ns" ) );
        BOOST_CHECK( j->get_namespace().get_uri() == std::string( "http://the.com" ) );

        j = attrs.find( "foo", & nspace );
        BOOST_CHECK( j == attrs.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Searching an attribute test (26c) failed" );
    }
    return;
}

BOOST_AUTO_TEST_CASE( AttributeTest26d )
{
    try
    {
        xml::document               doc( "data/26.xml", NULL );
        xml::node &                 root = doc.get_root_node();
        xml::attributes &           attrs = root.get_attributes();
        xml::ns                     nspace( "some_ns", "http://the.wrong.com" );

        xml::attributes::iterator   j = attrs.find( "non-existing:bar" );
        BOOST_CHECK( j == attrs.end() );

        j = attrs.find( "bar", & nspace );
        BOOST_CHECK( j == attrs.end() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Searching an attribute test (26d) failed" );
    }
    return;
}


// The AttributeTest27 test must not produce core dump
xml::attributes::attr &  insert_attr(xml::document &  doc)
{

    xml::node &                 root = doc.get_root_node();
    xml::attributes &           attrs = root.get_attributes();

    attrs.insert("name", "value");

    xml::attributes::iterator   new_attr = attrs.find( "name" );
    if (new_attr == attrs.end())
        throw "Unexpected behavior";
    return *new_attr;
}
BOOST_AUTO_TEST_CASE( AttributeTest27 )
{
    std::string         xmldata( "<root><a></a></root>" );
    xml::document       doc( xmldata.c_str(), xmldata.size(), NULL );

    xml::attributes::attr &     a = insert_attr( doc );
    BOOST_CHECK( string( a.get_value() ) == string( "value" ) );
    BOOST_CHECK( string( a.get_name() ) == string( "name" ) );

    xml::attributes::iterator   found = doc.get_root_node().get_attributes().find( "name" );
    BOOST_CHECK( string( found->get_value() ) == string( "value" ) );
    BOOST_CHECK( string( found->get_name() ) == string( "name" ) );

    xml::attributes::const_iterator   c_found = doc.get_root_node().get_attributes().find( "name" );
    BOOST_CHECK( string( c_found->get_value() ) == string( "value" ) );
    BOOST_CHECK( string( c_found->get_name() ) == string( "name" ) );

    found->set_value( "bla" );
    BOOST_CHECK( string( found->get_value() ) == string( "bla" ) );
    BOOST_CHECK( string( found->get_name() ) == string( "name" ) );
    BOOST_CHECK( string( c_found->get_value() ) == string( "bla" ) );
    BOOST_CHECK( string( c_found->get_name() ) == string( "name" ) );
    BOOST_CHECK( string( a.get_value() ) == string( "bla" ) );
    BOOST_CHECK( string( a.get_name() ) == string( "name" ) );
}


BOOST_AUTO_TEST_CASE( AttributeTest28 )
{
    std::string             xmldata( "<root><a></a></root>" );
    xml::document           doc( xmldata.c_str(), xmldata.size(), NULL );

    xml::attributes &       attrs = doc.get_root_node().get_attributes();
    try
    {
        attrs.insert("", "b");
        BOOST_FAIL( "Insert attribute test (28) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
        ;
    }
}

BOOST_AUTO_TEST_CASE( AttributeTest29 )
{
    std::string             xmldata( "<root><a></a></root>" );
    xml::document           doc( xmldata.c_str(), xmldata.size(), NULL );

    xml::attributes &       attrs = doc.get_root_node().get_attributes();
    try
    {
        attrs.insert("  \r  \n  \t  ", "b");
        BOOST_FAIL( "Insert attribute test (29) failed" );
    }
    catch ( ... )
    {
        // Exception is expected
        ;
    }
}


