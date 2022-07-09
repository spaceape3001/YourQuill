////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <concepts>
#include <basic/trait/can_add.hpp>
#include <basic/trait/can_two_multiply.hpp>
#include <basic/trait/is_stdarray.hpp>
#include <basic/trait/is_stdspan.hpp>
#include <basic/trait/is_stdvector.hpp>
#include "preamble.hpp"
#include "Coords.hpp"
#include "IntRange.hpp"

namespace yq {
    
    namespace impl {
        template <uint8_t N>
        consteval Coord<uint8_t, N> coord_ascending()
        {
            union {
                uint8_t             values[N];
                Coord<uint8_t, N>   ret;
            };
            for(uint8_t n=0;n<N;++n)
                return values[n] = n+1;
            return ret;
        }

        template <uint8_t N>
        consteval Coord<uint8_t, N> coord_descending()
        {
            union {
                uint8_t             values[N];
                Coord<uint8_t, N>   ret;
            };
            for(uint8_t n=1;n<=N;++n)
                return values[N-n] = n;
            return ret;
        }
        
        template <typename T, uint8_t N>
        constexpr Coord<T, N>   uniform_coord(T val)
        {
            union {
                T             values[N];
                Coord<T, N>   ret;
            };
            for(uint8_t n=0;n<N;++n)
                values[n]  = val;
            return ret;
        }
        
        template <uint8_t N>
        Coord<size_t, N>        array_comingle(const Coord<size_t,N>& sizes, const Coord<uint8_t,N>& order)
        {
            using   uint8_p = std::pair<uint8_t, uint8_t>;
            uint8_p rote[N];
            
            for(uint8_t n=0;n<N;++n)
                rote[N] = { N-n-1, get(order, n) };
            std::stable_sort(std::begin(rote), std::end(rote), 
                [](uint8_p a, uint8_p b) -> bool { return a.second > b.second; }
            );
        
            size_t  ret[N] = {};
            size_t  z       = 1ULL;
            for(uint8_t n=0;n<N;++n){
                uint8_t i = rote[n].first;
                ret[i]  = z;
                z *= get(sizes, i);
            }
            
            return *reinterpret_cast<Coord<size_t, N>*>(ret);
        }
        
        template <typename T>
        T&      thread_safe_bad_data()
        {
            static thread_local T    s_ret{};
            return s_ret;
        }
    }
    
    
    
    /*! \brief "Array"
    
        \tparam DATA     container (ie, vector, span, etc)
        \tparam COORD    Coordinate type, expected to be Coord
        \tparam ORIGIN   True to enable to set origin
        \tparam GHOST    True to inflate sizes by a padding (per side)
        
        \property Permutation
        
            As this object maps a multi-dimensional array into a linear memory 
            object, the permutation governs which coordinate is "major" vs "minor" 
            in the index mapping function.  A lower-number gives that axis major 
            weight.  If two dimensions have the same number, it's has the same 
            effect as linearly increasing numbers.
            
            ie. { 0, 0, 0 } is the default to a 3-D array, which effectively becomes
            { 1, 2, 3 } implying that "i" is major-major, so incrementing "i" means
            a stride of (cnt-i * cnt-j) elements.  
            
            In a 2D array, { 0, 0} or {0,1} is row-major, while { 1, 0 } would be
            a column major ordering.
            
        \property Origin
            
            If the flag is set, then it's possible to start the indexing at something
            other than (0...*).  When set, the origin can be configured to ANY value.
            
        \property Ghost
        
            Ghost is basically padding around the array, useful for boundary conditions 
            (for instance).  Or, maybe wanting [0...N] to all be valid?  
            
        \property Dims
            
            This is the number of elements for the array, one per dimension. 
            The setters will only allow positive (or zero) numbers for sizes.  
            Note zero is ill-advised!
            
        
            
            
    */
    template <typename DATA, typename COORD, bool ORIGIN, bool GHOST>
    class Array {
    public:
    
        static_assert(trait::is_coord_v<COORD>, "COORD must be a coordinate object!");
        static_assert((1 <= COORD::DIMS) && (COORD::DIMS <= 6), "Invalid dimension count!");

        using container_type    = DATA;
        using value_type        = typename DATA::value_type;
        using coord_type        = COORD;
        using index_type        = typename COORD::component_type;
    
