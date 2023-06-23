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

class DreamMW;
class QAction;

class DockInfo : public yq::ObjectInfo {
public:

    template <typename C> class Writer;
    DockInfo(std::string_view, const ObjectInfo&, const std::source_location& sl = std::source_location::current());

    static const std::vector<const DockInfo*>&  all();

    Qt::DockWidgetArea  startArea() const { return m_startArea; }
    Qt::DockWidgetAreas allowedAreas() const { return m_allowed; }
    
    bool        isAutoStart() const { return m_bits[DockBit::AutoStart]; }
    bool        isCreatable() const { return m_bits[DockBit::Creatable]; }
    
    const QString&      label() const { return m_label; }
    const QIcon&        icon() const { return m_icon; }
    const QKeySequence& shortcut() const { return m_shortcut; }
    const QString&      toolTip() const { return m_toolTip; }
    
private:
    QString                 m_label;        // for the action
    QString                 m_toolTip;      // for the action
    QIcon                   m_icon;         // for the action
    QKeySequence            m_shortcut;     // for the action
    yq::Flags<DockBit>      m_bits;
    Qt::DockWidgetArea      m_startArea = Qt::LeftDockWidgetArea;
    Qt::DockWidgetAreas     m_allowed   = Qt::AllDockWidgetAreas;
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
    
protected:
    void        closeEvent(QCloseEvent*) override;
    void        showEvent(QShowEvent*) override;
    void        hideEvent(QHideEvent*) override;
    
private slots:
    void        triggered(bool);
private:
    friend class DreamMW;
    QAction*    m_action    = nullptr;
    
    void    _update();
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

    Writer& area(Qt::DockWidgetArea dwa)
    {
        if(m_meta)
            m_meta -> m_startArea   = dwa;
        return *this;
    }
    
    
    Writer& autoStart()
    {
        if(m_meta)
            m_meta -> m_bits |= DockBit::AutoStart;
        return *this;
    }

    Writer& icon(const QIcon& ico)
    {
        if(m_meta)
            m_meta -> m_icon    = ico;
        return *this;
    }
    
    Writer& label(const QString& s)
    {
        if(m_meta)
            m_meta -> m_label = s;
        return *this;
    }
    
    Writer& shortcut(const QKeySequence& ks)
    {
        if(m_meta)
            m_meta -> m_shortcut = ks;
        return *this;
    }
    
    Writer& toolTip(const QString& s)
    {
        if(m_meta)
            m_meta -> m_toolTip = s;
        return *this;
    }
    
private:    
    DockInfo*   m_meta;
};

