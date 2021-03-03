////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IconBarDelegate.hpp"
#include "DelegateImpl.hpp"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>


namespace {
    static const unsigned int kMaxPanel      = 32;
}

struct IconBarDelegate::Panel {
    QImage      off, on;
    double      left, right;
    int         x, width;
    uint8_t     index;
    
    Panel() : left(0.), right(0.), x(0), width(0), index(0) {}
};

IconBarDelegate::IconBarDelegate(QObject* parent) : Delegate(parent), m_size(0,0)
{
    
}

IconBarDelegate::~IconBarDelegate()
{
}


uint8_t     IconBarDelegate::addPanel(const QString& on, const QString& off)
{
    if(m_panels.size() >= kMaxPanel){
        qWarning() << "This panel is FULL!";
        return 0xFF;
    }
    if(on.isEmpty()){
        qWarning() << "No 'on' panel specified!";
        return 0xFF;
    }
    
    Panel       p;
    p.on            = QImage(on);
    if(p.on.isNull()){
        qWarning() << "Unable to load image:" << on;
        return 0xFF;
    }
    if(!off.isEmpty()){
        p.off       = QImage(off);
        if(p.off.isNull()){
            qWarning() << "Unable to load image:" << off;
            return 0xFF;
        }
    }
    
    p.x         = m_size.width();
    p.width     = std::max(p.on.width(), p.off.width());
    int     mh  = std::max(p.on.height(), p.off.height());
    p.index     = (uint8_t) m_panels.size();
    m_panels << p;
    int     mw  = m_size.width() + p.width;
    m_size      = QSize( mw, std::max(mh, m_size.height()));
    
    //  Readjust the panels....
    for(Panel& pc : m_panels){
        pc.left     = pc.x / (double) mw;
        pc.right    = (pc.x + pc.width) / (double) mw;
    }
    
    return p.index;
}

uint8_t     IconBarDelegate::count() const
{
    return (uint8_t) m_panels.size();
}


bool    IconBarDelegate::editorEvent(QEvent*evt, QAbstractItemModel*model, const QStyleOptionViewItem&style, const QModelIndex&index)  
{
    if(evt->type() == QEvent::MouseButtonRelease){
        QMouseEvent* mouse = static_cast<QMouseEvent*>(evt);
        if(mouse->button() == Qt::LeftButton){
            unsigned int    flag    = index.data(Qt::DisplayRole).toUInt();
            int dx = mouse->pos().x() - style.rect.left();
            int dw = style.rect.right() - style.rect.left();
            double f = dx / (double) dw;
            for(const Panel& p : m_panels){
                if(f >= p.left && f < p.right){
                    if(!p.off.isNull() || (flag & (1 << p.index)))
                        emit clicked(index, p.index, mouse->modifiers());
                    break;
                }
            }
            return true;
        }
    } 
    
    return Delegate::editorEvent(evt, model, style, index);
}

bool    IconBarDelegate::paint(QPainter*painter, const QStyleOptionViewItem&style, const QVariant&value) const 
{
    unsigned int flag = value.toUInt();
    int y0  = style.rect.top();
    int dh  = style.rect.bottom() - y0;
    int x0  = style.rect.left();
    int dw  = style.rect.right() - x0;
    
    for(const Panel& p : m_panels){
        int left   = (int)(x0 + dw*p.left + 0.5);
        int right  = (int)(x0 + dw*p.right + 0.5);
        QRect rect(left, y0, right-left, dh);
        if(flag & (1 << p.index)){
            painter->drawImage(rect, p.on);
        } else if(p.off.isNull()){
            painter->fillRect(rect, style.backgroundBrush);
        } else
            painter->drawImage(rect, p.off);
    }
    return true;
}

QSize       IconBarDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const 
{
    return m_size;
}

MO_CONCRETE(IconBarDelegate)

#include "moc_IconBarDelegate.cpp"

