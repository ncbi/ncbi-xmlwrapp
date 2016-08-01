/*  $Id: test_xslt_ext_elem.cpp 388454 2013-02-07 20:03:39Z satskyse $
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


BOOST_AUTO_TEST_CASE( XSLTExtElemTest01 )
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
                                                  "<xsl:when test=\"element-available('my:test')\">"
                                                  "my:test extension element is available"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension element is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );
        xml::document           result = sheet.apply( doc );

        // The element was not registered, so check it
        std::string             txt;
        result.save_to_string( txt, xml::save_op_no_decl | xml::save_op_no_format );

        BOOST_CHECK( txt == "my:test extension element is NOT available" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Extention element test 01 failed" );
    }
    return;
}



// Define my extension element which does nothing
class myExtElem : public xslt::extension_element
{
    public:
        virtual void process (xml::node &               input_node,
                              const xml::node &         instruction_node,
                              xml::node &               insert_point,
                              const xml::document &     doc)
        {
        }
};



BOOST_AUTO_TEST_CASE( XSLTExtElemTest02 )
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
                                                  "<xsl:when test=\"element-available('my:test')\">"
                                                  "my:test extension element is available"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension element is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtElem *             myElem = new myExtElem;
        sheet.register_extension_element( myElem, "test", "http://bla.bla.bla",
                                          xml::type_own );

        xml::document           result = sheet.apply( doc );

        // The element was registered, so check it
        std::string             txt;
        result.save_to_string( txt, xml::save_op_no_decl | xml::save_op_no_format );

        BOOST_CHECK( txt == "my:test extension element is available" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Extention element test 02 failed" );
    }
    return;
}


// Define my extension element which inserts a node
class myExtElem2 : public xslt::extension_element
{
    public:
        virtual void process (xml::node &               input_node,
                              const xml::node &         instruction_node,
                              xml::node &               insert_point,
                              const xml::document &     doc)
        {
            xml::node   my( "inserted", "content" );
            insert_point.push_back( my );
        }
};


BOOST_AUTO_TEST_CASE( XSLTExtElemTest03 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested/></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\" extension-element-prefixes=\"my\">"
                                                  "<xsl:output method=\"xml\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<xsl:choose>"
                                                  "<xsl:when test=\"element-available('my:test')\">"
                                                  "<my:test/>"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension element is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtElem2 *            myElem = new myExtElem2;
        sheet.register_extension_element( myElem, "test", "http://bla.bla.bla",
                                          xml::type_own );

        xml::document           result = sheet.apply( doc );

        // The element was registered and used
        xml::node &             result_root = result.get_root_node();

        BOOST_CHECK( result_root.get_name() == std::string( "inserted" ) );
        BOOST_CHECK( result_root.get_content() == std::string( "content" ) );
    }
    catch ( const xslt::exception &  ex )
    {
        BOOST_FAIL( std::string( "Extension element test 3 failed, xslt::exception: " ) + ex.what() );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Extension element test 3 failed, std::exception: " ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Extention element test 03 failed" );
    }
    return;
}


// Define my extension element which generates an error
class myExtElem3 : public xslt::extension_element
{
    public:
        virtual void process (xml::node &               input_node,
                              const xml::node &         instruction_node,
                              xml::node &               insert_point,
                              const xml::document &     doc)
        {
            report_error( "ERROR REPORT TEST" );
        }
};


BOOST_AUTO_TEST_CASE( XSLTExtElemTest04 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><root><nested/></root>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://bla.bla.bla\" extension-element-prefixes=\"my\">"
                                                  "<xsl:output method=\"xml\"/>"
                                                  "<xsl:template match=\"/root/nested\">"
                                                  "<xsl:choose>"
                                                  "<xsl:when test=\"element-available('my:test')\">"
                                                  "<my:test/>"
                                                  "</xsl:when>"
                                                  "<xsl:otherwise>"
                                                  "my:test extension element is NOT available"
                                                  "</xsl:otherwise>"
                                                  "</xsl:choose>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtElem3 *            myElem = new myExtElem3;
        sheet.register_extension_element( myElem, "test", "http://bla.bla.bla",
                                          xml::type_own );

        xml::document           result = sheet.apply( doc );

        // Exception is expected
        BOOST_FAIL( "Expected exception for an extension element. Received nothing (04)" );
    }
    catch ( const xslt::exception &  ex )
    {
        std::string     msg( ex.what() );
        BOOST_CHECK( msg.find( "ERROR REPORT TEST" ) != std::string::npos );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Expected xslt::exception, received std:exception: (04)" ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Expected xslt::exception, received unknown exception (04)" );
    }
    return;
}




class myExtElem5 : public xslt::extension_element
{
    public:
        virtual void process (xml::node &               input_node,
                              const xml::node &         instruction_node,
                              xml::node &               insert_point,
                              const xml::document &     doc)
        {
            const xml::attributes &             attrs = instruction_node.get_attributes();
            xml::attributes::const_iterator     i = attrs.find( "attr" );

            // For this test case both versions of the 'evaluate' should work
            // 1. xslt::xpath_object  value = evaluate( i->get_value() );
            // 2. xslt::xpath_object  value = evaluate( i->get_value(), input_node );
            xslt::xpath_object  value = evaluate( i->get_value() );
            xml::node           my( "inserted", value.get_as_string().c_str() );

            insert_point.push_back( my );
        }
};


BOOST_AUTO_TEST_CASE( XSLTExtElemTest05 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><Node Status='deleted'>Node text</Node>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://blah.blah.blah\" extension-element-prefixes=\"my\">"
                                                  "<xsl:output method=\"xml\"/>"
                                                  "<xsl:template match=\"Node\">"
                                                  "<my:elem name=\"somename\" attr=\"@Status\"/>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtElem5 *            myElem = new myExtElem5;
        sheet.register_extension_element( myElem, "elem", "http://blah.blah.blah",
                                          xml::type_own );

        xml::document           result = sheet.apply( doc );

        // The element was registered and used
        xml::node &             result_root = result.get_root_node();

        BOOST_CHECK( result_root.get_name() == std::string( "inserted" ) );
        BOOST_CHECK( result_root.get_content() == std::string( "deleted" ) );
    }
    catch ( const xslt::exception &  ex )
    {
        BOOST_FAIL( std::string( "Extension element test 5 failed, xslt::exception: " ) + ex.what() );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Extension element test 5 failed, std::exception: " ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Extention element test 5 failed" );
    }
    return;
}




class myExtElem6 : public xslt::extension_element
{
    public:
        virtual void process (xml::node &               input_node,
                              const xml::node &         instruction_node,
                              xml::node &               insert_point,
                              const xml::document &     doc)
        {
            const xml::attributes &             attrs = instruction_node.get_attributes();
            xml::attributes::const_iterator     i = attrs.find( "attr" );

            // For this test case both versions of the 'evaluate' should work
            // 1. xslt::xpath_object  value = evaluate( i->get_value() );
            // 2. xslt::xpath_object  value = evaluate( i->get_value(), input_node );
            xslt::xpath_object  value = evaluate( i->get_value() );
            xml::node           my( "inserted", value.get_as_string().c_str() );

            insert_point.push_back( my );
        }
};


BOOST_AUTO_TEST_CASE( XSLTExtElemTest06 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><Node Status='deleted'>Node text</Node>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://blah.blah.blah\" extension-element-prefixes=\"my\">"
                                                  "<xsl:output method=\"xml\"/>"
                                                  "<xsl:template match=\"Node\">"
                                                  "<xsl:variable name=\"a_var\">"
                                                  "<xsl:value-of select=\"concat(@Status, '_old')\"/>"
                                                  "</xsl:variable>"
                                                  "<my:elem name=\"somename\" attr=\"$a_var\"/>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtElem6 *            myElem = new myExtElem6;
        sheet.register_extension_element( myElem, "elem", "http://blah.blah.blah",
                                          xml::type_own );

        xml::document           result = sheet.apply( doc );

        // The element was registered and used
        xml::node &             result_root = result.get_root_node();

        BOOST_CHECK( result_root.get_name() == std::string( "inserted" ) );
        BOOST_CHECK( result_root.get_content() == std::string( "deleted_old" ) );
    }
    catch ( const xslt::exception &  ex )
    {
        BOOST_FAIL( std::string( "Extension element test 6 failed, xslt::exception: " ) + ex.what() );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Extension element test 6 failed, std::exception: " ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Extention element test 6 failed" );
    }
    return;
}



class myExtElem7 : public xslt::extension_element
{
    public:
        virtual void process (xml::node &               input_node,
                              const xml::node &         instruction_node,
                              xml::node &               insert_point,
                              const xml::document &     doc)
        {
            const xml::attributes &             attrs = instruction_node.get_attributes();
            xml::attributes::const_iterator     i = attrs.find( "attr" );

            // For this test case both versions of the 'evaluate' should work
            // 1. xslt::xpath_object  value = evaluate( i->get_value() );
            // 2. xslt::xpath_object  value = evaluate( i->get_value(), input_node );
            xslt::xpath_object  value = evaluate( i->get_value() );
            xml::node           my( "inserted", value.get_as_string().c_str() );

            insert_point.push_back( my );
        }
};


BOOST_AUTO_TEST_CASE( XSLTExtElemTest07 )
{
    try
    {
        std::string             doc_as_string = "<?xml version=\"1.0\"?><Node Status='deleted'><SubNode>Some text</SubNode></Node>";
        xml::error_messages     msgs;
        xml::document           doc( doc_as_string.c_str(), doc_as_string.size(), &msgs );

        // The expression
        // <my:elem name="somename" attr="SubNode/text()"/>
        // works too.
        std::string             style_as_string = "<?xml version=\"1.0\"?> "
                                                  "<xsl:stylesheet version=\"1.0\" "
                                                  "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" "
                                                  "xmlns:my=\"http://blah.blah.blah\" extension-element-prefixes=\"my\">"
                                                  "<xsl:output method=\"xml\"/>"
                                                  "<xsl:template match=\"Node\">"
                                                  "<my:elem name=\"somename\" attr=\"SubNode\"/>"
                                                  "</xsl:template>"
                                                  "</xsl:stylesheet>";
        xslt::stylesheet        sheet( style_as_string.c_str(), style_as_string.size() );

        myExtElem7 *            myElem = new myExtElem7;
        sheet.register_extension_element( myElem, "elem", "http://blah.blah.blah",
                                          xml::type_own );

        xml::document           result = sheet.apply( doc );

        // The element was registered and used
        xml::node &             result_root = result.get_root_node();

        BOOST_CHECK( result_root.get_name() == std::string( "inserted" ) );
        BOOST_CHECK( result_root.get_content() == std::string( "Some text" ) );
    }
    catch ( const xslt::exception &  ex )
    {
        BOOST_FAIL( std::string( "Extension element test 7 failed, xslt::exception: " ) + ex.what() );
    }
    catch ( const std::exception &  ex )
    {
        BOOST_FAIL( std::string( "Extension element test 7 failed, std::exception: " ) + ex.what() );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Extention element test 7 failed" );
    }
    return;
}


