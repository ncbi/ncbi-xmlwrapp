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
 * This file demonstrates how to use the xml::document class to parse XML
 * data and create a tree of xml::node objects. It also shows how you can
 * walk the tree using xml::node iterators.
 */

#include <ncbi_pch.hpp>

// xmlwrapp include
#include <misc/xmlwrapp/xmlwrapp.hpp>

// standard includes
#include <iostream>
#include <exception>
#include <string.h>


void printDtdProperties( const std::string &  header,
                         const xml::dtd&      subset )
{
    std::cout << header << std::endl;

    const char *        value;

    value = subset.get_public_id();
    if ( value )
        std::cout << "    Public id: " << value << std::endl;
    else
        std::cout << "    No public id" << std::endl;

    value = subset.get_system_id();
    if ( value )
        std::cout << "    System id: " << value << std::endl;
    else
        std::cout << "    No system id" << std::endl;

    value = subset.get_name();
    if ( value )
        std::cout << "    Name: " << value << std::endl;
    else
        std::cout << "    No name" << std::endl;
}

void printAttribute( const xml::attributes::attr &  att )
{
    std::cout << "    " << att.get_name() << " = '" << att.get_value() << "', ";

    xml::ns     nspace = att.get_namespace();
    if ( nspace.is_void() )
        std::cout << "namespace is void" << std::endl;
    else
        std::cout << "namespace: prefix: " << nspace.get_prefix()
                  << " uri: " << nspace.get_uri() << std::endl;
}

xml::ns *  buildNamespace( char *  input )
{
    if ( strcmp( input, "void" ) == 0 )
        return new xml::ns( xml::ns::type_void );

    char *  separator( strchr( input, ':' ) );
    if ( separator == NULL )
        return new xml::ns( NULL, input );  // default namespace

    *separator = '\0';
    return new xml::ns( input, ++separator );
}


int main ( int  argc, char *  argv[] )
{
    if ( argc != 3 && argc != 4 )
    {
        std::cerr << "Usage:" << std::endl
                  << argv[0]
                  << " <XML file name> <root node attribute name to find> [namespace]"
                  << std::endl;
        return 1;
    }

    try
    {
        xml::init::load_external_subsets( true );
        xml::init::validate_xml( true );

        xml::document       doc( argv[1], NULL );
        xml::node &         root = doc.get_root_node();
        xml::attributes &   attrs = root.get_attributes();

        std::cout << "Encoding: " << doc.get_encoding() << std::endl;
        std::cout << "Version: " << doc.get_version() << std::endl;
        std::cout << "Standalone: " << doc.get_is_standalone() << std::endl;


        if ( doc.has_internal_subset() )
            printDtdProperties( "Internal subset:", doc.get_internal_subset() );
        else
            std::cout << "The document does not have internal subset" << std::endl;

        if ( doc.has_external_subset() )
            printDtdProperties( "External subset:", doc.get_external_subset() );
        else
            std::cout << "The document does not have external subset" << std::endl;

        std::cout << std::endl;
        std::cout << "root node is '" << root.get_name() << "'" << std::endl;
        if ( attrs.empty() )
            std::cout << "    The root node does not have attributes" << std::endl;
        else
        {
            for ( xml::attributes::const_iterator  k = attrs.begin();
                  k != attrs.end(); ++k )
            {
                printAttribute( *k );
            }
        }

        std::cout << std::endl;
        std::cout << "Searching for the '" << argv[2]
                  << "' attribute in the root node..." << std::endl;
        xml::ns *   nspace( NULL );
        if ( argc == 4 )
        {
            nspace = buildNamespace( argv[ 3 ] );
            std::cout << "Namespace provided, prefix: '" << nspace->get_prefix()
                      << "' uri: '" << nspace->get_uri() << "'" << std::endl;
        }
        else
            std::cout << "No namespace provided" << std::endl;

        xml::attributes::const_iterator     f = attrs.find( argv[2], nspace );
        delete nspace;
        if ( f == attrs.end() )
            std::cout << "Attribute has not been found" << std::endl;
        else
        {
            std::cout << "Attribute has been found. " << f->get_name()
                      << " = '" << f->get_value() << "'" << std::endl;
            printAttribute( *f );
        }

    }
    catch (std::exception &e)
    {
        std::cerr << argv[0] << ": " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

