////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "preamble.hpp"
#include "Application.ipp"
#include "Camera.ipp"
#include "Event.ipp"
#include "Manipulator.ipp"
#include "Taskable.ipp"
#include "Tool.ipp"
#include "Undo.ipp"
#include "Widget.ipp"

#include "render/BufferObjectInfo.ipp"
#include "render/PipelineBuilder.ipp"
#include "render/PipelineConfig.ipp"
#include "render/PipelineUtils.ipp"

#include <basic/meta/Init.hpp>


#include "vulqan/VqAllocator.ipp"
#include "vulqan/VqBuffer.ipp"
#include "vulqan/VqCommand.ipp"
#include "vulqan/VqCommandBuffers.ipp"
#include "vulqan/VqCommandPool.ipp"
#include "vulqan/VqDescriptorPool.ipp"
#include "vulqan/VqDevice.ipp"
#include "vulqan/VqEnums.ipp"
#include "vulqan/VqFence.ipp"
#include "vulqan/VqFencePool.ipp"
#include "vulqan/VqFrameBuffers.ipp"
#include "vulqan/VqImageViews.ipp"
#include "vulqan/VqLogging.ipp"
#include "vulqan/VqMonitor.ipp"
#include "vulqan/VqQueues.ipp"
#include "vulqan/VqRenderPass.ipp"
#include "vulqan/VqSemaphore.ipp"
#include "vulqan/VqShaderStages.ipp"
#include "vulqan/VqSurface.ipp"
#include "vulqan/VqSwapchain.ipp"
#include "vulqan/VqUtils.ipp"
#include "vulqan/VqWindow.ipp"



YQ_TYPE_IMPLEMENT(yq::engine::BorderColor)
YQ_TYPE_IMPLEMENT(yq::engine::ColorComponent)
YQ_TYPE_IMPLEMENT(yq::engine::ColorComponentFlags)
YQ_TYPE_IMPLEMENT(yq::engine::ComponentSwizzle)
YQ_TYPE_IMPLEMENT(yq::engine::CullMode)
YQ_TYPE_IMPLEMENT(yq::engine::DataActivity)
YQ_TYPE_IMPLEMENT(yq::engine::DataFormat)
YQ_TYPE_IMPLEMENT(yq::engine::DescriptorType)
YQ_TYPE_IMPLEMENT(yq::engine::FilterType)
YQ_TYPE_IMPLEMENT(yq::engine::FrontFace)
YQ_TYPE_IMPLEMENT(yq::engine::ImageLayout)
YQ_TYPE_IMPLEMENT(yq::engine::ImageTiling)
YQ_TYPE_IMPLEMENT(yq::engine::ImageType)
YQ_TYPE_IMPLEMENT(yq::engine::IndexType)
YQ_TYPE_IMPLEMENT(yq::engine::LogicOp)
YQ_TYPE_IMPLEMENT(yq::engine::MipmapMode)
YQ_TYPE_IMPLEMENT(yq::engine::PolygonMode)
YQ_TYPE_IMPLEMENT(yq::engine::PresentMode)
YQ_TYPE_IMPLEMENT(yq::engine::SamplerAddressMode)
YQ_TYPE_IMPLEMENT(yq::engine::SamplerMipmapMode)
YQ_TYPE_IMPLEMENT(yq::engine::ShaderType)
YQ_TYPE_IMPLEMENT(yq::engine::ShaderTypeFlags)
YQ_TYPE_IMPLEMENT(yq::engine::StencilOp)
YQ_TYPE_IMPLEMENT(yq::engine::SubpassContents)
YQ_TYPE_IMPLEMENT(yq::engine::SystemAllocationScope)
YQ_TYPE_IMPLEMENT(yq::engine::Topology)
YQ_TYPE_IMPLEMENT(yq::engine::Tristate)
