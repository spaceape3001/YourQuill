////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Flags.hpp>

namespace yq::mithril {
    struct default_t {};
    static constexpr const default_t DEFAULT;


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
