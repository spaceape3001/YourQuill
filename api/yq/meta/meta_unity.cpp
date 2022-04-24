////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "ReservedIDs.hpp"

#include "MetaRepo.ipp"

#include "ArgInfo.ipp"
#include "CompoundInfo.ipp"
#include "Global.ipp"
#include "Meta.ipp"
#include "MetaWriter.ipp"
#include "MethodInfo.ipp"
#include "ObjectInfo.ipp"
#include "PropertyInfo.ipp"
#include "PropGetter.ipp"
#include "PropSetter.ipp"
#include "TypeInfo.ipp"


YQ_INVOKE(
    yq::invalid();
    yq::any();
    yq::GlobalInfo::instance();
);




#if 0
    //  SIMPLY AN EXAMPLE TO USAGE
void        yq_invoke()
{
    meta::object<T>().description().arg()
        << meta::description("")
        << meta::property("name", mv/getter/setter )
        << meta::tag()
    ;
    meta::function<T::FN>() << meta::Arg()
    meta::value<T>()
    meta::method<T::FN>().description().arg()....
    meta::
}
#endif
