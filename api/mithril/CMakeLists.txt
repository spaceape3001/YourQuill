################################################################################
##
##  YOUR QUILL
##
################################################################################

project(yq_mithril)

add_library(${PROJECT_NAME} SHARED
    atom/Atom.cpp
    atom/AtomArg.cpp
    atom/AtomCDB.cpp
    atom/AtomHtml.cpp
    atom/AtomJson.cpp
    atom/AtomPost.cpp
    atom/AtomProvider.cpp
    atom/AtomSearch.cpp
    atom/AtomSpec.cpp
    #atom/AtomUpdate.cpp
    
    attribute/Attribute.cpp
    attribute/AttributeArg.cpp
    attribute/AttributeCDB.cpp
    attribute/AttributeDiff.cpp
    attribute/AttributeHtml.cpp
    attribute/AttributeJson.cpp
    attribute/AttributeProvider.cpp
    attribute/AttributeSearch.cpp
    
    auth/Authentication.cpp
    auth/AutoAccept.cpp
    auth/AutoReject.cpp
    auth/Simple.cpp
    
    bit/Context.cpp
    bit/Copyright.cpp
    bit/Policy.cpp
    
    book/Book.cpp
    book/BookArg.cpp
    book/BookCDB.cpp
    book/BookHtml.cpp
    book/BookJson.cpp
    book/BookPost.cpp
    book/BookProvider.cpp
    book/BookSearch.cpp
    
    db/DBUtils.cpp
    
    file/FileSpec.cpp
    
    id/id.cpp
    id/id_auto.cpp
    
    io/StdObject.cpp
    
    ipc/DirWatcher.cpp
    ipc/ipcSocket.cpp
    
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
    web/web_unity.cpp
    
    wksp/CacheQuery.hpp
    wksp/CacheStatement.hpp
    wksp/Workspace.cpp

    all_arg.cpp
    all_cdb.cpp
    all_html.cpp
    all_json.cpp
    all_meta.cpp
    all_provider.cpp
    all_search.cpp
    directories.cpp
    doodle.cpp
    mithril.cpp
    notify.cpp
    html/XEntry.cpp
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

