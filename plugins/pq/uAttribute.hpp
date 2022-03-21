////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/Attribute.hpp>
#include <yq/bit/KeyValue.hpp>
#include <yq/enum/Change.hpp>
#include <util/Vector.hpp>

struct KVTree;
struct KeyValue;


cdb::KVReport   attr_changes(Document, const KVTree&, bool fRecurse=true);
cdb::KVReport   attr_changes(Attribute, const KVTree&, bool fRecurse=true);

void            execute_removals(cdb::KVReport&);

