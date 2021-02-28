////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <QWidget>
#include "UndoBase.hpp"
#include <optional>

class LineEdit;
class QToolButton;


class DirBox : public QWidget, public UndoBase {
    Q_OBJECT
public:
    DirBox(QWidget*parent=nullptr);
    virtual ~DirBox();
    
    void                        set(const std::optional<QString>&);
    std::optional<QString>      get() const;

    void    setDirPath(const QString&);
    QString dirPath() const;
    void    setTitle(const QString&);
    
    //  eventually, get UNDO working....
public slots:
    void        clear();
    void        setReadOnly(bool);
signals:
    void        dirPathChanged(const QString&);    
private slots:
    void        browse();
private:
    QString         m_title;
    LineEdit*       m_edit;
    QToolButton*    m_button;
};
