if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
    set(engine_test_source ./test.cpp)
endif()

set(yq_engine_sources
    ${engine_test_source}
    
    ./asset/Asset.cpp
    ./asset/Asset.hpp
    ./asset/AssetCache.hpp
    ./asset/AssetCompiler.hpp
    ./asset/AssetLoader.hpp
    ./asset/AssetInfoWriter.hpp
    
    ./shader/Shader.cpp
    ./shader/Shader.hpp
    ./shader/ShaderCache.hpp
    ./shader/ShaderCompiler.hpp
    ./shader/ShaderLoader.hpp
    ./shader/ShaderInfoWriter.hpp
    
    ./texture/Texture.cpp
    ./texture/Texture.hpp
    ./texture/TextureCache.hpp
    ./texture/TextureLoader.hpp
    ./texture/TextureInfoWriter.hpp
    
    ./vulqan/VqCore.cpp
    ./vulqan/VqCore.hpp
    ./vulqan/VqEnums.cpp
    ./vulqan/VqGLFW.cpp
    ./vulqan/VqGLFW.hpp
    ./vulqan/VqInstance.cpp
    ./vulqan/VqInstance.hpp
    ./vulqan/VqMonitor.cpp
    ./vulqan/VqMonitor.hpp
    ./vulqan/VqWindow.cpp
    ./vulqan/VqWindow.hpp
)

add_library(yqengine SHARED ${yq_engine_sources})
target_link_libraries(yqengine PUBLIC yqbasic yqconfig ${Vulkan_LIBRARIES} ${GLFW3_LIBRARY})
LinkTest(yqengine)


