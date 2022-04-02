////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Notifier.hpp"
#include "Fragment.hpp"

namespace yq {
        ////////////////////////////////////////////////////////////////////////////////
    
    template <void (*FN)()> 
    class SingleFileNotifier : public Notifier {
    public:
    
        SingleFileNotifier(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, const std::source_location&sl) :
            Notifier(t, ch, folder, ext, fname, sl)
        {
            
        }
    
        bool change(Fragment, Change) const override
        {
            FN();
            return true;
        }
    };
    
    template <void (*FN)()> 
    Notifier::Writer    on_change(const std::filesystem::path&fp, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new SingleFileNotifier<FN>(Notifier::ByFile, all_set<Change>(), Folder(), std::string_view(), fp, sl)};
    }
    
        ////////////////////////////////////////////////////////////////////////////////

    template <void(*FN)(Fragment)>
    class FragmentFileNotifier : public Notifier {
    public:
        FragmentFileNotifier(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, const std::source_location&sl) :
            Notifier(t, ch, folder, ext, fname, sl)
        {
        }

        bool change(Fragment f, Change) const override
        {
            FN(f);
            return true;
        }
    };
    
    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByExtension, all_set<Change>(), Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByFolderFile, all_set<Change>(), f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByExtension, ch, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByFolderFile, ch, f, ext, std::filesystem::path(), sl)};
    }


    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_add(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByExtension, Change::Added, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_add(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByFolderFile, Change::Added, f, ext, std::filesystem::path(), sl)};
    }


    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_modify(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByExtension, Change::Modified, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_modify(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByFolderFile, Change::Modified, f, ext, std::filesystem::path(), sl)};
    }


    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_remove(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByExtension, Change::Removed, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_remove(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByFolderFile, Change::Removed, f, ext, std::filesystem::path(), sl)};
    }


    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_startup(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByExtension, Change::Startup, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_startup(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::ByFolderFile, Change::Startup, f, ext, std::filesystem::path(), sl)};
    }

        ////////////////////////////////////////////////////////////////////////////////

    template <void(*FN)(Fragment,Change)>
    class FragmentChangeFileNotifier : public Notifier {
    public:
        FragmentChangeFileNotifier(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, const std::source_location&sl) :
            Notifier(t, ch, folder, ext, fname, sl)
        {
        }

        bool change(Fragment f, Change c) const override
        {
            FN(f, c);
            return true;
        }
    };
    
    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByExtension, all_set<Change>(), Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, all_set<Change>(), f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByExtension, ch, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, ch, f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_add(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByExtension, Change::Added, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_add(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, Change::Added, f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_modify(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByExtension, Change::Modified, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_modify(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, Change::Modified, f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_remove(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByExtension, Change::Removed, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_remove(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, Change::Removed, f, ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_startup(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByExtension, Change::Startup, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_startup(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, Change::Startup, f, ext, std::filesystem::path(), sl)};
    }
        ////////////////////////////////////////////////////////////////////////////////

    template <bool(*FN)(Fragment,Change)>
    class BoolFragmentChangeFileNotifier : public Notifier {
    public:
        BoolFragmentChangeFileNotifier(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, const std::source_location&sl) :
            Notifier(t, ch, folder, ext, fname, sl)
        {
        }

        bool change(Fragment f, Change c) const override
        {
            return FN(f, c);
        }
    };

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFile, all_set<Change>(), Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, all_set<Change>(), f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFile, ch, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, ch, f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_add(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFile, Change::Added, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_add(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, Change::Added, f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_modify(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFile, Change::Modified, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_modify(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, Change::Modified, f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_remove(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFile, Change::Removed, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_remove(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, Change::Removed, f, ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_startup(std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFile, Change::Startup, Folder(), ext, std::filesystem::path(), sl)};
    }

    template <bool  (*FN)(Fragment, Change)> 
    Notifier::Writer    on_startup(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{ new BoolFragmentChangeFileNotifier<FN>(Notifier::ByFolderFile, Change::Startup, f, ext, std::filesystem::path(), sl) };
    }
    
        ////////////////////////////////////////////////////////////////////////////////
}
