////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "imaging.hpp"

#include <basic/CmdArgs.hpp>
#include <basic/Logging.hpp>

#include <yq/db/IDLock.hpp>
#include <yq/db/SQ.hpp>
#include <yq/file/Document.hpp>
#include <yq/file/DocumentCDB.hpp>
#include <yq/file/Fragment.hpp>
#include <yq/file/FragmentCDB.hpp>
#include <yq/image/Image.hpp>
#include <yq/image/ImageCDB.hpp>
#include <yq/shape/Size2Ops.hpp>

#include <MagickCore/magick-baseconfig.h>
//  Know this is obsolete, however, there's no good way to automate it otherwise w/o tolerating the warning
#define MAGICKCORE_QUANTUM_DEPTH MAGICKCORE_QUANTUM_DEPTH_OBSOLETE_IN_H
#define MAGICKCORE_HDRI_ENABLE MAGICKCORE_HDRI_ENABLE_OBSOLETE_IN_H
#include <Magick++.h>

using namespace yq;
using namespace yq::cdb;

Size2<unsigned>    size_for(const Magick::Geometry& sz)
{
    return { (unsigned) sz.width(), (unsigned) sz.height() };
}

void    init_magicka()
{
    Magick::InitializeMagick( CmdArgs::appName().data());
}

void    update_image(Image img, cdb_options_t opts)
{
    if(!img)
        return;
        
    Magick::Blob            bS, bM, bL;
    Fragment                frag = fragment(img);
    
    static thread_local SQ u("UPDATE Images SET small=?,medium=?,large=? WHERE id=?");

    if(is_raster(img)){
        std::filesystem::path   fp  = path(frag);
    
        Fragment::Lock lk;
        if(!(opts&DONT_LOCK)){
            lk = Fragment::Lock::read(frag);
            if(!lk){
                yWarning() << "Unable to get fragment lock for " << fp;
                return;
            }
        }
            
        try {
            Magick::Image theImage( fp.string());
            
            std::string magickString    = theImage.magick();    // needed for saving
 
            Size2<unsigned> isize   = size_for(theImage.size());
            Size2<unsigned> ssize   = shrink_to_fit_within(isize, Image::kSmall);
            Size2<unsigned> msize   = shrink_to_fit_within(isize, Image::kMedium);
            Size2<unsigned> lsize   = shrink_to_fit_within(isize, Image::kLarge);
            
            if(ssize != isize){
                Magick::Image   imgSmall    = theImage;
                imgSmall.resize(Magick::Geometry(ssize.x, ssize.y));
                imgSmall.write(&bS);
            } else
                theImage.write(&bS);

            if(msize != isize){
                Magick::Image   imgMedium   = theImage;
                imgMedium.resize(Magick::Geometry(msize.x, msize.y));
                imgMedium.write(&bM);
            } else
                theImage.write(&bM);
            
            if(lsize != isize){
                Magick::Image   imgLarge    = theImage;
                imgLarge.resize(Magick::Geometry(lsize.x, lsize.y));
                imgLarge.write(&bL);
            } else
                theImage.write(&bL);
        }
        catch( Magick::Exception& error_)
        {
            yError()  << "Caught exception: " << error_.what();
        }
        
        if(bS.length() != 0)
            u.bind(1, bS.data(), bS.length());
        else
            u.bind(1);
        if(bM.length() != 0)
            u.bind(2, bM.data(), bM.length());
        else
            u.bind(2);
        if(bL.length() != 0)
            u.bind(3, bL.data(), bL.length());
        else
            u.bind(3);
    } else {
        u.bind(1);
        u.bind(2);
        u.bind(3);
    }
    
    u.bind(4, img.id);
    u.exec();
    u.reset();
}
