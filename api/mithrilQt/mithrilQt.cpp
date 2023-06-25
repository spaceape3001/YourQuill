////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <tbb/spin_rw_mutex.h>  // avoids emit issues
#include <concepts>

#include "atom.ipp"
#include "atom/AtomList.ipp"
#include "atom/AtomTable.ipp"

#include "attribute.ipp"

#include "book.ipp"
#include "book/BookList.ipp"
#include "book/BookTable.ipp"

#include "category.ipp"
#include "category/CategoryList.ipp"
#include "category/CategoryTable.ipp"

#include "character.ipp"
#include "character/CharacterList.ipp"
#include "character/CharacterTable.ipp"

#include "class.ipp"
#include "class/ClassList.ipp"
#include "class/ClassTable.ipp"

#include "directory.ipp"
#include "directory/DirectoryList.ipp"
#include "directory/DirectoryTable.ipp"

#include "document.ipp"
#include "document/DocumentList.ipp"
#include "document/DocumentTable.ipp"

#include "entity.ipp"

#include "event.ipp"
#include "event/EventList.ipp"
#include "event/EventTable.ipp"

#include "field.ipp"
#include "field/FieldList.ipp"
#include "field/FieldTable.ipp"

#include "folder.ipp"
#include "folder/FolderList.ipp"
#include "folder/FolderTable.ipp"


#include "fragment.ipp"
#include "fragment/FragmentList.ipp"
#include "fragment/FragmentTable.ipp"

#include "game.ipp"
#include "game/GameList.ipp"
#include "game/GameTable.ipp"

#include "graph.ipp"
#include "group.ipp"

#include "id.ipp"


#include "id/IdColumn.ipp"
// #include "id/IdFunctor.ipp" // TODO
#include "id/IdList.ipp"
#include "id/IdListView.ipp"
#include "id/IdMimeList.ipp"
#include "id/IdModel.ipp"
#include "id/IdSignalMapper.ipp"
#include "id/IdTable.ipp"
#include "id/IdTableView.ipp"
#include "id/IdTreeView.ipp"

#include "image.ipp"

#include "leaf.ipp"
#include "leaf/LeafList.ipp"
#include "leaf/LeafTable.ipp"

#include "organization.ipp"

#include "place.ipp"
#include "place/PlaceList.ipp"
#include "place/PlaceTable.ipp"

#include "root.ipp"
#include "root/RootList.ipp"
#include "root/RootTable.ipp"


#include "tag.ipp"
#include "tag/TagList.ipp"
#include "tag/TagTable.ipp"

#include "thing.ipp"
#include "user.ipp"
#include "value.ipp"

#include "id_auto.ipp"

#include "atom/moc_AtomList.cpp"
#include "atom/moc_AtomTable.cpp"

#include "book/moc_BookList.cpp"
#include "book/moc_BookTable.cpp"

#include "category/moc_CategoryList.cpp"
#include "category/moc_CategoryTable.cpp"

#include "character/moc_CharacterList.cpp"
#include "character/moc_CharacterTable.cpp"

#include "class/moc_ClassList.cpp"
#include "class/moc_ClassTable.cpp"

#include "directory/moc_DirectoryList.cpp"
#include "directory/moc_DirectoryTable.cpp"

#include "document/moc_DocumentList.cpp"
#include "document/moc_DocumentTable.cpp"

#include "event/moc_EventList.cpp"
#include "event/moc_EventTable.cpp"

#include "field/moc_FieldList.cpp"
#include "field/moc_FieldTable.cpp"

#include "folder/moc_FolderList.cpp"
#include "folder/moc_FolderTable.cpp"

#include "fragment/moc_FragmentList.cpp"
#include "fragment/moc_FragmentTable.cpp"


#include "game/moc_GameList.cpp"
#include "game/moc_GameTable.cpp"

#include "id/moc_IdList.cpp"
#include "id/moc_IdListView.cpp"
#include "id/moc_IdMimeList.cpp"
#include "id/moc_IdModel.cpp"
#include "id/moc_IdSignalMapper.cpp"
#include "id/moc_IdTable.cpp"
#include "id/moc_IdTableView.cpp"
#include "id/moc_IdTreeView.cpp"

#include "leaf/moc_LeafList.cpp"
#include "leaf/moc_LeafTable.cpp"

#include "place/moc_PlaceList.cpp"
#include "place/moc_PlaceTable.cpp"

#include "root/moc_RootList.cpp"
#include "root/moc_RootTable.cpp"

#include "tag/moc_TagList.cpp"
#include "tag/moc_TagTable.cpp"
