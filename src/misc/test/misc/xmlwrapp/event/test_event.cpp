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
 * $Id: test_event.cpp 460080 2015-02-24 19:50:48Z satskyse $
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
#include <stdexcept>



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





class test_parser1 : public xml::event_parser
{
    public:
        test_parser1( ostream &  stream ) :
            ostr( stream )
        { ostr << "-- CTOR:\n"; }

        ~test_parser1( void )
        { ostr << "-- DTOR:\n"; }

    private:
        bool start_element( const std::string &  name,
                            const xml::event_parser::attrs_type & )
        {
            ostr << "-- START_ELEMENT: " << name << "\n";
            return true;
        }

        bool end_element( const std::string &  name )
        {
            ostr << "-- END_ELEMENT: " << name << "\n";
            return true;
        }

        bool text( const std::string &  contents )
        {
            ostr << "-- TEXT: " << contents << "\n";
            return true;
        }

        bool processing_instruction( const std::string &  target,
                                     const std::string &  data )
        {
            ostr << "-- PI: TARGET=" << target << " DATA=" << data << "\n";
            return true;
        }

        bool comment( const std::string &  contents )
        {
            ostr << "-- COMMENT: " << contents << "\n";
            return true;
        }

        bool cdata( const std::string &  contents )
        {
            ostr << "-- CDATA: " << contents << "\n";
            return true;
        }

    private:
        ostream &   ostr;
};



BOOST_AUTO_TEST_CASE( EventTest01a1 )
{
    CNcbiOstrstream     ostr;
    test_parser1        parser( ostr );
    xml::error_messages msgs;

    if ( !parser.parse_file( "data/01.xml", &msgs ) )
    {
        ostr << "\n" << "error parsing file: data/01.xml: " << msgs.print() << "\n";
        saveStreamToFile( ostr, "data/01a1.out.real" );
        BOOST_FAIL( "Event parser test (01a1) failed" );
    }

    saveStreamToFile( ostr, "data/01a1.out.real" );
    BOOST_CHECK( compareStreamAndFile( ostr, "data/01.out" ) );

    return;
}


BOOST_AUTO_TEST_CASE( EventTest01a2 )
{
    CNcbiOstrstream     ostr;
    test_parser1        parser( ostr );

    if ( !parser.parse_file( "data/01.xml", NULL ) )
    {
        ostr << "\n" << "error parsing file: data/01.xml\n";
        saveStreamToFile( ostr, "data/01a2.out.real" );
        BOOST_FAIL( "Event parser test (01a2) failed" );
    }

    saveStreamToFile( ostr, "data/01a2.out.real" );
    BOOST_CHECK( compareStreamAndFile( ostr, "data/01.out" ) );

    return;
}




//
// make sure that returning false or throwing an exception will cause the
// parser to stop.
//

class test_parser2 : public xml::event_parser
{
    public:
        test_parser2( ostream &          stream,
                      const char *       cbname,
                      bool               use_throw ) :
            ostr( stream ), cbname_( cbname ), throw_( use_throw )
        {}

    private:
        bool start_element( const std::string &, const xml::event_parser::attrs_type & )
        {
            if ( cbname_ == "start_element" )
            {
                ostr << cbname_;
                if ( throw_ ) { ostr << " throw"; throw cbname_; }
                return false;
            }
            return true;
        }

        bool end_element( const std::string & )
        {
            if ( cbname_ == "end_element" )
            {
                ostr << cbname_;
                if ( throw_ ) { ostr << " throw"; throw cbname_; }
                return false;
            }
            return true;
        }

        bool text( const std::string & )
        {
            if ( cbname_ == "text" )
            {
                ostr << cbname_;
                if ( throw_ ) { ostr << " throw"; throw cbname_; }
                return false;
            }
            return true;
        }

        bool processing_instruction( const std::string &, const std::string & )
        {
            if ( cbname_ == "processing_instruction" )
            {
                ostr << cbname_;
                if ( throw_ ) { ostr << " throw"; throw cbname_; }
                return false;
            }
            return true;
        }

        bool comment( const std::string & )
        {
            if ( cbname_ == "comment" )
            {
                ostr << cbname_;
                if ( throw_ ) { ostr << " throw"; throw cbname_; }
                return false;
            }
            return true;
        }

