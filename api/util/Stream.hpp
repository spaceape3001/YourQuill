#pragma once

//! Abstract baseis for a stream, yes it'll use virtuals
class Stream {
public:
    virtual ~Stream(){}
    
    /*! \brief Writes data
    
        \param[in] z    Pointer to character buffer
        \param[in] cb   Number of bytes to write
        \return TRUE if successful, FALSE otherwise
    */
    virtual bool    write(const char*z, size_t cb) = 0;
    
    /*! \brief Flush any and all buffers to destination
    
        This forces the stream to flush any buffers.
        For streams that wrap other streams, flush
        your buffer, then call flush on the sub-stream.
    */
    virtual void    flush() {}
};

