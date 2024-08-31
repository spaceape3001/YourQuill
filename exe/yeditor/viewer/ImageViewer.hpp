////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ystudio.hpp"
#include "Window.hpp"

#include <mithril/image/Image.hpp>

using yq::mithril::Image;
namespace yq::gluon {
    class GraphicsView;
}
using yq::gluon::GraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;


class ImageViewer : public Window {
    Q_OBJECT
public:

    ImageViewer(QWidget*parent=nullptr);
    ~ImageViewer();
    
    void    setImage(Image);
    void    setImage(const std::filesystem::path&);

private:

    QGraphicsScene*         m_scene     = nullptr;
    GraphicsView*           m_view      = nullptr;
    QGraphicsPixmapItem*    m_pixmap    = nullptr;
};
