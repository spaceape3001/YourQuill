#pragma once

#include "yq/util/Enum.hpp"
#include "yq/meta/MetaValue.hpp"

YQ_ENUM(Vcs, , None, RCS, SVN, GIT)

MV_DECLARE(Vcs)
