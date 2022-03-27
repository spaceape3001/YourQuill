#pragma once

#include "Meta.hpp"

namespace yq {

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  META WRITER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////

        /*! \brief "Writes" to a meta
        
            This is the base for a writer to a meta.
        */
        class Meta::Writer {
        public:
        
            Writer&     alias(std::string_view zAlias);
            Writer&     description(std::string_view zDescription);
            Writer&     label(std::string_view zLabel);
            Writer&     tag(std::string_view zKey); // implies boolean TRUE
            Writer&     tag(std::string_view zKey, Variant&& value);
            Writer&     tag(std::string_view zKey, const Variant& value);
            Writer&     tls();  //!< Marks the TLS flag ... only meaningful on static variables, still
        
        protected:
                //  In proper usage, should never be null, however, guard!
            Meta* const m_meta = nullptr;
            
            Writer(Meta* myMeta) : m_meta(myMeta) 
            {
                assert(myMeta);
                assert(thread_safe_write());
            }
        };

}
