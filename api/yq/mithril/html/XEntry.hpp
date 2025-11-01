////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/enum/SizeDesc.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/folder/Folder.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/Leaf.hpp>

namespace yq::mithril {
    
    struct RootDir;
    class WebHtml;
    class WebAutoClose;
    
    struct XEntry {
        enum Type {
            None    = 0,
            IsFolder,
            IsDocument,
            IsLeaf
        };
        
        using Link  = std::variant<std::monostate, std::string>;
        
        struct Details {
            SizeDesc            icon_size  = SizeDesc::Small;
            
            struct {
                std::string     folder, document, leaf;
            } link;
            
            struct {
                bool    folders     = false;
                bool    documents   = false;
                bool    leafs       = false;
            } query;
            
            struct {
                    //! Search for hidden things
                bool    hidden      = false;
                    //! Want root_dir distinctions
                bool    root_dirs       = false;
                    //! Sort the entries by skey's, otherwise kept in folder/document/leaf order
                bool    merge       = false;
            } option;
            
            
            std::string_view    get_link(Type) const;
            bool                get_query(Type) const;
            
            Details(){}
        };
        
            // we'd do a union... but argh!
        union {
            uint64_t            id = 0ULL;
            Leaf                leaf;
            Document            document;
            Folder              folder;
        };
        Type                        type    = None;
        
        Image                       icon;
        std::string                 key, skey, name, label, suffix, title;
        std::vector<const RootDir*>    root_dirs;
        
        XEntry(){}
        XEntry(Leaf);
        XEntry(Document);
        XEntry(Folder);
        
        XEntry(const XEntry&);
        XEntry(XEntry&&);
        XEntry& operator=(const XEntry&);
        XEntry& operator=(XEntry&&);
        
        ~XEntry();
        
        //std::string         cdb_key() const;
        std::string_view    link(const Details&) const;
        
        //! The best thumbnail image (icon is first)
        void            write_thumbnail(class WebHtml&, const Details& dt = Details()) const;
        WebAutoClose    write_link(class WebHtml&, const Details& dt = Details()) const;
        void            write_text(class WebHtml&, const Details& dt = Details()) const;
        
        
        static bool isLess_label(const XEntry& a, const XEntry& b);
        static bool isLess_skey(const XEntry& a, const XEntry& b);
        static bool isLess_name(const XEntry& a, const XEntry& b);
        static bool isLess_title(const XEntry& a, const XEntry& b);
        
        static std::vector<XEntry>  query(Folder x, const Details& dt=Details());
        
        static size_t               query_documents(std::vector<XEntry>&, Folder x, const Details& dt=Details());
        static size_t               query_folders(std::vector<XEntry>&, Folder x, const Details& dt=Details());
        static size_t               query_leafs(std::vector<XEntry>&, Folder x, const Details& dt=Details());
        
        struct Tokens;
    };

}

