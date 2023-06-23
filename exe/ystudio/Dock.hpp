////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Flags.hpp>
#include <basic/Object.hpp>
#include <meta/ObjectInfoWriter.hpp>
#include <QDockWidget>
#include <QIcon>
#include <QKeySequence>

enum class DockBit : uint8_t {
    AutoStart,
    Creatable
};

class DockInfo : public yq::ObjectInfo {
public:

    template <typename C> class Writer;
    DockInfo(std::string_view, const ObjectInfo&, const std::source_location& sl = std::source_location::current());

    static const std::vector<const DockInfo*>&  all();

    
private:
    QString                 m_label;        // for the action
    QIcon                   m_icon;         // for the action
    QKeySequence            m_shortcut;     // for the action
    yq::Flags<DockBit>      m_bits;
    Qt::DockWidgetArea      m_startArea = Qt::LeftDockWidgetArea;
};

/*! \brief Singular dock

    This is a singular dock for the main window.  
    It'll use singals/slots for communications to main window.
*/
class Dock : public QDockWidget, public yq::Object {
    Q_OBJECT
    YQ_OBJECT_INFO(DockInfo)
    YQ_OBJECT_DECLARE(Dock, Object)
public:

    Dock(QWidget*parent=nullptr);
    ~Dock();
    
};

template <typename C> 
class DockInfo::Writer : public yq::ObjectInfo::Writer<C> {
public:
    Writer(DockInfo* dockInfo) : yq::ObjectInfo::Writer<C>(dockInfo), m_meta(dockInfo)
    {
        if constexpr ((!std::is_same_v<C,DockInfo>) && (!std::is_abstract_v<C>) && std::is_constructible_v<C>){
            m_meta -> m_bits |= DockBit::Creatable;
        }
    }
    
    Writer(DockInfo& dockInfo) : Writer(&dockInfo)
    {
    }

    Writer& label(const QString& s)
    {
        if(m_meta)
            m_meta -> m_label = s;
        return *this;
    }
    
    Writer& autoStart()
    {
        if(m_meta)
            m_meta -> m_bits |= DockBit::AutoStart;
        return *this;
    }
    
private:    
    DockInfo*   m_meta;
    
};

