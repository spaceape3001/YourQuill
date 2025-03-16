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
    
    category/Category.cpp
    category/CategoryArg.cpp
    category/CategoryCDB.cpp
    category/CategoryData.cpp
    category/CategoryFile.cpp
    category/CategoryHtml.cpp
    category/CategoryJson.cpp
    category/CategoryPost.cpp
    category/CategoryProvider.cpp
    category/CategorySearch.cpp
    
    character/Character.cpp
    character/CharacterArg.cpp
    character/CharacterCDB.cpp
    character/CharacterHtml.cpp
    character/CharacterJson.cpp
    character/CharacterPost.cpp
    character/CharacterProvider.cpp
    character/CharacterSearch.cpp
    
    class/Class.cpp
    class/ClassArg.cpp
    class/ClassCDB.cpp
    class/ClassData.cpp
    class/ClassFile.cpp
    class/ClassHtml.cpp
    class/ClassJson.cpp
    class/ClassPost.cpp
    class/ClassProvider.cpp
    class/ClassSearch.cpp
    #class/ClassUpdate.cpp
    
    db/DBUtils.cpp
    
    directory/Directory.cpp
    directory/DirectoryArg.cpp
    directory/DirectoryCDB.cpp
    directory/DirectoryHtml.cpp
    directory/DirectoryJson.cpp
    directory/DirectoryPost.cpp
    directory/DirectoryProvider.cpp
    directory/DirectorySearch.cpp
    
    document/Document.cpp
    document/DocumentArg.cpp
    document/DocumentCDB.cpp
    document/DocumentHtml.cpp
    document/DocumentJson.cpp
    document/DocumentPost.cpp
    document/DocumentProvider.cpp
    document/DocumentSearch.cpp
    #document/DocumentUpdate.cpp
    
    doodle/Doodle.cpp
    doodle/DoodleArg.cpp
    doodle/DoodleCDB.cpp
    doodle/DoodleHtml.cpp
    doodle/DoodleJson.cpp
    doodle/DoodlePost.cpp
    doodle/DoodleProvider.cpp
    doodle/DoodleSearch.cpp
    
    entity/Entity.cpp
    entity/EntityArg.cpp
    entity/EntityCDB.cpp
    entity/EntityHtml.cpp
    entity/EntityJson.cpp
    entity/EntityPost.cpp
    entity/EntityProvider.cpp
    entity/EntitySearch.cpp
    
    event/Event.cpp
    event/EventArg.cpp
    event/EventCDB.cpp
    event/EventHtml.cpp
    event/EventJson.cpp
    event/EventPost.cpp
    event/EventProvider.cpp
    event/EventSearch.cpp
    
    field/Field.cpp
    field/FieldArg.cpp
    field/FieldCDB.cpp
    field/FieldData.cpp
    field/FieldFile.cpp
    field/FieldHtml.cpp
    field/FieldJson.cpp
    field/FieldPost.cpp
    field/FieldProvider.cpp
    field/FieldSearch.cpp
    #field/FieldUpdate.cpp
    
    file/FileSpec.cpp
    
    folder/Folder.cpp
    folder/FolderArg.cpp
    folder/FolderCDB.cpp
    folder/FolderHtml.cpp
    folder/FolderJson.cpp
    folder/FolderPost.cpp
    folder/FolderProvider.cpp
    folder/FolderSearch.cpp
    
    fragment/Fragment.cpp
    fragment/FragmentArg.cpp
    fragment/FragmentCDB.cpp
    fragment/FragmentHtml.cpp
    fragment/FragmentJson.cpp
    fragment/FragmentPost.cpp
    fragment/FragmentProvider.cpp
    fragment/FragmentSearch.cpp
    
    id/id.cpp
    id/id_auto.cpp
    
    io/StdObject.cpp
    
    ipc/DirWatcher.cpp
    ipc/ipcSocket.cpp
    
    
    util/Markdown.cpp
    
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

