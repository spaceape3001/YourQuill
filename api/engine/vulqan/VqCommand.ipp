////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VqCommand.hpp"

namespace yq {
    struct VqCmdState {
        VkCommandBuffer     buffer;
        VkPipeline          pipeline    = nullptr;
    
        void    apply(const VqCommand& cmd)
        {
            if(cmd.pipeline && (cmd.pipeline != pipeline)){
                vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, cmd.pipeline);
                pipeline    =    cmd.pipeline;
            }
            
            if(cmd.layout && cmd.push){
                const auto& p = *(cmd.push);
                vkCmdPushConstants(buffer, cmd.layout, p.mask, 0, p.size, p.data);
            }
            
            if(cmd.vbo){
                const auto& v = *(cmd.vbo);
                vkCmdBindVertexBuffers(buffer, v.first, v.count, v.buffers, v.offsets);
            }
            
            if(cmd.draw){
                const auto& d = *(cmd.draw);
                vkCmdDraw(buffer, d.vertexCount, d.instanceCount, d.firstVertex, d.firstInstance);
            }
        }
        
    };

    void    record_draw(VkCommandBuffer buffer, const VqCommand& cmd)
    {
        VqCmdState  cstate(buffer);
        cstate.apply(cmd);
    }
    
    void    record_draw(VkCommandBuffer buffer, std::span<const VqCommand> cmds)
    {
        VqCmdState  cstate(buffer);
        for(auto& c : cmds)
            cstate.apply(c);
    }
    
    void    record_draw(VkCommandBuffer buffer, const std::span<VqCommand> cmds)
    {
        VqCmdState  cstate(buffer);
        for(auto& c : cmds)
            cstate.apply(c);
    }
}
