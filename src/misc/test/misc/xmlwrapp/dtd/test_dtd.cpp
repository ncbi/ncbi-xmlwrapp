/*  $Id: test_dtd.cpp 487761 2015-12-21 19:43:34Z satskyse $
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


BOOST_AUTO_TEST_CASE( DtdTest01 )
{
    try
    {
        xml::error_messages     msgs;
        xml::dtd                d( "data/01.dtd", &msgs );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xsd schema test (01) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest01a )
{
    try
    {
        xml::error_messages msgs;
        xml::dtd            d( "data/01.dtd", &msgs );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xsd schema test (01a) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest01b )
{
    try
    {
        xml::dtd            d( "data/01.dtd", NULL );
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing xsd schema test (01b) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest02 )
{
    try
    {
        xml::dtd    d();
    }
    catch ( ... )
    {
        BOOST_FAIL( "Constructing empty dtd test (02) failed" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest03 )
{
    try
    {
        xml::error_messages     msgs;
        xml::dtd                d( "data/not-existed-file.dtd", &msgs );
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
    BOOST_FAIL( "Constructing dtd test (03) failed (not existed file)" );
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest03a )
{
    try
    {
        xml::error_messages msgs;
        xml::dtd            d( "data/not-existed-file.dtd", &msgs );
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
    BOOST_FAIL( "Constructing dtd test (03a) failed (not existed file)" );
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest03b )
{
    try
    {
        xml::dtd            d( "data/not-existed-file.dtd", NULL );
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
    BOOST_FAIL( "Constructing dtd test (03b) failed (not existed file)" );
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest04 )
{
    try
    {
        xml::error_messages     msgs;
        xml::dtd                d( NULL, &msgs );
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
    BOOST_FAIL( "Constructing dtd test (04) failed (NULL as a file name)" );
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest05 )
{
    try
    {
        xml::error_messages     msgs;
        xml::dtd                d( "data/05.dtd", &msgs );
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
        std::ofstream   f( "data/05.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing dtd test (05) failed (bad file content)" );
    return;
}

BOOST_AUTO_TEST_CASE( DtdTest05a )
{
    try
    {
        xml::error_messages msgs;
        xml::dtd            d( "data/05.dtd", &msgs );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/05a.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        std::ofstream   f( "data/05a.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing dtd test (05a) failed (bad file content)" );
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest05b )
{
    try
    {
        xml::dtd            d( "data/05.dtd", NULL );
    }
    catch ( std::exception & ex )
    {
        // Exception is expected
        std::ofstream   f( "data/05b.out" );
        f << ex.what();
        f.close();
        return;
    }
    catch ( ... )
    {
        std::ofstream   f( "data/05b.out" );
        f << "Unknown exception";
        f.close();
        return;
    }
    BOOST_FAIL( "Constructing dtd test (05b) failed (bad file content)" );
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest06 )
{
    try
    {
        xml::document       doc( "data/06.xml", NULL );

        if ( doc.validate() == false )
        {
            BOOST_FAIL( "Validation test (06) failed (embedded dtd)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/06.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Validation test (06) failed (embedded dtd, exception)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/06.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Validation test (06) failed (embedded dtd, exception)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest06a )
{
    try
    {
        xml::document       doc( "data/06.xml", NULL );
        xml::error_messages msgs;

        if ( doc.validate( &msgs ) == false )
        {
            BOOST_FAIL( "Validation test (06a-1) failed (embedded dtd)" );
        }
        if ( doc.validate( NULL, xml::type_warnings_are_errors ) == false )
        {
            BOOST_FAIL( "Validation test (06a-2) failed (embedded dtd)" );
        }
        if ( doc.validate( (xml::error_messages*) NULL ) == false )
        {
            BOOST_FAIL( "Validation test (06a-3) failed (embedded dtd)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/06a.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Validation test (06a) failed (embedded dtd, exception)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/06a.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Validation test (06a) failed (embedded dtd, exception)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest08a )
{
    try
    {
        xml::document       doc( "data/08.xml", NULL );
        xml::error_messages msgs;

        if ( doc.validate( &msgs ) == false )
        {
            BOOST_FAIL( "Validation test (08a-1) failed (embedded dtd)" );
        }
        BOOST_CHECK( msgs.get_messages().empty() );

        if ( doc.validate( (xml::error_messages*) NULL ) == false )
        {
            BOOST_FAIL( "Validation test (08a-2) failed (embedded dtd)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/08a.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Validation test (08a) failed (embedded dtd, exception)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/08a.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Validation test (08a) failed (embedded dtd, exception)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest09a )
{
    try
    {
        xml::document       doc( "data/09.xml", NULL );
        xml::error_messages msgs;

        if ( doc.validate( &msgs ) == true )
        {
            BOOST_FAIL( "Validation test (09a-1) failed (embedded dtd, invalid doc)" );
        }
        BOOST_CHECK( !msgs.get_messages().empty() );

        if ( doc.validate( (xml::error_messages*) NULL ) == true )
        {
            BOOST_FAIL( "Validation test (09a-2) failed (embedded dtd, invalid doc)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/09a.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Validation test (09a) failed (embedded dtd, exception)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/09a.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Validation test (09a) failed (embedded dtd, exception)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest10a )
{
    try
    {
        xml::document       doc( "data/10.xml", NULL );
        xml::error_messages dtd_msgs;
        xml::dtd            d( "data/10.dtd", &dtd_msgs );
        xml::error_messages msgs;

        if ( doc.validate( d, &msgs ) == false )
        {
            BOOST_FAIL( "Validation test (10a-1) failed (external dtd, valid doc)" );
        }
        BOOST_CHECK( msgs.get_messages().empty() );

        if ( doc.validate( d, NULL ) == false )
        {
            BOOST_FAIL( "Validation test (10a-2) failed (external dtd, valid doc)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/10a.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Validation test (10a) failed (external dtd, exception)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/10a.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Validation test (10a) failed (external dtd, exception)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest11a )
{
    try
    {
        xml::document       doc( "data/11.xml", NULL );
        xml::error_messages dtd_msgs;
        xml::dtd            d( "data/11.dtd", &dtd_msgs );
        xml::error_messages msgs;

        if ( d.validate( doc, &msgs, xml::type_warnings_not_errors ) == false )
        {
            BOOST_FAIL( "Validation test (11a-1) failed (external dtd, valid doc)" );
        }
        BOOST_CHECK( msgs.get_messages().empty() );

        if ( d.validate( doc, NULL, xml::type_warnings_not_errors ) == false )
        {
            BOOST_FAIL( "Validation test (11a-2) failed (external dtd, valid doc)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/11a.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Validation test (11a) failed (external dtd, exception)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/11a.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Validation test (11a) failed (external dtd, exception)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest12a )
{
    try
    {
        xml::document       doc( "data/12.xml", NULL );
        xml::error_messages dtd_msgs;
        xml::dtd            d( "data/12.dtd", &dtd_msgs );
        xml::error_messages msgs;

        if ( doc.validate( d, &msgs ) == true )
        {
            BOOST_FAIL( "Validation test (12a-1) failed (external dtd, invalid doc)" );
        }
        BOOST_CHECK( !msgs.get_messages().empty() );

        if ( doc.validate( d, NULL ) == true )
        {
            BOOST_FAIL( "Validation test (12a-2) failed (external dtd, invalid doc)" );
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/12a.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Validation test (12a) failed (external dtd, exception)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/12a.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Validation test (12a) failed (external dtd, exception)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest16 )
{
    try
    {
        xml::document       doc( "data/16.xml", NULL );

        BOOST_CHECK( doc.has_internal_subset() == false );
        BOOST_CHECK( doc.has_external_subset() == false );

        try
        {
            doc.get_internal_subset();
            BOOST_FAIL( "Dtd accessors test (16) failed (internal dtd is not expected)" );
        }
        catch ( std::exception & ex )
        {
            // Exception is expected
        }

        try
        {
            doc.get_external_subset();
            BOOST_FAIL( "Dtd accessors test (16) failed (external dtd is not expected)" );
        }
        catch ( std::exception & ex )
        {
            // Exception is expected
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/16.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Dtd accessors test (16) failed (no external dtd, no internal dtd)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/16.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Dtd accessors test (16) failed (no external dtd, no internal dtd)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest17 )
{
    try
    {
        xml::document       doc( "data/17.xml", NULL );

        BOOST_CHECK( doc.has_internal_subset() == true );
        BOOST_CHECK( doc.has_external_subset() == false );

        const xml::dtd &    int_dtd( doc.get_internal_subset() );

        BOOST_CHECK( std::string( "urn:newsml:iptc.org:20001006:NewsMLv1.0:1" ) == int_dtd.get_public_id() );
        BOOST_CHECK( std::string( "17.dtd" ) == int_dtd.get_system_id() );
        BOOST_CHECK( std::string( "NewsML" ) == int_dtd.get_name() );

        try
        {
            doc.get_external_subset();
            BOOST_FAIL( "Dtd accessors test (17) failed (external dtd is not expected)" );
        }
        catch ( std::exception & ex )
        {
            // Exception is expected
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/17.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Dtd accessors test (17) failed (no external dtd, with internal dtd)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/17.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Dtd accessors test (17) failed (no external dtd, with internal dtd)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest18 )
{
    try
    {
        xml::document       doc( "data/18.xml", NULL );

        BOOST_CHECK( doc.has_internal_subset() == true );
        BOOST_CHECK( doc.has_external_subset() == true );

        const xml::dtd &    int_dtd( doc.get_internal_subset() );

        BOOST_CHECK( NULL == int_dtd.get_public_id() );
        BOOST_CHECK( std::string( "18.dtd" ) == int_dtd.get_system_id() );
        BOOST_CHECK( std::string( "document" ) == int_dtd.get_name() );

        try
        {
            doc.get_external_subset();
        }
        catch ( std::exception & ex )
        {
            // Exception is expected
        }
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/18.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Dtd accessors test (18) failed (with external dtd, no internal dtd)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/18.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Dtd accessors test (18) failed (with external dtd, no internal dtd)" );
    }
    return;
}


BOOST_AUTO_TEST_CASE( DtdTest19 )
{
    // In case of absent external dtd file libxml2 generates a warning but not
    // an error!

    try
    {
        xml::error_messages     msgs;
        xml::document           doc( "data/19.xml", &msgs, xml::type_warnings_are_errors );
        BOOST_FAIL( "The xml file points to a non-existent file "
                    "however a warning is not generated (19)" );
    }
    catch ( ... )
    {
        // Exception is expected
    }
    return;
}

// Note: I was not able to simulate the case when libxml2 forms the extSubset
// pointer in the document structure. The functionality to access the extSubset
// is kept in the xmlwrapp hoping to find some examples in the future.



BOOST_AUTO_TEST_CASE( DtdTest20 )
{
    try
    {
        xml::document       doc( "data/20.xml", NULL );
        xml::error_messages dtd_msgs;
        xml::dtd            d( "data/20.dtd", &dtd_msgs );
        xml::error_messages msgs;

        bool                ret = doc.validate( d, &msgs );
        BOOST_CHECK( ret == false );

        BOOST_CHECK( msgs.get_messages().size() == 2 );
        const xml::error_messages::error_messages_type &
                                            messages = msgs.get_messages();
        const xml::error_message            m = * messages.begin();

        xml::error_messages::error_messages_type::const_iterator  k = 
                                            messages.begin();


        BOOST_CHECK( k->get_line() == 2 );
        BOOST_CHECK( k->get_filename() == "data/20.xml" );

        ++k;
        BOOST_CHECK( k->get_line() == 3 );
        BOOST_CHECK( k->get_filename() == "data/20.xml" );
    }
    catch ( std::exception & ex )
    {
        std::ofstream   f( "data/20.out" );
        f << ex.what();
        f.close();
        BOOST_FAIL( "Validation test (20) failed (dtd error messages)" );
    }
    catch ( ... )
    {
        std::ofstream   f( "data/20.out" );
        f << "Unknown exception";
        f.close();
        BOOST_FAIL( "Validation test (20) failed (dtd error messages)" );
    }
}


