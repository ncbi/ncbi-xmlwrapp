/*  $Id: schema.hpp 543412 2017-08-09 18:22:55Z satskyse $
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
 * Credits: Denis Vakatov, NCBI (help with the original design)
 *
 */


/** @file
 * XML schema API for XmlWrapp.
**/

#ifndef _xmlwrapp_schema_hpp_
#define _xmlwrapp_schema_hpp_

// for NCBI_DEPRECATED
#include <ncbiconf.h>

// standard includes
#include <string>

// xmlwrapp includes
#include <misc/xmlwrapp/errors.hpp>

namespace xml {

// forward declarations
class document;
namespace impl {
struct schema_impl;
}


/**
 * The xml::schema class represents an XML schema.
**/
class schema
{
public:
    typedef std::size_t size_type;

    /**
     * Create a new xml::schema object by parsing the given XML schema file.
     *
     * @param filename The XML schema file name.
     * @param messages A pointer to the object where all the warnings are
     *                 collected. If NULL then no messages will be collected.
     * @param how How to treat warnings (default: warnings are not treated as
     *            errors). If warnings are treated as errors then an exception
     *            is thrown in case of both errors and/or warnings. If warnings
     *            are not treated as errors then an exception will be thrown
     *            only when there are errors.
     * @exception Throws xml::parser_exception in case of schema parsing errors
     *            and std::exception in case of other problems.
     * @author Sergey Satskiy, NCBI
    **/
    schema (const char* filename, error_messages* messages = NULL,
            warnings_as_errors_type how = type_warnings_not_errors);

    /**
     * Create a new xml::schema object by parsing the given XML schema from a
     * memory buffer.
     *
     * @param data The XML schema memory buffer.
     * @param size Size of the memory buffer.
     * @param messages A pointer to the object where all the warnings are
     *                 collected. If NULL then no messages will be collected.
     * @param how How to treat warnings (default: warnings are not treated as
     *            errors). If warnings are treated as errors then an exception
     *            is thrown in case of both errors and/or warnings. If warnings
     *            are not treated as errors then an exception will be thrown
     *            only when there are errors.
     * @exception Throws xml::parser_exception in case of schema parsing errors
     *            and std::exception in case of other problems.
     * @author Sergey Satskiy, NCBI
    **/
    schema (const char* data, size_type size,
            error_messages* messages,
            warnings_as_errors_type how = type_warnings_not_errors);

    /**
     * Validate the given XML document.
     *
     * @param doc XML document.
     * @param messages A pointer to the object where all the warnings are
     *                 collected. If NULL then no messages will be collected.
     * @param how How to treat warnings (default: warnings are treated as
     *            errors). If warnings are treated as errors false is
     *            returned in case of both errors and/or warnings. If warnings
     *            are not treated as errors then false is returned
     *            only when there are errors.
     * @return true if the document is valid, false otherwise.
     * @exception Throws std::exception in case of problems.
     * @author Sergey Satskiy, NCBI
    **/
    bool validate (const document& doc, error_messages* messages,
                   warnings_as_errors_type how = type_warnings_are_errors) const;

    /**
     * Destroy the object.
     *
     * @author Sergey Satskiy, NCBI
    **/
    virtual ~schema();

    /**
     * Moving constructor.
     * @param other The other schema.
    **/
    schema (schema &&other);

    /**
     * Moving assignment.
     * @param other The other schema.
    **/
    schema& operator= (schema &&other);

private:
    impl::schema_impl *pimpl_;
    void construct (const char* file_or_data, size_type size,
                    error_messages* messages,
                    warnings_as_errors_type how);

    // prohibited
    schema (const schema&);
    schema& operator= (const schema&);
};

} // xml namespace

#endif

