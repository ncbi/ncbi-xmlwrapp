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
 * The following code demonstrates the usage of the xml::event_parser class.
 * This class must be derived from and the new subclass must implement the
 * pure virtual member functions from xml::event_parser. These member
 * functions are called when parsing events occur.
 */


#include <ncbi_pch.hpp>

// xmlwrapp include
#include <misc/xmlwrapp/xmlwrapp.hpp>

// standard includes
#include <stdlib.h>
#include <iostream>
#include <exception>

// libxml2 include for struct _xmlElementContent
#include <libxml/tree.h>


std::string get_element_content_occur_name( xmlElementContentOccur  occur )
{
    switch ( occur )
    {
        case XML_ELEMENT_CONTENT_ONCE:  return "once";
        case XML_ELEMENT_CONTENT_OPT:   return "opt";
        case XML_ELEMENT_CONTENT_MULT:  return "mult";
        case XML_ELEMENT_CONTENT_PLUS:  return "plus";
        default: ;
    }
    return "unknown element content occur";
}

std::string get_element_content_type_name( xmlElementContentType  type )
{
    switch ( type )
    {
        case XML_ELEMENT_CONTENT_PCDATA:    return "pcdata";
        case XML_ELEMENT_CONTENT_ELEMENT:   return "element";
        case XML_ELEMENT_CONTENT_SEQ:       return "seq";
        case XML_ELEMENT_CONTENT_OR:        return "or";
        default: ;
    }
    return "unknown element content type";
}

void print_spaces( int level )
{
    for ( int k(0); k < level * 4; ++k ) std::cout << " ";
}

// Helper function to print the low level libxml2 structure
void printXMLElementContent( int  level,
                             const struct _xmlElementContent *  content )
{
    if (!content) return;

    std::string     name( content->name ? reinterpret_cast<const char *>(content->name) : "" );
    std::string     prefix( content->prefix ? reinterpret_cast<const char *>(content->prefix) : "" );

    print_spaces( level );
    std::cout << "element content. Name: '" << name
              << "' type: '" << get_element_content_type_name( content->type )
              << "' occur: '" << get_element_content_occur_name( content->ocur )
              << "' prefix: '" << prefix
              << "' parent pointer: " << content->parent
              << " first child pointer: " << content->c1
              << " second child pointer: " << content->c2 << "\n";
    printXMLElementContent( level + 1, content->c1 );
    printXMLElementContent( level + 1, content->c2 );
}



/*
 * Here we create a class that will receive the parsing events.
 */
class myparser : public xml::event_parser {
public:
    myparser (xml::event_parser::sax_handlers_mask mask):
        xml::event_parser(mask)
    { std::cout << "myparser constructor\n"; }

    ~myparser (void)
    { std::cout << "myparser destructor\n"; }
private:
    bool start_document ()
    { std::cout << "begin document\n"; return true; }

    bool end_document ()
    { std::cout << "end document\n"; return true; }

    bool start_element (const std::string &name, const attrs_type& attrs)
    {
        std::cout << "begin tag '" << name << "'";
        for (attrs_type::const_iterator k(attrs.begin()); k != attrs.end(); ++k)
            std::cout << " " << k->first << "='" << k->second << "'";
        std::cout << "\n";
        return true;
    }

    bool end_element (const std::string &name)
    { std::cout << "end tag '" << name << "'\n"; return true;   }

    bool text (const std::string& txt)
    { std::cout << "text '" << txt << "'\n"; return true; }

    bool cdata (const std::string &contents)
    { std::cout << "cdata '" << contents << "'\n"; return true; }

    bool processing_instruction (const std::string &target, const std::string &data)
    { std::cout << "processing_instruction target '" << target << "' data '" << data << "\n"; return true; }

    bool comment (const std::string &contents)
    { std::cout << "comment '" << contents << "'\n"; return true; }

    bool warning (const std::string &message)
    { std::cout << "warning '" << message << "'\n"; return true; }

    bool notation_declaration (const std::string &name,
                               const std::string &public_id,
                               const std::string &system_id)
    {
        std::cout << "notation declaration. name '" << name
                  << "' public ID '" << public_id
                  << "' system ID '" << system_id << "'\n";
        return true;
    }

    std::string get_entity_type_name (entity_type type)
    {
        switch (type)
        {
            case type_internal_general_entity:          return "internal general entity";
            case type_external_general_parsed_entity:   return "external general parsed entity";
            case type_external_general_unparsed_entity: return "external general unparsed entity";
            case type_internal_parameter_entity:        return "internal parameter entity";
            case type_external_parameter_entity:        return "external parameter entity";
            case type_internal_predefined_entity:       return "internal predefined entity";
            default: ;
        }
        return "unknown entity";
    }

