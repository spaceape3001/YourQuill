if(NOT TARGET asio)
    set(asio_dir ${CMAKE_CURRENT_LIST_DIR}/asio/asio)
    add_library(asio INTERFACE)
    target_sources(asio INTERFACE
        ${asio_dir}/include/asio.hpp
    )
    target_include_directories(asio INTERFACE ${asio_dir}/include)
endif()