        bool cdata( const std::string & )
        {
            if ( cbname_ == "cdata" )
            {
                ostr << cbname_;
                if ( throw_ ) { ostr << " throw"; throw cbname_; }
                return false;
            }
            return true;
        }

        ostream &           ostr;
        std::string         cbname_;
        bool                throw_;
};



BOOST_AUTO_TEST_CASE( EventTest02a1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "start_element", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "start_element" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02a2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "start_element", false );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "start_element" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02b1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "end_element", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "end_element" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02b2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "end_element", false );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "end_element" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02c1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "text", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "text" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02c2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "text", false );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "text" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02d1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "processing_instruction", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "processing_instruction" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02d2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "processing_instruction", false );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "processing_instruction" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02e1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "comment", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "comment" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02e2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "comment", false );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "comment" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02f1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "cdata", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "cdata" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02f2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "cdata", false );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "cdata" );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest02g1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "start_element", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "start_element throw" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02g2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "start_element", true );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "start_element throw" );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest02h1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "end_element", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "end_element throw" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02h2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "end_element", true );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "end_element throw" );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest02i1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "text", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "text throw" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02i2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "text", true );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "text throw" );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest02j1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "processing_instruction", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "processing_instruction throw" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02j2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "processing_instruction", true );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "processing_instruction throw" );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest02k1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "comment", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "comment throw" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02k2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "comment", true );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "comment throw" );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest02l1 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "cdata", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "cdata throw" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02l2 )
{
    CNcbiOstrstream     ostr;
    test_parser2        parser( ostr, "cdata", true );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( string(CNcbiOstrstreamToString(ostr)) == "cdata throw" );
    return;
}




//
// make sure that CDATA text goes to the text callback when there is no cdata
// callback.
//

class test_parser3 : public xml::event_parser
{
    public:
        test_parser3( ostream &  stream ) :
            ostr( stream )
        {}

    private:
        bool start_element( const std::string &,
                            const xml::event_parser::attrs_type & )
        {
            return true;
        }

        bool end_element( const std::string & )
        {
            return true;
        }

        bool text( const std::string &  contents )
        {
            ostr << contents;
            return true;
        }

    private:
        ostream &  ostr;
};



BOOST_AUTO_TEST_CASE( EventTest02m1 )
{
    CNcbiOstrstream     ostr;
    test_parser3        parser( ostr );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( status );
    saveStreamToFile( ostr, "data/02m1.out.real" );
    BOOST_CHECK( compareStreamAndFile( ostr, "data/02m.out" ) );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest02m2 )
{
    CNcbiOstrstream     ostr;
    test_parser3        parser( ostr );
    bool                status( parser.parse_file( "data/01.xml", NULL ) );

    BOOST_CHECK( status );
    saveStreamToFile( ostr, "data/02m2.out.real" );
    BOOST_CHECK( compareStreamAndFile( ostr, "data/02m.out" ) );
    return;
}


//
// Handlers mask test
//

class test_parser4 : public xml::event_parser
{
    public:
            // Handler calls counters
        int     start_doc_counter;
        int     end_doc_counter;
        int     start_element_counter;
        int     end_element_counter;
        int     char_counter;
        int     pi_counter;
        int     comment_counter;
        int     cdata_counter;
        int     notation_decl_counter;
        int     entity_decl_counter;
        int     unparsed_entity_decl_counter;
        int     external_subset_counter;
        int     internal_subset_counter;
        int     attr_decl_counter;
        int     element_decl_counter;
        int     reference_counter;

    public:
        test_parser4( xml::event_parser::sax_handlers_mask  mask ) :
            xml::event_parser( mask ),
            start_doc_counter( 0 ), end_doc_counter( 0 ),
            start_element_counter( 0 ), end_element_counter( 0 ),
            char_counter( 0 ), pi_counter( 0 ),
            comment_counter( 0 ), cdata_counter( 0 ),
            notation_decl_counter( 0 ), entity_decl_counter( 0 ),
            unparsed_entity_decl_counter( 0 ), external_subset_counter( 0 ),
            internal_subset_counter( 0 ), attr_decl_counter( 0 ),
            element_decl_counter( 0 ), reference_counter( 0 )
        {}

