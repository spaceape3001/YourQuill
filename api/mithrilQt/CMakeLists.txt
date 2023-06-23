################################################################################
##
##  YOUR QUILL
##
################################################################################

project(mithrilQt)


find_package(Qt6 COMPONENTS Core Gui Widgets)
find_package(TBB REQUIRED)
#find_package(Qt5 COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_library(${PROJECT_NAME} SHARED
    
    atom/AtomColumn.hpp
    atom/AtomColumn.ipp
    atom/AtomList.hpp
    atom/AtomList.ipp
    atom/AtomModel.hpp
    atom/AtomModel.ipp
    atom/AtomTable.hpp
    atom/AtomTable.ipp
    atom/atom.cpp

    book/BookColumn.hpp
    book/BookColumn.ipp
    book/BookList.hpp
    book/BookList.ipp
    book/BookModel.hpp
    book/BookModel.ipp
    book/BookTable.hpp
    book/BookTable.ipp
    book/book.cpp
    
    category/CategoryColumn.hpp
    category/CategoryColumn.ipp
    category/CategoryList.hpp
    category/CategoryList.ipp
    category/CategoryModel.hpp
    category/CategoryModel.ipp
    category/CategoryTable.hpp
    category/CategoryTable.ipp
    category/category.cpp
    
    character/CharacterColumn.hpp
    character/CharacterColumn.ipp
    character/CharacterList.hpp
    character/CharacterList.ipp
    character/CharacterModel.hpp
    character/CharacterModel.ipp
    character/CharacterTable.hpp
    character/CharacterTable.ipp
    character/character.cpp
    
    class/ClassColumn.hpp
    class/ClassColumn.ipp
    class/ClassList.hpp
    class/ClassList.ipp
    class/ClassModel.hpp
    class/ClassModel.ipp
    class/ClassTable.hpp
    class/ClassTable.ipp
    class/class.cpp
    
    directory/DirectoryColumn.hpp
    directory/DirectoryColumn.ipp
    directory/DirectoryList.hpp
    directory/DirectoryList.ipp
    directory/DirectoryModel.hpp
    directory/DirectoryModel.ipp
    directory/DirectoryTable.hpp
    directory/DirectoryTable.ipp
    directory/directory.cpp
    
    document/DocumentColumn.hpp
    document/DocumentColumn.ipp
    document/DocumentList.hpp
    document/DocumentList.ipp
    document/DocumentModel.hpp
    document/DocumentModel.ipp
    document/DocumentTable.hpp
    document/DocumentTable.ipp
    document/document.cpp
    
    event/EventColumn.hpp
    event/EventColumn.ipp
    event/EventList.hpp
    event/EventList.ipp
    event/EventModel.hpp
    event/EventModel.ipp
    event/EventTable.hpp
    event/EventTable.ipp
    event/event.cpp
    
    field/FieldColumn.hpp
    field/FieldColumn.ipp
    field/FieldList.hpp
    field/FieldList.ipp
    field/FieldModel.hpp
    field/FieldModel.ipp
    field/FieldTable.hpp
    field/FieldTable.ipp
    field/field.cpp
    
    folder/FolderColumn.hpp
    folder/FolderColumn.ipp
    folder/FolderList.hpp
    folder/FolderList.ipp
    folder/FolderModel.hpp
    folder/FolderModel.ipp
    folder/FolderTable.hpp
    folder/FolderTable.ipp
    folder/folder.cpp
    
    fragment/FragmentColumn.hpp
    fragment/FragmentColumn.ipp
    fragment/FragmentList.hpp
    fragment/FragmentList.ipp
    fragment/FragmentModel.hpp
    fragment/FragmentModel.ipp
    fragment/FragmentTable.hpp
    fragment/FragmentTable.ipp
    fragment/fragment.cpp
    
    game/GameColumn.hpp
    game/GameColumn.ipp
    game/GameList.hpp
    game/GameList.ipp
    game/GameModel.hpp
    game/GameModel.ipp
    game/GameTable.hpp
    game/GameTable.ipp
    game/game.cpp

    id/IdColumn.hpp
    id/IdColumn.ipp
    id/IdList.hpp
    id/IdList.ipp
    id/IdListView.hpp
    id/IdListView.ipp
    id/IdModel.hpp
    id/IdModel.ipp
    id/IdTable.hpp
    id/IdTable.ipp
    id/IdTableView.hpp
    id/IdTableView.ipp
    id/IdTreeView.hpp
    id/IdTreeView.ipp
    id/id.cpp
    
    image/ImageUtils.hpp
    image/ImageUtils.ipp
    image/image.cpp
    
    leaf/LeafColumn.hpp
    leaf/LeafColumn.ipp
    leaf/LeafList.hpp
    leaf/LeafList.ipp
    leaf/LeafModel.hpp
    leaf/LeafModel.ipp
    leaf/LeafTable.hpp
    leaf/LeafTable.ipp
    leaf/leaf.cpp
    
    place/PlaceColumn.hpp
    place/PlaceColumn.ipp
    place/PlaceList.hpp
    place/PlaceList.ipp
    place/PlaceModel.hpp
    place/PlaceModel.ipp
    place/PlaceTable.hpp
    place/PlaceTable.ipp
    place/place.cpp
    
    root/RootColumn.hpp
    root/RootColumn.ipp
    root/RootList.hpp
    root/RootList.ipp
    root/RootModel.hpp
    root/RootModel.ipp
    root/RootTable.hpp
    root/RootTable.ipp
    root/root.cpp
    
    tag/TagColumn.hpp
    tag/TagColumn.ipp
    tag/TagList.hpp
    tag/TagList.ipp
    tag/TagModel.hpp
    tag/TagModel.ipp
    tag/TagTable.hpp
    tag/TagTable.ipp
    tag/tag.cpp
    
    mithrilQt.qrc
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        yqtoolbox
        mithril
        gluon
        
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets

    PRIVATE
        ${LOG4CPP_LIBRARIES} 
        ${TBB_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/..
    PRIVATE
        ${TBB_INCLUDE_DIR}
        ${LOG4CPP_INCLUDE_DIR}
)

LinkTest(${PROJECT_NAME})

