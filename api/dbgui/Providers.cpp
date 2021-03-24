////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <db/Cache.hpp>
#include <dbgui/atom/AtomProvider.hpp>
#include <dbgui/class/ClassProvider.hpp>
#include <dbgui/directory/DirectoryProvider.hpp>
#include <dbgui/document/DocumentProvider.hpp>
#include <dbgui/field/FieldProvider.hpp>
#include <dbgui/folder/FolderProvider.hpp>
#include <dbgui/fragment/FragmentProvider.hpp>
#include <dbgui/leaf/LeafProvider.hpp>
#include <dbgui/tag/TagProvider.hpp>
#include <dbgui/wksp/RootProvider.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace {
    struct AllAtoms : public Provider<Atom> {
        AllAtoms() {}
        ~AllAtoms(){}
        virtual Vector<Atom>    all() const override { return cdb::all_atoms(); }
    };
}

namespace provider {
    AtomProvider    all_atoms()
    {
        return std::make_shared<AllAtoms>();
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
    struct AllClasses : public Provider<Class> {
        AllClasses(){}
        ~AllClasses(){}
        virtual Vector<Class>   all() const override { return cdb::all_classes(); }
        virtual bool        can_create() const override { return true; }
        virtual Class       create(const QString&k, const Root* rt) const override
        {
            return cdb::make_class(k,rt);
        }
    };
}

namespace provider {
    ClassProvider       all_classes()
    {
        return std::make_shared<AllClasses>();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
    struct AllDirectories : public Provider<Directory> {
        AllDirectories(){}
        ~AllDirectories(){}
        virtual Vector<Directory>   all() const override { return cdb::all_directories(); }
    };
}

namespace provider {
    DirectoryProvider   all_directories()
    {
        return std::make_shared<AllDirectories>();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
    struct AllDocuments : public Provider<Document> {
        AllDocuments(){}
        ~AllDocuments(){}
        virtual Vector<Document>    all() const override { return cdb::all_documents(); }
    };
}

namespace provider {
    DocumentProvider    all_documents()
    {
        return std::make_shared<AllDocuments>();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace {
    struct AllFields : public Provider<Field> {
        AllFields(){}
        ~AllFields(){}
        virtual Vector<Field>   all() const override { return cdb::all_fields(); }
    };
}

namespace provider {
    FieldProvider       all_fields()
    {   
        return std::make_shared<AllFields>();
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
    struct AllFolders : public Provider<Folder> {
        AllFolders(){}
        ~AllFolders(){}
        virtual Vector<Folder>    all() const override { return cdb::all_folders(); }
    };
}

namespace provider {
    FolderProvider      all_folders()
    {
        return std::make_shared<AllFolders>();
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
    struct AllFragments : public Provider<Fragment> {
        AllFragments(){}
        ~AllFragments(){}
        virtual Vector<Fragment>    all() const override { return cdb::all_fragments(); }
    };
}

namespace provider {
    FragmentProvider      all_fragments()
    {
        return std::make_shared<AllFragments>();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
    struct AllLeafs : public Provider<Leaf> {
        AllLeafs(){}
        ~AllLeafs(){}
        virtual Vector<Leaf>       all() const override { return cdb::all_leafs(); }
    };
}


namespace provider {
    LeafProvider      all_leafs()
    {
        return std::make_shared<AllLeafs>();
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
    struct AllRoots : public Provider<const Root*>{
        AllRoots(){}
        ~AllRoots(){}
        virtual Vector<const Root*> all() const override { return wksp::roots(); }
    };
}

namespace provider {
    RootProvider      all_roots()
    {
        return std::make_shared<AllRoots>();
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
    struct AllTags : public Provider<Tag> {
        AllTags(){}
        ~AllTags(){}
        virtual Vector<Tag>     all() const override { return cdb::all_tags(); }
        virtual bool            can_create() const override { return true; }
        virtual Tag             create(const QString& k, const Root* rt) const override
        {
            return cdb::make_tag(k,rt);
        }
    };
}

namespace provider {
    TagProvider     all_tags()
    {
        return std::make_shared<AllTags>();
    }
}

