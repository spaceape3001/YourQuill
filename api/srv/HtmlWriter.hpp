////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "NetWriter.hpp"

#include <db/Cache.hpp>
#include <db/enum/DataRole.hpp>
#include <db/enum/SubmitLabel.hpp>

#include <util/Array2.hpp>
#include <util/Comma.hpp>
#include <util/Set.hpp>
#include <util/Vector.hpp>


//#include "db/Cache.hpp"
//#include "db/Enums.hpp"
//#include "util/Enum.hpp"
//#include "util/List.hpp"


//class QDate;
//class QDateTime;
//class QDir;
//class QUrl;
//class QVariant;
//class Page;
//class Class;
//class Graph;

class Root;


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



namespace html {
    struct Bold {
        QString txt;
        Bold(const QString& _txt) : txt(_txt) {}
    };
    
    template <typename T>
    struct Edit {
        T           value;
        const Root* root;
        bool        inspect;    // this is an override
        Edit(const T& v=T(), bool i=false) : value(v), root(nullptr), inspect(i) {}
        Edit(const Root*r, const T& v=T(), bool i=false) : value(v), root(r), inspect(i) {}
    };
    
    struct Escape {
        QString txt;
        Escape(const QString& _txt) : txt(_txt) {}
    };
    
    struct Form {
        QString url, args;
        bool    inspect;    // this is an override
    };
    
    struct H1 {
        QString txt;
        H1(const QString& _txt) : txt(_txt) {}
    };
    
    struct H2 {
        QString txt;
        H2(const QString& _txt) : txt(_txt) {}
    };

    struct H3 {
        QString txt;
        H3(const QString& _txt) : txt(_txt) {}
    };

    struct H4 {
        QString txt;
        H4(const QString& _txt) : txt(_txt) {}
    };

    struct H5 {
        QString txt;
        H5(const QString& _txt) : txt(_txt) {}
    };

    struct H6 {
        QString txt;
        H6(const QString& _txt) : txt(_txt) {}
    };

    template <typename T>
    struct Icon {
        T       src;
        QString alt;
        Icon(){}
        Icon(const T& v, const QString& a=QString()) : src(v), alt(a) {}
    };
    
    template <typename T>
    Icon<T> icon_for(const T& v, const QString& a=QString())
    {
        return Icon<T>(v, a);
    }
    
    inline Icon<QString> icon_for(const char* z, const QString& a=QString())
    {
        return Icon<QString>(z, a);
    }

    struct Image {
        QString     src;
        QString     alt;
        QString     style;
        
        Image(const QString& _url, const QString& _alt=QString(), const QString& sty=QString()) :
            src(_url), alt(_alt), style(sty) {}
    };
    
    
    struct Italic {
        QString txt;
        Italic(const QString& _txt) : txt(_txt) {}
    };
    
    struct Underline {
        QString txt;
        Underline(const QString& _txt) : txt(_txt) {}
    };
    
    struct Mark {
        QByteArray  bytes;
        Mark(const QByteArray& _b) : bytes(_b) {}
        Mark(QByteArray&& _b) : bytes(std::move(_b)) {}
    };
    
    struct Pre {
        QByteArray  bytes;
        Pre(const QByteArray& _b) : bytes(_b) {}
        Pre(QByteArray&& _b) : bytes(std::move(_b)) {}
    };
    
    template <typename T>
    struct Config {
        T       v;
    };
    
    template <typename T> 
    Config<T>   config(T v) { return Config<T>{v}; }
    
    
    template <typename T>
    struct Full {
        T       v;
    };
    
    
    //struct Form {
        //QString url, args;
        //Form(const QString& u, const QString& a=QString()) : url(u), args(a) {}
    //};
    
    template <typename T>
    Full<T>     full(T v) { return Full<T>{v}; }
    
    template <typename T>
    struct Dev {
        T       v;
    };
    
    template <typename T>
    Dev<T>     dev(T v) { return Dev<T>{v}; }
    
    
    struct IRoot { 
        DataRole dr; 
    };
    
    
    struct IKey {};
    struct IEditReq {};
    struct ISubmit 
    { 
        SubmitLabel   submit; 
        //SubmitLabel   reset;
    };
    
    struct ICheck {
        QString     key, label;
        bool        checked;
        ICheck(const QString&k, const QString&l, bool c) : key(k), label(l), checked(c) {}
    };
    
    struct IHidden {
        QString     key, value;
        IHidden(const QString&k, const QString& v) : key(k), value(v) {}
        IHidden(const QString&k, uint64_t v) : key(k), value(QString::number((quint64) v)){}
    };
    
    struct ILabel {
        QString     key, label;
        ILabel(const QString& k, const QString&l) : key(k), label(l) {}
    };
    
    struct ILine {
        QString key, value;
        int     size;
        ILine(const QString& k, const QString& v=QString(), int z=-1) : key(k), value(v), size(z) {}
    };
    
    struct IText {
        QString key, value;
        int     rows, cols;
        IText(const QString&k, const QString& v=QString(), int r=10, int c=80) : key(k), value(v), rows(r), cols(c) {}
    };
    
    template <typename T>
    struct Link {
        QString url;
        T   value;
        Link(const QString& u, const T& v) : url(u), value(v) {}
    };
    
    template <typename T>
    Link<T> link(const QString& u, const T& v)
    {
        return Link<T>(u, v);
    }
    
    struct ControlRoot { };
    
    class TabBar;
    
}


class HtmlWriter : public NetWriter {
public:

    HtmlWriter();
    ~HtmlWriter();

    class Table;
    class KVRow;
    class FormAF;
    class LinkAF;

    Table       table();
    KVRow       key(const QString&, const QString& url=QString());
    
    template <typename T, typename P>
    void        table(const Array2<T>&, P);

