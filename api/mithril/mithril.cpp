////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/preamble.hpp>


#include "atom/Atom.ipp"
#include "atom/AtomArg.ipp"
#include "atom/AtomDiff.hpp"    // syntax check
#include "atom/AtomHtml.ipp"
#include "atom/AtomPost.ipp"
#include "atom/AtomProvider.ipp"
#include "atom/AtomSpec.ipp"

//#include "atom/Property.hpp"


#include "attribute/AttributeArg.ipp"
#include "attribute/AttributeDiff.ipp"
#include "attribute/AttributeHtml.ipp"

//#include "auth/Authentication.ipp"

#include "bit/Context.ipp"
#include "bit/Copyright.ipp"
#include "bit/KeyValue.ipp"
#include "bit/Policy.ipp"

//#include "book/BookArg.ipp"
#include "book/BookHtml.ipp"
//#include "book/BookPost.ipp"
#include "book/BookProvider.ipp"


#include "category/CategoryArg.ipp"
#include "category/CategoryData.ipp"
#include "category/CategoryDiff.hpp"        // syntax check
#include "category/CategoryFile.ipp"
#include "category/CategoryHtml.ipp"
#include "category/CategoryPost.ipp"
#include "category/CategoryProvider.ipp"

//#include "character/CharacterArg.ipp"
#include "character/CharacterHtml.ipp"
//#include "character/CharacterPost.ipp"
#include "character/CharacterProvider.ipp"

#include "class/Class.ipp"
#include "class/ClassArg.ipp"
#include "class/ClassData.ipp"
#include "class/ClassDiff.hpp"          // syntax check
#include "class/ClassFile.ipp"
#include "class/ClassHtml.ipp"
#include "class/ClassPost.ipp"
#include "class/ClassProvider.ipp"
//#include "class/ClassUpdate.ipp"


#include "directory/DirectoryArg.ipp"
#include "directory/DirectoryHtml.ipp"
#include "directory/DirectoryPost.ipp"
#include "directory/DirectoryProvider.ipp"

#include "document/DocumentArg.ipp"
#include "document/DocumentHtml.ipp"
#include "document/DocumentPost.ipp"
#include "document/DocumentProvider.ipp"
#include "document/DocumentUpdate.ipp"

#include "event/EventHtml.ipp"
#include "event/EventProvider.ipp"

#include "field/FieldArg.ipp"
#include "field/FieldData.ipp"
#include "field/FieldDiff.hpp"      // syntax check
#include "field/FieldFile.ipp"
#include "field/FieldHtml.ipp"
#include "field/FieldPost.ipp"
#include "field/FieldProvider.ipp"

#include "file/FileSpec.ipp"

#include "folder/FolderArg.ipp"
#include "folder/FolderHtml.ipp"
#include "folder/FolderPost.ipp"
#include "folder/FolderProvider.ipp"

#include "fragment/FragmentArg.ipp"
#include "fragment/FragmentHtml.ipp"
#include "fragment/FragmentPost.ipp"
#include "fragment/FragmentProvider.ipp"

//#include "game/GameArg.ipp"
#include "game/GameHtml.ipp"
//#include "game/GamePost.ipp"
#include "game/GameProvider.ipp"

#include "graphviz/Graphviz.ipp"
#include "graphviz/GraphvizArg.ipp"
#include "graphviz/GraphvizBuilder.ipp"
#include "graphviz/GraphvizHtml.ipp"

//#include "group/GroupArg.ipp"
#include "group/GroupHtml.ipp"
//#include "group/GroupPost.ipp"

#include "html/Html.ipp"
#include "html/Post.ipp"

#include "id/id.ipp"        // order sensitive with id_auto.ipp

#include "image/ImageArg.ipp"
#include "image/ImageDiff.hpp"  // syntax check
#include "image/ImageHtml.ipp"
#include "image/ImagePost.ipp"

#include "io/StdFile.ipp"
#include "io/StdObject.ipp"

#include "leaf/LeafArg.ipp"
#include "leaf/LeafData.ipp"
#include "leaf/LeafDiff.hpp"    // syntax check
#include "leaf/LeafFile.ipp"
#include "leaf/LeafHtml.ipp"
#include "leaf/LeafPost.ipp"
#include "leaf/LeafProvider.ipp"

#include "meta/MetaHtml.ipp"
#include "meta/MetaObjectArg.ipp"
#include "meta/MetaTypeArg.ipp"

#include "notify/AtomNotifier.ipp"
#include "notify/FileWatch.ipp"
#include "notify/FunctionalNotifier.ipp"
#include "notify/FunctionalStage4.ipp"
#include "notify/FileNotifier.ipp"
#include "notify/Notifier.hxx"
#include "notify/Stage2.ipp"
#include "notify/Stage3.ipp"
#include "notify/Stage4.ipp"
#include "notify/Stage5.ipp"


//#include "place/PlaceArg.ipp"
#include "place/PlaceHtml.ipp"
//#include "place/PlacePost.ipp"
#include "place/PlaceProvider.ipp"

#include "quill/QuillData.ipp"
#include "quill/QuillFile.ipp"

#include "root/RootArg.ipp"
#include "root/RootHtml.ipp"
#include "root/RootPost.ipp"
#include "root/RootProvider.ipp"

//#include "rule/Action.ipp"
//#include "rule/Constraint.ipp"
//#include "rule/Trigger.ipp"

#include "style/StyleArg.ipp"
#include "style/StyleData.ipp"
#include "style/StyleFile.ipp"
#include "style/StyleHtml.ipp"
#include "style/StylePost.ipp"
#include "style/StyleProvider.ipp"

//#include "tag/Tag.ipp"
#include "tag/TagArg.ipp"
#include "tag/TagData.ipp"
#include "tag/TagDiff.hpp"  // syntax check
#include "tag/TagFile.ipp"
#include "tag/TagHtml.ipp"
#include "tag/TagPost.ipp"
#include "tag/TagProvider.ipp"

//#include "thing/ThingArg.ipp"
#include "thing/ThingHtml.ipp"
//#include "thing/ThingPost.ipp"

#include "update/U.hxx"

#include "user/UserArg.ipp"
#include "user/UserData.ipp"
#include "user/UserDiff.hpp"
#include "user/UserFile.ipp"
#include "user/UserHtml.ipp"
#include "user/UserPost.ipp"

#include "util/Markdown.ipp"


#include "value/ValueArg.ipp"
#include "value/ValueData.ipp"
#include "value/ValueFile.ipp"
#include "value/ValueHtml.ipp"


namespace yq::mithril {
    template class Notifier<const Atom::Diff&>;
    template class Notifier<const Category::Diff&>;
    template class Notifier<const Class::Diff&>;
    template class Notifier<const Field::Diff&>;
    template class Notifier<const Image::Diff&>;
    template class Notifier<const Leaf::Diff&>;
    template class Notifier<const Style::Diff&>;
    template class Notifier<const Tag::Diff&>;
    template class Notifier<const User::Diff&>;
}

