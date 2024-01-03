////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ystudio.hpp"
#include "Window.hpp"

#include <0/basic/DelayInit.hpp>
#include <mithril/image/Image.hpp>

using yq::mithril::Image;
namespace yq::gluon {
    class GraphicsView;
}
using yq::gluon::GraphicsView;
class QGraphicsScene;
class QGraphicsSvgItem;


class SvgViewer : public Window {
    Q_OBJECT
public:

    SvgViewer(QWidget*parent=nullptr);
    ~SvgViewer();
    
    void    setImage(Image);
    void    setImage(const std::filesystem::path&);

private:

    QGraphicsScene*         m_scene     = nullptr;
    GraphicsView*           m_view      = nullptr;
    QGraphicsSvgItem*       m_item      = nullptr;
};