        static constexpr const bool         is_resizeable   = trait::is_stdvector_v<DATA>;
        static constexpr const bool         is_array        = trait::is_stdarray_v<DATA>;
        static constexpr const bool         is_span         = trait::is_stdspan_v<DATA>;
        static constexpr const bool         is_vector       = trait::is_stdvector_v<DATA>;
        static constexpr const bool         is_constant     = std::is_const_v<value_type>;
        static constexpr const bool         is_mutable      = !std::is_const_v<value_type>;
        static constexpr const bool         is_signed       = std::is_signed_v<index_type>;
        static constexpr const bool         is_unsigned     = std::is_unsigned_v<index_type>;
        static constexpr const uint8_t      DIMS            = COORD::DIMS;

        static_assert(std::is_integral_v<index_type>, "COORD must be integer based");

        using size_c            = Coord<size_t, DIMS>;
        using uint8_c           = Coord<uint8_t, DIMS>;
        using range_type        = IntRange<index_type>;
        using ranges_type       = Coord<range_type, DIMS>;

        
        template <typename=void>
        requires (!ORIGIN && !GHOST && is_vector)
        explicit Array(const COORD& c, const value_type& v={})
        {
            set_count(c);
            build(v);
        }


        /*! \brief Builds up the vector
        
            *IF* the vector option is chosen, then this will 
            recompute & build according to the current specifications.
        */
        template <typename=void>
        requires (is_vector)
        bool                    build(const value_type& v = {})
        {
            if(needs_compute())
                compute();
            
            if(!m_data.empty())
                m_data.clear();
                
            try {
                m_data.resize(m_total, v);
                return true;
            } 
            catch (std::bad_alloc&)
            {
                return false;
            }
        }
        
        /*! \brief Clears & resets the structure
        */
        void    clear()
        {
            *this   = {};
        }

        template <typename=void>
        requires (DIMS == 2)
        range_type          columns() const
        {
            return range(m_softLo.j, m_softHi.j);
        }


        /*! \brief Recomputes dimensions & stride
        
            This computes the strides, dimensions, and total size required.
        */
        void                compute()
        {
            m_dim           = (size_c) m_count;
            if constexpr (GHOST)
                m_dim       = m_dim  + (size_c) m_ghostLo + (size_c) m_ghostHi;
            m_stride        = impl::array_comingle<DIMS>(m_dim, m_order);
            m_total         = product(m_dim);
            m_softLo        = m_zero;
            m_softHi        = m_zero + m_count;
            m_hardLo        = m_zero - m_ghostLo;
            m_hardHi        = m_softHi + m_ghostHi;
            m_compute       = false;
        }

        /*! \brief Counts per dimension
        
            These are the user specified counts (below). 
        */
        constexpr const COORD&  count() const noexcept
        {
            return m_count;
        }
        
        
        /*! \brief Total count per dimension
        
            This is the count plus any ghost, updated with compute().
        */
        constexpr const COORD&  dim() const noexcept
        {
            return m_dim;
        }
        
        /*! \brief Reference to the underlying data
        */
        constexpr const DATA&   data() const noexcept
        { 
            return m_data; 
        }

        /*! \brief Checks for an empty container
        
            Checks the underlying container, returning TRUE if empty
        */
        constexpr bool          empty() const noexcept
        {
            return m_data.empty();
        }
        
        
        
        /*! \brief Ghost on high-side
        
            This is the current ghost configuration for the high-side of indices.
        */
        constexpr const COORD&  ghost_high() const noexcept
        { 
            return m_ghostHi; 
        }


        /*! \brief Ghost on low-side
        
            This is the current ghost configuration for the low-side of indices.
        */
        constexpr const COORD&  ghost_low() const noexcept
        { 
            return m_ghostLo; 
        }

        /*! \brief FULL upper-index value
        
            Always available, this routine returns the first invalid coordinate on the high side per dim.
        */
        constexpr const COORD&  hard_high() const noexcept
        {
            return m_hardHi;
        }

        /*! \brief FULL lower-index value
        
            Always available, this routine returns the lower valid coordinate per dim.
        */
        constexpr const COORD&  hard_low() const noexcept
        {
            return m_hardLo;
        }
        
        /*! \brief FULL lower-index value
        
            Always available, this routine returns the lower valid coordinate per dim.
        */
        constexpr const COORD&  hard_lower() const noexcept
        {
            return m_hardLo;
        }

