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

    mithrilQt.cpp

    atom/AtomList.hpp
    atom/AtomTable.hpp

    book/BookList.hpp
    book/BookTable.hpp
    
    category/CategoryList.hpp
    category/CategoryTable.hpp
    
    character/CharacterList.hpp
    character/CharacterTable.hpp
    
    class/ClassList.hpp
    class/ClassTable.hpp
    
    directory/DirectoryList.hpp
    directory/DirectoryTable.hpp
    
    document/DocumentList.hpp
    document/DocumentTable.hpp
    
    event/EventList.hpp
    event/EventTable.hpp
    
    field/FieldList.hpp
    field/FieldTable.hpp
    
    folder/FolderList.hpp
    folder/FolderTable.hpp
    
    fragment/FragmentList.hpp
    fragment/FragmentTable.hpp
    
    game/GameList.hpp
    game/GameTable.hpp

    id/IdList.hpp
    id/IdListView.hpp
    id/IdMimeList.hpp
    id/IdModel.hpp
    id/IdSignalMapper.hpp
    id/IdTable.hpp
    id/IdTableView.hpp
    id/IdTreeView.hpp
    
    leaf/LeafList.hpp
    leaf/LeafTable.hpp
    
    place/PlaceList.hpp
    place/PlaceTable.hpp
    
    root/RootList.hpp
    root/RootTable.hpp
    
    tag/TagList.hpp
    tag/TagTable.hpp
    
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

