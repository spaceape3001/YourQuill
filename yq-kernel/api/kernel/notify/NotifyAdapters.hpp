////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Notifier.hpp"
#include <kernel/file/Fragment.hpp>
#include <functional>

namespace yq {
        ////////////////////////////////////////////////////////////////////////////////
    
    template <void (*FN)()> 
    class SingleFileNotifier : public Notifier {
    public:
    
        SingleFileNotifier(Flag<Change> ch, const FileSpec& spec, int _order, const std::source_location&sl) :
            Notifier(ch, spec, _order, sl)
        {
            
        }
    
        bool change(Fragment, Change) const override
        {
            FN();
            return true;
        }
    };
    
    template <void (*FN)()> 
    Notifier::Writer    on_change(const FileSpec&spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new SingleFileNotifier<FN>(all_set<Change>(), spec, 0, sl)};
    }

    template <void (*FN)()> 
    Notifier::Writer    on_change(int order, const FileSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new SingleFileNotifier<FN>(all_set<Change>(), spec, order, sl)};
    }

    Notifier::Writer    on_change(const FileSpec&, std::function<void()>, const std::source_location& sl = std::source_location::current());
    //Notifier::Writer    on_change(Folder f, const FileSpec&, std::function<void()>, const std::source_location& sl = std::source_location::current());

        ////////////////////////////////////////////////////////////////////////////////

    template <void(*FN)(Fragment)>
    class FragmentFileNotifier : public Notifier {
    public:
        FragmentFileNotifier(Flag<Change> ch, const FileSpec& spec, int _order, const std::source_location&sl) :
            Notifier(ch, spec, _order, sl)
        {
        }

        bool change(Fragment f, Change) const override
        {
            FN(f);
            return true;
        }
    };
    
    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(const FileSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(all_set<Change>(), spec, 0, sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(ChangeFlags ch, const FileSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(ch, spec, 0, sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(int order, const FileSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(all_set<Change>(), spec, order, sl)};
    }

    template <void (*FN)(Fragment)> 
    Notifier::Writer    on_change(int order, ChangeFlags ch, const FileSpec&spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentFileNotifier<FN>(ch, spec, order, sl)};
    }


        ////////////////////////////////////////////////////////////////////////////////

    template <void(*FN)(Fragment,Change)>
    class FragmentChangeFileNotifier : public Notifier {
    public:
        FragmentChangeFileNotifier(Flag<Change> ch, const FileSpec& spec, int _order, const std::source_location&sl) :
            Notifier(ch, spec, _order, sl)
        {
        }

        bool change(Fragment f, Change c) const override
        {
            FN(f, c);
            return true;
        }
    };
    
    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(const FileSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(all_set<Change>(), spec, 0, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(ChangeFlags ch, const FileSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(ch, spec, 0, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(int order, const FileSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(all_set<Change>(), spec, order, sl)};
    }

    template <void (*FN)(Fragment, Change)> 
    Notifier::Writer    on_change(int order, ChangeFlags ch, const FileSpec& spec, const std::source_location& sl = std::source_location::current())
    {
        return Notifier::Writer{new FragmentChangeFileNotifier<FN>(ch, spec, order, sl)};
    }
}
