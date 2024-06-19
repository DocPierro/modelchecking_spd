
#include "modelwriter.hh"



void ModelWriter::init_model(const XmlString formalismUrl)
{
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl << std::endl;
    out << "<grml:model formalismUrl=\"" << formalismUrl << "\" xmlns:grml=\"http://cosyverif.org/ns/model\">";
    indent = indent_size = "  ";
}

ModelWriter::ModelWriter(const XmlString formalismUrl, std::ostream& out) : out(out)
{
    init_model(formalismUrl);
}

ModelWriter::~ModelWriter()
{
    out << std::endl << "</grml:model>" << std::endl << std::flush;
}

void ModelWriter::decrease_indent()
{
    indent.resize(indent.size() - indent_size.size());
}

void ModelWriter::increase_indent()
{
    indent.append(indent_size);
}

void ModelWriter::write_attribute(const Attribute& attribute)
{
    write_attribute(attribute, attribute.begin());
}

void ModelWriter::write_attribute(const Attribute& attribute, const Attribute::iterator node)
{
    int number_of_children = attribute.number_of_children(node);
    if (number_of_children == 0) {
        out << (*node);
    } else {
        out << std::endl;
        out << indent << "<grml:attribute name=\"" << (*node) << "\">";
        increase_indent();
        for (int index = 0; index < number_of_children; ++index)
        {
            write_attribute(attribute, attribute.child(node, index));
        }
        decrease_indent();
        if (number_of_children != 1 || attribute.number_of_children(attribute.child(node, 0)) != 0)
        {
            out << std::endl << indent;
        }
        out << "</grml:attribute>";
    }
}

void ModelWriter::write_reference(const XmlString reference)
{
    out << std::endl << indent;
    out << "<grml:ref href=\"" << reference << "\" />";
}

void ModelWriter::write_model_attribute(const Attribute& attribute)
{
    write_attribute(attribute);
}

void ModelWriter::write_node(const XmlString id,
        const XmlString nodeType,
        const AttributeMap& attributes,
        const XmlStringList& references)
{
    out << std::endl << indent << "<grml:node id=\"" << id << "\" nodeType=\"" << nodeType << "\"";
    if (attributes.size() + references.size() == 0)
        out << " />";
    else
    {
        out << ">";
        increase_indent();
        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
        {
            write_attribute(it->second);
        }
        for (XmlStringList::const_iterator it = references.begin(); it != references.end(); ++it)
        {
            write_reference(*it);
        }
        decrease_indent();
        out << std::endl << indent << "</grml:node>";
    }
}

void ModelWriter::write_arc(const XmlString id,
        const XmlString arcType,
        const XmlString source,
        const XmlString target,
        const AttributeMap& attributes,
        const XmlStringList& references)
{
    out << std::endl << indent << "<grml:arc id=\"" << id << "\" arcType=\"" << arcType << "\"";
    out << " source=\"" << source << "\" target=\"" << target << "\"";
    if (attributes.size() + references.size() == 0)
        out << " />";
    else
    {
        out << ">";
        increase_indent();
        for (AttributeMap::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
        {
            write_attribute(it->second);
        }
        for (XmlStringList::const_iterator it = references.begin(); it != references.end(); ++it)
        {
            write_reference(*it);
        }
        decrease_indent();
        out << std::endl << indent << "</grml:arc>";
    }
}