        constexpr ranges_type hard_ranges() const noexcept
        {
            return range(m_hardLo, m_hardHi);
        }

        constexpr ranges_type hard_ranges(const COORD& bumper) const noexcept
        {
            COORD   L   = hard_low();
            COORD   U   = hard_upper() - bumper;
            return range(L, max(L,U));
        }

        constexpr ranges_type hard_ranges(index_type bumper) const noexcept
        {
            return hard_ranges(impl::uniform_coord<index_type, DIMS>(bumper));
        }

        /*! \brief FULL upper-index value
        
            Always available, this routine returns the first invalid coordinate on the high side per dim.
        */
        constexpr const COORD&  hard_upper() const noexcept
        {
            return m_hardHi;
        }

        constexpr const COORD & high() const noexcept
        {
            return m_softHi;
        }
        
        /*! \brief Index mapping function
        
            \note No bounds checking is done, an index is returned.
        */
        constexpr size_t    index(const COORD& c) const noexcept
        {
            COORD   l;
            if constexpr (!GHOST && !ORIGIN){
                l   = c;
            } else
                l   = c - m_hardLo;
            return sum( m_stride * (size_c) l );
        }
        
        /*! \brief Tests for a coordinate in the ghost-region
        */
        constexpr bool      is_ghost(const COORD& c) const noexcept
        {
            if constexpr (GHOST){
                return is_valid(c) && !is_interior(c);
            } else {
                return false;
            }
        }
        
        /*! \brief Tests for a coordinate in the interior (non-ghost)
        */
        constexpr bool      is_interior(const COORD&c) const noexcept
        {
            if constexpr (ORIGIN || is_signed){
                return all_greater_equal(m_softLo, c) && all_less(c, m_softHi);
            } else {
                return all_less(c, m_softHi);
            }
        }
        
        /*! \brief Tests for a coordinate outside the interior
        
            (Should be opposite of is_valid)
        */
        constexpr bool  is_exterior(const COORD& c) const noexcept
        {
            return !is_valid(c);
        }
        
        /*! \brief Checks for a good array
        
            A good array is one whose parameters are in good standing AND the container's size 
            has the minimum number of elements.
        */
        constexpr bool  is_good() const noexcept
        {
            return (m_data.size() >= m_total) && !m_compute;
        }

        /*! \brief Tests that coordinate is either interior or ghots
        */
        constexpr bool  is_valid(const COORD& c) const noexcept
        {
            if constexpr (GHOST || ORIGIN || is_signed){
                return all_greater_equal(m_hardLo, c) && all_less(c, m_hardHi);
            } else {
                return all_less(c, m_hardHi);
            }
        }

        /*! \brief Low-side
        
            This is the low-side coordinate for each dimension.  
            (Only valid for non-ghost).
        */
        constexpr const COORD & low() const noexcept
        {
            return m_softLo;
        }
        
        template <typename F>
        requires (std::is_floating_point_v<F> && trait::can_add_v<value_type> && trait::can_two_multiply_v<value_type,F>)
        value_type  linear(const COORD& c, const Coord<F,DIMS>& frac) const
        {
            value_type  ret = {};
            linear_march(index(c), frac, 0, (F) 1., [&](size_t idx, F f){
                ret += f * m_data[idx];
            });
            return ret;
        }
        

        /*! \brief Needs a compute() call
        
            If a "set_()" call routine alters the general shape of the array (ie, dimensions, ghosts),
            then this will return TRUE until compute() is called.  This allows the user to control
            the data & rebuild process.
        */
        bool    needs_compute() const noexcept
        {
            return m_compute;
        }

        /*! \brief Permutation order 
        */
        constexpr uint8_c   permutation() const noexcept
        { 
            return m_order; 
        }
        
        constexpr ranges_type ranges() const noexcept
        {
            return range(low(), high());
        }

        /*! \brief Ranges with a bumper
        
            The bumper can be used to not hit the edge of the domain (ie, 2)
        */
        constexpr ranges_type ranges(const COORD& bumper) const noexcept
        {
            COORD   L   = low();
            COORD   U   = high() - bumper;
            return range(L, max(L,U));
        }

        /*! \brief Ranges with a bumper
        
            The bumper can be used to not hit the edge of the domain (ie, 2)
        */
        constexpr ranges_type ranges(index_type bumper) const noexcept
        {
            return ranges(impl::uniform_coord<index_type, DIMS>(bumper));
        }