        ~test_parser4( void )
        {}

    private:
        bool start_document()
        { ++start_doc_counter; return true; }

        bool end_document()
        { ++end_doc_counter; return true; }

        bool start_element( const std::string &,
                            const xml::event_parser::attrs_type & )
        { ++start_element_counter; return true; }

        bool end_element( const std::string & )
        { ++end_element_counter; return true; }

        bool text( const std::string & )
        { ++char_counter; return true; }

        bool processing_instruction( const std::string &,
                                     const std::string & )
        { ++pi_counter; return true; }

        bool comment( const std::string & )
        { ++comment_counter; return true; }

        bool cdata( const std::string & )
        { ++cdata_counter; return true; }

        bool notation_declaration( const std::string &,
                                   const std::string &,
                                   const std::string & )
        { ++notation_decl_counter; return true; }

        bool entity_declaration( const std::string &,
                                 entity_type,
                                 const std::string &,
                                 const std::string &,
                                 const std::string & )
        { ++entity_decl_counter; return true; }

        bool unparsed_entity_declaration( const std::string &,
                                          const std::string &,
                                          const std::string &,
                                          const std::string & )
        { ++unparsed_entity_decl_counter; return true; }

        bool external_subset_declaration( const std::string &,
                                          const std::string &,
                                          const std::string & )
        { ++external_subset_counter; return true; }

        bool internal_subset_declaration( const std::string &,
                                          const std::string &,
                                          const std::string & )
        { ++internal_subset_counter; return true; }

        bool attribute_declaration( const std::string &,
                                    const std::string &,
                                    attribute_type,
                                    attribute_default_type,
                                    const std::string &,
                                    const values_type & )
        { ++attr_decl_counter; return true; }

        bool element_declaration( const std::string &,
                                  element_content_type,
                                  _xmlElementContent * )
        { ++element_decl_counter; return true; }

        bool entity_reference( const std::string & )
        { ++entity_decl_counter; return true; }
};


