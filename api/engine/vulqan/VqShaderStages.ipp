////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "VqUtils.hpp"
#include "VqShaderStages.hpp"
#include <engine/Vulqan.hpp>
#include <basic/Logging.hpp>
#include <engine/Shader.hpp>

namespace yq {
    namespace engine {
        VqShaderStages::VqShaderStages(VkDevice dev, const std::vector<ShaderSpec>& shaders) : m_device(dev)
        {
            m_stages.reserve(shaders.size());
            for(auto& s : shaders){
                
                ShaderPtr       sh;
                if(const ShaderPtr* ptr = std::get_if<ShaderPtr>(&s)){
                    sh  = *ptr;
                } else if(const std::string* ptr = std::get_if<std::string>(&s)){
                    sh  = Shader::get(*ptr);
                } else {
                    yWarning() << "Empty shader spec, skipping";
                    continue;
                }
            
                if(!sh){
                    yCritical() << "Null/unresolved shader, skipping";
                    continue;
                }
            
                VqPipelineShaderStageCreateInfo stage;
                
                switch(sh->shader_type()){
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
                
                m_mask |= stage.stage;
                stage.pName = "main";
            
                const ByteArray&    code    = sh->payload();
                VqShaderModuleCreateInfo createInfo;
                createInfo.codeSize = code.size();
                createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());
                if (vkCreateShaderModule(m_device, &createInfo, nullptr, &stage.module) != VK_SUCCESS) {
                    yError() << "Unable to create shader module.";
                    continue;
                }
                
                m_stages.push_back(stage);
            }        
        }

        VqShaderStages::VqShaderStages(Vulqan& win, const std::vector<ShaderSpec>& shaders) : 
            VqShaderStages(win.logical(), shaders)
        {
        }
        

        VqShaderStages::~VqShaderStages()
        {
            for(auto& i : m_stages){
                if(i.module){
                    vkDestroyShaderModule(m_device, i.module, nullptr);
                }
            }
            m_stages.clear();
        }
        
        
        VkGraphicsPipelineCreateInfo&  operator<<(VkGraphicsPipelineCreateInfo& pci, const VqShaderStages& ps)
        {
            pci.stageCount  = ps.m_stages.size();
            if(pci.stageCount)
                pci.pStages = ps.m_stages.data();
            return pci;
        }
        
    }
}
