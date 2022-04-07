////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/type/Ref.hpp>
#include <vector>
#include <string_view>

namespace yq {  
    class HttpDataStream;
    
    class HttpData {
        friend class HttpDataStream;
    public:
        static constexpr const uint8_t      kClean  = 12;   // means we'll round n to the nearest 2^kClean
        
        void                    advance(size_t n);
        
        size_t                  append(const char*, size_t);
        size_t                  append(const std::string_view&);
        
        std::string_view        as_view() const;
        std::string_view        as_view(uint32_t n) const;
        
        //! Number of free bytes available
        size_t                  available() const;
    
        //! Gets pointer to the data (immediately *AFTER* the data structure
        char*                   data() const { return (char*)(1 + &m_count); }
        
        //! Pointer to available (unfilled) space
        char*                   freespace() const { return data() + m_count; }
        
        //! Gets the data buffer size (whether it's filled or not)
        static uint32_t         size();
        
        //! Gets the count
        uint32_t                count() const { return m_count; }
        
        //! Sets the count
        void                    count(uint32_t);
        
        //! Detects emptiness
        bool                    empty() const { return !m_count; }
        
        bool                    full() const { return m_count >= size(); }
        
        //! Note, return size may be smaller if there's insufficient room!
        std::string_view        copy(const std::string_view&);
        
        /*!  \brief Starts/initializes the pool
         
              This initializes the pool to use the desired size for each data block.  
              (The ref-counted will be subtracted to keep the size to a rounded number.)
        */
        static bool             start_pool(uint32_t n=262144);
        static Ref<HttpData>    make();
    
    private:
        struct Pool;
        friend class Ref<HttpData>;
        
        static Pool&                    pool();
        mutable std::atomic<uint32_t>   m_ref;

            //  becuase of the frequency of use, we'll go with count
        uint32_t                        m_count       = 0;
        
        HttpData();
        ~HttpData();
        
        // disable copy & move
        HttpData(const HttpData&) = delete;
        HttpData(HttpData&&) = delete;
        HttpData& operator=(const HttpData&) = delete;
        HttpData& operator=(HttpData&&) = delete;
        
        
        void	incRef() const { ++m_ref; }
        void    decRef() const;
    };
    
    using HttpDataPtr       = Ref<HttpData>;
    
}