BOOST_AUTO_TEST_CASE( EventTest03 )
{
    test_parser4        parser( 0 );    // No handlers allowed
    bool                status( parser.parse_file( "data/03.xml", NULL ) );

    BOOST_CHECK( status );
    BOOST_CHECK( parser.start_doc_counter == 0 );
    BOOST_CHECK( parser.end_doc_counter == 0 );
    BOOST_CHECK( parser.start_element_counter == 0 );
    BOOST_CHECK( parser.end_element_counter == 0 );
    BOOST_CHECK( parser.char_counter == 0 );
    BOOST_CHECK( parser.pi_counter == 0 );
    BOOST_CHECK( parser.comment_counter == 0 );
    BOOST_CHECK( parser.cdata_counter == 0 );
    BOOST_CHECK( parser.notation_decl_counter == 0 );
    BOOST_CHECK( parser.entity_decl_counter == 0 );
    BOOST_CHECK( parser.unparsed_entity_decl_counter == 0 );
    BOOST_CHECK( parser.external_subset_counter == 0 );
    BOOST_CHECK( parser.internal_subset_counter == 0 );
    BOOST_CHECK( parser.attr_decl_counter == 0 );
    BOOST_CHECK( parser.element_decl_counter == 0 );
    BOOST_CHECK( parser.reference_counter == 0 );

    // All handlers are allowed
    test_parser4        parser1( xml::event_parser::default_set                  |
                                 xml::event_parser::start_document_handler       |
                                 xml::event_parser::end_document_handler         |
                                 xml::event_parser::notation_decl_handler        |
                                 xml::event_parser::entity_decl_handler          |
                                 xml::event_parser::unparsed_entity_decl_handler |
                                 xml::event_parser::external_subset_handler      |
                                 xml::event_parser::internal_subset_handler      |
                                 xml::event_parser::attribute_decl_handler       |
                                 xml::event_parser::element_decl_handler         |
                                 xml::event_parser::reference_handler );

    status = parser1.parse_file( "data/03.xml", NULL );
    BOOST_CHECK( status );

    BOOST_CHECK( parser1.start_doc_counter == 1 );
    BOOST_CHECK( parser1.end_doc_counter == 1 );
    BOOST_CHECK( parser1.start_element_counter == 1 );
    BOOST_CHECK( parser1.end_element_counter == 1 );
    BOOST_CHECK( parser1.char_counter == 1 );
    BOOST_CHECK( parser1.pi_counter == 0 );
    BOOST_CHECK( parser1.comment_counter == 32 );
    BOOST_CHECK( parser1.cdata_counter == 0 );
    BOOST_CHECK( parser1.notation_decl_counter == 0 );
    BOOST_CHECK( parser1.entity_decl_counter == 0 );
    BOOST_CHECK( parser1.unparsed_entity_decl_counter == 0 );
    BOOST_CHECK( parser1.external_subset_counter == 1 );
    BOOST_CHECK( parser1.internal_subset_counter == 1 );
    BOOST_CHECK( parser1.attr_decl_counter == 10 );
    BOOST_CHECK( parser1.element_decl_counter == 26 );
    BOOST_CHECK( parser1.reference_counter == 0 );


    // Switch on some of the handlers
    test_parser4        parser2( xml::event_parser::default_set                  |
                                 xml::event_parser::end_document_handler         |
                                 xml::event_parser::notation_decl_handler        |
                                 xml::event_parser::entity_decl_handler          |
                                 xml::event_parser::unparsed_entity_decl_handler |
                                 xml::event_parser::attribute_decl_handler       |
                                 xml::event_parser::element_decl_handler         |
                                 xml::event_parser::reference_handler );

    status = parser2.parse_file( "data/03.xml", NULL );
    BOOST_CHECK( status );

    BOOST_CHECK( parser2.start_doc_counter == 0 );
    BOOST_CHECK( parser2.end_doc_counter == 1 );
    BOOST_CHECK( parser2.start_element_counter == 1 );
    BOOST_CHECK( parser2.end_element_counter == 1 );
    BOOST_CHECK( parser2.char_counter == 1 );
    BOOST_CHECK( parser2.pi_counter == 0 );
    BOOST_CHECK( parser2.comment_counter == 32 );
    BOOST_CHECK( parser2.cdata_counter == 0 );
    BOOST_CHECK( parser2.notation_decl_counter == 0 );
    BOOST_CHECK( parser2.entity_decl_counter == 0 );
    BOOST_CHECK( parser2.unparsed_entity_decl_counter == 0 );
    BOOST_CHECK( parser2.external_subset_counter == 0 );
    BOOST_CHECK( parser2.internal_subset_counter == 0 );
    BOOST_CHECK( parser2.attr_decl_counter == 10 );
    BOOST_CHECK( parser2.element_decl_counter == 26 );
    BOOST_CHECK( parser2.reference_counter == 0 );

    return;
}



class test_parser5 : public xml::event_parser
{
    public:
            // Handler calls counters
        int     warning_counter;
        int     error_counter;

    public:
        test_parser5() :
            xml::event_parser(),
            warning_counter( 0 ), error_counter( 0 )
        {}

        ~test_parser5( void )
        {}

        void reset_counters( void )
        { warning_counter = 0; error_counter = 0; }

    private:
        bool warning( const std::string & )
        { ++warning_counter; return true; }

        bool error( const std::string & )
        { ++error_counter; return true; }

        bool start_element( const std::string &,
                            const xml::event_parser::attrs_type & )
        { return true; }

        bool end_element( const std::string & )
        { return true; }

        bool text( const std::string & )
        { return true; }
};


