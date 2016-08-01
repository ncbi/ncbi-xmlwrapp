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


void printDtdProperties( const std::string &  header,
                         const xml::dtd&      subset )
{
    std::cout << header << "\n";

    const char *        value;

    value = subset.get_public_id();
    if ( value )
        std::cout << "    Public id: " << value << "\n";
    else
        std::cout << "    No public id\n";

    value = subset.get_system_id();
    if ( value )
        std::cout << "    System id: " << value << "\n";
    else
        std::cout << "    No system id\n";

    value = subset.get_name();
    if ( value )
        std::cout << "    Name: " << value << "\n";
    else
        std::cout << "    No name\n";
}


int main ( int  argc, char *  argv[] )
{
    if ( argc != 2 )
    {
        std::cerr << argv[0]
                  << ": you must give one and only one XML file name\n";
        return 1;
    }

    try
    {
        xml::document       doc( argv[1], NULL );

        std::cout << "Encoding: " << doc.get_encoding() << "\n";
        std::cout << "Version: " << doc.get_version() << "\n";
        std::cout << "Standalone: " << doc.get_is_standalone() << "\n";


        if ( doc.has_internal_subset() )
            printDtdProperties( "Internal subset:", doc.get_internal_subset() );
        else
            std::cout << "The document does not have internal subset\n";

        if ( doc.has_external_subset() )
            printDtdProperties( "External subset:", doc.get_external_subset() );
        else
            std::cout << "The document does not have external subset\n";


        xml::node &root = doc.get_root_node();
        std::cout << "root node is '" << root.get_name() << "'\n";

        xml::node::const_iterator child(root.begin()), child_end(root.end());
        for ( ; child != child_end; ++child )
        {
            if (child->is_text()) continue;
            std::cout << "child node '" << child->get_name() << "'\n";
        }

    }
    catch (std::exception &e)
    {
        std::cerr << argv[0] << ": " << e.what() << "\n";
        return 1;
    }

    return 0;
}

