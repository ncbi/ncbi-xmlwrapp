/*
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
 * This file demonstrates how to use xml::schema class to validate an
 * XML document.
 */

#include <ncbi_pch.hpp>

// xmlwrapp include
#include <misc/xmlwrapp/xmlwrapp.hpp>

// standard includes
#include <iostream>
#include <exception>

int main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Incorrect number of arguments\n"
                     "Usage: xml_schema  <XML document>  <schema>\n";
        return 1;
    }

    try {
        xml::document       doc( argv[1], NULL );
        xml::schema         schema( argv[2] );
        xml::error_messages errors;

        if ( schema.validate(doc, &errors) ) {
            std::cout << "Validation succeded\n";
        } else {
            std::cerr << "Validation failed:\n" << errors.print() << "\n";
        }
    }
    catch (std::exception &e) {
        std::cerr << "Exception is caught\n" << e.what() << "\n";
        return 1;
    }

    return 0;
}