BOOST_AUTO_TEST_CASE( EventTest05a )
{
    test_parser5        parser;
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/05.xml", &msgs ) );

    BOOST_CHECK( status == false );
    BOOST_CHECK( parser.warning_counter == 1 );
    BOOST_CHECK( parser.error_counter == 1 );
    BOOST_CHECK( msgs.get_messages().size() == 2 );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest05b )
{
    test_parser5        parser;
    bool                status( parser.parse_file( "data/05.xml", NULL ) );

    BOOST_CHECK( status == false );
    BOOST_CHECK( parser.warning_counter == 1 );
    BOOST_CHECK( parser.error_counter == 1 );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest06a )
{
    test_parser5        parser;
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/06.xml", &msgs,
                                                   xml::type_warnings_are_errors ) );

    BOOST_CHECK( status == false );
    BOOST_CHECK( parser.warning_counter == 1 );
    BOOST_CHECK( parser.error_counter == 0 );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest06b )
{
    test_parser5        parser;
    bool                status( parser.parse_file( "data/06.xml", NULL,
                                                   xml::type_warnings_are_errors ) );

    BOOST_CHECK( status == false );
    BOOST_CHECK( parser.warning_counter == 1 );
    BOOST_CHECK( parser.error_counter == 0 );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest07a )
{
    test_parser5        parser;
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/07.xml", &msgs,
                                                   xml::type_warnings_not_errors ) );

    BOOST_CHECK( status == true );
    BOOST_CHECK( parser.warning_counter == 1 );
    BOOST_CHECK( parser.error_counter == 0 );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest07b )
{
    test_parser5        parser;
    bool                status( parser.parse_file( "data/07.xml", NULL,
                                                   xml::type_warnings_not_errors ) );

    BOOST_CHECK( status == true );
    BOOST_CHECK( parser.warning_counter == 1 );
    BOOST_CHECK( parser.error_counter == 0 );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest08a )
{
    test_parser5        parser;
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/08a.xml", &msgs ) );

    BOOST_CHECK( status == false );
    BOOST_CHECK( parser.warning_counter == 0 );
    BOOST_CHECK( parser.error_counter == 1 );
    BOOST_CHECK( msgs.get_messages().size() == 1 );

    parser.reset_counters();
    status = parser.parse_file( "data/08b.xml", &msgs );
    BOOST_CHECK( status == true );
    BOOST_CHECK( parser.warning_counter == 0 );
    BOOST_CHECK( parser.error_counter == 0 );
    BOOST_CHECK( msgs.get_messages().size() == 0 );
    return;
}



class test_parser6 : public xml::event_parser
{
    public:
            // Handler calls counters
        int     warning_counter;
        int     error_counter;

    public:
        test_parser6() :
            xml::event_parser(),
            warning_counter( 0 ), error_counter( 0 )
        {}

        ~test_parser6( void )
        {}

    private:
        // Stop after warning
        bool warning( const std::string & )
        { ++warning_counter; return false; }

        bool error( const std::string & )
        { ++error_counter; return true; }

        bool start_element( const std::string &,
                            const xml::event_parser::attrs_type & )
        { return true; }

        bool end_element( const std::string & )
        { return true; }

        bool text( const std::string & )
        { return true; }
};


BOOST_AUTO_TEST_CASE( EventTest09a )
{
    test_parser6        stop_afetr_warning_parser;
    xml::error_messages msgs;
    bool                status( stop_afetr_warning_parser.parse_file( "data/09.xml", &msgs ) );

    BOOST_CHECK( status == false );
    BOOST_CHECK( stop_afetr_warning_parser.warning_counter == 1 );
    BOOST_CHECK( stop_afetr_warning_parser.error_counter == 0 );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    return;
}



class test_parser10 : public xml::event_parser
{
    public:
        test_parser10( const char *       cbname,
                       bool               use_throw_std ) :
            cbname_( cbname ), throw_std( use_throw_std )
        {}

    private:
        bool start_element( const std::string &, const xml::event_parser::attrs_type & )
        {
            if ( cbname_ == "start_element" )
            {
                if ( throw_std )
                    throw std::runtime_error("start_element std exception");
                throw 42;
            }
            return true;
        }

        bool end_element( const std::string & )
        {
            if ( cbname_ == "end_element" )
            {
                if ( throw_std )
                    throw std::runtime_error("end_element std exception");
                throw 42;
            }
            return true;
        }

        bool text( const std::string & )
        {
            if ( cbname_ == "text" )
            {
                if ( throw_std )
                    throw std::runtime_error("text std exception");
                throw 42;
            }
            return true;
        }

        bool processing_instruction( const std::string &, const std::string & )
        {
            if ( cbname_ == "processing_instruction" )
            {
                if ( throw_std )
                    throw std::runtime_error("processing_instruction std exception");
                throw 42;
            }
            return true;
        }

