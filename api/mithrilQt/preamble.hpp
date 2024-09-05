////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/basic/Flags.hpp>

namespace yq::mithril {


    /*! Enumerated list of columns 
    */
    enum class Column {
        //! No column
        None = 0,
        //! Brief/Description
        Brief,
        //! Hidden
        Hidden,
        //! Id
        Id,
        //! Key
        Key,
        //! Name
        Name,
        //! Path
        Path,
        //! Is/Name of template
        Template,
        //! Title
        Title,
        //! Type of item
        Type,
        //! Value
        Value
    };
    
    enum class ColOpt : uint8_t {
        Set,
        Add,
        Icon
    };
    
    using ColOpts   = Flags<ColOpt>;
    
    struct ColumnSpec {
        Column  column;
        ColOpts options = {};
        
        constexpr ColumnSpec(Column c=Column::None, ColOpts opts={}) noexcept : column(c), options(opts) {}
    };
}
