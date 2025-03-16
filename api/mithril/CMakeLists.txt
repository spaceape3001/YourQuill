################################################################################
##
##  YOUR QUILL
##
################################################################################

project(yq_mithril)

add_library(${PROJECT_NAME} SHARED
    atom/AtomArg.cpp
    atom/AtomCDB.cpp
    atom/AtomHtml.cpp
    atom/AtomJson.cpp
    atom/AtomPost.cpp
    atom/AtomSearch.cpp
    atom/AtomSpec.cpp
    #atom/AtomUpdate.cpp
    
    file/FileSpec.cpp
    
    web/PageTemplate.cpp
    web/Template.cpp
    web/TypedBytes.cpp
    web/VarTemplate.cpp
    web/WebAdapters.cpp
    web/WebAutoClose.cpp
    web/WebContext.cpp
    web/WebHtml.cpp
    web/WebImage.cpp
    web/WebPage.cpp
    web/WebRedirect.cpp
    web/WebRepo.cpp
    web/WebTemplate.cpp
    web/WebVariable.cpp

    all_arg.cpp
    all_cdb.cpp
    all_html.cpp
    all_ipc.cpp
    all_json.cpp
    all_meta.cpp
    all_provider.cpp
    all_search.cpp
    bit.cpp
    directories.cpp
    doodle.cpp
    mithril.cpp
    notify.cpp
    html/XEntry.cpp
    web/web_unity.cpp
    wksp/Workspace.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yq_core
        yq_keyv
        yq_net
        yq_process
        yq_sql
        yq_xml
        
    PRIVATE
        yq_asio
        #${ImageMagick_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/..
    #PRIVATE
        #${ImageMagick_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        -DDEV_BUILD=1
        "-DYQ_BUILD_ROOT=\"${CMAKE_SOURCE_DIR}\"" 
        "-DYQ_SHARE_DIRS=\"${CMAKE_SOURCE_DIR}/share\"" 
)

LinkTest(${PROJECT_NAME})

