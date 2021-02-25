////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DataRole.hpp"
#include "Sorted.hpp"
#include "Image.hpp"
#include "CacheFwd.hpp"
#include <QString>

/*! \brief Directory in the cache

    This structure represents an directory on the drive, in cache.
*/
struct Directory {
    struct Info;
    uint64_t  id = 0ULL;
    constexpr auto    operator<=>(const Directory&rhs) const = default;
    constexpr operator bool() const { return id != 0ULL; }
};


/*! \brief Document in the cache

    This represents a document in the cache, which is case insensitive and
    fused across all matching fragments in the workspace
*/
struct Document {
    struct Info;
    uint64_t  id = 0ULL;
    constexpr auto    operator<=>(const Document&rhs) const = default;
    constexpr operator bool() const { return id != 0ULL; }
};

/*! \brief Folder of the cache/workspace

    Folder represents a fusion o fthe same named directory across the roots of the worksapce (case insensitive).
   
*/
struct Folder {
    struct Info;
    uint64_t  id = 0ULL;
    constexpr auto    operator<=>(const Folder&rhs) const = default;
    static constexpr const uint64_t TOP     = 1ULL;
    static constexpr const uint64_t CONFIG  = 2ULL;
    static constexpr const uint64_t CLASSES = 3ULL;
    static constexpr const uint64_t TAGS    = 4ULL;
    constexpr operator bool() const { return id != 0ULL; }
};


/*! \brief Represents single fragment
*/
struct Fragment {
    struct Info;
    uint64_t  id = 0ULL;
    constexpr auto    operator<=>(const Fragment& rhs) const = default;
    constexpr operator bool() const { return id != 0ULL; }
};


struct Directory::Info {
    Folder      folder;
    bool        hidden;
    QString     name;
    Directory   parent;
    QString     path;
    bool        removed;
    const Root* root;
    
    Info() : hidden{}, removed{}, root{} {}
    auto operator<=>(const Info&) const = default;
};

struct Document::Info {
    QString     base;
    Folder      folder;
    bool        hidden;
    QString     key;
    QString     name;
    bool        removed;
    QString     skey;
    QString     suffix;
    Image       icon;
    
    Info() : hidden{}, removed{} {}
    auto operator<=>(const Info&) const = default;
};


struct Folder::Info {
    QString     brief;
    bool        hidden;
    QString     key;
    QString     name;
    Folder      parent;
    bool        removed;
    QString     skey;
    Image       icon;
    
    Info() : hidden{}, removed{} {}
    auto operator<=>(const Info&) const = default;
};

struct Fragment::Info {
    Document    document;
    Directory   directory;
    Folder      folder;
    bool        hidden;
    uint64_t    modified;
    QString     name;
    QString     path;
    bool        removed;
    bool        rescan;
    const Root* root;
    size_t      size;
    
    Info() : hidden{}, modified{}, removed{}, rescan{}, root{}, size{} {}
    auto operator<=>(const Info&) const = default;
};


namespace cdb {
    /*! \brief Returns all directories in the cache
    
        \param[in]  sorted  Yes/No to sort by path (default is no)
        \return Vector of directories
    */
    Vector<Directory>       all_directories(Sorted sorted=Sorted());
    Vector<Directory>       all_directories(const Root*, Sorted sorted=Sorted());
    size_t                  all_directories_count();
    size_t                  all_directories_count(const Root*);
    
    Vector<Document>        all_documents(Sorted sorted=Sorted());
    size_t                  all_documents_count();
    Vector<Document>        all_documents_suffix(const QString&, Sorted sorted=Sorted());

    Vector<Folder>          all_folders(Sorted sorted=Sorted());
    size_t                  all_folders_count();

    Vector<Fragment>        all_fragments(Sorted sorted=Sorted());
    Vector<Fragment>        all_fragments(const Root*,Sorted sorted=Sorted());
    size_t                  all_fragments_count();
    size_t                  all_fragments_count(const Root*);

    
    QString                 base_key(Document);         //!< key w/o final extensions
    QString                 base_key(Fragment);
    QString                 base_key(const QString&);

    QString                 brief(Folder);

