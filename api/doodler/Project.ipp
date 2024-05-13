////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/Project.hpp>
#include <0/io/XmlFile.hpp>
#include <0/io/XmlUtils.hpp>
#include <0/basic/errors.hpp>
#include <doodler/IOStrings.hpp>

namespace yq::errors {
    using no_doodle_element = error_db::entry<"No doodle element found, therefore not a doodle file">;
}

namespace yq::doodler {
    //  ///////////////////////////////////////////////////////////////////////////////////

    Expect<Census>     Project::census(const std::filesystem::path&fp, DFormat fmt)
    {
        Census              ret;
        std::error_code     ec  = errors::bad_argument();
        if(fmt == DFormat::AUTO)
            fmt = DFormat::XML;
            
        switch(fmt){
        case DFormat::XML:
            ec = census_xml(ret, fp);
            break;
        default:
            break;
        }
        
        if(ec != std::error_code()){
            return unexpected(ec);
        } else {
            return ret;
        }
    }

    std::error_code      Project::census_xml(Census& ret, const std::filesystem::path& fp)
    {
        XmlDocument     doc;
        std::error_code     ec  = read_file(doc, fp);
        if(ec != std::error_code())
            return ec;
        const XmlNode*  top = doc.first_node(szYQDoodle);
        if(!top)
            return errors::no_doodle_element();
        
        return errors::todo();
    }

    std::unique_ptr<Project>    Project::load(const std::filesystem::path&fp, DFormat fmt)
    {
        std::unique_ptr<Project>    prj = std::make_unique<Project>();
        if(prj->load_impl(fp, fmt) != std::error_code())
            return {};
        return prj;
    }
    
    
    

    //  ///////////////////////////////////////////////////////////////////////////////////

    Project::Project() : m_revision{0ULL}
    {
        m_objects.push_back(nullptr);
    }
    
    
    Project::~Project()
    {
    }

    D    Project::_insert(DObject*obj)
    {
        D    n   = {(id_t) m_objects.size()};
        m_objects.push_back(obj);
        return n;
    }

    void                Project::bump()
    {
        ++m_revision;
    }

    std::error_code     Project::load_impl(const std::filesystem::path&fp, DFormat fmt)
    {
        if(fmt == DFormat::AUTO)
            fmt = DFormat::XML;
        
        std::error_code ec  = errors::bad_argument();
        
        switch(fmt){
        case DFormat::XML:
            ec  = load_xml(fp);
            break;
        default:
            break;
        }
        
        if(ec == std::error_code()){
            m_file      = fp;
            m_format    = fmt;
        }
        
        return ec;
    }

    std::error_code     Project::load_xml(const std::filesystem::path&fp)
    {
        XmlDocument     doc;
        
        std::error_code ec  = read_file(doc, fp);
        if(ec != std::error_code())
            return ec;
        
        const XmlNode*  top = doc.first_node(szYQDoodle);
        if(!top)
            return errors::no_doodle_element();
        
        
        return errors::todo();
    }

    DObject*            Project::object(D i)
    {
        if((size_t) i.id >= m_objects.size())
            return nullptr;
        return m_objects[i.id];
    }
    
    const DObject*      Project::object(D  i) const
    {
        if((size_t) i.id >= m_objects.size())
            return nullptr;
        return m_objects[i.id];
    }

    uint64_t            Project::revision() const
    {
        return m_revision;
    }

    std::error_code     Project::save() const
    {
        if(m_file.empty())
            return errors::no_file_name();
        return save_to(m_file);
    }

    std::error_code     Project::save_as(const std::filesystem::path&fp, DFormat fmt)
    {
        std::error_code     ec  = save_to(fp, fmt);
        if(ec == std::error_code())
            m_file      = fp;
        return ec;
    }
    
    std::error_code     Project::save_to(const std::filesystem::path&fp, DFormat fmt) const
    {
        if(fmt == DFormat::AUTO){
            if(m_format != DFormat::AUTO){
                fmt = m_format;
            } else {
                fmt = DFormat::XML;
            }
        }
    
        switch(fmt){
        case DFormat::XML:
            return save_xml(fp);
        default:
            return errors::bad_argument();
        }
    }
    
    std::error_code     Project::save_xml(const std::filesystem::path&fp) const
    {
        XmlDocument doc;
        xml_start(doc);
        XmlNode*    top = create_child(doc, szYQDoodle);
        
        //  assuming it's all right
        
        return save_file(doc, fp);
    }
    
    void                Project::set_title(const std::string&v)
    {
        m_title = v;
        bump();
    }
    

}