        /*! \brief Resizes a vector array
        
            This resizes a vector-based array, equivalent to set_limits() & build().
        */
        template <typename=void>
        requires (is_vector && ORIGIN && !GHOST)
        void    resize(const COORD&a, const COORD& b, const value_type& v = {})
        {
            set_limits(a, b);
            build(v);
        }

        /*! \brief Resizes a vector array
        
            This resizes a vector-based array, equivalent to set_limits() & build().
        */
        template <typename=void>
        requires (is_vector && !ORIGIN && !GHOST)
        void    resize(const COORD&sz, const value_type& v = {})
        {
            set_count(sz);
            build(v);
        }

        template <typename=void>
        requires (DIMS == 2)
        range_type          rows() const
        {
            return range(m_softLo.j, m_softHi.j);
        }

        /*! \brief Set the counts for the array
        
            This is LIKELY what you want.  
            Resizes the count, requires a recompute & rebuild.
            
            \note It'll force the counts to be at least one
        */
        void    set_count(const COORD& v) noexcept
        {
            m_count     = max(v, impl::uniform_coord<index_type, DIMS>(1));
            m_compute   = true;
        }
        
        /*! \brief Sets the count (uniform)
        
            \note It'll force the counts to be at least one
        */
        void    set_count(index_type v) noexcept
        {
            m_count     = max(v, index_type{1});
            m_compute   = true;
        }
    
    
        /*! \brief Sets the data object (copies)
        */
        void    set_data(const DATA& d) 
        {
            m_data      = d;
        }
        
        /*! \brief Sets the data object (moves)
        */
        void    set_data(DATA&& d) 
        {
            m_data      = std::move(d);
        }

        /*! \brief Sets the ghost 
        
            This sets the ghost on dimension-by-dimension basis, 
            both low & high will be set to the same values.

            \note A rebuild/reallocation will be required after this call.
        */
        template <typename=void>
        requires GHOST
        void    set_ghost(const COORD&v) noexcept
        {
            m_ghostLo   = m_ghostHi     = std::max(v, {});
            m_compute   = true;
        }
        
        /*! \brief Sets the ghost 
        
            This sets the ghost on a uniform value basis,
            both low & high will be set to this value.
        */
        template <typename=void>
        requires GHOST
        void    set_ghost(index_type v) noexcept
        {
            m_ghostLo   = m_ghostHi     = std::max(v, {});
            m_compute   = true;
        }

        /*! \brief Sets the high-side ghost 
        
            This sets the high-side ghost on dimension-by-dimension basis.

            \note A rebuild/reallocation will be required after this call.
        */
        template <typename=void>
        requires GHOST
        void    set_ghost_high(const COORD&v) noexcept
        {
            m_ghostHi   = max(v, {});
            m_compute   = true;
        }
        
        /*! \brief Sets the high-side ghost 
        
            This sets the high-side ghost uniformly to the specified value.

            \note A rebuild/reallocation will be required after this call.
        */
        template <typename=void>
        requires GHOST
        void    set_ghost_high(index_type v) noexcept
        {
            m_ghostHi   = max(v, {});
            m_compute   = true;
        }

        /*! \brief Sets the low-side ghost 
        
            This sets the low-side ghost on dimension-by-dimension basis.

            \note A rebuild/reallocation will be required after this call.
        */
        template <typename=void>
        requires GHOST
        void    set_ghost_low(const COORD&v) noexcept
        {
            m_ghostLo   = max(v, {});
            m_compute   = true;
        }

        /*! \brief Sets the low-side ghost 
        
            This sets the low-side ghost uniformly to the specified value.

            \note A rebuild/reallocation will be required after this call.
        */
        template <typename=void>
        requires GHOST
        void    set_ghost_low(index_type v) noexcept
        {
            m_ghostLo   = max(v, {});
            m_compute   = true;
        }

        /*! \brief Sets counts & zero based on the values
        */
        template <typename=void>
        requires ORIGIN
        void    set_limits(const COORD& a, const COORD& b) noexcept
        {
            COORD L = min(a,b);
            COORD U = max(a,b);
            m_zero      = L;
            m_count     = U - L;
            m_compute   = true;
        }
        
        /*! \brief Sets the low-side "zero"
        
            This is the zero-index for the array, ie, the low-side.
        */
        template <typename=void>
        requires ORIGIN
        void    set_low(const COORD& v) noexcept
        {
            m_zero      = v;
        }
    
