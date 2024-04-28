////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


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

#include <mithril/atom/Atom.hpp>
#include <mithril/category/Category.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/doodle/Doodle.hpp>
#include <mithril/field/Field.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithril/style/Style.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/user/User.hpp>


namespace yq::mithril {
    template class Notifier<const Atom::Diff&>;
    template class Notifier<const Category::Diff&>;
    template class Notifier<const Class::Diff&>;
    template class Notifier<const Doodle::Diff&>;
    template class Notifier<const Field::Diff&>;
    template class Notifier<const Image::Diff&>;
    template class Notifier<const Leaf::Diff&>;
    template class Notifier<const Style::Diff&>;
    template class Notifier<const Tag::Diff&>;
    template class Notifier<const User::Diff&>;
}

