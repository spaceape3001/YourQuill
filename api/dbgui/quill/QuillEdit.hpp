////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QTabWidget>

class QuillDataEdit;
class QuillDirTable;

class QuillData;

class QuillEdit : public QTabWidget {
    Q_OBJECT
public:
    QuillEdit(QWidget*parent=nullptr);
    ~QuillEdit();

    void            set(const QuillData&);
    void            get(QuillData&) const;
    QStringSet      templates() const;

signals:
    void            cmdAddRootDir();
    void            cmdAddTemplate();
    
public slots:
    void            addRoot(const QString&);
    void            addTemplate(const QString&);
    void            clear();
    
private:
    QuillDataEdit*  m_data;
    QuillDirTable*  m_roots;
    QuillDirTable*  m_templates;
};
