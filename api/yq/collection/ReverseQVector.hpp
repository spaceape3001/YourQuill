////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include <QVector>

namespace yq {

    template <typename T>
    struct ReverseQVector {
        //ReverseQVector(QVector<T>& _c) : c(_c) {}

        class iterator {
        public:

            iterator() {}
            iterator(int n, QVector<T>& _c) : value(n), c(_c) {}

            iterator& operator++()
            {
                ++ value;
                return *this;
            }

            bool operator != (const iterator& b) const
            {
                return value != b.value;
            }

            T& operator*() { return c[c.size()-value-1]; }
        private:
            int     value;
            QVector<T>& c;
        };

        ReverseQVector(QVector<T>& _c) : c(_c) {}
        iterator    begin() { return iterator(0, c); };
        iterator    end() { return iterator(c.size(), c); }

    private:
        QVector<T> & c;
    };

    template <typename T>
    ReverseQVector<T> reverse(QVector<T>& c)
    {
        return ReverseQVector<T>(c);
    }
}
