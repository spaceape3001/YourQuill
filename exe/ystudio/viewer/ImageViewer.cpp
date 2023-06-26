////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ImageViewer.hpp"
#include "Command.hpp"
#include "DreamMW.hpp"

#include <basic/DelayInit.hpp>
#include <gluon/core/Logging.hpp>
#include <gluon/widget/GraphicsView.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;

ImageViewer::ImageViewer(QWidget*parent) : Window(parent)
{
    m_scene     = new QGraphicsScene(this);
    m_view      = new GraphicsView(m_scene);
    setWidget(m_view);
}

ImageViewer::~ImageViewer()
{
}


void    ImageViewer::setImage(Image img)
{
    setImage(cdb::path(cdb::fragment(img)));
}

void    ImageViewer::setImage(const std::filesystem::path&pth)
{
    QString fname   = QString::fromStdString(pth.string());
    QPixmap pix(fname);
    if(pix.isNull()){
        yError() << "Unable to load image file '" << pth << "'";
        return ;
    }

    QGraphicsPixmapItem*    gpi = m_scene->addPixmap(pix);
    
    if(!gpi){
        yError() << "Unable to create a pixmap item on the scene for '" << pth << "'";
        return ;
    }

    if(m_pixmap){
        m_scene -> removeItem(m_pixmap);
        delete m_pixmap;
    }
    
    m_pixmap        = gpi;
    QRectF  bounds = m_pixmap -> boundingRect().adjusted(-10,-10,-10,-10);
    m_scene -> setSceneRect(bounds);
    m_view -> fitToScene();
    
    setWindowTitle(fname);
}

namespace {

    bool    acceptImage(Image img)
    {
        return cdb::is_raster(img);
    }
    
    bool    acceptFragment(Fragment frag)
    {
        Image   img = cdb::image(frag);
        if(img)
            return acceptImage(img);
        return false;
    }
    
    bool    accept(Id i)
    {
        switch(i.type()){
        case Image::ID:
            return acceptImage(Image(i.id()));
        case Fragment::ID:
            return acceptFragment(Fragment(i.id()));
        default:
            return false;
        }
    }
    
    void    create(DreamMW* mw, Id i)
    {
        Image       img;
    
        switch(i.type()){
        case Image::ID:
            img     = Image(i.id());
            break;
        case Fragment::ID:
            img     = cdb::image(Fragment(i.id()));
            break;
        default:
            break;
        }
        
        if(!img)
            return ;

        ImageViewer*    iv  = new ImageViewer();
        iv -> setImage(img);
        mw -> addWindow(iv);
    }
    

    void    reg_imageViewer()
    {
        Command::reg("Image Viewer", &accept, &create).viewer<Image>().type<Fragment>();
    }

    YQ_INVOKE(reg_imageViewer();)
}
