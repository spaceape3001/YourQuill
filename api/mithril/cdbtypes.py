#!/usr/bin/python

################################################################################
##
##  YOUR QUILL
##
################################################################################

##
##  This file is to auto-maintain IDs for structures...
##

import os, sys, os.path, re
from pathlib import Path

#   Directories to exclude
EXCLUDE = [ 'id', 'html', 'rule' ]

def pluralize(s):
    if s.endswith('y'):
        return s[0:-1] + 'ies'
    if s.endswith('s'):
        return s + 'es'
    return s + 's'
    
def writeIfChanged(fname, text):
    old = ''
    if Path(fname).exists():
        f   = open(fname, 'r')
        old = f.read()
        f.close();
    if old != text:
        print("Writing to %s" % fname)
        f   = open(fname, 'w')
        f.write(text)
        f.close()


items   = []
class Item:
    def __init__(self, x, sub=None):
        self.id             = 0
        self.args           = dict();

        if isinstance(x, Item):
            assert sub is not None
            self.main           = x
            self.args           = x.args.copy()
            self.dir            = x.dir
            self.name           = x.name + '::' + sub
            self.hroot          = x.hroot
            self.hname          = x.hname
            self.header         = x.header
            self.sub            = sub
            self.args['sub']    = sub
            self.args['plural'] = pluralize(self.sub.lower())

        else:
            assert isinstance(x, Path)
            assert sub is None
            
            self.args           = dict()
            self.dir            = x
            self.name           = x.name.capitalize()
            self.hroot          = x.name + '/' + self.name 
            self.hname          = self.name + ".hpp"
            self.header         = x / self.hname
            self.sub            = None
            self.args['name']   = self.name
            self.args['lname']  = self.name.lower()
            self.args['hroot']  = self.hroot
            self.args['plural'] = pluralize(self.name.lower())
            

root    = Path('.')
for x in root.iterdir():
    if not x.is_dir():
        continue
    if x.name in EXCLUDE:
        continue

    i = Item(x)
    if not i.header.exists():
        continue
    
    f       = open(i.header, 'r')
    i.lines = f.readlines()
    f.close()
    
    items.append(i)
    
items.sort(key=lambda i: i.name)

subitems    = []

n           = 1
for i in items:
    i.id        = n
    n           = n + 1
    f           = open(i.header, 'r')
    lines       = f.readlines();
    f.close()
    output      = ''
    spew        = False
    chg         = False
    hasId       = False
    hasParents  = False
    struct      = ''
    sub         = None
    
    for l in lines:
        if 'struct' in l:
            s  = re.match(r"\s*struct\s+(\w+)\:\:(\w+)\s*{", l)
            if s is not None:
                sub  = s.group(2)
    
        if ('IdTypes' in l) and ('PARENTS' in l):
            hasParents  = True

        if ('IdTypeId' in l) and ('ID' in l):
            use = i.id
            if sub is not None:
                x       = Item(i, sub)
                x.id    = n
                n = n + 1
                subitems.append(x)
                use     = x.id
        
            oldtxt  = re.search(r"\d+", l).group(0)
            oldnum  = int(oldtxt)
            if oldnum != use:
                newtxt  = str(use)
                output += l.replace(oldtxt,newtxt)
                print("%s is being altered" % i.name)
            else:
                output += l
            spew    = True
            hasId   = True
        else:
            output += l

    writeIfChanged(i.header, output)
    
    if not hasId:
        print("WARNING... type '%(name)s' has no ID!" % i.args)
    if not hasParents:
        print("WARNING... type '%(name)s' has no PARENTS!" % i.args)

items.extend(subitems);
items.sort(key=lambda i: i.id)


################################################################################

mith  = """////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

/*
    WARNING... this file is auto generated!
*/

"""

for i in items:
    if i.sub is None:
        mith += """
#include <mithril/%(hroot)s.hpp>
#include <mithril/%(hroot)sCDB.hpp>   
""" % i.args 

