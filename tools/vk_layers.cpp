////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <engine/vulqan/VqCore.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    auto data = yq::vqEnumerateInstanceLayerProperties();
    std::cout << data.size() << " vulkan layer(s)\n";
    for(const auto& lay : data)
        std::cout << '\t' << lay.layerName << '\n';
    return 0;
}

