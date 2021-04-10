////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

/*! \macro YQ_DBGREL
    \brief Simple macro for debug vs release
    \param[in]  Debug argument
    \param[in]  Release argument
*/

#ifdef _DEBUG
    #define YQ_DBGREL(a,b) a
#else
    #define YQ_DBGREL(a,b) b
#endif

