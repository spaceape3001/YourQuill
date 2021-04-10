////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


DateTimeDelegate::DateTimeDelegate(QObject*parent) : Delegate(parent)
{
}

DateTimeDelegate::~DateTimeDelegate()
{
}
    
Compare     DateTimeDelegate::compare(const QVariant&va, const QVariant&vb) const
{
    return ::compare(va.toDateTime(), vb.toDateTime());
}

QWidget*    DateTimeDelegate::createEditor(QWidget* parent) const
{
    return new DateTimeWidget(parent);
}

bool        DateTimeDelegate::setEditorData(QWidget*editor, const QVariant&val) const
{
    static_cast<DateTimeWidget*>(editor)->setDateTime(val.toDateTime());
    return true;
}

QVariant    DateTimeDelegate::getEditorData(const QWidget*editor) const
{
    return static_cast<const DateTimeWidget*>(editor)->dateTime();
}

void        DateTimeDelegate::connectEditedSignal(QWidget*editor, QObject*obj, const char*slot) const
{
    connect(editor, SIGNAL(editingFinished()), obj, slot);
}

QVariant    DateTimeDelegate::render(const QVariant&input) const 
{
    QDateTime d =  input.value<QDateTime>();
    if(d.isNull())
        return QVariant();
    return d.toString(Qt::ISODate);
}


