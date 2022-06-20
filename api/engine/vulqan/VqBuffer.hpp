////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>
#include <span>

namespace yq {
    struct VqInternal;
    
    class VqBuffer : trait::not_copyable {
    public:
    
    
        VqBuffer(){}
        
        VqBuffer(VqInternal&, VkBufferUsageFlags, const void*, size_t);
        
        template <typename T>
        VqBuffer(VqInternal&, VkBufferUsageFlags, std::span<T>);

        template <typename T>
        VqBuffer(VqInternal&, VkBufferUsageFlags, std::span<const T>);

        template <typename T, size_t N>
        VqBuffer(VqInternal&, VkBufferUsageFlags, const T(&)[N]);

        VqBuffer(VqBuffer&&);
        VqBuffer& operator=(VqBuffer&&);
        ~VqBuffer();
        
        bool    good() const noexcept { return m_buffer != nullptr; }
        operator bool() const noexcept { return good(); }
        operator VkBuffer() const noexcept { return m_buffer; }
        VkBuffer buffer() const noexcept { return m_buffer; }
    
    private:
        void    dtor();
        void    move(VqBuffer&&);
        
        void            assign(const void*, size_t);
        
        VkDevice        m_device        = nullptr;
        VmaAllocator    m_allocator     = nullptr;
        VkBuffer        m_buffer        = nullptr;
        VmaAllocation   m_allocation    = nullptr;
        size_t          m_size          = 0;
    };

    template <typename T>
    VqBuffer::VqBuffer(VqInternal& w, VkBufferUsageFlags flags, std::span<T> data) : 
        VqBuffer(w, flags, data.data(), data.size() * sizeof(T))
    {
    }
    

    template <typename T>
    VqBuffer::VqBuffer(VqInternal& w, VkBufferUsageFlags flags, std::span<const T>data) :
        VqBuffer(w, flags, data.data(), data.size() * sizeof(T))
    {
    }

    template <typename T, size_t N>
    VqBuffer::VqBuffer(VqInternal&w, VkBufferUsageFlags flags, const T (&data) [N]) : 
        VqBuffer(w,flags, data,N*sizeof(T))
    {
    }
}
