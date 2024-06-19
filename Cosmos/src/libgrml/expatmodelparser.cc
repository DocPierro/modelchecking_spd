#include "expatmodelparser.hh"
#include "tree/tree_util.hh"
#include <string>



IOError::IOError(std::string message) throw() : std::runtime_error(message) { }

ExpatError::ExpatError(XML_Error error_code) throw() : std::runtime_error(""), error_code(error_code) { }
const char* ExpatError::what() const throw()
{
    return XML_ErrorString(error_code);
}

ExpatModelParser::ExpatModelParser(ModelHandlerPtr h) : handler(h)
{
    parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser, this);

    XML_SetStartElementHandler(parser, ExpatModelParser::on_start_element);
    XML_SetEndElementHandler(parser, ExpatModelParser::on_end_element);
    XML_SetCharacterDataHandler(parser, ExpatModelParser::on_characters);
}

ExpatModelParser::~ExpatModelParser()
{
    XML_ParserFree(parser);
}

void ExpatModelParser::parse_file(const std::string& filename)
{
    std::ifstream ifs(filename.c_str());
    if (ifs.fail()) {
        throw IOError("Cannot open the file");
    }
    parse_stream(ifs);
}

void ExpatModelParser::parse_stream(std::istream& in) 
{
    XML_Status status;
    while(in.good())
    {
        char* buffer = (char*) XML_GetBuffer(parser, BUFFER_SIZE);
        in.read(buffer, BUFFER_SIZE);
        status = XML_ParseBuffer(parser, in.gcount(), in.eof());

        if (status == XML_STATUS_ERROR)
        {
            XML_Error error_code = XML_GetErrorCode(parser);
            throw ExpatError(error_code);
        }
    }
}

void ExpatModelParser::on_start_element(void *userData, const XML_Char *cname, const XML_Char **catts)
{
    ExpatModelParser* self = (ExpatModelParser*) userData;

    // Convert c variables to c++ objects
    XmlString name = cname;
        size_t pos = name.find_last_of(":");
        if (pos != std::string::npos)
                name = name.substr(pos+1, name.size());
    std::map<XmlString, XmlString> atts;
    for(int i = 0; catts[i] != NULL; i +=2)
    {
        XmlString attr_name = catts[i];
        XmlString attr_value = catts[i + 1];
        atts[attr_name] = attr_value;
    }

    if (name == "model") { // <model>
        self->handler->on_read_model(atts["formalismUrl"]);
        self->state.push(MODEL);

    } else if (name == "node") { // <node id= type= >
        self->id = atts["id"];
        self->type = atts["nodeType"];
        self->attributes.clear();
        self->references.clear();
        self->state.push(NODE);

    } else if (name == "arc") { // <arc id= type= source= target= >
        self->id = atts["id"];
        self->type = atts["arcType"];
        self->source = atts["source"];
        self->target = atts["target"];
        self->attributes.clear();
        self->references.clear();
        self->state.push(ARC);

    } else if (name == "attribute") { // <attribute name= >
        std::string attributeName = atts["name"];

        // root GrML attribute
        if (self->state.top() != ATTRIBUTE) {
            self->attribute.clear();
            self->attributeIterator = self->attribute.insert(self->attribute.begin(), attributeName);
        }

        // child GrML attribute
        else {
            self->attributeIterator = self->attribute.append_child(self->attributeIterator, attributeName);
        }

        self->xmlData.clear();
        self->state.push(ATTRIBUTE);
    } else if (name == "ref") {
        self->references.push_back(atts["href"]);
    }
}

void ExpatModelParser::on_end_element(void *userData, const XML_Char *cname)
{
    ExpatModelParser* self = (ExpatModelParser*) userData;
    XmlString name = cname;
    try{
        size_t pos = name.find_last_of(":");
        if (pos != std::string::npos)
        name = name.substr(pos+1, name.size());
        if (name == "model") { // </model>
            self->state.pop();

        } else if (name == "node") { // </node>
            self->state.pop();
            self->handler->on_read_node(self->id, self->type, self->attributes, self->references);

        } else if (name == "arc") { // </arc>
            self->state.pop();
            self->handler->on_read_arc(self->id, self->type, self->source, self->target, self->attributes, self->references);

        } else if (name == "attribute") { // </attribute>
            self->state.pop();

            // leaf node with data
            if (!self->xmlData.empty() &&  self->attribute.number_of_children(self->attributeIterator) == 0) {
                self->trim(self->xmlData);
                self->attribute.append_child(self->attributeIterator, self->xmlData);
                self->xmlData.clear();
            }

            // root GrML attribute
            if (self->state.top() == MODEL) {
                self->handler->on_read_model_attribute(self->attribute);
            } else if (self->state.top() == NODE || self->state.top() == ARC) {
                self->attributes[*self->attribute.begin()] = self->attribute;
            }

            // child GrML attribute
            else {
                self->attributeIterator = self->attribute.parent(self->attributeIterator);
            }
        }
    } catch (std::exception &e){
        std::cerr << "fail to parse '"+name+"' :"+e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void ExpatModelParser::on_characters(void *userData, const XML_Char *s, int len)
{
    ExpatModelParser* self = (ExpatModelParser*) userData;
    self->xmlData += std::string(s, len);
}

void ExpatModelParser::trim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c){return !std::isspace(c);}));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int c){return !std::isspace(c);}).base(), s.end());
}
