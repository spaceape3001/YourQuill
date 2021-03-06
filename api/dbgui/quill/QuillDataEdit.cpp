////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "QuillDataEdit.hpp"

#include <db/QuillData.hpp>
#include <dbgui/edit/CopyrightEdit.hpp>

//#include "core/Set.hpp"

#include <gui/edit/DirBox.hpp>
//#include "gui/EnumDelegate.hpp"
#include <gui/edit/FileBox.hpp>
#include <gui/edit/LineEdit.hpp>
#include <gui/edit/SpinBox.hpp>
#include <gui/edit/UShortSetEdit.hpp>

//#include <QFormLayout>
//#include <QHBoxLayout>
//#include <QHeaderView>
//#include <QIcon>
//#include <QToolBar>
//#include <QVBoxLayout>

QuillDataEdit::QuillDataEdit(QWidget*parent) : FormWidget(parent),
    m_abbr(nullptr),
    m_author(nullptr),
    m_cache(nullptr),
    m_copyright(nullptr),
    m_ini(nullptr),
    m_localUser(nullptr),
    m_logDir(nullptr),
    m_home(nullptr),
    m_name(nullptr),
    m_port(nullptr),
    m_readTimeout(nullptr),
    m_tempDir(nullptr),
    m_readOnly(false)
{
    setWindowTitle(tr("Quill Data Edit"));
    m_abbr          = new LineEdit;
    m_abbr -> setPlaceholderText(tr("(abbr"));
    m_author        = new LineEdit;
    m_auxPorts      = new UShortSetEdit;
    m_cache         = new FileBox;
    m_copyright     = new CopyrightEdit;
    m_ini           = new FileBox;
    m_localUser     = new LineEdit;
    m_logDir        = new DirBox;
    m_home          = new LineEdit;
    m_name          = new LineEdit;
    m_name -> setPlaceholderText(tr("(name)"));
    m_port          = new SpinBox;
    m_port -> setRange(0,65535);
    m_readTimeout   = new SpinBox;
    m_readTimeout -> setRange(0,99999);
    m_tempDir       = new DirBox;
    
    addRow(tr("Project"), {m_abbr, m_name}, true);
    addRow(tr("Author"), m_author);
    addRow(tr("Legal"), m_copyright);
    addSeparator();
    
    addRow(tr("Cache<br><small>$TempDir/cache</small>"), m_cache);
    addRow(tr("Ini File<br><small>$TempDir/ini</small>"), m_ini);
    addRow(tr("Local User<br><small>Developer Use</small>"), m_localUser);
    addRow(tr("Log Directory<br><small>$TempDir/logs</small>"), m_logDir);
    addRow(tr("Port<br><small>Point your browser here</small>"), {m_port, m_auxPorts}, true);
    addRow(tr("Read Timeout"), m_readTimeout);
    addRow(tr("TempDir<br><small>(Default is under /tmp)"), m_tempDir);
}

QuillDataEdit::~QuillDataEdit()
{
}


void    QuillDataEdit::get(QuillData& v) const
{
    v.abbr          = m_abbr -> text();
    v.aux_ports     = m_auxPorts -> get();
    v.author        = m_author -> text();
    v.cache         = m_cache -> file();
    v.copyright     = m_copyright -> get();
    v.home          = m_home -> text();
    v.ini           = m_ini -> file();
    v.local_user    = m_localUser -> text();
    v.log_dir       = m_logDir -> dirPath();
    v.name          = m_name -> text();
    v.port          = m_port -> value();
    v.read_timeout  = m_readTimeout -> value();
    v.temp_dir      = m_tempDir -> dirPath();
}

void    QuillDataEdit::set(const QuillData& v)
{
    m_abbr          -> setText( v.abbr.qString() );
    m_author        -> setText( v.author.qString() );
    m_auxPorts      -> set(v.aux_ports);
    m_cache         -> setFile( v.cache.qString() );
    m_copyright     -> set( v.copyright );
    m_home          -> setText( v.home.qString() );
    m_ini           -> setFile( v.ini.qString() );
    m_localUser     -> setText( v.local_user.qString() );
    m_logDir        -> setDirPath( v.log_dir.qString() );
    m_name          -> setText( v.name.qString() );
    m_port          -> setValue( v.port );
    m_readTimeout   -> setValue( v.read_timeout );
    m_tempDir       -> setDirPath( v.temp_dir.qString() );
}

void    QuillDataEdit::clear()
{
    m_abbr -> clear();
    m_author -> clear();
    m_auxPorts -> clear();
    m_cache -> clear();
    m_copyright -> clear();
    m_home -> clear();
    m_ini -> clear();
    m_localUser -> clear();
    m_logDir    -> clear();
    m_name -> clear();
    m_port -> setValue(0);
    m_readTimeout -> setValue(0);
    m_tempDir -> clear();
}

void    QuillDataEdit::setReadOnly(bool f)
{
    m_abbr -> setReadOnly(f);
    m_author -> setReadOnly(f);
    m_auxPorts -> setReadOnly(f);
    m_cache -> setReadOnly(f);
    m_copyright -> setReadOnly(f);
    m_home -> setReadOnly(f);
    m_ini -> setReadOnly(f);
    m_localUser -> setReadOnly(f);
    m_logDir -> setReadOnly(f);
    m_name -> setReadOnly(f);
    m_port -> setReadOnly(f);
    m_readTimeout -> setReadOnly(f);
    m_tempDir -> setReadOnly(f);
}

#include "moc_QuillDataEdit.cpp"
