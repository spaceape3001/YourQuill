////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <gui/widget/FormWidget.hpp>


class CopyrightEdit;
class DirBox;
class FileBox;
class LineEdit;
class SpinBox;
class UShortSetEdit;

class QuillData;

class QuillDataEdit : public FormWidget {
    Q_OBJECT
public:
    QuillDataEdit(QWidget*parent=nullptr);
    ~QuillDataEdit();
    
    void    get(QuillData&) const;
    void    set(const QuillData&);
    
    bool            readOnly() const { return m_readOnly; }


public slots:
    void        clear();
    void        setReadOnly(bool);
private:

    LineEdit*       m_abbr;
    LineEdit*       m_author;
    UShortSetEdit*  m_auxPorts;
    FileBox*        m_cache;
    CopyrightEdit*  m_copyright;
    FileBox*        m_ini;
    LineEdit*       m_localUser;
    DirBox*         m_logDir;
    LineEdit*       m_home;
    LineEdit*       m_name;
    SpinBox*        m_port;
    SpinBox*        m_readTimeout;
    DirBox*         m_tempDir;
    bool            m_readOnly;
};
