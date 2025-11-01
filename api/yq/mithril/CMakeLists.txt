################################################################################
##
##  YOUR QUILL
##
################################################################################

project(yq_mithril)

add_library(${PROJECT_NAME} SHARED
    directories.cpp

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
    
    enum/enums.cpp
    
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
    
    game/Game.cpp
    game/GameArg.cpp
    game/GameCDB.cpp
    game/GameHtml.cpp
    game/GameJson.cpp
    game/GamePost.cpp
    game/GameProvider.cpp
    game/GameSearch.cpp
    
    graphviz/Graphviz.cpp
    graphviz/GraphvizArg.cpp
    graphviz/GraphvizBuilder.cpp
    graphviz/GraphvizCDB.cpp
    graphviz/GraphvizHtml.cpp
    graphviz/GraphvizJson.cpp
    graphviz/GraphvizPost.cpp
    graphviz/GraphvizProvider.cpp
    graphviz/GraphvizSearch.cpp
    
    group/Group.cpp
    group/GroupArg.cpp
    group/GroupCDB.cpp
    group/GroupHtml.cpp
    group/GroupJson.cpp
    group/GroupPost.cpp
    group/GroupProvider.cpp
    group/GroupSearch.cpp
    
    html/Html.cpp
    html/Post.cpp
    html/XEntry.cpp

    id/id.cpp
    id/id_auto.cpp
    
    image/Image.cpp
    image/ImageArg.cpp
    image/ImageCDB.cpp
    image/ImageHtml.cpp
    image/ImageJson.cpp
    image/ImagePost.cpp
    image/ImageProvider.cpp
    image/ImageSearch.cpp
    
    io/StdObject.cpp
    
    ipc/DirWatcher.cpp
    ipc/ipcSocket.cpp
    
    leaf/Leaf.cpp
    leaf/LeafArg.cpp
    leaf/LeafCDB.cpp
    leaf/LeafData.cpp
    leaf/LeafFile.cpp
    leaf/LeafHtml.cpp
    leaf/LeafJson.cpp
    leaf/LeafPost.cpp
    leaf/LeafProvider.cpp
    leaf/LeafSearch.cpp
    #leaf/LeafUpdate.cpp
    
    meta/Meta.cpp
    meta/MetaArgArg.cpp
    meta/MetaCDB.cpp
    meta/MetaHtml.cpp
    meta/MetaMethodArg.cpp
    meta/MetaObjectArg.cpp
    meta/MetaOperatorArg.cpp
    meta/MetaPropertyArg.cpp
    meta/MetaTypeArg.cpp
    
    novel/Novel.cpp
    novel/NovelArg.cpp
    novel/NovelCDB.cpp
    novel/NovelHtml.cpp
    novel/NovelJson.cpp
    novel/NovelPost.cpp
    novel/NovelProvider.cpp
    novel/NovelSearch.cpp
    
    notify/AtomNotifier.cpp
    notify/FileNotifier.cpp
    notify/FileWatch.cpp
    notify/FunctionalNotifier.cpp
    notify/FunctionalStage4.cpp
    notify/Stage2.cpp
    notify/Stage3.cpp
    notify/Stage4.cpp
    notify/Stage5.cpp
    
    organization/Organization.cpp
    organization/OrganizationArg.cpp
    organization/OrganizationCDB.cpp
    organization/OrganizationHtml.cpp
    organization/OrganizationJson.cpp
    organization/OrganizationPost.cpp
    organization/OrganizationProvider.cpp
    organization/OrganizationSearch.cpp
    
    place/Place.cpp
    place/PlaceArg.cpp
    place/PlaceCDB.cpp
    place/PlaceHtml.cpp
    place/PlaceJson.cpp
    place/PlacePost.cpp
    place/PlaceProvider.cpp
    place/PlaceSearch.cpp
    
    quill/QuillData.cpp
    quill/QuillFile.cpp
    
    root/Root.cpp
    root/RootArg.cpp
    root/RootCDB.cpp
    root/RootHtml.cpp
    root/RootJson.cpp
    root/RootPost.cpp
    root/RootProvider.cpp
    root/RootSearch.cpp
    
    rule/Action.cpp
    rule/Constraint.cpp
    rule/Rule.cpp
    rule/Trigger.cpp
    
    style/Style.cpp
    style/StyleArg.cpp
    style/StyleCDB.cpp
    style/StyleData.cpp
    style/StyleFile.cpp
    style/StyleHtml.cpp
    style/StyleJson.cpp
    style/StylePost.cpp
    style/StyleProvider.cpp
    style/StyleSearch.cpp
    
    tag/Tag.cpp
    tag/TagArg.cpp
    tag/TagCDB.cpp
    tag/TagData.cpp
    tag/TagFile.cpp
    tag/TagHtml.cpp
    tag/TagJson.cpp
    tag/TagPost.cpp
    tag/TagProvider.cpp
    tag/TagSearch.cpp
    
    thing/Thing.cpp
    thing/ThingArg.cpp
    thing/ThingCDB.cpp
    thing/ThingHtml.cpp
    thing/ThingJson.cpp
    thing/ThingPost.cpp
    thing/ThingProvider.cpp
    thing/ThingSearch.cpp
    
    user/User.cpp
    user/UserArg.cpp
    user/UserCDB.cpp
    user/UserData.cpp
    user/UserFile.cpp
    user/UserHtml.cpp
    user/UserJson.cpp
    user/UserPost.cpp
    user/UserProvider.cpp
    user/UserSearch.cpp
    
    util/Markdown.cpp
    
    value/Value.cpp
    value/ValueArg.cpp
    value/ValueCDB.cpp
    value/ValueData.cpp
    value/ValueFile.cpp
    value/ValueHtml.cpp
    value/ValueJson.cpp
    value/ValuePost.cpp
    value/ValueProvider.cpp
    value/ValueSearch.cpp
    
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
    
    wksp/CacheQuery.cpp
    wksp/CacheStatement.cpp
    wksp/Workspace.cpp

    #all_meta.cpp
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
        ${YQUILL_API_DIR}
    #PRIVATE
        #${ImageMagick_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        -DDEV_BUILD=1
        "-DYQ_BUILD_ROOT=\"${YQ_UNIVERSE_BUILD_ROOT}\"" 
        "-DYQ_SHARE_DIRS=\"${YQ_UNIVERSE_SHARE_DIR}\"" 
)

LinkTest(${PROJECT_NAME})

