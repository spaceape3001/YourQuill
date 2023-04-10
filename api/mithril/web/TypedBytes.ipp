////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq::mithril {
    Ref<TypedBytes> TypedBytes::info(const std::filesystem::path& fp)
    {
        Ref<TypedBytes>     ret = new TypedBytes(fp);
        if(!ret -> do_info())
            return {};
        return ret;
    }

    Ref<TypedBytes> TypedBytes::load(const std::filesystem::path& fp)
    {
        Ref<TypedBytes>     ret = new TypedBytes(fp);
        if(!ret -> do_info())
            return {};
        if(!ret -> do_read())
            return {};
        return ret;
    }

    //  ------------------------------------------------

    bool    TypedBytes::do_info()
    {
        status  = HttpStatus(); // reset the status
        auto ext    = file_extension(path.native());
        if(!ext.empty())
            type    = mimeTypeForExt(ext);
        else
            type    = ContentType();
        
        struct stat buf;
        if(::stat(path.c_str(), &buf) != 0){
            status  = HttpStatus::NotFound;
            return false;
        }

        size        = buf.st_size;
        struct tm   ftime;
        gmtime_r(&buf.st_mtim.tv_sec, &ftime);
        strftime(modified, sizeof(modified), "%a, %d %b %Y %H:%M:%S GMT", &ftime);
        status = HttpStatus::Success;
        return true;
    }

    
    bool    TypedBytes::do_read()
    {
        if(path.empty() || !size){
            status  = HttpStatus::NotFound;
            return false;
        }
        
            // reset for read
        data    = std::make_shared<ByteArray>();
        status  = HttpStatus(); 
        
        int fd   = open(path.c_str(), O_RDONLY);
        if(fd == -1){
            status  = HttpStatus::NotFound;
            return false;
        }
        
        auto sc = safety([fd]() { close(fd); }); // auto-close if necessary
        
        
        ByteArray&ba    = *data;
        ba.resize(size);
        
        size_t          amt_read    = 0;
        
        while(amt_read < size){
            size_t      n   = read(fd, ba.data()+amt_read, size-amt_read);
            if(n <= 0)
                break;
            amt_read += n;
        }
        
        if(amt_read < size){
            status = HttpStatus::InternalError;
        } else {
            status = HttpStatus::Success;
        }
        return true;
    }
}
