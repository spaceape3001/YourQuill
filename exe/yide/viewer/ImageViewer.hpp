////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ystudio.hpp"
#include "Window.hpp"

#include <yq/mithril/image/Image.hpp>
#include <yq/typedef/filesystem_path.hpp>

using yq::mithril::Image;
namespace yq::gluon {
    class GraphicsScene;
    class GraphicsView;
}
using yq::gluon::GraphicsScene;
using yq::gluon::GraphicsView;
class QGraphicsPixmapItem;


class ImageViewer : public Window {
    Q_OBJECT
public:

    ImageViewer(QWidget*parent=nullptr);
    ~ImageViewer();
    
    void    setImage(Image);
    void    setImage(const std::filesystem::path&);

private:

    GraphicsScene*          m_scene     = nullptr;
    GraphicsView*           m_view      = nullptr;
    QGraphicsPixmapItem*    m_pixmap    = nullptr;
};