        /*! \brief Sets the low-side "zero"
        
            This is the zero-index for the array, ie, the low-side.
        */
        template <typename=void>
        requires ORIGIN
        void    set_low(index_type v) noexcept
        {
            m_zero      = v;
            m_compute   = true;
        }
        
        /*! \brief Sets the permutation order

            \note A rebuild/reallocation will be required after this call.
        */
        void    set_permutation(const uint8_c& v) noexcept
        {
            m_order     = v;
            m_compute   = true;
        }

        /*! \brief Sets permutation to ascending
        
            In 2D this will result in row-major (default)

            \note A rebuild/reallocation will be required after this call.
        */
        void    set_permutation_ascending() noexcept
        {
            set_permutation(impl::coord_ascending<DIMS>());
        }
        
        /*! \brief Sets permutation to descending
        
            In 2D this will result in column-major.

            \note A rebuild/reallocation will be required after this call.
        */
        void    set_permutation_descending() noexcept
        {
            set_permutation(impl::coord_descending<DIMS>());
        }
        
        

        /*! \brief Number of elements in the data
        */
        constexpr size_t  size() const  noexcept{ return m_data.size(); }
        

        /*! \brief Interior upper-index value
        
            Always available, this routine returns the first non-interior 
            coordinate on the high side per dim.
        */
        constexpr const COORD& soft_high() const noexcept
        {
            return m_softHi;
        }

        /*! \brief Interior lower-index value
        
            Always available, this routine returns the lower interior coordinate per dim.
        */
        constexpr const COORD&    soft_low() const noexcept
        {
            return m_softLo;
        }

        /*! \brief Interior lower-index value
        
            Always available, this routine returns the lower interior coordinate per dim.
        */
        constexpr const COORD&    soft_lower() const noexcept
        {
            return m_softLo;
        }

        constexpr ranges_type soft_ranges() const noexcept
        {
            return range(soft_low(), soft_high());
        }
        
        constexpr ranges_type soft_ranges(const COORD& bumper) const noexcept
        {
            COORD L = soft_low();
            COORD U = soft_high() - bumper;
            return range(L, max(L,U));
        }

        constexpr ranges_type soft_ranges(index_type bumper) const noexcept
        {
            return soft_ranges(impl::uniform_coord<index_type, DIMS>(bumper));
        }

        /*! \brief Interior upper-index value
        
            Always available, this routine returns the first non-interior 
            coordinate on the high side per dim.
        */
        constexpr const COORD& soft_upper() const noexcept
        {
            return m_softHi;
        }

        constexpr size_t    total_bytes() const noexcept
        {
            return m_total * sizeof(value_type);
        }


        /*! \brief Total number of elements (required)
        
            This is the TOTAL number of data elements needed to 
            meet what's been specified.
        */
        constexpr size_t    total_elements() const noexcept
        {
            return m_total;
        }
        
        /*! \brief TRUE if the array's data is undersized.
        */
        constexpr bool    undersized() const noexcept
        {
            return size() < total_elements();
        }
        
        const value_type&    operator()( const COORD& c) const 
        {
            size_t  idx = index(c);
            if(idx >= m_data.size()) [[unlikely]]
                return impl::thread_safe_bad_data<value_type>();
            return m_data[idx];
        }
        
        template <typename=void>
        requires is_mutable
        value_type&    operator()( const COORD& c)
        {
            size_t  idx = index(c);
            if(idx >= m_data.size()) [[unlikely]]
                return impl::thread_safe_bad_data<value_type>();
            return m_data[idx];
        }

        template <typename=void>
        requires (DIMS==1)
        const value_type&   operator()( index_type i ) const
        {
            return operator()({ i });
        }

        template <typename=void>
        requires (is_mutable && DIMS==1)
        value_type&   operator()( index_type i ) 
        {
            return operator()({ i });
        }

        template <typename=void>
        requires (DIMS==2)
        const value_type&   operator()( index_type i, index_type j ) const
        {
            return operator()({ i, j });
        }

        template <typename=void>
        requires (is_mutable && DIMS==2)
        value_type&   operator()( index_type i, index_type j ) 
        {
            return operator()({ i, j });
        }

        template <typename=void>
        requires (DIMS==3)
        const value_type&   operator()( index_type i, index_type j, index_type k ) const
        {
            return operator()({ i, j, k });
        }

