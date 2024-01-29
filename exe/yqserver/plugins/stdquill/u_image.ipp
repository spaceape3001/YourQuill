////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    Size2U    size_for(const Magick::Geometry& sz)
    {
        return { (unsigned) sz.width(), (unsigned) sz.height() };
    }
    
    Size2U    u_thumbnail(Image x)
    {
        if(!x)
            return ZERO;
        if(!cdb::is_raster(x))
            return ZERO;
        
        Magick::Blob            bS, bM, bL;
        Size2U                  isize;
        
        static thread_local CacheQuery u("UPDATE Images SET small=?,medium=?,large=?,width=?,height=? WHERE id=?");
        auto u_af = u.af();
        
        Fragment                frag    = cdb::fragment(x);
        std::filesystem::path   fp  = cdb::path(frag);
        cdb::update(frag);
        
        try {
            Magick::Image theImage( fp.string());
            
            std::string magickString    = theImage.magick();    // needed for saving

            isize          = size_for(theImage.size());
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
            return {};
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
        u.bind(4, (int) isize.x);
        u.bind(5, (int) isize.y);
        u.bind(6, x.id);
        u.exec();

        return isize;
    }
    
    void    u_image(Image v, Change chg)
    {
        static thread_local CacheQuery xImage[] = {
            CacheQuery("UPDATE Atoms SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Categories SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Classes SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Documents SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Fields SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Leafs SET icon=0 WHERE icon=?"),
            CacheQuery("UPDATE Tags SET icon=0 WHERE icon=?"),
            CacheQuery("DELETE FROM Images WHERE id=?")
        };
    
        Image::Diff     x{.x = v, .chg = chg};
        
        if(chg != Change::Added){
            Image::Info     ii  = cdb::info(v);
            x.type              = ii.type;
            x.dim.from          = ii.dim;
        }
        
        if(chg != Change::Removed)
            x.dim.to            = u_thumbnail(v);

        Image::Notify::notify(x);
    
        if(chg == Change::Removed){
            for(auto& sq : xImage)
                sq.exec(v.id);
        }
    }
    

    void    s2_image()
    {
        Magick::InitializeMagick( BasicApp::app_name().data());
    }
    
    void    s3_image(Document doc)
    {
        for(Fragment frag : cdb::fragments(doc))
            u_image(cdb::db_image(frag), Change::Added);
    }

    void    s5_image(Fragment frag, Change chg)
    {
        Image   v;
        switch(chg){
        case Change::Added:
        case Change::Modified:
            u_image(cdb::db_image(frag), chg);
            break;
        case Change::Removed:
            v   = cdb::image(frag);
            if(v)
                u_image(v, chg);
            break;
        default:
            break;
        }
    }
}
