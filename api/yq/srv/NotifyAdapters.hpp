////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Notifier.hpp"
#include <db/filesys/fragment.hpp>
#include <functional>

namespace yq {
        ////////////////////////////////////////////////////////////////////////////////
    
    template <void (*FN)()> 
    class SingleFileNotifier : public Notifier {
    public:
    
        SingleFileNotifier(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, int _order, const std::source_location&sl) :
            Notifier(t, ch, folder, ext, fname, _order, sl)
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
        return Notifier::Writer{new SingleFileNotifier<FN>(Notifier::ByFile, all_set<Change>(), Folder(), std::string_view(), fp, 0, sl)};
    }
    
    template <void (*FN)()> 
    Notifier::Writer    on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new SingleFileNotifier<FN>(Notifier::ByFolderExt, all_set<Change>(), f, ext, std::filesystem::path(), 0, sl)};
    }

    template <void (*FN)()> 
    Notifier::Writer    on_change(int order, const std::filesystem::path&fp, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new SingleFileNotifier<FN>(Notifier::ByFile, all_set<Change>(), Folder(), std::string_view(), fp, order, sl)};
    }
    
    template <void (*FN)()> 
    Notifier::Writer    on_change(int order, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new SingleFileNotifier<FN>(Notifier::ByFolderExt, all_set<Change>(), f, ext, std::filesystem::path(), order, sl)};
    }


    Notifier::Writer    on_change(const std::filesystem::path&fp, std::function<void()>, const std::source_location& sl = std::source_location::current());;
    Notifier::Writer    on_change(std::string_view ext, std::function<void()>, const std::source_location& sl = std::source_location::current());
    Notifier::Writer    on_change(Folder f, std::string_view ext, std::function<void()>, const std::source_location& sl = std::source_location::current());

        ////////////////////////////////////////////////////////////////////////////////

    template <void(*FN)(Fragment)>
    class FragmentFileNotifier : public Notifier {
    public:
        FragmentFileNotifier(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, int _order, const std::source_location&sl) :
            Notifier(t, ch, folder, ext, fname, _order, sl)
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
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecName, all_set<Change>(), Folder(), ext, std::filesystem::path(), 0, sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecFolderName, all_set<Change>(), f, ext, std::filesystem::path(), 0, sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecName, ch, Folder(), ext, std::filesystem::path(), 0, sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecFolderName, ch, f, ext, std::filesystem::path(), 0, sl)};
    }


    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(int order, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecName, all_set<Change>(), Folder(), ext, std::filesystem::path(), order, sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(int order, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecFolderName, all_set<Change>(), f, ext, std::filesystem::path(), order, sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(int order, ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecName, ch, Folder(), ext, std::filesystem::path(), order, sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(int order, ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecFolderName, ch, f, ext, std::filesystem::path(), order, sl)};
    }


    //template <void (*FN)(Fragment)> 
    //Notifier::Writer    on_add(std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecName, Change::Added, Folder(), ext, std::filesystem::path(), 0, sl)};
    //}

    //template <void (*FN)(Fragment)> 
    //Notifier::Writer    on_add(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecFolderName, Change::Added, f, ext, std::filesystem::path(), 0, sl)};
    //}


    //template <void (*FN)(Fragment)> 
    //Notifier::Writer    on_modify(std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecName, Change::Modified, Folder(), ext, std::filesystem::path(), 0, sl)};
    //}

    //template <void (*FN)(Fragment)> 
    //Notifier::Writer    on_modify(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecFolderName, Change::Modified, f, ext, std::filesystem::path(), 0, sl)};
    //}


    //template <void (*FN)(Fragment)> 
    //Notifier::Writer    on_remove(std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecName, Change::Removed, Folder(), ext, std::filesystem::path(), 0, sl)};
    //}

    //template <void (*FN)(Fragment)> 
    //Notifier::Writer    on_remove(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentFileNotifier<FN>(Notifier::SpecFolderName, Change::Removed, f, ext, std::filesystem::path(), 0, sl)};
    //}


        ////////////////////////////////////////////////////////////////////////////////

    template <void(*FN)(Fragment,Change)>
    class FragmentChangeFileNotifier : public Notifier {
    public:
        FragmentChangeFileNotifier(Trigger t, Flag<Change> ch, Folder folder, std::string_view ext, const std::filesystem::path& fname, int _order, const std::source_location&sl) :
            Notifier(t, ch, folder, ext, fname, _order, sl)
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
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecName, all_set<Change>(), Folder(), ext, std::filesystem::path(), 0, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecFolderName, all_set<Change>(), f, ext, std::filesystem::path(), 0, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecName, ch, Folder(), ext, std::filesystem::path(), 0, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecFolderName, ch, f, ext, std::filesystem::path(), 0, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(int order, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecName, all_set<Change>(), Folder(), ext, std::filesystem::path(), order, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(int order, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecFolderName, all_set<Change>(), f, ext, std::filesystem::path(), order, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(int order, ChangeFlags ch, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecName, ch, Folder(), ext, std::filesystem::path(), order, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(int order, ChangeFlags ch, Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecFolderName, ch, f, ext, std::filesystem::path(), order, sl)};
    }

    //template <void (*FN)(Fragment, Change)> 
    //Notifier::Writer    on_add(std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecName, Change::Added, Folder(), ext, std::filesystem::path(), 0, sl)};
    //}

    //template <void (*FN)(Fragment, Change)> 
    //Notifier::Writer    on_add(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecFolderName, Change::Added, f, ext, std::filesystem::path(), 0, sl)};
    //}

    //template <void (*FN)(Fragment, Change)> 
    //Notifier::Writer    on_modify(std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecName, Change::Modified, Folder(), ext, std::filesystem::path(), 0, sl)};
    //}

    //template <void (*FN)(Fragment, Change)> 
    //Notifier::Writer    on_modify(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecFolderName, Change::Modified, f, ext, std::filesystem::path(), 0, sl)};
    //}

    //template <void (*FN)(Fragment, Change)> 
    //Notifier::Writer    on_remove(std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecName, Change::Removed, Folder(), ext, std::filesystem::path(), 0, sl)};
    //}

    //template <void (*FN)(Fragment, Change)> 
    //Notifier::Writer    on_remove(Folder f, std::string_view ext, const std::source_location& sl = std::source_location::current())
    //{
        //return Notifier::Writer{new FragmentChangeFileNotifier<FN>(Notifier::SpecFolderName, Change::Removed, f, ext, std::filesystem::path(), 0, sl)};
    //}

}
