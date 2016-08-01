/*  $Id: test_xslt_ext_func.cpp 390870 2013-03-04 14:33:50Z satskyse $
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
#include <misc/xmlwrapp/xsltwrapp.hpp>
#include <iostream>
#include <cstdlib>


USING_NCBI_SCOPE;


//
// xpath_object
//
BOOST_AUTO_TEST_CASE( XPathObjectTest01 )
{
    try
    {
        xslt::xpath_object      o1;
        xslt::xpath_object      o2;

        o1 = o2;

        xslt::xpath_object      o3(o1);
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xpath_object failed (01)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathObjectTest02 )
{
    try
    {
        {
            xslt::xpath_object      o1( "new value" );
            xslt::xpath_object      o2 = o1;
            xslt::xpath_object      o3;

            o3 = o1;
            BOOST_CHECK( o1.get_type() == o3.get_type() );
            BOOST_CHECK( o1.get_type() == xslt::type_string );
            BOOST_CHECK( o3.get_as_string() == "new value" );
        }

        {
            xslt::xpath_object      o1( true );
            xslt::xpath_object      o2 = o1;
            xslt::xpath_object      o3;

            o3 = o1;
            BOOST_CHECK( o1.get_type() == o3.get_type() );
            BOOST_CHECK( o1.get_type() == xslt::type_boolean );
            BOOST_CHECK( o3.get_as_bool() == true );
        }

        {
            xslt::xpath_object      o1( 42 );
            xslt::xpath_object      o2 = o1;
            xslt::xpath_object      o3;

            o3 = o1;
            BOOST_CHECK( o1.get_type() == o3.get_type() );
            BOOST_CHECK( o1.get_type() == xslt::type_number );
            BOOST_CHECK( o3.get_as_int() == 42 );
        }

        {
            xslt::xpath_object      o1( 3.14159265 );
            xslt::xpath_object      o2 = o1;
            xslt::xpath_object      o3;

            o3 = o1;
            BOOST_CHECK( o1.get_type() == o3.get_type() );
            BOOST_CHECK( o1.get_type() == xslt::type_number );
            BOOST_CHECK( o3.get_as_float() == 3.14159265 );
        }
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xpath_object failed (02)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathObjectTest03 )
{
    try
    {
        xml::node               new_node( "name", "value" );
        xslt::xpath_object      o1( new_node );
        xslt::xpath_object      o2;

        o2 = o1;

        xslt::xpath_object      o3(o1);

        BOOST_CHECK( o1.get_type() == xslt::type_nodeset );
        BOOST_CHECK( o2.get_type() == xslt::type_nodeset );
        BOOST_CHECK( o3.get_type() == xslt::type_nodeset );

        xml::node &             val = o3.get_as_node();
        BOOST_CHECK( val.get_name() == std::string( "name" ) );
        BOOST_CHECK( val.get_content() == std::string( "value" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xpath_object failed (03)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathObjectTest04 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested/></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\">"
                                                  "<xsl:output method=\"text\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<xsl:choose>"
                                                  "<xsl:when test=\"function-available('my:test')\">"
                                                  "my:test extension function is available"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension function is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );
        xml::document           result = sheet.apply( doc );

        // The function was not registered, so check it
        std::string             txt;
        result.save_to_string( txt, xml::save_op_no_decl | xml::save_op_no_format );

        BOOST_CHECK( txt == "my:test extension function is NOT available" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Non-registered function (04)" );
    }
    return;
}



// Define my extension function
class myExtFunc : public xslt::extension_function
{
    public:
        virtual void execute (const std::vector<xslt::xpath_object> &  args,
                              const xml::node &                        node,
                              const xml::document &                    doc)
        {
            set_return_value( xslt::xpath_object( 42 ) );
        }
};



BOOST_AUTO_TEST_CASE( XPathObjectTest05 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested/></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\">"
                                                  "<xsl:output method=\"text\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<xsl:choose>"
                                                  "<xsl:when test=\"function-available('my:test')\">"
                                                  "my:test extension function is available"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension function is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtFunc *             myFunc = new myExtFunc;
        sheet.register_extension_function( myFunc, "test", "http://bla.bla.bla",
                                           xml::type_own );

        xml::document           result = sheet.apply( doc );

        // The function was registered, so check it
        std::string             txt;
        result.save_to_string( txt, xml::save_op_no_decl | xml::save_op_no_format );

        BOOST_CHECK( txt == "my:test extension function is available" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Registered function (05)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathObjectTest06 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested/></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\">"
                                                  "<xsl:output method=\"text\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<xsl:choose>"
                                                  "<xsl:when test=\"function-available('my:test')\">"
                                                  "<xsl:value-of select=\"my:test(15)\"/>"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension function is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtFunc *             myFunc = new myExtFunc;
        sheet.register_extension_function( myFunc, "test", "http://bla.bla.bla",
                                           xml::type_own );

        xml::document           result = sheet.apply( doc );

        // The function was registered, so check it
        std::string             txt;
        result.save_to_string( txt, xml::save_op_no_decl | xml::save_op_no_format );

        BOOST_CHECK( txt == "42" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Registered function (06)" );
    }
    return;
}

// Define my second extension function
class myExtFunc2 : public xslt::extension_function
{
    public:
        virtual void execute (const std::vector<xslt::xpath_object> &  args,
                              const xml::node &                        node,
                              const xml::document &                    doc)
        {
            if ( args.size() != 5 )
            {
                report_error( "Invalid arity" );
                return;
            }

            // Argument 0: must be nodes
            if ( args[ 0 ].get_type() != xslt::type_nodeset )
            {
                report_error( "Invalid type of operand #1" );
                return;
            }

            xml::node_set   nset = args[ 0 ].get_as_node_set();
            if ( nset.size() != 1 )
            {
                report_error( "Unexpected number of nodes in operand #1" );
                return;
            }
            xml::node_set::iterator     first = nset.begin();
            xml::node &                 n = *first;

            if ( n.get_name() != std::string( "nested" ) )
            {
                report_error( "Unexpected node in operand #2" );
                return;
            }

            // Argument 1: must be 'tst_str'
            if ( args[ 1 ].get_type() != xslt::type_string )
            {
                report_error( "Invalid type of operand #2" );
                return;
            }
            if ( args[ 1 ].get_as_string() != "tst_str" )
            {
                report_error( "Unexpected value of operand #2" );
                return;
            }

            // Argument 2: must be 15
            if ( args[ 2 ].get_type() != xslt::type_number )
            {
                report_error( "Invalid type of operand #3" );
                return;
            }
            if ( args[ 2 ].get_as_int() != 15 )
            {
                report_error( "Unexpected value of operand #3" );
                return;
            }

            // Argument 3: must be 2.718
            if ( args[ 3 ].get_type() != xslt::type_number )
            {
                report_error( "Invalid type of operand #4" );
                return;
            }
            if ( args[ 3 ].get_as_float() != 2.718 )
            {
                report_error( "Unexpected value of operand #4" );
                return;
            }

            // Argument 4: must be false
            if ( args[ 4 ].get_type() != xslt::type_boolean )
            {
                report_error( "Invalid type of operand #5" );
                return;
            }
            if ( args[ 4 ].get_as_bool() != false )
            {
                report_error( "Unexpected value of operand #5" );
                return;
            }

            // Arguments look OK
            set_return_value( xslt::xpath_object( "arguments are fine" ) );
        }
};


BOOST_AUTO_TEST_CASE( XPathObjectTest07 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested/></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\">"
                                                  "<xsl:output method=\"text\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<xsl:choose>"
                                                  "<xsl:when test=\"function-available('my:test')\">"
                                                  "<xsl:variable name=\"my_node_set\" select=\"//root/nested\"/>"
                                                  "<xsl:value-of select=\"my:test($my_node_set,'tst_str',15,2.718,false())\"/>"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension function is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtFunc2 *            myFunc = new myExtFunc2;
        sheet.register_extension_function( myFunc, "test", "http://bla.bla.bla",
                                           xml::type_own );

        xml::document           result = sheet.apply( doc );

        // The function was registered, so check it
        std::string             txt;
        result.save_to_string( txt, xml::save_op_no_decl | xml::save_op_no_format );

        BOOST_CHECK( txt == "arguments are fine" );
    }
    catch ( const xslt::exception &  ex )
    {
        BOOST_FAIL( std::string( "Registered function (07) xslt:exception: " ) + ex.what() );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Registered function (07) std:exception: " ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Registered function (07)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathObjectTest08 )
{
    std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested/><nested/></root>";
    xml::error_messages     msgs;
    xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

    xml::node_set           nset( doc.get_root_node().run_xpath_query( "/root/nested" ) );

    try
    {
        xslt::xpath_object      o1( nset );
        xslt::xpath_object      o2;

        o2 = o1;

        xslt::xpath_object      o3(o1);

        BOOST_CHECK( o1.get_type() == xslt::type_nodeset );
        BOOST_CHECK( o2.get_type() == xslt::type_nodeset );
        BOOST_CHECK( o3.get_type() == xslt::type_nodeset );

        xml::node &             val = o3.get_as_node();
        BOOST_CHECK( val.get_name() == std::string( "nested" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xpath_object failed (08)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( XPathObjectTest09 )
{
    xml::node                       n1( "one", "1" );
    xml::node                       n2( "two", "2" );
    xml::node                       n3( "three", "3" );

    std::vector< xml::node >        src;
    src.push_back( n1 );
    src.push_back( n2 );
    src.push_back( n3 );

    try
    {
        xslt::xpath_object      o1( src );
        xslt::xpath_object      o2;

        o2 = o1;

        xslt::xpath_object      o3(o1);

        BOOST_CHECK( o1.get_type() == xslt::type_nodeset );
        BOOST_CHECK( o2.get_type() == xslt::type_nodeset );
        BOOST_CHECK( o3.get_type() == xslt::type_nodeset );

        xml::node &             val = o3.get_as_node();
        BOOST_CHECK( val.get_name() == std::string( "one" ) );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xpath_object failed (09)" );
    }
    return;
}


// Define my second extension function
class myExtFunc3 : public xslt::extension_function
{
    public:
        virtual void execute (const std::vector<xslt::xpath_object> &  args,
                              const xml::node &                        node,
                              const xml::document &                    doc)
        {
            report_error( "ERROR REPORT TEST" );
            return;
        }
};


BOOST_AUTO_TEST_CASE( XPathObjectTest10 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested/></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\">"
                                                  "<xsl:output method=\"text\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<xsl:choose>"
                                                  "<xsl:when test=\"function-available('my:test')\">"
                                                  "<xsl:variable name=\"my_node_set\" select=\"//root/nested\"/>"
                                                  "<xsl:value-of select=\"my:test($my_node_set,'tst_str',15,2.718,false())\"/>"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension function is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtFunc3 *            myFunc = new myExtFunc3;
        sheet.register_extension_function( myFunc, "test", "http://bla.bla.bla",
                                           xml::type_own );

        xml::document           result = sheet.apply( doc );

        // Exception is expected
        BOOST_FAIL( "Expected exception for an extension function. Received nothing (10)" );
    }
    catch ( const xslt::exception &  ex )
    {
        std::string     msg( ex.what() );
        BOOST_CHECK( msg.find( "ERROR REPORT TEST" ) != std::string::npos );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Expected xslt::exception, received std:exception: (10)" ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Expected xslt::exception, received unknown exception (10)" );
    }
    return;
}



class extFunc11 : public xslt::extension_function
{
    public:
        virtual void execute (const std::vector<xslt::xpath_object> &  args,
                              const xml::node &                        node,
                              const xml::document &                    doc)
        {
            xml::node                   new_node_1( "fourty", "1" );
            xml::node                   new_node_2( "fourty", "2" );
            xml::node                   new_node_3( "fourty", "3" );

            std::vector< xml::node >    nset;
            nset.push_back( new_node_1 );
            nset.push_back( new_node_2 );
            nset.push_back( new_node_3 );

            set_return_value( xslt::xpath_object( nset ) );
        }
};

BOOST_AUTO_TEST_CASE( XPathObjectTest11 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested/></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\">"
                                                  "<xsl:output method=\"xml\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<root>"

                                                  "<sum><xsl:value-of select=\"sum(my:test())\"/></sum>"

                                                  "<xsl:for-each select=\"my:test()\">"
                                                  "<iter><xsl:value-of select=\".\"/></iter>"
                                                  "</xsl:for-each>"

                                                  "<content1><xsl:value-of select=\"my:test()\"/></content1>"
                                                  "<content2><xsl:value-of select=\"my:test()/node()\"/></content2>"

                                                  "  <xsl:variable name=\"v1\" select=\"count(my:test())\"/>"
                                                  "  <xsl:variable name=\"v2\" select=\"count(my:test()/node())\"/>"
                                                  "<count1><xsl:value-of select=\"$v1\"/></count1>"
                                                  "<count2><xsl:value-of select=\"$v2\"/></count2>"

                                                  "</root>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        extFunc11 *             myFunc = new extFunc11;
        sheet.register_extension_function( myFunc, "test", "http://bla.bla.bla",
                                           xml::type_own );

        xml::document               result = sheet.apply( doc );
        xml::node &                 root = result.get_root_node();

        xml::node::const_iterator   n = root.find( "sum" );
        BOOST_CHECK( std::string( "6" ) == n->get_content() );

        n = root.find( "iter" );
        BOOST_CHECK( std::string( "1" ) == n->get_content() );
        n = root.find( "iter", ++n );
        BOOST_CHECK( std::string( "2" ) == n->get_content() );
        n = root.find( "iter", ++n );
        BOOST_CHECK( std::string( "3" ) == n->get_content() );

        n = root.find( "content1" );
        BOOST_CHECK( std::string( "1" ) == n->get_content() );

        n = root.find( "content2" );
        BOOST_CHECK( std::string( "1" ) == n->get_content() );

        n = root.find( "count1" );
        BOOST_CHECK( std::string( "3" ) == n->get_content() );

        n = root.find( "count2" );
        BOOST_CHECK( std::string( "3" ) == n->get_content() );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Exception: " ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Registered function (11)" );
    }
    return;
}



class extFunc12 : public xslt::extension_function
{
    public:
        virtual void execute (const std::vector<xslt::xpath_object> &  args,
                              const xml::node &                        node,
                              const xml::document &                    doc)
        {
            xml::node_set           nset( doc.get_root_node().run_xpath_query( "/root/nested" ) );
            set_return_value( xslt::xpath_object( nset ) );
        }
};

BOOST_AUTO_TEST_CASE( XPathObjectTest12 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested>42</nested></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\">"
                                                  "<xsl:output method=\"xml\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<root>"

                                                  "<sum><xsl:value-of select=\"sum(my:test())\"/></sum>"

                                                  "<xsl:for-each select=\"my:test()\">"
                                                    "<iter><xsl:value-of select=\".\"/></iter>"
                                                  "</xsl:for-each>"

                                                  "<content1><xsl:value-of select=\"my:test()\"/></content1>"
                                                  "<content2><xsl:value-of select=\"my:test()/node()\"/></content2>"

                                                  "  <xsl:variable name=\"v1\" select=\"count(my:test())\"/>"
                                                  "  <xsl:variable name=\"v2\" select=\"count(my:test()/node())\"/>"
                                                  "<count1><xsl:value-of select=\"$v1\"/></count1>"
                                                  "<count2><xsl:value-of select=\"$v2\"/></count2>"

                                                  "</root>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        extFunc12 *             myFunc = new extFunc12;
        sheet.register_extension_function( myFunc, "test", "http://bla.bla.bla",
                                           xml::type_own );

        xml::document               result = sheet.apply( doc );
        xml::node &                 root = result.get_root_node();

        xml::node::const_iterator   n = root.find( "sum" );
        BOOST_CHECK( std::string( "42" ) == n->get_content() );

        n = root.find( "iter" );
        BOOST_CHECK( std::string( "42" ) == n->get_content() );

        n = root.find( "content1" );
        BOOST_CHECK( std::string( "42" ) == n->get_content() );

        n = root.find( "content2" );
        BOOST_CHECK( std::string( "42" ) == n->get_content() );

        n = root.find( "count1" );
        BOOST_CHECK( std::string( "1" ) == n->get_content() );

        n = root.find( "count2" );
        BOOST_CHECK( std::string( "1" ) == n->get_content() );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Exception: " ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Registered function (12)" );
    }
    return;
}



class myExtFunc13 : public xslt::extension_function
{
    public:
        virtual void execute (const std::vector<xslt::xpath_object> &  args,
                              const xml::node &                        node,
                              const xml::document &                    doc)
        {
            // Argument 0: must be nodes
            if ( args[ 0 ].get_type() != xslt::type_nodeset )
            {
                report_error( "Invalid type of operand #1" );
                return;
            }

            xml::node_set   nset = args[ 0 ].get_as_node_set();
            if ( nset.size() != 1 )
            {
                report_error( "Unexpected number of nodes in operand #1" );
                return;
            }
            xml::node_set::iterator     first = nset.begin();
            xml::node &                 n = *first;

            if ( n.get_name() != std::string( "nested" ) )
            {
                report_error( "Unexpected node in operand #1" );
                return;
            }

            // Arguments look OK
            set_return_value( args[ 0 ] );
        }
};

BOOST_AUTO_TEST_CASE( XPathObjectTest13 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested>42</nested></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\">"
                                                  "<xsl:output method=\"text\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<xsl:choose>"
                                                  "<xsl:when test=\"function-available('my:test')\">"
                                                  "<xsl:variable name=\"my_node_set\" select=\"//root/nested\"/>"
                                                  "<xsl:value-of select=\"my:test($my_node_set)\"/>"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension function is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtFunc13 *           myFunc = new myExtFunc13;
        sheet.register_extension_function( myFunc, "test", "http://bla.bla.bla",
                                           xml::type_own );

        xml::document           result = sheet.apply( doc );

        // The function was registered, so check it
        std::string             txt;
        result.save_to_string( txt, xml::save_op_no_decl | xml::save_op_no_format );

        BOOST_CHECK( txt == "42" );
    }
    catch ( const xslt::exception &  ex )
    {
        BOOST_FAIL( std::string( "Registered function (13) xslt:exception: " ) + ex.what() );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Registered function (13) std:exception: " ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Registered function (13)" );
    }
    return;
}


