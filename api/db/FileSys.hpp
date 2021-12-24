////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CacheFwd.hpp"
#include "Image.hpp"
#include <db/enum/DataRole.hpp>
#include <db/enum/Sorted.hpp>

#include <filesystem>
#include <util/String.hpp>

class ByteArray;

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
    Folder                  folder;
    bool                    hidden      = false;
    String                  name;
    Directory               parent;
    std::filesystem::path   path;
    bool                    removed     = false;
    const Root*             root        = nullptr;
    
    bool operator==(const Info&) const = default;
};

struct Document::Info {
    String      base;
    Folder      folder;
    bool        hidden      = false;
    String      key;
    String      name;
    bool        removed     = false;
    String      skey;
    String      suffix;
    Image       icon;
    
    bool operator==(const Info&) const = default;
};


struct Folder::Info {
    String      brief;
    bool        hidden      = false;
    String      key;
    String      name;
    Folder      parent;
    bool        removed     = false;
    String      skey;
    Image       icon;

    bool operator==(const Info&) const = default;
};

struct Fragment::Info {
    Document                document;
    Directory               directory;
    Folder                  folder;
    bool                    hidden      = false;
    uint64_t                modified    = 0ULL;
    String                  name;
    std::filesystem::path   path;
    bool                    removed     = false;
    bool                    rescan      = false;
    const Root*             root        = nullptr;
    size_t                  size        = 0;
    
    bool operator==(const Info&) const = default;
};


namespace cdb {
    /*! \brief Returns all directories in the cache
    
        \param[in]  sorted  Yes/No to sort by path (default is no)
        \return Vector of directories
    */
    Vector<Directory>       all_directories(unsigned opts=0);
    Vector<Directory>       all_directories(Sorted sorted);
    Vector<Directory>       all_directories(const Root*, Sorted sorted=Sorted());
    size_t                  all_directories_count();
    size_t                  all_directories_count(const Root*);
    
    Vector<Document>        all_documents(Sorted sorted=Sorted());
    size_t                  all_documents_count();
    Vector<Document>        all_documents_suffix(const String&, Sorted sorted=Sorted());

    Vector<Folder>          all_folders(Sorted sorted=Sorted());
    size_t                  all_folders_count();

    Vector<Fragment>        all_fragments(Sorted sorted=Sorted());
    Vector<Fragment>        all_fragments(const Root*,Sorted sorted=Sorted());
    size_t                  all_fragments_count();
    size_t                  all_fragments_count(const Root*);
    Vector<Fragment>        all_fragments_suffix(const String&, Sorted sorted=Sorted());

    
    String                  base_key(Document);         //!< key w/o final extensions
    String                  base_key(Fragment);
    String                  base_key(const String&);

    String                  brief(Folder);

    DirOrFrag               child(Directory, const String&);
    DocOrFold               child(Folder, const String&);

    size_t                  child_count(Directory);
    size_t                  child_count(Folder);

#if 0
    String                  child_key(Directory);       //<! Key inside the directory (with extensions)
    String                  child_key(Document);
    String                  child_key(Folder);
    String                  child_key(Fragment);
#endif

    Vector<DirOrFrag>       children(Directory,Sorted sorted=Sorted());
    Vector<DocOrFold>       children(Folder,Sorted sorted=Sorted());
    Vector<DirOrFrag>       children(const Root*,Sorted sorted=Sorted());

    Vector<DirOrFragStr>    children_and_names(Directory);
    Vector<DocOrFoldStr>    children_and_names(Folder);

    consteval Folder        classes_folder() { return Folder{Folder::CLASSES}; }
    consteval Folder        config_folder() { return Folder{Folder::CONFIG}; }

    //Vector<uint8_t>         data(Fragment);   // TODO

    Directory               db_directory(Directory, const String&, bool *wasCreated=nullptr);
    Directory               db_directory(const Root*, bool *wasCreated=nullptr);
    Document                db_document(Folder, const String&, bool *wasCreated=nullptr);

    Folder                  db_folder(Folder, const String&, bool *wasCreated=nullptr);
    Fragment                db_fragment(Directory, const String&, bool *wasCreated=nullptr);
    //Fragment                db_fragment(Document, const Root*);   // TODO


    Directory               directory(uint64_t);
    Directory               directory(const String&);
    Directory               directory(const std::filesystem::path&);
    Directory               directory(Fragment);
    Directory               directory(Directory, const String&);
    Directory               directory(const Root*);
    
    Vector<Directory>       directories(Directory, Sorted sorted=Sorted());
    Vector<Directory>       directories(Folder, Sorted sorted=Sorted());
    Vector<Directory>       directories(const Root*, Sorted sorted=Sorted());
    
    size_t                  directories_count(Directory);
    size_t                  directories_count(Folder);
    size_t                  directories_count(const Root*);
    
    Vector<DirString>       directories_with_names(Directory, Sorted sorted=Sorted());

    Document                document(Folder, const String&);
    Document                document(Fragment);

    Document                document(const String&);
    Document                document(uint64_t);

        //Vector<Document>      documents(Directory);   // TODO
    Vector<Document>        documents(Folder, unsigned opts=0);
    Vector<Document>        documents(Folder, Sorted sorted);
    size_t                  documents_count(Folder, unsigned opts=0);
    Vector<Document>        documents_by_suffix(Folder, const String&, Sorted sorted=Sorted());
    Vector<Document>        documents_by_suffix_excluding(Folder, const String&, Sorted sorted=Sorted());

