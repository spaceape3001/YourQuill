////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Delegate.hpp"


/*! \brief Delegate to show buttons, allow the user to "click" on them

    Buttons can have two types, ones that alternate, ones that show/hide.  
    If an "off" icon is specifed, then the former behavior is chosen,
    whereas a single icon implies the later.
    
    The value is bitwise unsigned integer, thereby *restricting*
    to 32 icons, shouldn't be a real problem for most anticipated usages.
    
    It's bitwise, btw
*/
class IconBarDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(IconBarDelegate, Delegate)
public:
    IconBarDelegate(QObject* parent=nullptr);
    ~IconBarDelegate();
    
    /*! Adds a panel.
    
        If off is specified, then it's assumed to be a togggle, and 
        the panel will always send a "clicked" notification.  If the off
        is omitted, then it's assumed to be a regular button, and
        clicked notifications will be restricted to "on-state" only.
    */
    uint8_t     addPanel(const QString& on, const QString& off=QString());
    QSize       sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const override;
    
    uint8_t     count() const;
    
protected:
    bool    editorEvent(QEvent*, QAbstractItemModel*, const QStyleOptionViewItem&, const QModelIndex&) override;
    bool    paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const override;

signals:
    //  User clicked on an icon.  
    void        clicked(const QModelIndex&, uint8_t, Qt::KeyboardModifiers);

private:
    struct Panel;
    
    Vector<Panel>   m_panels;
    QSize           m_size;
};
