////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/trait/always_false.hpp>
#include <0/basic/Array.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>


namespace yq::mithril {

    /*! \brief Creates an Array2, column wise
    
        The column wise array is arranged by going down the first column,
        wrapping into the second, etc.
    */
    template <typename T>
    Array2<T>   colwise(const std::vector<T>& data, unsigned int cols)
    {
        if(cols < 1)
            cols    = 1;
        unsigned int         rows    = (data.size() + cols - 1) / cols;
        if(rows < 1)
            rows    = 1;
        Array2<T>   ret(Coord2U{rows, cols});    
        for(unsigned int i=0;i<cols;++i)
            for(unsigned int j=0;j<rows;++j)
        {
            size_t  n   = (size_t) i * (size_t) rows + (size_t) j;
            if(n >= data.size())
                continue;
            ret(j,i)   = data[n];
        }
        return ret;
    }

    template <typename T>
    Array2<T>   rowwise(const std::vector<T>& data, unsigned int cols)
    {
        if(cols < 1)
            cols    = 1;
        unsigned int         rows    = (data.size() + cols - 1) / cols;
        if(rows < 1)
            rows    = 1;
        Array2<T>   ret(Coord2U{rows, cols});    
        for(unsigned int i=0;i<cols;++i)
            for(unsigned int j=0;j<rows;++j)
        {
            size_t  n   = (size_t) j * (size_t) cols + (size_t) i;
            if(n >= data.size())
                continue;
            ret({j,i})   = data[n];
        }
        return ret;
    }
    namespace html {

        template <typename T, typename Pred>
        void        matrix(WebHtml&h, const Array2<T>& values, Pred p)
        {
            for(int i : values.rows()){
                h << "<tr>";
                for(int j : values.columns()){
                    h << "<td>";
                    if constexpr (std::is_invocable_v<Pred, T>){
                        p(values(i,j));
                    } else if constexpr (std::is_invocable_v<Pred, WebHtml&, T>){
                        p(h, values(i,j));
                    } else {
                        static_assert(always_false_v<Pred>, "Argument deduction failed");
                    }
                    h << "</td>";
                }
                h << "</tr>\n";
            }
        }

        template <typename T, typename Pred1, typename Pred2>
        void        matrix(WebHtml&h, const Array2<T>& values, Pred1 p1, Pred2 p2)
        {
            for(int i : values.rows()){
                h << "<tr>";
                for(int j : values.columns()){
                    h << "<td>";
                    if constexpr (std::is_invocable_v<Pred1, T>){
                        p1(values(i,j));
                    } else if constexpr (std::is_invocable_v<Pred1, WebHtml&, T>){
                        p1(h, values(i,j));
                    } else {
                        static_assert(always_false_v<Pred1>, "Argument deduction failed");
                    }
                    h << "</td><td>";
                    if constexpr (std::is_invocable_v<Pred2, T>){
                        p2(values(i,j));
                    } else if constexpr (std::is_invocable_v<Pred2, WebHtml&, T>){
                        p2(h, values(i,j));
                    } else {
                        static_assert(always_false_v<Pred2>, "Argument deduction failed");
                    }
                    h << "</td>";
                }
                h << "</tr>\n";
            }
        }
        
        template <typename T, typename Pred1>
        void        columns(WebHtml&h, const std::vector<T>& values, Pred1 p1)
        {
            matrix(h, colwise(values, h.context().columns()), p1);
        }

        template <typename T, typename Pred1, typename Pred2>
        void        columns(WebHtml&h, const std::vector<T>& values, Pred1 p1, Pred2 p2)
        {
            matrix(h, colwise(values, h.context().columns()), p1, p2);
        }

        template <typename T, typename Pred1>
        void        rows(WebHtml&h, const std::vector<T>& values, Pred1 p1)
        {
            matrix(h, rowwise(values, h.context().columns()), p1);
        }

        template <typename T, typename Pred1, typename Pred2>
        void        rows(WebHtml&h, const std::vector<T>& values, Pred1 p1, Pred2 p2)
        {
            matrix(h, rowwise(values, h.context().columns()), p1, p2);
        }
    }
}
