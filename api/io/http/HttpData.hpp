////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <util/type/Ref.hpp>
#include <vector>

namespace yq {  
    class HttpData {
    public:
        static constexpr const uint8_t      kClean  = 12;   // means we'll round n to the nearest 2^kClean
    

        char*                   data() const { return (char*)(1 + &m_count); }
        static uint32_t         size();
        
        /*!  \brief Starts/initializes the pool
         
              This initializes the pool to use the desired size for each data block.  
              (The ref-counted will be subtracted to keep the size to a rounded number.)
        */
        static bool             start_pool(uint32_t n=1048576);
        static Ref<HttpData>    make();
    
    private:
        struct Pool;
        friend class Ref<HttpData>;
        
        static Pool&        pool();
        mutable std::atomic<uint16_t> m_count;
        
        HttpData();
        ~HttpData();
        
        void	incRef() const { ++m_count; }
        void    decRef() const;
    };
    
    using HttpDataPtr       = Ref<HttpData>;
    
}

