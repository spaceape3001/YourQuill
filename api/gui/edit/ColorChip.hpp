////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "gui/undo/UndoBase.hpp"
#include <QColor>
#include <QWidget>

class ColorChip : public QWidget, public UndoBase {
    Q_OBJECT
public:
    ColorChip(QWidget* parent=nullptr);
    ~ColorChip();

    void        setChooserEnable(bool);
    bool        chooserEnable() const { return m_chooser; }
    
    void        setAlphaEnable(bool);
    bool        alphaEnable() const { return m_alpha; }
    
    //  only valid when edit policy is Chooser
    bool        readOnly() const { return m_readOnly; }
    QColor      color() const { return m_color; }
public slots:
    void        setColor(const QColor&);
    void        setReadOnly(bool);
    void        executeChooser(QWidget*parent=nullptr);
    
signals:
    void        doubleClicked();
    void        colorChanged(const QColor&);

private slots:
    void        updateNotify(const QColor&);

protected:    
    
    virtual void paintEvent ( QPaintEvent * event );
    virtual void mouseDoubleClickEvent ( QMouseEvent * event );

private:
    QColor      m_color;
    bool        m_alpha;
    bool        m_chooser;
    bool        m_readOnly;
};


