/*  $Id: xpath_run.cpp 312440 2011-07-14 15:23:15Z satskyse $
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
 * Authors:  Sergey Satskiy
 *
 * File Description:
 *   XPath expression running example
 *
 */


#include <ncbi_pch.hpp>

// xmlwrapp include
#include <misc/xmlwrapp/xmlwrapp.hpp>
using namespace xml;

// standard includes
#include <iostream>
#include <exception>
using namespace std;


void run_nested_xpath_query( node &  nd, xpath_expression *  expr )
{
    if ( !expr ) return;

    node_set        nset( nd.run_xpath_query( *expr ) );
    cout << "    Number of nodes in the nested query result set: "
         << nset.size() << "\n";
    size_t      index = 1;
    for ( node_set::iterator  k = nset.begin(); k != nset.end(); ++k, ++index )
    {
        cout << "    " << index << ": " << k->get_path() << "\n";
    }
    return;
}


int  main( int  argc, char *  argv[] )
{
    if ( argc != 3 && argc != 4 )
    {
        cerr << "Usage:\n"
             << argv[0] << " <file.xml>  <doc level xpath expr>  "
                           "[expr for all found nodes]\n";
        return 1;
    }

    xpath_expression *  nested_expr = NULL;
    int                 retVal = 0;
    try
    {
        document            doc( argv[ 1 ], NULL );
        node &              root = doc.get_root_node();
        xpath_expression    expr( argv[ 2 ], xpath_expression::type_compile );

        if ( argc == 4 )
            nested_expr = new xpath_expression( argv[ 3 ],
                                                xpath_expression::type_compile );

        cout << "root node is '" << root.get_name() << "'\n";
        cout << "top level xpath expression is '" << argv[ 2 ] << "'\n";
        cout << "nested xpath expression is ";
        if ( argc == 4 ) cout << "'" << argv[ 3 ] << "'\n";
        else             cout << "not provided\n";

        node_set        nset( root.run_xpath_query( expr ) );

        cout << "Number of nodes in the top level query result set: "
             << nset.size() << "\n";
        size_t      index = 1;
        for ( node_set::iterator  k = nset.begin(); k != nset.end(); ++k, ++index )
        {
            cout << index << ": " << k->get_path() << "\n";
            run_nested_xpath_query( *k, nested_expr );
        }
    }
    catch ( std::exception &  e )
    {
        cerr << argv[0] << ": " << e.what() << "\n";
        retVal = 1;
    }

    delete nested_expr;
    return retVal;
}

