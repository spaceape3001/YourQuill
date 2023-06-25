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
    

items   = []
class Item:
    def __init__(self, x):
        self.dir            = x
        self.name           = x.name.capitalize()
        self.hroot          = x.name + '/' + self.name 
        self.hname          = self.name + ".hpp"
        self.header         = x / self.hname
        self.parent         = 0
        self.id             = 0
        self.args           = dict()
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
    
    for l in lines:
        if ('IdTypes' in l) and ('PARENTS' in l):
            hasParents  = True

        if spew:
            output += l
            continue

        if ('IdTypeId' in l) and ('ID' in l):
            oldtxt  = re.search(r"\d+", l).group(0)
            oldnum  = int(oldtxt)
            if oldnum != i.id:
                newtxt  = str(i.id)
                output += l.replace(oldtxt,newtxt)
                chg = True
                print("%s is being altered" % i.name)
            else:
                output += l
            spew    = True
            hasId   = True
        else:
            output += l

    if chg:
        f   = open(i.header, 'w')
        f.write(output)
        f.close()
    
    if not hasId:
        print("WARNING... type '%(name)s' has no ID!" % i.args)
    if not hasParents:
        print("WARNING... type '%(name)s' has no PARENTS!" % i.args)

def writeIfChanged(fname, text):
    old = ''
    if Path(fname).exists():
        f   = open(fname, 'r')
        old = f.read()
        f.close();
    if old != text:
        f   = open(fname, 'w')
        f.write(text)
        f.close()

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
    mith += """
        case %(name)s::ID:
            return "%(name)s"sv;""" % i.args

mith += """
        default:
            return "Unknown"sv;
        }
    }
    
    const TypeInfo*  Id::type_info(IdTypeId ct)
    {
        switch(ct){"""
        
for i in items:
    mith += """
        case %(name)s::ID:
            return &meta<%(name)s>();""" % i.args

mith +="""
        default:
            return nullptr;
        }
    }
    
    IdTypes  Id::base_types(IdTypeId ct)
    {
        switch(ct){"""
        
for i in items:
    mith += """
        case %(name)s::ID:
            return %(name)s::PARENTS;""" % i.args
    
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
    mith += """
        case %(name)s::ID:
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
    mith += """
        case %(name)s::ID:
            return cdb::name(%(name)s(id()));""" % i.args

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
    mith += """
        case %(name)s::ID:
            return qIcon(%(name)s(i.id()));""" % i.args

mith += """
        default:
            return s_ico;
        }
    }
}
"""

writeIfChanged('../mithrilQt/id_auto.ipp', mith)

