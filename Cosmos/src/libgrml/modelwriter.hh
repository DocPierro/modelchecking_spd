#pragma once

#include <iostream>
#include <ostream>
#include <map>
#include <string>

#include "tree/tree.hh"
#include "modelhandler.hh"

class ModelWriter
{
private:
    std::ostream& out;
    std::string indent;
    std::string indent_size;

    void init_model(const XmlString formalismUrl);
    void write_attribute(const Attribute& attribute);
    void write_attribute(const Attribute& attribute, const Attribute::iterator node);
    void write_reference(const XmlString reference);
    void decrease_indent();
    void increase_indent();

public:
    ModelWriter(const XmlString formalismUrl, std::ostream& out);
    ~ModelWriter();

    void write_model_attribute(const Attribute& attribute);

    void write_node(const XmlString id,
            const XmlString nodeType,
            const AttributeMap& attributes = AttributeMap(),
            const XmlStringList& references = XmlStringList());

    void write_arc(const XmlString id,
            const XmlString arcType,
            const XmlString source,
            const XmlString target,
            const AttributeMap& attributes = AttributeMap(),
            const XmlStringList& references = XmlStringList());

};

