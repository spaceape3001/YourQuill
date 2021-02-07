#pragma once

#include "MetaInfo.hpp"

class MetaArg : public MetaInfo {
public:

    const MetaValue& type() const { return m_type; }

private:
    MetaArg(const MetaValue&, const MetaInfo*);
    
    const MetaValue& m_type;
};
