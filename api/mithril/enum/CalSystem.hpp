////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/InfoBinder.hpp>
#include <0/basic/Enum.hpp>

namespace yq {

    namespace mithril {
        /*! 
            \enum CalSystem
            \brief Calendar System
            
            \var CalSystem::Gregorian
            \brief  This is the Gregorian calendar
            
                \note This system will auto-populate the "fields" on run-time.

            \var CalSystem::Julian
            \brief  This is the Julian calendar
            
                \note This system will auto-populate the "fields" on run-time.

            \var CalSystem::DateZero
            \brief  New system based on Gregorian
            
                This is a calendar system that utilizes the Gregorian calendar, 
                in general, but assigns a custom date of "zero" and runs the years
                relative to that date.
                

                \note This system will auto-populate the "fields" on run-time.

            \var CalSystem::Custom
            \brief  You define it!
            
                Whatever calendar the user wants, they can specify the fields, including
                min/max values, relative sizes (if important)

        */
        YQ_ENUM(CalSystem, ,
            Gregorian,
            Julian,
            DateZero,
            Custom
        )



    }
}

YQ_TYPE_DECLARE(yq::mithril::CalSystem)