    QByteArray              bytes(Fragment);                    // reads the specified fragment
    QByteArray              bytes(const QString&);              // equiv to bytes(fragment(document(k)))
    QByteArray              bytes(const QString&, DataRole);

    Vector<char>            chars(Fragment);
    Vector<char>            chars(const QString&);
    Vector<char>            chars(const QString&, DataRole);
    
    DirOrFrag               child(Directory, const QString&);
    DocOrFold               child(Folder, const QString&);

    size_t                  child_count(Directory);
    size_t                  child_count(Folder);

#if 0
    QString                 child_key(Directory);       //<! Key inside the directory (with extensions)
    QString                 child_key(Document);
    QString                 child_key(Folder);
    QString                 child_key(Fragment);
#endif

    Vector<DirOrFrag>       children(Directory,Sorted sorted=Sorted());
    Vector<DocOrFold>       children(Folder,Sorted sorted=Sorted());
    Vector<DirOrFrag>       children(const Root*,Sorted sorted=Sorted());

    Vector<DirOrFragStr>    children_and_names(Directory);
    Vector<DocOrFoldStr>    children_and_names(Folder);

    consteval Folder        classes_folder() { return Folder{Folder::CLASSES}; }
    consteval Folder        config_folder() { return Folder{Folder::CONFIG}; }

    //Vector<uint8_t>         data(Fragment);   // TODO

    Directory               db_directory(Directory, const QString&, bool *wasCreated=nullptr);
    Directory               db_directory(const Root*, bool *wasCreated=nullptr);
    Document                db_document(Folder, const QString&, bool *wasCreated=nullptr);

    Folder                  db_folder(Folder, const QString&, bool *wasCreated=nullptr);
    Fragment                db_fragment(Directory, const QString&, bool *wasCreated=nullptr);
    //Fragment                db_fragment(Document, const Root*);   // TODO


    Directory               directory(uint64_t);
    Directory               directory(const QString&);
    Directory               directory(Fragment);
    Directory               directory(Directory, const QString&);
    Directory               directory(const Root*);
    
    Vector<Directory>       directories(Directory, Sorted sorted=Sorted());
    Vector<Directory>       directories(Folder, Sorted sorted=Sorted());
    Vector<Directory>       directories(const Root*, Sorted sorted=Sorted());
    
    size_t                  directories_count(Directory);
    size_t                  directories_count(Folder);
    size_t                  directories_count(const Root*);
    
    Vector<DirString>       directories_with_names(Directory, Sorted sorted=Sorted());

    Document                document(Folder, const QString&);
    Document                document(Fragment);

    Document                document(const QString&);
    Document                document(uint64_t);

        //Vector<Document>      documents(Directory);   // TODO
    Vector<Document>        documents(Folder, Sorted sorted=Sorted());
    size_t                  documents_count(Folder);
    Vector<Document>        documents_by_suffix(Folder, const QString&, Sorted sorted=Sorted());
    Vector<Document>        documents_by_suffix_excluding(Folder, const QString&, Sorted sorted=Sorted());

    bool                    exists(Directory);
    bool                    exists(Document);
    bool                    exists(Folder);
    bool                    exists(Folder, const Root*);
    bool                    exists(Fragment);

    bool                    exists(const Root*, const char*);
    bool                    exists(const Root*, const QByteArray&);
    bool                    exists(const Root*, const QString&);
    bool                    exists(const Root*, const std::string&);

    bool                    exists_directory(uint64_t);
    bool                    exists_document(uint64_t);
    bool                    exists_folder(uint64_t);
    bool                    exists_fragment(uint64_t);

    Fragment                first(Document);
    Fragment                first(Document, const Root*);
    Fragment                first(Document, DataRole);

    Folder                  folder(Directory);
    Folder                  folder(Document);
    Folder                  folder(Fragment);
    Folder                  folder(Folder, const QString&);

    Folder                  folder(uint64_t);
    Folder                  folder(const QString&);
    
    Vector<Folder>          folders(Folder, Sorted sorted=Sorted());
    size_t                  folders_count(Folder);

    Fragment                fragment(const QString&);
    Fragment                fragment(uint64_t);
    Fragment                fragment(const Root*, const char*);
    Fragment                fragment(const Root*, const QByteArray&);
    Fragment                fragment(const Root*, const QString&);
    Fragment                fragment(const Root*, const std::string&);
    Fragment                fragment(Directory, const QString&);
    
