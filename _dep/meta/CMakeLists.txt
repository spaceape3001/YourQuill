set(meta_sources
    ./Global.hpp
    ./GlobalImpl.hpp
    ./InfoWriterImpl.hpp
    ./Meta.cpp
    ./MetaObject.hpp
    ./MetaType.hpp
    ./MetaValue.hpp
    ./Object.cpp
    ./Object.hpp
    ./StdTypes.cpp
    ./Variant.cpp
    ./Variant.hpp
)

add_library(yqmeta SHARED ${meta_sources})
target_compile_definitions(yqmeta PRIVATE YQ__CORE__OVERRIDE)
target_link_libraries(yqmeta PUBLIC yqutil)

