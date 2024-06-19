#pragma once

#include <iostream>
#include <istream>
#include <fstream>
#include <map>
#include <stack>
#include <string>
#include <algorithm>
#include <functional>
#include <locale>

#include <expat.h>

#include "tree/tree.hh"
#include "modelhandler.hh"

#define BUFFER_SIZE 10240

enum parser_state
{
    MODEL,
    NODE,
    ARC,
    ATTRIBUTE
};

class IOError: public std::runtime_error
{
public:
    IOError(std::string message) throw();
};


class ExpatError: public std::runtime_error
{
private:
    XML_Error error_code;

public:
    ExpatError(XML_Error error_code) throw();
    const char* what() const throw();
};


/**
 * Parser of model using expat library.
 * This parser can read a GrML file or a standard input stream.
 */
class ExpatModelParser
{
private:
    ModelHandlerPtr handler;
    XML_Parser parser;
    XmlString xmlData;

    // Stack of state of the parser
    std::stack<parser_state> state;

    // Temporary stored parsed data
    XmlString id, type, source, target;
    Attribute attribute;
    Attribute::iterator attributeIterator;
    AttributeMap attributes;
    XmlStringList references;

    // Expat functions
    static void on_start_element(void *userData, const XML_Char *name, const XML_Char **atts);
    static void on_end_element(void *userData, const XML_Char *name);
    static void on_characters(void *userData, const XML_Char *s, int len);

public:
    /**
     * Constructor
     * @param h pointer to a model handler.
     */
    ExpatModelParser(ModelHandlerPtr h);
    ~ExpatModelParser();

    /**
     * Parse a file.
     * @param filename path to the file
     */
    void parse_file(const std::string& filename);

    /**
     * Parse from an input stream
     * @param in input stream
     */
    void parse_stream(std::istream& in);

    /**
     * Trim a string
     * @param s the string
     */
    static void trim(std::string& s);

};
