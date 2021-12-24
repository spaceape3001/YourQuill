////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

XmlFile::XmlFile()
{
}

XmlFile::~XmlFile()
{
}

bool    XmlFile::read(ByteArray&&buffer, const std::string& fname )
{
    ByteArray   chars   = std::move(buffer);
    chars << '\0';
    XmlDocument     doc;
    try {
        doc.parse<0>(chars.data());
    } catch(const rapidxml::parse_error& pe){
        size_t  pt  = pe.where<char>() - buffer.data();
        yError() << "Xml parse error: " << pe.what() << " (at byte " << pt << ") : " << fname;
        return false;
    }
    
    return read(doc,fname);
}

bool    XmlFile::write(Stream & str) const
{
    Vector<char>    s;
    XmlDocument     doc;
    XmlNode*        x   = doc.allocate_node(rapidxml::node_pi, "xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.append_node(x);
    write(doc);
    rapidxml::print(std::back_inserter(s), doc, 0);
    str.write(s.data(), s.size());
    return true;
}

