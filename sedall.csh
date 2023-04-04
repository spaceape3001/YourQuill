#!/bin/csh

#sed -e $1 -i

set dirs="api exe tests tools qt-utilities/api yq-toolbox/api yq-toolbox/test yq-toolbox/tool yq-vulqan/api yq-vulqan/plugin yq-vulqan/sdk yq-vulqan/test yq-vulqan/tool"

set headers=`find ${dirs} -name \*.hpp | paste -s`
set headers1=`find ${dirs} -name \*.hxx | paste -s`
set impls=`find ${dirs} -name \*.ipp | paste -s`
set sources=`find ${dirs} -name \*.cpp | paste -s`

sed -e $1 -i ${headers} ${headers1} ${impls} ${sources}