    template <typename T>
    FormAF      form(const html::Edit<T>&);
    FormAF      form(const QString& url, const QString& args=QString(), bool fInspectOverride=false);
    LinkAF      link(const QString&);

    using NetWriter::write;

    void        write(const Atom&);
    void        write(const Class&);
    void        write(const Comma&);
    void        write(const Field&);
    void        write(const Folder&);
    void        write(const Graph&);

    void        write(const html::Bold&);
    void        write(const html::ControlRoot&);
    void        write(const html::Edit<Class>&);
    void        write(const html::Escape&);
    void        write(const html::Form&);
    void        write(const html::H1&);
    void        write(const html::H2&);
    void        write(const html::H3&);
    void        write(const html::H4&);
    void        write(const html::H5&);
    void        write(const html::H6&);
    void        write(const html::Mark&);
    void        write(const html::Underline&);
    
    void        write(const html::Config<Class>&);
    
    void        write(const html::Dev<Atom>&);
    void        write(const html::Dev<Class>&);
    void        write(const html::Dev<Directory>&);
    void        write(const html::Dev<Document>&);
    void        write(const html::Dev<Folder>&);
    void        write(const html::Dev<Fragment>&);
    void        write(const html::Dev<Leaf>&);
    void        write(const html::Dev<const Root*>&);
    void        write(const html::Dev<Tag>&);
    void        write(const html::Full<Folder>&);
    void        write(const html::ICheck&);
    void        write(const html::IEditReq&);
    void        write(const html::IHidden&);
    void        write(const html::IKey&);
    void        write(const html::ILabel&);
    void        write(const html::ILine&);
    void        write(const html::IRoot&);
    void        write(const html::ISubmit&);
    void        write(const html::IText&);
    void        write(const html::Icon<Document>&);
    void        write(const html::Icon<Folder>&);
    void        write(const html::Icon<QString>&);
    void        write(const html::Image&);
    void        write(const html::Italic&);
    template <typename T>
    void        write(const html::Link<T>&);
    void        write(const html::Pre&);
    void        write(const html::TabBar&);
    
    void        write(const Leaf&);
    
    void        write(SubmitLabel);
    void        write(SubmitLabel::enum_t);
    
    template <typename T>
    void    write(const Vector<T>& values, const QString& sep = ", ")
    {
        Comma   comma(sep);
        for(const T& v : values){
            write(comma.text());
            write(v);
            ++comma;
        }
    }
    
    template <typename T>
    void    write(const html::Config<Vector<T>>& values, const QString& sep=", ")
    {
        Comma comma(sep);
        for(const T& t : values.v){
            write(comma.text());
            write(html::Config<T>(t));
            ++comma;
        }
        
    }
    
    template <typename T, typename C>
    void    write(const html::Config<Set<T,C>>& values, const QString& sep=", ")
    {
        Comma comma(sep);
        for(const T& t : values.v){
            write(comma.text());
            write(html::Config<T>(t));
            ++comma;
        }
    }

    template <typename T>
    HtmlWriter& operator<<(const T& v)
    {
        write(v);
        return *this;
    }

};


class HtmlWriter::FormAF {
public:
    FormAF(FormAF&&);
    FormAF& operator=(FormAF&&);
    ~FormAF();
    
    template <typename T>
    FormAF& operator<<(const T&data)
    {
        if(m_html)
            m_html -> write(data);
        return *this;
    }
    
    FormAF() : m_html(nullptr) {}
    
private:
    friend class HtmlWriter;
    FormAF(HtmlWriter* v) : m_html(v) {}
    HtmlWriter*     m_html;
};

class HtmlWriter::LinkAF {
public:
    LinkAF(LinkAF&&);
    LinkAF& operator=(LinkAF&&);
    ~LinkAF();
    
    template <typename T>
    LinkAF& operator<<(const T&data)
    {
        if(m_html)
            m_html -> write(data);
        return *this;
    }
    
    LinkAF() : m_html(nullptr) {}
    
private:
    friend class HtmlWriter;
    LinkAF(HtmlWriter* v) : m_html(v) {}
    HtmlWriter*     m_html;
};

class HtmlWriter::Table {
public:
    Table(Table&&);
    Table& operator=(Table&&);
    ~Table();
    
    template <typename T>
    Table& operator<<(const T&data)
    {
        if(m_html)
            m_html -> write(data);
        return *this;
    }
    
    KVRow       key(const QString&, const QString& url=QString());
    
    Table() : m_html(nullptr) {}
    
private:
    friend class HtmlWriter;
    Table(HtmlWriter* v) : m_html(v) {}
    HtmlWriter*     m_html;
};


class HtmlWriter::KVRow {
public:
    KVRow(KVRow&&);
    KVRow& operator=(KVRow&&);
    ~KVRow();
    
    template <typename T>
    KVRow& operator<<(const T& data)
    {
        if(m_html)
            m_html -> write(data);
        return *this;
    }
    
    KVRow() : m_html(nullptr) {}
private:
    friend class HtmlWriter;
    KVRow(HtmlWriter* v) : m_html(v) {}
    HtmlWriter*     m_html;
};

template <typename T>
HtmlWriter::FormAF     HtmlWriter::form(const html::Edit<T>& e)
{
    write(e);
    return FormAF(this);
}

template <typename T>
void        HtmlWriter::write(const html::Link<T>& v)
{
    auto    af = link(v.url);
    write(v.value);
}

template <typename T, typename P>
void        HtmlWriter::table(const Array2<T>&data, P pred)
{
    write("<table>\n");
    for(int i : data.rows()){
        write("<tr>");
        for(int j : data.columns()){
            write("<td>");
            pred(data(i,j));
            write("</td>");
        }
        write("</tr>\n");
    }
    write("</table>\n");
}