        bool comment( const std::string & )
        {
            if ( cbname_ == "comment" )
            {
                if ( throw_std )
                    throw std::runtime_error("comment std exception");
                throw 42;
            }
            return true;
        }

        bool cdata( const std::string & )
        {
            if ( cbname_ == "cdata" )
            {
                if ( throw_std )
                    throw std::runtime_error("cdata std exception");
                throw 42;
            }
            return true;
        }

        std::string         cbname_;
        bool                throw_std;
};



BOOST_AUTO_TEST_CASE( EventTest10a )
{
    test_parser10       parser( "start_element", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "start_element std exception" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10a1 )
{
    test_parser10       parser( "start_element", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "user exception in start_element handler" );

    BOOST_CHECK( !status );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10b )
{
    test_parser10       parser( "end_element", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "end_element std exception" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10b1 )
{
    test_parser10       parser( "end_element", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "user exception in end_element handler" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10c )
{
    test_parser10       parser( "text", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "text std exception" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10c1 )
{
    test_parser10       parser( "text", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "user exception in text handler" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10d )
{
    test_parser10       parser( "processing_instruction", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "processing_instruction std exception" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10d1 )
{
    test_parser10       parser( "processing_instruction", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "user exception in processing_instruction handler" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10e )
{
    test_parser10       parser( "comment", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "comment std exception" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10e1 )
{
    test_parser10       parser( "comment", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "user exception in comment handler" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10f )
{
    test_parser10       parser( "cdata", true );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "cdata std exception" );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest10f1 )
{
    test_parser10       parser( "cdata", false );
    xml::error_messages msgs;
    bool                status( parser.parse_file( "data/01.xml", &msgs ) );

    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 1 );
    BOOST_CHECK( msgs.get_messages().begin()->get_message() == "user exception in cdata handler" );
    return;
}



struct test_parser11 : public xml::event_parser
{
    test_parser11() :
        xml::event_parser(xml::event_parser::default_set                  |
                          xml::event_parser::start_document_handler       |
                          xml::event_parser::end_document_handler         |
                          xml::event_parser::notation_decl_handler        |
                          xml::event_parser::entity_decl_handler          |
                          xml::event_parser::unparsed_entity_decl_handler |
                          xml::event_parser::external_subset_handler      |
                          xml::event_parser::internal_subset_handler      |
                          xml::event_parser::attribute_decl_handler       |
                          xml::event_parser::element_decl_handler         |
                          xml::event_parser::reference_handler ),
        start_doc_count(0), end_doc_count(0),
        start_element_count(0), end_element_count(0),
        text_count(0)
    {}

    size_t  start_doc_count;
    size_t  end_doc_count;
    size_t  start_element_count;
    size_t  end_element_count;
    size_t  text_count;

    bool start_document()
    { ++start_doc_count; return true; }

    bool end_document()
    { ++end_doc_count; return true; }

    bool start_element( const std::string &,
                        const xml::event_parser::attrs_type & )
    { ++start_element_count; return true; }

    bool end_element( const std::string & )
    { ++end_element_count; return true; }

    bool text( const std::string & txt)
    { ++text_count; return true; }
};


BOOST_AUTO_TEST_CASE( EventTest11 )
{
    test_parser11           parser;
    xml::error_messages     msgs;
    bool                    status( parser.parse_file( "data/11.xml", &msgs ) );

    BOOST_CHECK( status );
    BOOST_CHECK( msgs.get_messages().size() == 0 );
    BOOST_CHECK( parser.start_doc_count == 1 );
    BOOST_CHECK( parser.end_doc_count == 1 );

    msgs.get_messages().clear();
    status = parser.parse_file( "data/11.xml", &msgs );
    BOOST_CHECK( status );
    BOOST_CHECK( msgs.get_messages().size() == 0 );
    BOOST_CHECK( parser.start_doc_count == 2 );
    BOOST_CHECK( parser.end_doc_count == 2 );
    return;
}

BOOST_AUTO_TEST_CASE( EventTest12 )
{
    test_parser11           parser;
    xml::error_messages     msgs;
    bool                    status( parser.parse_file( "data/12.xml", &msgs ) );

    BOOST_CHECK( status );
    BOOST_CHECK( msgs.get_messages().size() == 0 );
    BOOST_CHECK( parser.start_doc_count == 1 );
    BOOST_CHECK( parser.end_doc_count == 1 );
    BOOST_CHECK( parser.start_element_count == 27 );
    BOOST_CHECK( parser.end_element_count == 27 );
    BOOST_CHECK( parser.text_count == 53 );

    msgs.get_messages().clear();
    status = parser.parse_file( "data/12.xml", &msgs );
    BOOST_CHECK( status );
    BOOST_CHECK( msgs.get_messages().size() == 0 );
    BOOST_CHECK( parser.start_doc_count == 2 );
    BOOST_CHECK( parser.end_doc_count == 2 );
    BOOST_CHECK( parser.start_element_count == 54 );
    BOOST_CHECK( parser.end_element_count == 54 );
    BOOST_CHECK( parser.text_count == 106 );
    return;
}


BOOST_AUTO_TEST_CASE( EventTest13 )
{
    std::string             input( "<?xml version=\"1.0\"?>"
                                   "<DiscrepancyCounts></DiscrepancyCounts>" );
    test_parser11           parser;
    xml::error_messages     msgs;
    bool                    status( parser.parse_chunk( input.c_str(),
                                                        input.size(),
                                                        &msgs ) );
    status = status && parser.parse_finish( &msgs );

    BOOST_CHECK( status );
    BOOST_CHECK( msgs.get_messages().size() == 0 );
    BOOST_CHECK( parser.start_doc_count == 1 );
    BOOST_CHECK( parser.end_doc_count == 1 );

    msgs.get_messages().clear();
    status = parser.parse_chunk( input.c_str(), input.size(), &msgs );
    status = status && parser.parse_finish( &msgs );
    BOOST_CHECK( status );
    BOOST_CHECK( msgs.get_messages().size() == 0 );
    BOOST_CHECK( parser.start_doc_count == 2 );
    BOOST_CHECK( parser.end_doc_count == 2 );
    return;
}



struct test_parser14 : public xml::event_parser
{
    test_parser14() :
        xml::event_parser(xml::event_parser::default_set                  |
                          xml::event_parser::start_document_handler       |
                          xml::event_parser::end_document_handler         |
                          xml::event_parser::notation_decl_handler        |
                          xml::event_parser::entity_decl_handler          |
                          xml::event_parser::unparsed_entity_decl_handler |
                          xml::event_parser::external_subset_handler      |
                          xml::event_parser::internal_subset_handler      |
                          xml::event_parser::attribute_decl_handler       |
                          xml::event_parser::element_decl_handler         |
                          xml::event_parser::reference_handler ),
        start_doc_count(0), end_doc_count(0)
    {}

    size_t  start_doc_count;
    size_t  end_doc_count;

    bool start_document()
    { ++start_doc_count; return false; }

    bool end_document()
    { ++end_doc_count; return true; }

    bool start_element( const std::string &,
                        const xml::event_parser::attrs_type & )
    { return true; }

    bool end_element( const std::string & )
    { return true; }

    bool text( const std::string & txt)
    { return true; }
};


BOOST_AUTO_TEST_CASE( EventTest14 )
{
    std::string             input( "<?xml version=\"1.0\"?>"
                                   "<DiscrepancyCounts></DiscrepancyCounts>" );
    test_parser14           parser;
    xml::error_messages     msgs;
    bool                    status( parser.parse_chunk( input.c_str(),
                                                        input.size(),
                                                        &msgs ) );
    BOOST_CHECK( !status );
    BOOST_CHECK( msgs.get_messages().size() == 0 );
    BOOST_CHECK( parser.start_doc_count == 1 );
    BOOST_CHECK( parser.end_doc_count == 0 );

    // parse_finish() is not called so an exception is expected
    try
    {
        msgs.get_messages().clear();
        // status = 
        parser.parse_chunk( input.c_str(), input.size(), &msgs );
        BOOST_FAIL( "Exception is expected - parse_finish() "
                    "was not called after an error" );
    }
    catch ( std::exception & exc )
    {
        // Expected exception:
        // parse_finish(...) was not called after an
        // error occured or the user stopped the parser
    }
    return;
}


