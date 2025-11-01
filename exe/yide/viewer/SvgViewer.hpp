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
using yq::gluon::GraphicsView;
using yq::gluon::GraphicsScene;
class QGraphicsSvgItem;


class SvgViewer : public Window {
    Q_OBJECT
public:

    SvgViewer(QWidget*parent=nullptr);
    ~SvgViewer();
    
    void    setImage(Image);
    void    setImage(const std::filesystem::path&);

private:

    GraphicsScene*          m_scene     = nullptr;
    GraphicsView*           m_view      = nullptr;
    QGraphicsSvgItem*       m_item      = nullptr;
};
