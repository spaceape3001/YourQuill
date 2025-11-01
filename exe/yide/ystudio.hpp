////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QObject>
#include <concepts>
#include <yq/mithril/id/Id.hpp>
#include <QList>

using IdFilter  = yq::mithril::IdFilter;
using IdTypeId  = yq::mithril::IdTypeId;
using IdTypes   = yq::mithril::IdTypes;
using Id        = yq::mithril::Id;

class QAction;

class Command;
class Dock;
class Window;
class DreamMW;

enum class ArgFlavor {
    //! Default (no-action)
    Dead    = 0,

    //! No extra arguments
    Main,
    
    //! Id matters
    Id
};

using QActionList = QList<QAction*>;

