////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Importer.hpp"
#include "Fragment.hpp"

namespace yq {
        ////////////////////////////////////////////////////////////////////////////////
    
    template <void (*FN)()> 
    class SingleFileImporter : public Importer {
    public:
    
        SingleFileImporter(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, const std::source_location&sl) :
            Importer(t, ch, folder, ext, fname, sl)
        {
            
        }
    
        bool change(Fragment, Change) const override
        {
            FN();
            return true;
        }
    };
    
    template <void (*FN)()> 
    Importer::Writer    on_change(const std::filesystem::path&fp, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new SingleFileImporter<FN>(Importer::ByFile, all_set<Change>(), Folder(), std::string_view(), fp, sl)};
    }
    
        ////////////////////////////////////////////////////////////////////////////////

    template <void(*FN)(Fragment)>
    class FragmentFileImporter : public Importer {
    public:
        FragmentFileImporter(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, const std::source_location&sl) :
            Importer(t, ch, folder, ext, fname, sl)
        {
        }

        bool change(Fragment f, Change) const override
        {
            FN(f);
            return true;
        }
    };
    
    template <void (*FN)(Fragment)> 
    Importer::Writer    on_change(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByExtension, all_set<Change>(), Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Importer::Writer    on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByFolderFile, all_set<Change>(), f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Importer::Writer    on_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByExtension, ch, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Importer::Writer    on_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByFolderFile, ch, f, ext, std::filesystem::path(), sl)};
    }


    template <void (*FN)(Fragment)> 
    Importer::Writer    on_add(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByExtension, Change::Added, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Importer::Writer    on_add(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByFolderFile, Change::Added, f, ext, std::filesystem::path(), sl)};
    }


    template <void (*FN)(Fragment)> 
    Importer::Writer    on_modify(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByExtension, Change::Modified, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Importer::Writer    on_modify(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByFolderFile, Change::Modified, f, ext, std::filesystem::path(), sl)};
    }


    template <void (*FN)(Fragment)> 
    Importer::Writer    on_remove(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByExtension, Change::Removed, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Importer::Writer    on_remove(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByFolderFile, Change::Removed, f, ext, std::filesystem::path(), sl)};
    }


    template <void (*FN)(Fragment)> 
    Importer::Writer    on_startup(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByExtension, Change::Startup, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Importer::Writer    on_startup(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentFileImporter<FN>(Importer::ByFolderFile, Change::Startup, f, ext, std::filesystem::path(), sl)};
    }

        ////////////////////////////////////////////////////////////////////////////////

    template <void(*FN)(Fragment,Change)>
    class FragmentChangeFileImporter : public Importer {
    public:
        FragmentChangeFileImporter(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, const std::source_location&sl) :
            Importer(t, ch, folder, ext, fname, sl)
        {
        }

        bool change(Fragment f, Change c) const override
        {
            FN(f, c);
            return true;
        }
    };
    
    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_change(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByExtension, all_set<Change>(), Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByFolderFile, all_set<Change>(), f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByExtension, ch, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByFolderFile, ch, f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_add(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByExtension, Change::Added, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_add(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByFolderFile, Change::Added, f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_modify(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByExtension, Change::Modified, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_modify(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByFolderFile, Change::Modified, f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_remove(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByExtension, Change::Removed, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_remove(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByFolderFile, Change::Removed, f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_startup(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByExtension, Change::Startup, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Importer::Writer    on_startup(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new FragmentChangeFileImporter<FN>(Importer::ByFolderFile, Change::Startup, f, ext, std::filesystem::path(), sl)};
    }
        ////////////////////////////////////////////////////////////////////////////////

    template <bool(*FN)(Fragment,Change)>
    class BoolFragmentChangeFileImporter : public Importer {
    public:
        BoolFragmentChangeFileImporter(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, const std::source_location&sl) :
            Importer(t, ch, folder, ext, fname, sl)
        {
        }

        bool change(Fragment f, Change c) const override
        {
            return FN(f, c);
        }
    };

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_change(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFile, all_set<Change>(), Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFolderFile, all_set<Change>(), f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFile, ch, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFolderFile, ch, f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_add(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFile, Change::Added, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_add(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFolderFile, Change::Added, f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_modify(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFile, Change::Modified, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_modify(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFolderFile, Change::Modified, f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_remove(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFile, Change::Removed, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_remove(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFolderFile, Change::Removed, f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_startup(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{new BoolFragmentChangeFileImporter<FN>(Importer::ByFile, Change::Startup, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Importer::Writer    on_startup(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Importer::Writer{ new BoolFragmentChangeFileImporter<FN>(Importer::ByFolderFile, Change::Startup, f, ext, std::filesystem::path(), sl) };
    }
    
        ////////////////////////////////////////////////////////////////////////////////
}
