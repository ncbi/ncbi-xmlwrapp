/*  $Id: test_schema.cpp 487956 2015-12-23 14:30:44Z satskyse $
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


BOOST_AUTO_TEST_CASE( SchemaTest01 )
{
    try
    {
        xml::schema     xsd( "data/01.xsd" );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xsd schema test (01) failed" );
    }
}


BOOST_AUTO_TEST_CASE( SchemaTest01a )
{
    try
    {
        xml::error_messages     msgs;
        xml::schema             xsd( "data/01.xsd", &msgs );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xsd schema test (01a) failed" );
    }
}


BOOST_AUTO_TEST_CASE( SchemaTest01b )
{
    try
    {
        xml::schema             xsd( "data/01.xsd",
                                     (xml::error_messages*) NULL );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xsd schema test (01a) failed" );
    }
}



static std::string  schema_as_string( "<?xml version=\"1.0\"?>"
                                      "<xsd:schema xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
                                      "  <xsd:element name=\"doc\">"
                                      "    <xsd:complexType>"
                                      "      <xsd:all minOccurs=\"1\">"
                                      "        <xsd:element name=\"a\"/>"
                                      "        <xsd:element name=\"b\"/>"
                                      "        <xsd:element name=\"c\"/>"
                                      "      </xsd:all>"
                                      "    </xsd:complexType>"
                                      "  </xsd:element>"
                                      "</xsd:schema>" );

BOOST_AUTO_TEST_CASE( SchemaTest02 )
{
    try
    {
        xml::error_messages     msgs;
        xml::schema             xsd( schema_as_string.c_str(),
                                     schema_as_string.size(), &msgs );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xsd schema test (02) failed" );
    }
}


BOOST_AUTO_TEST_CASE( SchemaTest02a )
{
    try
    {
        xml::error_messages     msgs;
        xml::schema             xsd( schema_as_string.c_str(),
                                     schema_as_string.size(),
                                     &msgs );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xsd schema test (02a) failed" );
    }
}


BOOST_AUTO_TEST_CASE( SchemaTest02b )
{
    try
    {
        xml::schema             xsd( schema_as_string.c_str(),
                                     schema_as_string.size(),
                                     NULL );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xsd schema test (02a) failed" );
    }
}


BOOST_AUTO_TEST_CASE( SchemaTest03 )
{
    try
    {
        xml::schema     xsd( "data/not-existed-file.xsd" );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/03.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        // Exception is expected
        std::ofstream   f( "data/03.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing xsd schema test (03) failed (not existed file)" );
}


BOOST_AUTO_TEST_CASE( SchemaTest03a )
{
    try
    {
        xml::error_messages msgs;
        xml::schema         xsd( "data/not-existed-file.xsd", &msgs );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/03a.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        // Exception is expected
        std::ofstream   f( "data/03a.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing xsd schema test (03a) failed (not existed file)" );
}


BOOST_AUTO_TEST_CASE( SchemaTest03b )
{
    try
    {
        xml::schema         xsd( "data/not-existed-file.xsd",
                                 (xml::error_messages*) NULL );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/03b.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        // Exception is expected
        std::ofstream   f( "data/03b.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing xsd schema test (03b) failed (not existed file)" );
}


BOOST_AUTO_TEST_CASE( SchemaTest04 )
{
    try
    {
        xml::schema     xsd( NULL );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/04.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        // Exception is expected
        std::ofstream   f( "data/04.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing xsd schema test (04) failed (NULL as a file name)" );
}


BOOST_AUTO_TEST_CASE( SchemaTest05 )
{
    try
    {
        xml::error_messages     msgs;
        xml::schema             xsd( NULL, 777, &msgs );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/05.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        // Exception is expected
        std::ofstream   f( "data/05.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing xsd schema test (05) failed (NULL as a pointer to data)" );
}


BOOST_AUTO_TEST_CASE( SchemaTest06a )
{
    try
    {
        xml::schema         xsd( "data/06.xsd" );
        xml::document       doc( "data/06.xml", NULL );
        xml::error_messages msgs;

        if (!xsd.validate(doc, &msgs))
        {
            BOOST_FAIL( "Validation test (06a) failed (correct document)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/06a.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Exception. Validation test (06a) failed (correct document)." );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/06a.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Unknown exception. Validation test (06a) failed (correct document)" );
    }
}


BOOST_AUTO_TEST_CASE( SchemaTest06b )
{
    try
    {
        xml::schema         xsd( "data/06.xsd" );
        xml::document       doc( "data/06.xml", NULL );

        if (!xsd.validate(doc, NULL))
        {
            BOOST_FAIL( "Validation test (06b) failed (correct document)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/06b.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Exception. Validation test (06b) failed (correct document)." );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/06b.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Unknown exception. Validation test (06b) failed (correct document)" );
    }
}


BOOST_AUTO_TEST_CASE( SchemaTest07 )
{
    try
    {
        xml::schema     xsd( "data/07.xsd" );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/07.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        // Exception is expected
        std::ofstream   f( "data/07.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing xsd schema test (07) failed (bad file content)" );
}


BOOST_AUTO_TEST_CASE( SchemaTest07a )
{
    try
    {
        xml::error_messages msgs;
        xml::schema         xsd( "data/07.xsd", &msgs );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/07a.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        // Exception is expected
        std::ofstream   f( "data/07a.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing xsd schema test (07a) failed (bad file content)" );
}


BOOST_AUTO_TEST_CASE( SchemaTest07b )
{
    try
    {
        xml::schema         xsd( "data/07.xsd",
                                 (xml::error_messages*) NULL );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/07b.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        // Exception is expected
        std::ofstream   f( "data/07b.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing xsd schema test (07b) failed (bad file content)" );
}


BOOST_AUTO_TEST_CASE( SchemaTest08a )
{
    try
    {
        xml::schema         xsd( "data/08.xsd" );
        xml::document       doc( "data/08.xml", NULL );
        xml::error_messages msgs;

        if (xsd.validate(doc, &msgs))
        {
            BOOST_FAIL( "Validation test (08a-1) failed (incorrect document)" );
        }
        BOOST_CHECK( !msgs.get_messages().empty() );

        if (xsd.validate(doc, NULL))
        {
            BOOST_FAIL( "Validation test (08a-2) failed (incorrect document)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/08a.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Exception. Validation test (08a) failed (incorrect document)." );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/08a.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Unknown exception. Validation test (08a) failed (incorrect document)" );
    }
}


BOOST_AUTO_TEST_CASE( SchemaTest09a )
{
    try
    {
        xml::schema         xsd( "data/09.xsd" );
        xml::document       incorrect_doc( "data/09incorrect.xml", NULL );
        xml::document       correct_doc( "data/09correct.xml", NULL );
        xml::error_messages msgs;

        if (xsd.validate(incorrect_doc, &msgs))
        {
            BOOST_FAIL( "Validation test (09a-1) failed (sequence, step 1)" );
        }
        BOOST_CHECK( !msgs.get_messages().empty() );

        if (xsd.validate(incorrect_doc, NULL))
        {
            BOOST_FAIL( "Validation test (09a-2) failed (sequence, step 1)" );
        }


        if (!xsd.validate(correct_doc, &msgs))
        {
            BOOST_FAIL( "Validation test (09a-1) failed (sequence, step 2)" );
        }
        BOOST_CHECK( msgs.get_messages().empty() );

        if (!xsd.validate(correct_doc, NULL))
        {
            BOOST_FAIL( "Validation test (09a-2) failed (sequence, step 2)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/09a.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Exception. Validation test (09a) failed (sequence)." );
    }
    catch ( ... )
    {
        // Exception is expected
        std::ofstream   f( "data/09a.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Unknown exception. Validation test (09a) failed (sequence)" );
    }
}


BOOST_AUTO_TEST_CASE( SchemaTest10 )
{
    xml::error_messages msgs;

    try
    {
        xml::schema         xsd( "data/10.xsd" );
        xml::document       incorrect_doc( "data/10incorrect.xml", NULL );

        bool    ret = xsd.validate(incorrect_doc, &msgs);
        BOOST_CHECK( ret == false );

        BOOST_CHECK( msgs.get_messages().size() == 2 );
        const xml::error_messages::error_messages_type &
                                            messages = msgs.get_messages();
        xml::error_messages::error_messages_type::const_iterator  k = 
                                            messages.begin();

        BOOST_CHECK( k->get_line() == 4 );
        BOOST_CHECK( k->get_filename() == "data/10incorrect.xml" );

        ++k;
        BOOST_CHECK( k->get_line() == 5 );
        BOOST_CHECK( k->get_filename() == "data/10incorrect.xml" );
    }
    catch ( std::exception &  Exception )
    {
        // std exception is expected
        std::ofstream   f( "data/10.out" );
        f << "Exception message: " << Exception.what() << "\n";
        f.close();
        BOOST_FAIL( "Unknown exception. Validation test (10) failed" );
    }
    catch ( ... )
    {
        // Any other exception is NOT expected
        std::ofstream   f( "data/10.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Unknown exception. Validation test (10) failed" );
    }
}


