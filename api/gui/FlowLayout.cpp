
#include "FlowLayout.hpp"

#include <QWidgetItem>
#include <QLayoutItem>
#include <QSize>
#include <QWidget>



     /****************************************************************************
     **
     ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
     ** All rights reserved.
     ** Contact: Nokia Corporation (qt-info@nokia.com)
     **
     ** This file is part of the examples of the Qt Toolkit.
     **
     ** $QT_BEGIN_LICENSE:LGPL$
     ** Commercial Usage
     ** Licensees holding valid Qt Commercial licenses may use this file in
     ** accordance with the Qt Commercial License Agreement provided with the
     ** Software or, alternatively, in accordance with the terms contained in
     ** a written agreement between you and Nokia.
     **
     ** GNU Lesser General Public License Usage
     ** Alternatively, this file may be used under the terms of the GNU Lesser
     ** General Public License version 2.1 as published by the Free Software
     ** Foundation and appearing in the file LICENSE.LGPL included in the
     ** packaging of this file.  Please review the following information to
     ** ensure the GNU Lesser General Public License version 2.1 requirements
     ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
     **
     ** In addition, as a special exception, Nokia gives you certain additional
     ** rights.  These rights are described in the Nokia Qt LGPL Exception
     ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
     **
     ** GNU General Public License Usage
     ** Alternatively, this file may be used under the terms of the GNU
     ** General Public License version 3.0 as published by the Free Software
     ** Foundation and appearing in the file LICENSE.GPL included in the
     ** packaging of this file.  Please review the following information to
     ** ensure the GNU General Public License version 3.0 requirements will be
     ** met: http://www.gnu.org/copyleft/gpl.html.
     **
     ** If you have questions regarding the use of this file, please contact
     ** Nokia at qt-info@nokia.com.
     ** $QT_END_LICENSE$
     **
     ****************************************************************************/

FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
    deleteAll();
}

void FlowLayout::addItem(QLayoutItem *item)
{
    m_items << item;
}

void FlowLayout::clear()
{
    deleteAll();
    invalidate();
}

int FlowLayout::count() const
{
    return m_items.size();
}

void FlowLayout::deleteAll()
{
    Vector<QLayoutItem*>    items;
    std::swap(m_items, items);
    for(QLayoutItem* item : items)
        delete item;
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int left, top, right, bottom;

    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    for(QLayoutItem* item : m_items ){
        QWidget *wid = item->widget();
        
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
            spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
            
        int spaceY = verticalSpacing();
        if (spaceY == -1)
            spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return Qt::Orientations();
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}


int FlowLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0) {
        return m_hSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int          FlowLayout::indexOf(QWidget* widget) const 
{
    for(size_t i =0;i<m_items.size(); ++i)
        if(m_items[i] && m_items[i]->widget() == widget)
            return (int) i;
    return -1;
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    if(index < 0)
        return nullptr;
    return m_items.value(index, nullptr);
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    for(QLayoutItem *item : m_items)
        size = size.expandedTo(item->minimumSize());

    size += QSize(2*margin(), 2*margin());
    return size;
}

void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    } else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    if(index < 0)
        return nullptr;
    return m_items.take((size_t) index);
}

int FlowLayout::verticalSpacing() const
{
    if (m_vSpace >= 0) {
        return m_vSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "moc_FlowLayout.cpp"
