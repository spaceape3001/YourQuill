////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PipelineConfig.hpp"
#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <set>
#include <span>
#include <cassert>

namespace yq {
    namespace engine {
        struct DataFormatData {
            unsigned int        type_id     = 0;    //!< Meta type ID
            unsigned int        bindings    = 1;    //!< Number of bindings to be used
            DataFormat          format;             //!< Data format
        };
        
        std::span<const DataFormatData>   data_format_data();

        DataFormat      data_format(const TypeInfo&);
        template <typename T>
        DataFormat      data_format()
        {
            return data_format(meta<T>());
        }
        
        unsigned int    data_binding(const TypeInfo&);
        
        unsigned int    min_binding(size_t);
        
        template <typename T>
        unsigned int    min_binding()
        {
            return min_binding(sizeof(T));
        }
        
        template <typename T>
        unsigned int    data_binding()
        {
            return data_binding(meta<T>());
        }


        class PipelineBuilder : trait::not_copyable, trait::not_moveable {
        public:
        
            void    add_shader(ShaderSpec);
            void    add_shaders(std::initializer_list<ShaderSpec>);
            void    topology(Topology);
            void    polygons(PolygonMode);
            void    front(FrontFace);
            void    culling(CullMode);
        
            template <typename V> struct VBO;
            template <typename V>
            VBO<V>     vbo(uint32_t location=UINT32_MAX);
            
            operator const PipelineConfig&() const { return m_config; }
            const PipelineConfig*   copy() const;
            
            
            template <typename T>
            void    push_constant()
            {
                m_config.push.type  = PushConfig::Custom;
                m_config.push.size  = sizeof(T);
            }
            
            void    push_constant(PushConfig::Type);
            
        private:
            PipelineConfig          m_config;
            std::set<uint32_t>      m_locations;
        };
        
        template <typename V>
        class PipelineBuilder::VBO : public VBOConfig, trait::not_copyable {
        public:
        
            VBO(PipelineBuilder* cfg, uint32_t loc) : m_pipeline(cfg), m_location(loc) 
            {
                stride  = sizeof(V);
            }
            
            VBO(VBO& mv) : VBOConfig(std::move(mv))
            {
                m_pipeline      = mv.m_pipeline;
                mv.m_pipeline   = nullptr;
                m_location      = mv.m_location;
            }
            
            
            VBO& operator=(VBO&&mv) 
            {
                if(this != &mv){
                    VBOConfig::operator=(std::move(mv));
                    m_pipeline      = mv.m_pipeline;
                    mv.m_pipeline   = nullptr;
                    m_location      = mv.m_location;
                }
                return *this;
            }
            
            ~VBO()
            {
                if(!m_pipeline)
                    return ;
                if(attrs.empty()){
                    if constexpr (is_type_v<V>){
                        typed_attribute<V>(m_location, 0);
                    }
                }
                assert(!attrs.empty());
                m_pipeline -> m_config.vbos.push_back(*this);
            }

            VBO&     rate(VertexInputRate vr)
            {
                inputRate   = vr;
                return *this;
            }
            
            template  <typename M>
            requires is_type_v<M>
            VBO& attribute(M V::*member, uint32_t location = UINT32_MAX)
            {
                typed_attribute<M>(location, (uint32_t) member_offset(member));
                return *this;
            }
            
            template  <typename M>
            requires (!is_type_v<M>)
            VBO& attribute(M V::*member, DataFormat fmt, uint32_t location = UINT32_MAX)
            {
                attr_impl(fmt, location, (uint32_t) member_offset(member), min_binding<M>());
                return *this;
            }
            

        private:

            PipelineBuilder*    m_pipeline;
            uint32_t            m_location;
            
            uint32_t    location_filter(uint32_t loc, uint32_t req)
            {
                if(loc != UINT32_MAX){
                    for(uint32_t i=loc; i<loc+req; ++i){
                        [[maybe_unused]] auto j = m_pipeline->m_locations.insert(i);
                        assert(j.second && "Location already assigned!");
                    }
                    return loc;
                }
                
                if(m_pipeline->m_locations.empty()){
                    for(uint32_t i=0; i<req; ++i){
                        m_pipeline->m_locations.insert(i);
                    }
                    return 0;
                }
                
                loc = *(m_pipeline->m_locations.rbegin()) + 1;
                for(uint32_t i=0; i<req; ++i){
                    m_pipeline->m_locations.insert(i+loc);
                }
                return loc;
            }
            
            template <typename A>
            requires is_type_v<A>
            void    typed_attribute(uint32_t loc, uint32_t offset)
            {
                attr_impl(data_format<A>(), loc, offset, min_binding<A>());
            }
            
            void    attr_impl(DataFormat df, uint32_t loc, uint32_t offset, uint32_t bindReq)
            {
                assert((df != DataFormat()) && "Bad data format!");
                if(df == DataFormat())
                    return ;
                VBOAttr     a;
                a.location  = location_filter(loc, bindReq);
                a.offset    = offset;
                a.format    = df;
                attrs.push_back(a);
            }
            
        };

        template <typename V>
        PipelineBuilder::VBO<V>     PipelineBuilder::vbo(uint32_t location)
        {
            return VBO<V>(this, location);
        }
    }
}
