////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <util/text/String.hpp>

namespace yq {

    /*! \brief Sequence generator for commas
    
        Used when you need commas.
    */
    class Comma {
    public:
        Comma(const String&sep=", ");
        
        
        /*! \brief Text
        
            On first call, this will be blank.  On subsequent calls, it'll have the separator 
        */
        const String&       text() const;
        
        //! text() but for Qt....
        QString             qText() const;
        
        //! Advances the sequence
        Comma&              operator++();
        
    private:
        String      m_separator;
        bool        m_first;
    };
}