    /*! \brief Returns the first fragmment found for the given document
    */
    Fragment                fragment(Document);
    Fragment                fragment(Document, const Root*);
    Fragment                fragment(Document, DataRole);
    
    Vector<Fragment>        fragments(Document, Sorted sorted=Sorted());
    Vector<Fragment>        fragments(Document, Sorted::Value sorted);
    Vector<Fragment>        fragments(Document, const Root*, Sorted sorted=Sorted());
    Vector<Fragment>        fragments(Document, const Root*, Sorted::Value sorted);
    Vector<Fragment>        fragments(Document, DataRole, Sorted sorted=Sorted());
    Vector<Fragment>        fragments(Directory, Sorted sorted=Sorted());
    Vector<Fragment>        fragments(Folder, Sorted sorted=Sorted());
    Vector<Fragment>        fragments(const Root*, Sorted sorted=Sorted());
    Vector<Fragment>        fragments(const QString&, Sorted sorted=Sorted());
    Vector<Fragment>        fragments(const QString&, DataRole, Sorted sorted=Sorted());

    size_t                  fragments_count(Document);
    size_t                  fragments_count(Document, const Root*);
    size_t                  fragments_count(Directory);
    size_t                  fragments_count(Folder);

    bool                    hidden(Directory);
    bool                    hidden(Document);
    bool                    hidden(Folder);
    bool                    hidden(Fragment);
    
    void                    hide(Document);

    Image                   icon(Document);
    Image                   icon(Folder);

    Directory::Info         info(Directory);
    Document::Info          info(Document);
    Folder::Info            info(Folder);
    Fragment::Info          info(Fragment);

    QString                 key(Directory);
    QString                 key(Document);
    QString                 key(Folder);
    QString                 key(Fragment);
    QString                 key(const Root*);

    //QString             label(Directory);
    QString                 label(Document);
    QString                 label(Folder);
    QString                 label(Fragment);

    uint64_t                modified(Fragment);

    QString                 name(Directory);
    QString                 name(Document);
    QString                 name(Folder);
    QString                 name(Fragment);

    NKI                     nki(Document, bool autoKeyToName=false);
    NKI                     nki(Folder, bool autoKeyToName=false);


    Directory               parent(Directory);
    Folder                  parent(Document);
    Folder                  parent(Folder);
    Directory               parent(Fragment);

    QString                 path(Directory);
    QString                 path(Fragment);
    QString                 path(const Root*, const char*, bool fMakePath=false);
    QString                 path(const Root*, const QByteArray&, bool fMakePath=false);
    QString                 path(const Root*, const QString&, bool fMakePath=false);
    QString                 path(const Root*, const std::string&, bool fMakePath=false);
    QString                 path(const Root*, Document, bool fMakePath=false);

    bool                    removed(Directory);
    bool                    removed(Document);
    bool                    removed(Folder);
    bool                    removed(Fragment);
    
    void                    rescan(Fragment);
    bool                    rescanning(Fragment);

    const Root*             root(Directory);
    const Root*             root(Fragment);
    
    Vector<const Root*>     roots(Document);
    Vector<const Root*>     roots(Folder);

    size_t                  roots_count(Document);
    size_t                  roots_count(Folder);

    void                    show(Document);
    size_t                  size(Fragment);

    QString                 skey(Directory);  //!< Key inside the directory
    QString                 skey(Document);   //!< Key inside the directory
    QString                 skey(Folder);     //!< Key inside the directory
    QString                 skey(Fragment);   //!< Key inside the directory

    QString                 skeyb(Directory); //!< Key inside the directory (w/o extensions)
    QString                 skeyb(Document);  //!< Key inside the directory (w/o extensions)
    QString                 skeyb(Folder);    //!< Key inside the directory (w/o extensions)
    QString                 skeyb(Fragment);  //!< Key inside the directory (w/o extensions)

    QString                 suffix(Document);

    consteval Folder        tags_folder() { return Folder{Folder::TAGS}; }
    consteval Folder        top_folder() { return Folder{Folder::TOP}; }

    void                    update(Fragment);

    Fragment                writable(Document, DataRole);
}