    std::string get_attribute_type_name (attribute_type type)
    {
        switch (type)
        {
            case type_attribute_cdata:          return "cdata";
            case type_attribute_id:             return "id";
            case type_attribute_idref:          return "idref";
            case type_attribute_idrefs:         return "idrefs";
            case type_attribute_entity:         return "entity";
            case type_attribute_entities:       return "entities";
            case type_attribute_nmtoken:        return "nmtoken";
            case type_attribute_nmtokens:       return "nmtokens";
            case type_attribute_enumeration:    return "enumeration";
            case type_attribute_notation:       return "notation";
            default: ;
        }
        return "unknown attribute type";
    }

    std::string get_attribute_default_type_name (attribute_default_type type)
    {
        switch (type)
        {
            case type_attribute_none:           return "none";
            case type_attribute_required:       return "required";
            case type_attribute_implied:        return "implied";
            case type_attribute_fixed:          return "fixed";
            default: ;
        }
        return "unknown attribute default type";
    }

    std::string get_element_content_type_name (element_content_type type)
    {
        switch (type)
        {
            case type_undefined:    return "undefined";
            case type_empty:        return "empty";
            case type_any:          return "any";
            case type_mixed:        return "mixed";
            case type_element:      return "element";
            default: ;
        }
        return "unknown element content type";
    }

    bool entity_declaration (const std::string &name,
                             entity_type        type,
                             const std::string &public_id,
                             const std::string &system_id,
                             const std::string &content)
    {
        std::cout << "entity declaration. name '" << name
                  << "' type '" << get_entity_type_name(type)
                  << "' public ID '" << public_id
                  << "' system ID '" << system_id
                  << "' content '" << content << "'\n";
        return true;
    }

    bool unparsed_entity_declaration (const std::string &name,
                                      const std::string &public_id,
                                      const std::string &system_id,
                                      const std::string &notation_name)
    {
        std::cout << "unparsed entity declaration. name '" << name
                  << "' public ID '" << public_id
                  << "' system ID '" << system_id
                  << "' notation name '" << notation_name << "'\n";
        return true;
    }

    bool external_subset_declaration (const std::string &name,
                                      const std::string &external_id,
                                      const std::string &system_id)
    {
        std::cout << "external subset declaration. root element name '" << name
                  << "' external ID '" << external_id
                  << "' system ID '" << system_id << "'\n";
        return true;
    }

    bool internal_subset_declaration (const std::string &name,
                                      const std::string &external_id,
                                      const std::string &system_id)
    {
        std::cout << "internal subset declaration. root element name '" << name
                  << "' external ID '" << external_id
                  << "' system ID '" << system_id << "'\n";
        return true;
    }

    bool attribute_declaration (const std::string &element_name,
                                const std::string &attribute_name,
                                attribute_type attr_type,
                                attribute_default_type default_type,
                                const std::string &default_value,
                                const values_type &default_values)
    {
        std::cout << "attribute declaration. element name '" << element_name
                  << "' attribute name '" << attribute_name
                  << "' attribute type '" << get_attribute_type_name(attr_type)
                  << "' attribute default type '" << get_attribute_default_type_name(default_type)
                  << "' default value '" << default_value << "'\n";
        for (values_type::const_iterator k(default_values.begin()); k != default_values.end(); ++k)
        {
            std::cout << "    values list item '" << *k << "'\n";
        }
        return true;
    }

    bool element_declaration (const std::string &name,
                              element_content_type type,
                              _xmlElementContent *content)
    {
        std::cout << "element declaration. name '" << name
                  << "' element content type '" << get_element_content_type_name(type) << "'\n";
        printXMLElementContent( 0, content );
        return true;
    }

    bool entity_reference (const std::string &name)
    {
        std::cout << "entity reference. name '" << name << "'\n";
        return true;
    }
};

/*
 * And here is good ol' main.
 */
int main (int argc, char *argv[]) {

    xml::event_parser::sax_handlers_mask    mask = xml::event_parser::default_set;
    if (argc == 3) {
        mask = atoi(argv[2]);
    }

    if ((argc != 2) && (argc != 3)) {
        std::cerr << "Usage: " << argv[0] << " <XML file> [mask]\n"
                     "If the mask is not given then default is used.\n"
                     "The mask is an integer value (see xmlwrapp doc).\n";
        return 1;
    }

    myparser parser(mask);
    xml::error_messages msgs;

    if (!parser.parse_file(argv[1], &msgs)) {
        std::cerr << argv[0] << ": error parsing XML file\n";
        std::cerr << msgs.print() << "\n";
        return 1;
    }

    return 0;
}
