////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "SvgViewer.hpp"
#include "Command.hpp"
#include "DreamMW.hpp"

#include <yq/core/DelayInit.hpp>
#include <gluon/core/Logging.hpp>
#include <gluon/graphics/GraphicsScene.hpp>
#include <gluon/graphics/GraphicsView.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>

#include <QGraphicsSvgItem>

using namespace yq;
using namespace yq::gluon;
using namespace yq::mithril;

SvgViewer::SvgViewer(QWidget*parent) : Window(parent)
{
    m_scene     = new GraphicsScene(this);
    m_view      = new GraphicsView(m_scene);
    m_view -> setMouseWheelZoomModifiers(Qt::ControlModifier);
    m_view -> featureEnable(GraphicsView::Feature_MouseWheelZoom);
    setWidget(m_view);
}

SvgViewer::~SvgViewer()
{
}

void    SvgViewer::setImage(Image img)
{
    setImage(cdb::path(cdb::fragment(img)));
}

void    SvgViewer::setImage(const std::filesystem::path&pth)
{
    QString fname   = QString::fromStdString(pth.string());
    
    QGraphicsSvgItem*       svg = new QGraphicsSvgItem(fname);
    svg -> setFlags(QGraphicsItem::ItemClipsToShape);
    svg -> setCacheMode(QGraphicsItem::NoCache);
    svg -> setZValue(0);
    
    if(m_item){
        m_scene -> removeItem(m_item);
        delete m_item;
    }
    
    m_item          = svg;
    m_scene -> addItem(m_item);
    
    QRectF  bounds = m_item -> boundingRect().adjusted(-10,-10,-10,-10);
    m_scene -> setSceneRect(bounds);
    m_view -> fitToScene();
    
    setWindowTitle(fname);
}

namespace {

    bool    acceptImage(Image img)
    {
        return cdb::mime_type(img) == ContentType::svg;
    }
    
    bool    acceptFragment(Fragment frag)
    {
        Image   img = cdb::image(frag);
        if(img)
            return acceptImage(img);
        return false;
    }
    
    bool    acceptDocument(Document doc)
    {
        return acceptFragment(cdb::fragment(doc));
    }
    
    bool    accept(Id i)
    {
        switch(i.type()){
        case Image::ID:
            return acceptImage(Image(i.id()));
        case Fragment::ID:
            return acceptFragment(Fragment(i.id()));
        case Document::ID:
            return acceptDocument(Document(i.id()));
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
        case Document::ID:
            img     = cdb::image(cdb::fragment(Document(i.id())));
            break;
        default:
            break;
        }
        
        if(!img)
            return ;

        SvgViewer*    iv  = new SvgViewer();
        iv -> setImage(img);
        mw -> addWindow(iv);
    }
    

    void    reg_imageViewer()
    {
        Command::reg("Svg Viewer", &accept, &create).viewer<Image>().type<Fragment>().type<Document>();
    }

    YQ_INVOKE(reg_imageViewer();)
}
