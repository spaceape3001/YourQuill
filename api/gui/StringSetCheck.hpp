////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UndoBase.hpp"
#include "util/Map.hpp"
#include <QWidget>

class CheckBox;
class FlowLayout;

class StringSetCheck : public QWidget, public UndoBase {
    Q_OBJECT
public:
    
    StringSetCheck(QWidget* parent=nullptr);
    ~StringSetCheck();
    
    void                setItems(const StringSet&);
    void                setItems(const QStringSet&);
    void                setChecked(const StringSet&);
    void                setChecked(const QStringSet&);
    void                setFixed(const StringSet&);
    void                setFixed(const QStringSet&);
    QStringSet          checked(bool fIncFixed=false) const;

public slots:
    void                clear();
    void                setReadOnly(bool);
    
signals:
    void                changed();
    
private:
    FlowLayout*                     m_layout;
    Map<QString,CheckBox*,IgCase>   m_checks;
};
