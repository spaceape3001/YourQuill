////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <vulqan/VqCore.hpp>
#include <vulqan/VqInstance.hpp>
#include <iostream>

using namespace yq;

int main(int argc, char* argv[])
{
    Ref<VqInstance> vi   = new VqInstance;
    vi -> initialize();

    auto data = yq::vqEnumeratePhysicalDevices(vi -> instance());
    std::cout << data.size() << " physical device(s)\n";
    for(const auto& dev : data){
        VkPhysicalDeviceProperties  props;
        vkGetPhysicalDeviceProperties(dev, &props);
        std::cout << '\t' << props.deviceName  << '\n';
    }
    return 0;
}

