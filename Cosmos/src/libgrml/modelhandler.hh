#pragma once

/**
 * \mainpage LibGrML
 * Library to help developper to read and/or write model using GrML syntax.
 * \author   Clément Démoulins
 */

#include <iostream>
#include <map>
#include <string>
//#ifdef __APPLE__
#include <memory>
//#else
//#include <tr1/memory>
//#endif

#include "tree/tree.hh"
#include <exception>


/** An exception throw when the parsing of an XML document fail. */
class GrmlInputException: public std::exception {
private:
    std::string msg;
public:
    GrmlInputException():msg(){};
    GrmlInputException(const std::string& msg):msg(msg){};
    
    virtual const char* what() const throw()
    {
        std::string error("The input GrML file could not be imported : ");
        error.append(msg);
        return std::move(error).c_str();
    }
};


// Types definitions

/** Type representing a string from an XML document. */
typedef std::string XmlString;

/** Type representing a GrML attribute element.
 * Ex:
 * <attribute name="declaration">
 *   <attribute name="classDeclaration">
 *     <attribute name="name">C1</attribute>
 *     <attribute name="classType">
 *       …
 *     </attribute>
 *   </attribute>
 * </attribute>
 *
 * saved in memory in this way :
 * + "declaration"
 *       + "classDeclaration"
 *           + "name"
 *               + "C1"
 *           + "classType"
 *               …
 */
typedef tree<XmlString> Attribute;
typedef tree<std::string>::sibling_iterator treeSI;

/** Type representing a map of Attribute. */
typedef std::map<XmlString, Attribute> AttributeMap;

/** Type representing a list of reference. */
typedef std::vector<XmlString> XmlStringList;

class ModelHandler;
/** Pointer to a model handler using std::shared_ptr */
typedef std::shared_ptr<ModelHandler> ModelHandlerPtr;


/**
 * Model handler interface. Implementation of this interface receive event
 * when parsing a GrML model.
 */
class ModelHandler
{
protected:
    /** Constructor */
//    ModelHandler();

    /** Descructor */
    virtual ~ModelHandler() {}

public:

    /**
     * Event raised when reading a model. This event raised before all other.
     * @param formalismUrl formalism of the model
     */
    virtual void on_read_model(const XmlString& formalismUrl) = 0;

    /**
     * Event raise when the parser read an attribute attach to the model.
     * @param attribute attribute
     */
    virtual void on_read_model_attribute(const Attribute& attribute) = 0;

    /**
     * Event raise when the parser read a node element
     * @param id identifier of the node
     * @param nodeType type of the node
     * @param attributes attributes attach to the node
     * @param references list of references
     */
    virtual void on_read_node(const XmlString& id,
            const XmlString& nodeType,
            const AttributeMap& attributes,
            const XmlStringList& references) = 0;

    /**
     * Event raise when the parser read an arc element
     * @param id identifier of the arc
     * @param arcType type of the arc
     * @param source node identifier of the source of the arc
     * @param target node identifier of the target of the arc
     * @param attributes attributes attach to the node
     * @param references list of references
     */
    virtual void on_read_arc(const XmlString& id,
            const XmlString& arcType,
            const XmlString& source,
            const XmlString& target,
            const AttributeMap& attributes,
            const XmlStringList& references) = 0;
};

