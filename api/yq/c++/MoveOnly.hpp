////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <type_traits>
#include "NoCopy.hpp"
#include <yq/c++/trait/has_inequality.hpp>

namespace yq {
    template <typename T>
    class MoveOnly : NoCopy {
    public:
        
        static_assert( std::is_move_assignable_v<T>, "Typename must be moveable." );
        static_assert( std::is_default_constructible_v<T>, "Typename must be default constructible." );
        static_assert( trait::has_inequality_v<T>, "Typename must be != capable." );
        using MoveHandler = std::function<void(T&)>;
    
    private:    MoveHandler m_move;
    protected:  T           m_data;
        
        MoveOnly(MoveHandler move, T data) : m_move(move), m_data(data) 
        {
            assert(move);
        }
        
        MoveOnly(MoveOnly&& other)
        {
            std::swap(m_data, other.m_data);
            std::swap(m_move, other.m_move);
        }
        
        MoveOnly& operator=(MoveOnly&&other)
        {
            if(this != &other){
                if(m_data != T{})
                    m_move(m_data);
                    
                m_data  = std::move(other.m_data);
                m_move  = std::move(other.m_move);
                
                other.m_data    = T{};
                other.m_move    = {};
            }
            return *this;
        }
        
        ~MoveOnly()
        {
            if(m_data != T{}){
                m_move(m_data);

                #ifndef NDEBUG
                m_data  = T{};
                #endif
            }
        }
        
    };
}
