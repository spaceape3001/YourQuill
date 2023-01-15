////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/file/Document.hpp>
#include <kernel/file/Folder.hpp>
#include <kernel/image/Image.hpp>
#include <kernel/leaf/Leaf.hpp>

namespace yq {
    
    struct Root;
    
    struct XEntry {
        enum Type {
            None    = 0,
            IsFolder,
            IsDocument,
            IsLeaf
        };
        
            // we'd do a union... but argh!
        union {
            uint64_t            id = 0ULL;
            Leaf                leaf;
            Document            doc;
            Folder              folder;
        };
        Type                        type    = None;
        
        Image                       icon;
        std::string                 skey, name, label, suffix, title;
        std::vector<const Root*>    roots;
        
        XEntry(){}
        XEntry(Leaf);
        XEntry(Document);
        XEntry(Folder);
        
        XEntry(const XEntry&);
        XEntry(XEntry&&);
        XEntry& operator=(const XEntry&);
        XEntry& operator=(XEntry&&);
        
        ~XEntry();
        
        static bool isLess_label(const XEntry& a, const XEntry& b);
        static bool isLess_skey(const XEntry& a, const XEntry& b);
        static bool isLess_name(const XEntry& a, const XEntry& b);
        static bool isLess_title(const XEntry& a, const XEntry& b);
        
        static std::vector<XEntry>   query_documents(Folder x, unsigned int opts=0);
        static size_t                query_documents(std::vector<XEntry>&, Folder x, unsigned int opts=0);
        static std::vector<XEntry>   query_folders(Folder x, unsigned int opts=0);
        static size_t                query_folders(std::vector<XEntry>&, Folder x, unsigned int opts=0);
        static std::vector<XEntry>   query_leafs(Folder x, unsigned int opts=0);
        static size_t                query_leafs(std::vector<XEntry>&, Folder x, unsigned int opts=0);
    };

}

