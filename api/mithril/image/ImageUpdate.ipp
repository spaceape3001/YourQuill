////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <MagickCore/magick-baseconfig.h>
//  Know this is obsolete, however, there's no good way to automate it otherwise w/o tolerating the warning
#define MAGICKCORE_QUANTUM_DEPTH MAGICKCORE_QUANTUM_DEPTH_OBSOLETE_IN_H
#define MAGICKCORE_HDRI_ENABLE MAGICKCORE_HDRI_ENABLE_OBSOLETE_IN_H
#include <Magick++.h>

#include <mithril/image/ImageUpdate.hpp>

#include <0/basic/BasicApp.hpp>
#include <0/basic/Logging.hpp>

#include <mithril/document/DocumentCDB.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>

#include <0/math/shape/Size2.hxx>


namespace yq::mithril::update {
    Size2U    size_for(const Magick::Geometry& sz)
    {
        return { (unsigned) sz.width(), (unsigned) sz.height() };
    }

    UImage   UImage::get(Image img)
    {
        return UImage(img);
    }
    
    UImage   UImage::get(Fragment frag)
    {
        return UImage(cdb::image(frag));
    }

    void     UImage::notify(Fragment frag,Change chg)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UImage::notify('" << cdb::path(frag) << "', " << chg.key() << ")";
        #endif

        switch(chg){
        case Change::Added:
        case Change::Modified:
            get(frag).u_thumbnail({F::LOCK});
            break;
        case Change::Removed:
            get(cdb::image(frag)).x_erase();
            break;
        default:
            break;
        }
    }

    void     UImage::s2()
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UImage::s2()";
        #endif

        Magick::InitializeMagick( BasicApp::app_name().data());
    }
    
    void     UImage::s3(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "UImage::s3('" << cdb::key(doc) << "')";
        #endif

        for(Fragment frag : cdb::fragments(doc))
            get(cdb::db_image(frag)).u_thumbnail();
    }
    

    ////////////////////////////////////////////////////////////////////////////

    UImage::UImage(Image img) : U<Image>(img, {}), frag(img.id)
    {
    }

    void    UImage::u_thumbnail(FF flags)
    {
        if(!x)
            return;
        if(!cdb::is_raster(x))
            return ;

        Magick::Blob            bS, bM, bL;
        
        static thread_local CacheQuery u("UPDATE Images SET small=?,medium=?,large=? WHERE id=?");
        auto u_af = u.af();

        std::filesystem::path   fp  = cdb::path(frag);
        cdb::update(frag);

        Id::Lock lk;
        if(flags[F::LOCK]){
            lk = Id(frag).lock(false);
            if(!lk){
                yWarning() << "Unable to get fragment lock for " << fp;
                return;
            }
        }
            
        try {
            Magick::Image theImage( fp.string());
            
            std::string magickString    = theImage.magick();    // needed for saving

            Size2U isize   = size_for(theImage.size());
            Size2U ssize   = shrink_to_fit_within(isize, Image::kSmall);
            Size2U msize   = shrink_to_fit_within(isize, Image::kMedium);
            Size2U lsize   = shrink_to_fit_within(isize, Image::kLarge);
            
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
        
        u.bind(4, id);
        u.exec();
    }
    
    void    UImage::x_erase()
    {
        if(!x)
            return ;
            
        static thread_local CacheQuery stmts[] = {
            CacheQuery("UPDATE Atoms SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Categories SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Classes SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Documents SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Fields SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Leafs SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Tags SET icon=0 WHERE icon=?"),
            CacheQuery("DELETE FROM Images WHERE id=?")
        };
            
        for(auto& sq : stmts)
            sq.exec(id);
    }
}
