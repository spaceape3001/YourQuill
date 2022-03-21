////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace rapidxml {
    // Forward declarations
    template<class Ch> class xml_node;
    template<class Ch> class xml_attribute;
    template<class Ch> class xml_document;
    template<class Ch> class xml_base;
}

namespace yq {

    using XmlBase       = rapidxml::xml_base<char>;
    using XmlNode       = rapidxml::xml_node<char>;
    using XmlAttribute  = rapidxml::xml_attribute<char>;
    using XmlDocument   = rapidxml::xml_document<char>;

}

