////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "yq/util/DelayInit.hpp"
#include "MetaInfo.hpp"
#include <type_traits>

template <typename T>
class MetaInfoWriter : public T, public DelayInit {
    static_assert(std::is_base_of_v<MetaInfo, T>>, "T must DERIVE from MetaInfo!");
public:
    
    static T&               info();

protected:    
    void                    init_me(T&);  // the "W" is for those needing intellisense (or similar), it's "*this"
    virtual void            initialize()
    {
        init_me(*this);
    }
    
private:
    template <typename ... Args>
    MetaInfoWriter(Args&&... args) : T(std::foward<Args>(args)...) {}
    MetaInfoWriter(const MetaInfoWriter&) = delete;
    MetaInfoWriter(MetaInfoWriter&&) = delete;
    MetaInfoWriter& operator=(const MetaInfoWriter&) = delete;
    MetaInfoWriter& operator=(MetaInfoWriter&) = delete;
    ~MetaInfoWriter() = delete;

    static T&               s_make;
};

/*! \brief Base macro for implementationn
*/
#define YQ_INFO_WRITER_IMPLEMENT(IW, ...)                                           \
    template <> IW&                 MetaInfoWriter<IW>::info()                          \
    {                                                                               \
        static MetaInfoWriter<IW>*      ret = new MetaInfoWriter<IW>(__VA_ARGS__);          \
        return *ret;                                                                \
    }                                                                               \
    template <> IW&                 MetaInfoWriter::s_make  = info();                   \
    template <> void                MetaInfoWriter<T>::init_me(MetaInfoWriter<T>&w)