        template <typename=void>
        requires (is_mutable && DIMS==3)
        value_type&   operator()( index_type i, index_type j, index_type k ) 
        {
            return operator()({ i, j, k });
        }

        template <typename=void>
        requires (DIMS==4)
        const value_type&   operator()( index_type i, index_type j, index_type k, index_type l ) const
        {
            return operator()({ i, j, k, l });
        }

        template <typename=void>
        requires (is_mutable && DIMS==4)
        value_type&   operator()( index_type i, index_type j, index_type k, index_type l ) 
        {
            return operator()({ i, j, k, l });
        }

        template <typename=void>
        requires (DIMS==5)
        const value_type&   operator()( index_type i, index_type j, index_type k, index_type l, index_type m ) const
        {
            return operator()({ i, j, k, l, m });
        }

        template <typename=void>
        requires (is_mutable && DIMS==5)
        value_type&   operator()( index_type i, index_type j, index_type k, index_type l, index_type m ) 
        {
            return operator()({ i, j, k, l, m });
        }

        template <typename=void>
        requires (DIMS==6)
        const value_type&   operator()( index_type i, index_type j, index_type k, index_type l, index_type m, index_type n ) const
        {
            return operator()({ i, j, k, l, m, n });
        }

        template <typename=void>
        requires (is_mutable && DIMS==6)
        value_type&   operator()( index_type i, index_type j, index_type k, index_type l, index_type m, index_type n )
        {
            return operator()({ i, j, k, l, m, n });
        }

        const value_type&   operator[]( size_t idx ) const
        {
            if(idx >= m_data.size()) [[unlikely]]
                return impl::thread_safe_bad_data<value_type>();
            return m_data[idx];
        }
        
        value_type&         operator[]( size_t idx ) 
        {
            if(idx >= m_data.size()) [[unlikely]]
                return impl::thread_safe_bad_data<value_type>();
            return m_data[idx];
        }

        constexpr auto begin() noexcept { return m_data.begin(); }
        constexpr auto end() noexcept { return m_data.end(); }
        constexpr auto begin() const noexcept { return m_data.begin(); }
        constexpr auto end() const noexcept { return m_data.end(); }
        constexpr auto cbegin() const noexcept { return m_data.cbegin(); }
        constexpr auto cend() const noexcept { return m_data.cend(); }

        constexpr auto rbegin() noexcept { return m_data.begin(); }
        constexpr auto rend() noexcept { return m_data.end(); }
        constexpr auto rbegin() const noexcept { return m_data.begin(); }
        constexpr auto rend() const noexcept { return m_data.end(); }
        constexpr auto crbegin() const noexcept { return m_data.cbegin(); }
        constexpr auto crend() const noexcept { return m_data.cend(); }

        Array() = default;
        Array(const DATA& cp) : m_data(cp) {}
        Array(DATA&& mv) : m_data(std::move(mv)) {}
        Array(const Array&) = default;
        Array(Array&&) = default;
        Array& operator=(const Array&) = default;
        Array& operator=(Array&&) = default;
        ~Array() = default;
        bool    operator==(const Array&) const = default;
    
    private:
        
        DATA        m_data;
        
        size_c      m_stride    = {};   // multipliers for coord -> index
        size_c      m_dim       = {};   // total sizes per axis

        union {
            COORD   m_zero      = {};
            COORD   m_softLo;
        };
        
        COORD       m_count     = {};   // our size (as user-defined)
        COORD       m_ghostLo   = {};
        COORD       m_ghostHi   = {};
        COORD       m_softHi    = {};
        COORD       m_hardLo    = {};
        COORD       m_hardHi    = {};
        size_t      m_total     = 0ULL; //  total size (required by numbers)
        uint8_c     m_order     = {};
        bool        m_compute   = false;
        
        template <typename F, typename Pred>
        requires (std::is_floating_point_v<F> && trait::can_add_v<value_type> && trait::can_two_multiply_v<value_type,F>)
        void    linear_march(uint64_t idx, const Coord<F, DIMS>& frac, uint8_t n, F prod, Pred pred)
        {
            if(n<DIMS){
                F   g   = get(frac, n);
                linear_march<F,Pred>(idx, n+1, prod*((F) 1.-g), pred);
                linear_march<F,Pred>(idx+get(m_stride, n), n+1, prod*g, pred);
            } else {
                pred(idx, prod);
            }
        }
    };
}
