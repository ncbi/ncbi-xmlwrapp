/*  $Id: dtd_validation.cpp 414573 2013-09-25 14:34:37Z satskyse $
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

// xmlwrapp include
#include <misc/xmlwrapp/xmlwrapp.hpp>
#include <misc/xmlwrapp/errors.hpp>

// standard includes
#include <iostream>
#include <exception>


int main( int argc, char *argv[] )
{
    if ( argc != 2 )
    {
        std::cerr << argv[0]
                  << ": you must give one and only one XML file name\n";
        return 1;
    }

    try
    {
        xml::document           doc( argv[1], NULL );
        xml::error_messages     errors;

        if ( doc.validate( &errors ) )
        {
            std::cout << "Document is valid\n";
        }
        else
        {
            std::cerr << "Document is invalid\n"
                         "Messages: " << errors.print() << std::endl;
        }
    }
    catch ( std::exception & e )
    {
        std::cerr << argv[0] << " exception:\n" << e.what() << "\n";
        return 1;
    }

    return 0;
}