mith += """
namespace yq::mithril {
    static constexpr const IdTypeId HIGH_ID = %d;
    """ % n
    
mith += """
    std::string_view  Id::type_name(IdTypeId ct)
    {
        switch(ct){"""

for i in items:
    if i.sub is None:
        mith += """
        case %(name)s::ID:
            return "%(name)s"sv;""" % i.args
    else:
        mith += """
        case %(name)s::%(sub)s::ID:
            return "%(name)s::%(sub)s"sv;""" % i.args

mith += """
        default:
            return "Unknown"sv;
        }
    }
    
    const TypeInfo*  Id::type_info(IdTypeId ct)
    {
        switch(ct){"""
        
for i in items:
    if i.sub is None:
        mith += """
        case %(name)s::ID:
            return &meta<%(name)s>();""" % i.args
    else:
        mith += """
        case %(name)s::%(sub)s::ID:
            return &meta<%(name)s::%(sub)s>();""" % i.args

mith +="""
        default:
            return nullptr;
        }
    }
    
    IdTypes  Id::base_types(IdTypeId ct)
    {
        switch(ct){"""
        
for i in items:
    if i.sub is None:
        mith += """
        case %(name)s::ID:
            return %(name)s::PARENTS;""" % i.args
    else:
        mith += """
        case %(name)s::%(sub)s::ID:
            return %(name)s::%(sub)s::PARENTS;""" % i.args
    
mith += """
        default:
            return {};
        }
    }
    """

mith += """
    IdTypeId  Id::max_type()
    {
        return HIGH_ID - 1;
    }
    """

mith += """
    std::string  Id::key() const
    {
        switch(type()){
    """
    
for i in items:
    if i.sub is None:
        mith += """
        case %(name)s::ID:
            return cdb::key(%(name)s(id()));""" % i.args
    else:
        mith += """
        case %(name)s::%(sub)s::ID:
            return cdb::key(%(name)s(id()));""" % i.args
            
mith += """
        default:
            return std::string();
        }
    }
    
    std::string Id::name() const
    {
        switch(type()){
    """

for i in items:
    if i.sub is None:
        mith += """
            case %(name)s::ID:
                return cdb::name(%(name)s(id()));""" % i.args
    else:
        mith += """
            case %(name)s::%(sub)s::ID:
                return cdb::name(%(name)s::%(sub)s(id()));""" % i.args

mith += """
        default:
            return std::string();
        }
    }
}

namespace yq::mithril::cdb {
    std::vector<Id>     all(IdTypeId ct, Sorted sorted)
    {
        switch(ct){"""
        
for i in items:
    if i.sub is None:
        mith += """
        case %(name)s::ID:
            return ids<%(name)s>(all_%(plural)s(sorted));""" % i.args
        
mith += """
        default:
            return std::vector<Id>();
        }
    }
}    
"""

writeIfChanged('id/id_auto.ipp', mith)

################################################################################

mith = """////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

/*
    WARNING... this file is auto generated!
*/

"""

for i in items:
    if i.sub is None:
        mith += """
#include <mithrilQt/%(lname)s.hpp>""" % i.args

mith += """
#include <mithrilQt/id.hpp>
#include <QIcon>

namespace yq::mithril {
    QIcon       id_qIcon(Id i)
    {
        static QIcon s_ico(":/generic/unknown.svg");
        switch(i.type()){
"""

for i in items:
    if i.sub is None:
        mith += """
        case %(name)s::ID:
            return qIcon(%(name)s(i.id()));""" % i.args
    else:
        mith += """
        case %(name)s::%(sub)s::ID:
            return qIcon(%(name)s::%(sub)s(i.id()));""" % i.args

mith += """
        default:
            return s_ico;
        }
    }
}
"""

writeIfChanged('../mithrilQt/id_auto.ipp', mith)

