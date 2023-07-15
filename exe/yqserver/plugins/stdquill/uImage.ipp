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

#include <basic/BasicApp.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <math/shape/Size2.hxx>

using namespace yq;
using namespace yq::mithril;


namespace {
    void    u_image_thumbnails(Image img, cdb_options_t opts=0);

    yq::Size2U    size_for(const Magick::Geometry& sz)
    {
        return { (unsigned) sz.width(), (unsigned) sz.height() };
    }


    void    u_image_erase(Image x)
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
        
        if(!x)
            return ;
            
        for(auto& sq : stmts)
            sq.exec(x.id);
    }

    void    u_image_erase(Fragment frag)
    {
        u_image_erase(cdb::image(frag));
    }

    void    u_image_notify(Fragment frag, Change chg)
    {
        switch(chg){
        case Change::Added:
        case Change::Modified:
            u_image_thumbnails(cdb::db_image(frag));
            break;
        case Change::Removed:
            u_image_erase(frag);
            break;
        default:
            break;
        }
    }

    void    u_image_stage2()
    {
        Magick::InitializeMagick( BasicApp::app_name().data());
    }

    void    u_image_stage3(Document doc)
    {
        for(Fragment frag : cdb::fragments(doc))
            u_image_thumbnails(cdb::db_image(frag), cdb::DONT_LOCK|cdb::IS_UPDATE);
    }

    void    u_image_thumbnails(Image img, cdb_options_t opts)
    {
        if(!img)
            return;
        if(!cdb::is_raster(img))
            return ;

        Magick::Blob            bS, bM, bL;
        Fragment                frag = cdb::fragment(img);
        
        static thread_local CacheQuery u("UPDATE Images SET small=?,medium=?,large=? WHERE id=?");
        auto u_af = u.af();

        std::filesystem::path   fp  = cdb::path(frag);

        Id::Lock lk;
        if(!(opts&cdb::DONT_LOCK)){
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
        
        u.bind(4, img.id);
        u.exec();
    }
}
