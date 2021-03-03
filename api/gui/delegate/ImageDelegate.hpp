////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Delegate.hpp"

class QImage;

class ImageDelegate : public Delegate {
    Q_OBJECT
    MO_DECLARE(ImageDelegate, Delegate)
public:
    using DataType = QImage;
    ImageDelegate(QObject* parent=nullptr);
    virtual ~ImageDelegate();
    
    virtual bool        paint(QPainter*, const QStyleOptionViewItem& option, const QVariant&) const override;
    virtual QSize       sizeHint(const QStyleOptionViewItem &option,
                            const QModelIndex &index) const override;
};