    bool                    exists(Directory);
    bool                    exists(Document);
    bool                    exists(Folder);
    bool                    exists(Folder, const Root*);
    bool                    exists(Fragment);

    bool                    exists(const Root*, const char*);
    bool                    exists(const Root*, const QByteArray&);
    bool                    exists(const Root*, const String&);

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
    Folder                  folder(Folder, const String&);

    Folder                  folder(uint64_t);
    Folder                  folder(const String&);
    
    Vector<Folder>          folder_path(Directory);
    Vector<Folder>          folder_path(Document);
    Vector<Folder>          folder_path(Folder);
    Vector<Folder>          folder_path(Fragment);
    
    Vector<Folder>          folders(Folder, unsigned int opts=0);
    Vector<Folder>          folders(Folder, Sorted sorted);
    size_t                  folders_count(Folder, unsigned int opts=0);

    ByteArray               frag_bytes(Fragment);                    // reads the specified fragment
    ByteArray               frag_bytes(const String&);              // equiv to bytes(fragment(document(k)))
    ByteArray               frag_bytes(const String&, DataRole);

    //String                  frag_string(Fragment);
    //String                  frag_string(const String&);
    //String                  frag_string(const String&, DataRole);

    Fragment                fragment(const std::filesystem::path&);
    Fragment                fragment(const String&);
    Fragment                fragment(uint64_t);
    Fragment                fragment(const Root*, const char*);
    Fragment                fragment(const Root*, const QByteArray&);
    Fragment                fragment(const Root*, const String&);
    Fragment                fragment(const Root*, const std::string&);
    Fragment                fragment(Directory, const String&);
    
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
    Vector<Fragment>        fragments(const String&, Sorted sorted=Sorted());
    Vector<Fragment>        fragments(const String&, DataRole, Sorted sorted=Sorted());

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

    String                  key(Directory);
    String                  key(Document);
    String                  key(Folder);
    String                  key(Fragment);
    String                  key(const Root*);

    //String             label(Directory);
    String                  label(Document);
    String                  label(Folder);
    String                  label(Fragment);

    uint64_t                modified(Fragment);

    String                  name(Directory);
    String                  name(Document);
    String                  name(Folder);
    String                  name(Fragment);

    NKI                     nki(Document, bool autoKeyToName=false);
    NKI                     nki(Folder, bool autoKeyToName=false);


    Directory               parent(Directory);
    Folder                  parent(Document);
    Folder                  parent(Folder);
    Directory               parent(Fragment);

    std::filesystem::path   path(Directory);
    std::filesystem::path   path(Fragment);
    std::filesystem::path   path(const Root*, const char*, bool fMakePath=false);
    std::filesystem::path   path(const Root*, const QByteArray&, bool fMakePath=false);
    std::filesystem::path   path(const Root*, const String&, bool fMakePath=false);
    std::filesystem::path   path(const Root*, const std::string&, bool fMakePath=false);
    std::filesystem::path   path(const Root*, Document, bool fMakePath=false);

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

    String                  skey(Directory);  //!< Key inside the directory
    String                  skey(Document);   //!< Key inside the directory
    String                  skey(Folder);     //!< Key inside the directory
    String                  skey(Fragment);   //!< Key inside the directory

    String                  skeyb(Directory); //!< Key inside the directory (w/o extensions)
    String                  skeyb(Document);  //!< Key inside the directory (w/o extensions)
    String                  skeyb(Folder);    //!< Key inside the directory (w/o extensions)
    String                  skeyb(Fragment);  //!< Key inside the directory (w/o extensions)

    String                  suffix(Document);
    String                  suffix(Fragment);

    consteval Folder        tags_folder() { return Folder{Folder::TAGS}; }
    consteval Folder        top_folder() { return Folder{Folder::TOP}; }

    void                    update(Fragment);

    /*! \brief Tests to see if directory is within the specified folder
    
        This tests to see if the spacified directory is within the folder.
        
        \param[in] parentFolder     Folder that we're checking
        \param[in] theDirectory     Directory under test
        \param[in] recursive        If TRUE, routine marches the path upward until top or parent is encountered.
        \return TRUE if the directory is within the specified folder
    */
    bool                    within(Folder parentFolder, Directory theDirectory, bool recursive=true);

    /*! \brief Tests to see if directory is within the specified folder
    
        This tests to see if the spacified directory is within the folder.
        
        \param[in] parentFolder     Folder that we're checking
        \param[in] theDocument      Document under test
        \param[in] recursive        If TRUE, routine marches the path upward until top or parent is encountered.
        \return TRUE if the directory is within the specified folder
    */
    bool                    within(Folder parentFolder, Document theDocument, bool recursive=true);

    /*! \brief Tests to see if directory is within the specified folder
    
        This tests to see if the spacified directory is within the folder.
        
        \param[in] parentFolder     Folder that we're checking
        \param[in] theFolder        Folder under test
        \param[in] recursive        If TRUE, routine marches the path upward until top or parent is encountered.
        \return TRUE if the directory is within the specified folder
    */
    bool                    within(Folder parentFolder, Folder theFolder, bool recursive=true);

    /*! \brief Tests to see if directory is within the specified folder
    
        This tests to see if the spacified directory is within the folder.
        
        \param[in] parentFolder     Folder that we're checking
        \param[in] theFragment      Fragment under test
        \param[in] recursive        If TRUE, routine marches the path upward until top or parent is encountered.
        \return TRUE if the directory is within the specified folder
    */
    bool                    within(Folder parentFolder, Fragment theFragment, bool recursive=true);

    Fragment                writable(Document, DataRole);
}

