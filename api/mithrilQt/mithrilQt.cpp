////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <tbb/spin_rw_mutex.h>  // avoids emit issues
#include <concepts>

#include "atom.ipp"
#include "attribute.ipp"
#include "book.ipp"
#include "category.ipp"
#include "character.ipp"
#include "class.ipp"
#include "directory.ipp"
#include "document.ipp"
#include "entity.ipp"
#include "event.ipp"
#include "field.ipp"
#include "folder.ipp"
#include "fragment.ipp"
#include "game.ipp"
#include "graph.ipp"
#include "group.ipp"

#include "id.ipp"


#include "id/IdColumn.ipp"
// #include "id/IdFunctor.ipp" // TODO
#include "id/IdListView.ipp"
#include "id/IdMimeList.ipp"
#include "id/IdModel.ipp"
#include "id/IdSignalMapper.ipp"
#include "id/IdTableView.ipp"
#include "id/IdTreeView.ipp"

#include "image.ipp"
#include "leaf.ipp"
#include "organization.ipp"

#include "place.ipp"
#include "root.ipp"
#include "tag.ipp"
#include "thing.ipp"
#include "user.ipp"
#include "value.ipp"

#include "id_auto.ipp"

#include "id/moc_IdListView.cpp"
#include "id/moc_IdMimeList.cpp"
#include "id/moc_IdModel.cpp"
#include "id/moc_IdSignalMapper.cpp"
#include "id/moc_IdTableView.cpp"
#include "id/moc_IdTreeView.cpp"

