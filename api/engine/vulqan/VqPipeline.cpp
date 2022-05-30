////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqCore.hpp"
#include "VqPipeline.hpp"
#include "VqWindow.hpp"

#include <basic/Logging.hpp>
#include <basic/Safety.hpp>
#include <engine/shader/Shader.hpp>

namespace yq {
    VqPipeline::VqPipeline(VkDevice dev, const std::vector<Ref<const Shader>>& shaders)
    {
        std::vector<VkPipelineShaderStageCreateInfo>    stages;
        auto s = safety([&](){
            for(auto& i : stages)
                vkDestroyShaderModule(dev, i.module, nullptr);
        });
        
        stages.reserve(shaders.size());
        
        uint32_t    stage_mask  = 0;
        
        for(auto& s : shaders){
            if(!s){
                yCritical() << "NULL SHADER detected, skipping";
                continue;
            }
        
            VqPipelineShaderStageCreateInfo stage;
            
            switch(s->shader_type()){
            case ShaderType::VERT:
                stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
                break;
            case ShaderType::TESC:
                stage.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
                break;
            case ShaderType::TESE:
                stage.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
                break;
            case ShaderType::FRAG:
                stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                break;
            case ShaderType::GEOM:
                stage.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
                break;
            case ShaderType::COMP:
                stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
                break;
            case ShaderType::UNKNOWN:
            default:
                yError() << "Unknown shader type...skipping.";
                continue;
            }
            
            stage_mask |= stage.stage;
            stage.pName = "main";
        
            const ByteArray&    code    = s->payload();
            VqShaderModuleCreateInfo createInfo;
            createInfo.codeSize = code.size();
            createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());
            if (vkCreateShaderModule(dev, &createInfo, nullptr, &stage.module) != VK_SUCCESS) {
                yError() << "Unable to create shader module.";
                continue;
            }
            
            stages.push_back(stage);
        }
        
    
    }
    
    VqPipeline::VqPipeline(VqWindow& win, const std::vector<Ref<const Shader>>& shaders) : VqPipeline(win.logical(), shaders)
    {
    }
    

    VqPipeline::~VqPipeline()
    {
    }
    
}
