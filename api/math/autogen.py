################################################################################
##
##  YOUR QUILL
##
################################################################################

##  NOTICE, PYTHON 3.9 or LATER REQUIRED!

import io, sys

#   Configuration

DIMS    = [ 'x', 'y', 'z', 'w' ]
N       = len(DIMS)
VNAMES  = [ 'Vec', 'Bi', 'Tri', 'Quad' ]
VNAME   = 'Vec'
TNAME   = 'Ten'
MNAME   = 'Mul'

DATA    = [
    #{ 'key':'C', 'type':'std::complex<double>', 'header':'<complex>' },
    { 'key':'D', 'type':'double' },
    { 'key':'F', 'type':'float' },
    { 'key':'I', 'type':'int' },
    { 'key':'U', 'type':'unsigned' }
]



# class DType:
    # def __init__(self, key, dtype):
        # self.key    = key
        # self.dtype  = dtype

# DATA    = [ DType('C', 'std::complex<double>'), DType('D', 'double'), DType('F', 'float'), DType('I', 'int'), DType('U', 'unsigned') ]

class DType:
    def __init__(self, par, dinfo):
        self.parent = par
        self.args   = par.args | dinfo
        self.name   = '%s%s' % (par.name, dinfo['key'])
        self.args['name']   = self.name
        self.args['lower']  = self.name.lower()

class Bit:
    def __init__(self, n, args=None):
        self.n              = n
        if isinstance(n, int):
            self.bit        = DIMS[n];
            self.bits       = [self.bit]
        if isinstance(n, str):
            self.bit        = n
            self.bits       = [n]
        if isinstance(n, list):
            self.bit        = ''
            for b in n:
                self.bit    = self.bit + DIMS[b]
        self.args       = dict();
        self.args['bit']    = self.bit
        if args is not None:
            self.args       = args | self.args
        self.first          = False


class VType:
    def __init__(self, n):
        self.n      = n
        self.name   = '%s%d' % (VNAME, n)
        self.header = '%s.hpp' % self.name
        self.args   = dict()
        self.args['name']   = self.name
        self.args['vector'] = self.name
        self.args['header'] = self.header
        self.args['N']      = n

        self.bits   = []
        for i in range(0,n):
            self.bits.append(Bit(i, self.args))
        self.bits[0].first  = True

        self.data   = []
        for d in DATA:
            self.data.append(DType(self, d))
        
        
class TType:
    def __init__(self, n, m):
        self.n      = n
        self.m      = m
        self.rows   = DIMS[ 0:n ]
        self.cols   = DIMS[ 0:m ]
        self.diag   = n == m
        self.name   = '%s%d%d' % (TNAME, n, m)
        self.header = '%s.hpp' % self.name
        self.bits   = []
        self.args   = dict()
        self.args['name']   = self.name
        self.args['tensor'] = self.name
        self.args['header'] = self.header
        self.args['N']      = n
        self.args['M']      = m

        for r in range(0, n):
            for c in range(0, m):
                self.bits.append(Bit([r,c], self.args))
        self.bits[0].first  = True

        self.data   = []
        for d in DATA:
            self.data.append(DType(self, d))
        
VECTORS = [  ]
for n in range(0,N):
    VECTORS.append(VType(n+1))

TENSORS = []
for n in range(0,N):
    for m in range(0,N):
        t       = TType(n+1,m+1)
        t.rowv  = VECTORS[m]
        t.colv  = VECTORS[n]
        t.args['rowv']  = t.rowv.name
        t.args['colv']  = t.colv.name
        TENSORS.append(t)

YQUILL  = """////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////
"""

ONCE    = "#pragma once\n"

WARNING = """
//  WARNING!  WARNING!
//
//  THIS FILE IS AUTO-GENERATED, CHANGES WILL BE CLOBBERED!
"""

    ############################################
    ##  FORWARD DECLARATIONS

with open('forward.hpp', 'w') as f:
    f.write(YQUILL)
    f.write(ONCE)
    f.write(WARNING)
    f.write("""

namespace std { template <typename> class complex; }

namespace yq {

""")

    for t in TENSORS:
        f.write("    template <typename T> struct %(tensor)s;\n" % t.args)
        for d in t.data:
            f.write("    using %(name)s = %(tensor)s<%(type)s>;\n" % d.args)
        f.write('\n')

    for v in VECTORS:
        f.write("    template <typename> struct %(vector)s;\n" % v.args)
        for d in v.data:
            f.write("    using %(name)s = %(vector)s<%(type)s>;\n" % d.args)
        f.write('\n')


    f.write("""
}
""")

    ############################################
    ##  VECTOR FILES

vmath = open("vector_python.hpp", 'w')
vmath.write(YQUILL)
vmath.write(ONCE)
vmath.write(WARNING)
vmath.write('\n')

for v in VECTORS:
    vmath.write("#include <math/vec/%s>\n" % v.header)
for t in TENSORS:
    vmath.write("#include <math/vec/%s>\n" % t.header)
    
vmath.write("""

namespace yq {
""")

for v in VECTORS:
    with open('vec/%s' % v.header, 'w') as f:
        f.write(YQUILL)
        f.write(ONCE)
        f.write(WARNING)
        f.write("""
#define YQ__API__MATH__VECTOR_%(N)d__HPP 1
#include <math/preamble.hpp>

namespace yq {
    /*! \\brief Vector of %(N)d dimension(s)
    
        This is a %(N)d dimensional cartesian vector of the given type.
        \\tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct %(vector)s {
        //! Component data type argument to this structure (ie, template parameter T)
        using component_type = T;
""" % v.args)
        for b in v.bits:
            f.write("""
        /*! \\brief Creates a unit-vector in the $(bit)s dimension.
        */
        static consteval %(vector)s unit_%(bit)s();""" % b.args)
                    
        f.write('\n')
        for b in v.bits:
            f.write("""
        //! Component for the $(bit)s dimension.
        T       %(bit)s;""" % b.args)
        f.write(""";
        
        //! Equality operator (using default)
        constexpr bool operator==(const %(vector)s&) const noexcept = default;
    };
    
//  --------------------------------------------------------
//  COMPOSITION

    /*! \\brief Creates a $(N)d dimensioal vector
    
        Helper function to create a $(N) dimensional cartesian vector where the component type is deduced from
        the first argument.
    */
    template <typename T>
    constexpr %(vector)s<T> vec(""" % v.args)
        for b in v.bits:
            if b.first:
                f.write("T %(bit)s" % b.args)
            else:
                f.write(", std::type_identity_t<T> %(bit)s" % b.args)
        f.write(""")
    {
        return {""")
        for b in v.bits:
            if not b.first:
                f.write(',')
            f.write(b.bit)
        f.write("""};
    }
    """)
        for b in v.bits:
            f.write("""
    template <typename T>
    consteval %(vector)s<T> %(vector)s<T>::unit_%(bit)s()
    {
        return {""" % b.args)
            for bb in v.bits:
                if not bb.first:
                    f.write(',')
                if b == bb:
                    f.write('one_v<T>')
                else:
                    f.write('zero_v<T>')
            f.write("""};
    }
""")
        
        f.write("""
    YQ_NAN_1(%(vector)s, %(vector)s<T>{""" % v.args)
        for b in v.bits:
            if not b.first:
                f.write(", ")
            f.write("nan_v<T>")
        f.write("""})
    YQ_ZERO_1(%(vector)s, %(vector)s<T>{""" % v.args)
        for b in v.bits:
            if not b.first:
                f.write(", ")
            f.write("zero_v<T>")
        f.write("""})
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS

    YQ_IS_NAN_1(%(vector)s, """ % v.args)
        for b in v.bits:
            if not b.first:
                f.write(" || ");
            f.write("is_nan(v.%(bit)s)" % b.args)
        f.write(""")
    YQ_IS_FINITE_1(%(vector)s, """ % v.args)
        for b in v.bits:
            if not b.first:
                f.write(" && ");
            f.write("is_finite(v.%(bit)s)" % b.args)
        f.write(""")
        
    /*! \\brief Square of the vector's length
    
        This returns the SQUARE of the given vector's length.
    */
    template <typename T>
    constexpr square_t<T> length2(const %(vector)s<T>& a)
    {
        return """ % v.args);
        for b in v.bits:
            if not b.first:
                f.write(" + ");
            f.write("a.%(bit)s*a.%(bit)s" % b.args)
        
        f.write(""";
    }    
    
    /*! \\brief Length of the vector
        
        This returns the length of the given vector.
    */""" % v.args)
        if v.n == 1:
        
            f.write("""
    template <typename T>
    requires trait::has_abs_v<T>
    auto    length(const %(vector)s<T>& a)
    {
        return abs(a.%(bit)s);
    }
""" % v.bits[0].args)
        else:
            f.write("""
    template <typename T>
    requires trait::has_sqrt_v<T>
    auto    length(const %(vector)s<T>& a)
    {
        return sqrt(length2(a));
    }
""" % v.args)            
                    
        f.write("""
//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr %(vector)s<T> operator+(const %(vector)s<T>& a)
    {
        return a;
    }

//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr %(vector)s<T> operator-(const %(vector)s<T>&a)
    {
        return {""" % v.args)
        for b in v.bits:
            if not b.first:
                f.write(',')
            f.write("-a.%s" % b.bit)
        f.write("""};
    }

//  --------------------------------------------------------
//  NORMALIZATION
""")        
        if v.n == 1:
            f.write("""
    template <typename T>
    requires trait::has_copysign_v<T>
    %(vector)s<T> operator~(const %(vector)s<T>& a)
    {
        return %(vector)s<T>(copysign(one_v<T>, a.x));
    }
""" % v.bits[0].args)
        else:
            f.write("""
    template <typename T>
    requires trait::has_sqrt_v<T>
    %(vector)s<quotient_t<T,T>> operator~(const %(vector)s<T>& a)
    {
        auto l = one_v<T>/length(a);
        return {""" % v.args)
            for b in v.bits:
                if not b.first:
                    f.write(", ")
                f.write("a.%s/l" % b.bit)
            f.write("""};
    }
""" % v.args)


        f.write("""
//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr %(vector)s<T> operator+(const %(vector)s<T>& a, const %(vector)s<T>& b)
    {
        return {""" % v.args)
        
        for b in v.bits:
            if not b.first:
                f.write(", ");
            f.write("a.%(bit)s+b.%(bit)s" % b.args)
        f.write("""};
    }
    
    template <typename T>
    %(vector)s<T>& operator+=(%(vector)s<T>& a, const %(vector)s<T>& b)
    {
""" % v.args)
        for b in v.bits:
            f.write("        a.%(bit)s += b.%(bit)s;\n" % b.args)
        f.write("""        return a;
    }

//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr %(vector)s<T> operator-(const %(vector)s<T>& a, const %(vector)s<T>& b)
    {
        return {""" % v.args)
        
        for b in v.bits:
            if not b.first:
                f.write(", ");
            f.write("a.%(bit)s-b.%(bit)s" % b.args)
        f.write("""};
    }
    
    template <typename T>
    %(vector)s<T>& operator-=(%(vector)s<T>& a, const %(vector)s<T>& b)
    {
""" % v.args)
        for b in v.bits:
            f.write("        a.%(bit)s -= b.%(bit)s;\n" % b.args)
        f.write("""        return a;
    }


//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr %(vector)s<product_t<T,U>> operator*(T a, const %(vector)s<U>&b)
    {
        return {""" % v.args)
        for b in v.bits:
            if not b.first:
                f.write(", ")
            f.write("a*b.%(bit)s" % b.args)
        f.write("""};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr %(vector)s<product_t<T,U>> operator*(const %(vector)s<T>& a, U b)
    {
        return {""" % v.args)
        for b in v.bits:
            if not b.first:
                f.write(", ");
            f.write("a.%(bit)s*b" % b.args)
        f.write("""};
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    %(vector)s<T>& operator*=(%(vector)s<T>& a, T b)
    {
""" % v.args)
        f.write("        a.%(bit)s *= b;\n" % b.args);
        f.write("""        return a;
    }
""")

        if v.n == 1:
            f.write("""
    template <typename T, typename U>
    product_t<T,U> operator*(const %(vector)s<T>&a, const %(vector)s<U>& b)
    {
        return a.%(bit)s*b.%(bit)s;
    }
""" % v.bits[0].args)

        f.write("""
//  --------------------------------------------------------
//  DIVISION

    template <typename T, typename U>
    requires (std::is_arithmetic_v<T>)
    constexpr  %(vector)s<quotient_t<T,U>> operator/(T a, const  %(vector)s<U>&b)
    {
        return (a*b) / length2(b);
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U>)
    constexpr  %(vector)s<quotient_t<T,U>> operator/(const  %(vector)s<T>& a, U b)
    {
        return {""" % v.args )
        
        for b in v.bits:
            if not b.first:
                f.write(", ");
            f.write("a.%(bit)s / b" % b.args)
        f.write("""};
    }

    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    %(vector)s<T>& operator/=(%(vector)s<T>& a, U b)
    {
""" % v.args)
        for b in v.bits:
            f.write("        a.%(bit)s /= b;\n" % b.args)
        f.write("""        return a;
    }
""")
        if v.n == 1:
            f.write("""
    template <typename T, typename U>
    quotient_t<T,U> operator/(const %(vector)s<T>&a, const %(vector)s<U>& b)
    {
        return (a*b)/length2(b);
    }
""" % v.args)

        f.write("""

//  --------------------------------------------------------
//  DOT PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator DOT (const %(vector)s<T>& a, const %(vector)s<U>&b)
    {
        return """ % v.args)
        for b in v.bits:
            if not b.first:
                f.write(" + ")
            f.write("a.%(bit)s*b.%(bit)s" % b.args);
        f.write(""";
    }

//  --------------------------------------------------------
//  INNER PRODUCT

    template <typename T, typename U>
    constexpr product_t<T,U> operator INNER (const %(vector)s<T>& a, const %(vector)s<U>&b)
    {
        return """ % v.args)
        for b in v.bits:
            if not b.first:
                f.write(" + ")
            f.write("a.%(bit)s*b.%(bit)s" % b.args);
        f.write(""";
    }
}

""")

        for d in v.data:
            f.write("YQ_TYPE_DECLARE(yq::%(name)s)\n" % d.args)
        f.write('\n')
    
        

    ############################################
    ##  TENSOR FILES

tmath   = open("tensor_python.hpp", 'w')
tmath.write(YQUILL)
tmath.write(ONCE)
tmath.write(WARNING)
tmath.write('\n')

for v in VECTORS:
    tmath.write("#include <math/vec/%s>\n" % v.header)
for t in TENSORS:
    tmath.write("#include <math/vec/%s>\n" % t.header)
    
tmath.write("""

namespace yq {
""")


for t in TENSORS:
    with open('vec/%s' % t.header, 'w') as f:
        f.write(YQUILL)
        f.write(ONCE)
        f.write(WARNING)
        f.write("""
#define YQ__API__MATH__TENSOR_%(N)d_%(M)d__HPP 1
#include <math/preamble.hpp>
""" % t.args)

        if t.n < t.m:
            f.write("#include <math/vec/%s>\n" % t.colv.header)
            f.write("#include <math/vec/%s>\n" % t.rowv.header)
        elif t.n > t.m:
            f.write("#include <math/vec/%s>\n" % t.rowv.header)
            f.write("#include <math/vec/%s>\n" % t.colv.header)
        else:
            f.write("#include <math/vec/%s>\n" % t.rowv.header)

        f.write("""
namespace yq {
    /*! \\brief %(N)dx%(M)d second order tensor (ie a matrix)
    
        \\tparam[T]  Data type to be used, recommended to be arithmetic in nature
    */
    template <typename T>
    struct %(tensor)s {
        using component_type = T;
""" % t.args)

        for r in range(0, t.n):
            f.write("        T ");
            for c in range(0, t.m):
                if c != 0:
                    f.write(", ");
                f.write("%s%s" % (DIMS[r], DIMS[c]))
            f.write(";\n");
        
        f.write("""
        constexpr bool operator==(const %(tensor)s&) const noexcept = default;
    };
        
//  --------------------------------------------------------
//  COMPOSITION

    template <typename T>
    constexpr %(tensor)s<T>  columns(""" % t.args)
        for c in range(0, t.m):
            if c != 0:
                f.write(", ")
            f.write("const %s<T>&%s" % (t.colv.name, DIMS[c]))
        f.write(""")
    {
        return {""")
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n            ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write("%s.%s" % (DIMS[c], DIMS[r]))
        f.write("""
        };
    }
""")

        if t.n == t.m:
            f.write("""
    template <typename T>
    constexpr %(tensor)s<T>  diagonal(const %(rowv)s<T>&v)
    {
        return {""" % t.args);        
            for r in range(0, t.n):
                if r != 0:
                    f.write(",")
                f.write("\n            ")
                for c in range(0, t.m):
                    if c != 0:
                        f.write(', ')
                    if c == r:
                        f.write("v.%s" % DIMS[r])
                    else:
                        f.write("zero_v<T>")
            f.write("""
        };
    }
    
    template <typename T>
    constexpr %(tensor)s<T>  diagonal(""" % t.args)
            for r in range(0, t.n):
                if r == 0:
                    f.write("T %s" % DIMS[0])
                else:
                    f.write(", std::type_identity_t<T> %s" % DIMS[r])
            f.write(""")
    {
        return {""")
            for r in range(0, t.n):
                if r != 0:
                    f.write(",")
                f.write("\n            ")
                for c in range(0, t.m):
                    if c != 0:
                        f.write(', ')
                    if c == r:
                        f.write("%s" % DIMS[r])
                    else:
                        f.write("zero_v<T>")
            f.write("""
        };
    }
""")
        f.write("""
    template <typename T>
    constexpr %(tensor)s<T>  rows(""" % t.args)
        for r in range(0, t.n):
            if r != 0:
                f.write(", ")
            f.write("const %s<T>&%s" % (t.rowv.name, DIMS[r]))
        f.write(""")
    {
        return {""")
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n            ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write("%s.%s" % (DIMS[r], DIMS[c]))
        f.write("""
        };
    }
    
    YQ_IDENTITY_1(%(tensor)s, {""" % t.args)
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n        ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                if c == r:
                    f.write("one_v<T>")
                else:
                    f.write("zero_v<T>")
        f.write("""
    })

    YQ_NAN_1(%(tensor)s, {""" % t.args)
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n        ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write("nan_v<T>")
        f.write(""" 
    })
    
    YQ_ZERO_1(%(tensor)s, {""" % t.args)
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n        ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write("zero_v<T>")
        f.write(""" 
     })
    
//  --------------------------------------------------------
//  BASIC FUNCTIONS


    YQ_IS_FINITE_1(%(tensor)s, """ % t.args)
        for r in range(0, t.n):
            if r != 0:
                f.write(" &&")
            f.write("\n        ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(' && ')
                f.write("is_finite(v.%s%s)" % (DIMS[r], DIMS[c]))
        f.write("""
    )
    
    YQ_IS_NAN_1(%(tensor)s,  """ % t.args)
        for r in range(0, t.n):
            if r != 0:
                f.write(" ||")
            f.write("\n        ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(' || ')
                f.write("is_nan(v.%s%s)" % (DIMS[r], DIMS[c]))
        f.write("""
    )
""")

        if t.n == t.m:
            tout = f
        else:
            tout = tmath
        
        tother = "%s%d%d" % ( TNAME, t.m, t.n )
        tout.write("""
    template <typename T>
    constexpr %s<T>  transpose(const %s<T>&v)
    {
        return {""" % (tother, t.name))
        for r in range(0, t.m):
            if r != 0:
                tout.write(",")
            tout.write("\n            ")
                
            for c in range(0, t.n):
                if c != 0:
                    tout.write(', ')
                tout.write("v.%s%s" % (DIMS[c], DIMS[r]))
        tout.write("""
        };
    }
""")
    
        f.write("""
//  --------------------------------------------------------
//  GETTERS
""")
        if t.n == t.m:
            f.write("""
    template <typename T>
    constexpr %(rowv)s<T>  diagonal(const %(tensor)s<T>& v)
    {
        return {""" % t.args)
            for r in range(0,t.n):
                if r != 0:
                    f.write(", ");
                f.write("v.%s%s" % (DIMS[r], DIMS[r]))
            f.write("""};
    }
""")
        
        for c in range(0, t.m):
            f.write("""
    template <typename T>
    constexpr %s<T>  %s_column(const %s<T>&v) 
    {
        return {""" % (t.colv.name, DIMS[c], t.name))
            for r in range(0, t.n):
                if r != 0:
                    f.write(', ')
                f.write('v.%s%s' % (DIMS[r], DIMS[c]))
            f.write("""};
    }
""")
        for r in range(0, t.n):
            f.write("""
    template <typename T>
    constexpr %s<T>  %s_row(const %s<T>&v)
    {
        return {""" % (t.rowv.name, DIMS[r], t.name))
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write('v.%s%s' % (DIMS[r], DIMS[c]))
            f.write("""};
    }
""")

        f.write("""

//  --------------------------------------------------------
//  SETTERS
""")

        if t.n == t.m:
            f.write("""
            
    template <typename T>
    %(tensor)s<T>&  set_diagonal(%(tensor)s<T>& ten, const %(colv)s<T>& v)
    {""" % t.args)
            for r in range(0, t.n):
                t.args['r'] = DIMS[r]
                t.args['c'] = DIMS[r]
                f.write("        ten.%(r)s%(c)s = v.%(r)s;\n" % t.args)
            f.write("""        return ten;
    }

""")
    
        for c in range(0, t.m):
            t.args['c'] = DIMS[c]
            f.write("""
    template <typename T>
    %(tensor)s<T>& set_%(c)s_column(%(tensor)s<T>&ten, const %(rowv)s<T>& v)
    {""" % t.args)
            for r in range(0, t.n):
                t.args['r']   = DIMS[r]
                f.write("""
        ten.%(r)s%(c)s = v.%(r)s;""" % t.args)
            f.write("""
        return ten;
    }
""")

        for r in range(0, t.n):
            t.args['r'] = DIMS[r]
            f.write("""
    template <typename T>
    %(tensor)s<T>& set_%(r)s_row(%(tensor)s<T>&ten, const %(colv)s<T>& v)
    {""" % t.args)
            for c in range(0, t.m):
                t.args['c'] = DIMS[c]
                f.write("""
        ten.%(r)s%(c)s = v.%(c)s;""" % t.args)
            f.write("""
        return ten;
    }
""")

        f.write("""
//  --------------------------------------------------------
//  POSITIVE

    template <typename T>
    constexpr %(tensor)s<T>  operator+(const %(tensor)s<T>& a) 
    { 
        return a; 
    }


//  --------------------------------------------------------
//  NEGATIVE

    template <typename T>
    constexpr %(tensor)s<T>  operator-(const %(tensor)s<T>& a) 
    {
        return {""" % t.args)
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n            ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write("-a.%s%s" % (DIMS[r], DIMS[c]))
        f.write("""
        };
    }


//  --------------------------------------------------------
//  ADDITION

    template <typename T>
    constexpr %(tensor)s<T>   operator+ (const %(tensor)s<T> &a, const %(tensor)s<T> &b) 
    {
        return {""" % t.args)
        
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n            ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                b = "%s%s" % (DIMS[r], DIMS[c])
                f.write("a.%s+b.%s" % (b, b))
            
        f.write("""
        };
    }

    template <typename T>
    %(tensor)s<T>&   operator+=(%(tensor)s<T> &a, const %(tensor)s<T> &b) 
    {""" % t.args)
    
        for r in range(0, t.n):
            f.write("\n        ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write("  ")
                b = "%s%s" % (DIMS[r], DIMS[c])
                f.write("a.%s+=b.%s;" % (b, b))
            
        f.write("""
        return a;
    }


//  --------------------------------------------------------
//  SUBTRACTION

    template <typename T>
    constexpr %(tensor)s<T>   operator- (const %(tensor)s<T> &a, const %(tensor)s<T> &b) 
    {
        return {""" % t.args)
        
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n            ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                b = "%s%s" % (DIMS[r], DIMS[c])
                f.write("a.%s-b.%s" % (b, b))
            
        f.write("""
        };
    }
    

    template <typename T>
    %(tensor)s<T>&   operator-=(%(tensor)s<T> &a, const %(tensor)s<T> &b) 
    {""" % t.args)
    
        for r in range(0, t.n):
            f.write("\n        ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write("  ")
                b = "%s%s" % (DIMS[r], DIMS[c])
                f.write("a.%s-=b.%s;" % (b, b))
            
        f.write("""
        return a;
    }
    
//  --------------------------------------------------------
//  MULTIPLICATION

    template <typename T, typename U>
    requires std::is_arithmetic_v<T>
    constexpr %(tensor)s<product_t<T,U>>  operator*(T a, const %(tensor)s<T>& b)
    {
        return {""" % t.args)
        
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n            ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write("a*b.%s%s" % (DIMS[r], DIMS[c]))

        f.write("""
        };
    }
    
    
    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr %(tensor)s<product_t<T,U>>  operator*(const %(tensor)s<T>& a, U b)
    {
        return {""" % t.args)
        
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n            ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write("a.%s%s*b" % (DIMS[r], DIMS[c]))

        f.write("""
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_mul_v<T,U>)
    %(tensor)s<product_t<T,U>>  operator*=(const %(tensor)s<T>& a, U b)
    {""" % t.args)

        for r in range(0, t.n):
            f.write("\n        ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(" ")
                b = "%s%s" % (DIMS[r], DIMS[c])
                f.write("a.%s*=b;" % b)

        f.write("""        
        return a;
    }
""")

        for k in range(0, N):
            k = k + 1
            if (k == t.m) and (t.m == t.n):
                tout = f
            else:
                tout = tmath

            T2  = "%s%d%d" % (TNAME, t.m, k)
            TP  = "%s%d%d" % (TNAME, t.n, k)
            
            tout.write("""
    template <typename T, typename U>
    constexpr %s<product_t<T,U>> operator*(const %s<T>& a, const %s<U>& b)
    {
        return {""" % (TP, t.name, T2))
        
            for r in range(0, t.n):
                if r != 0:
                    tout.write(',\n')
                for c in range(0, k):
                    if c != 0:
                        tout.write(',')
                    tout.write("\n            ")
                    for i in range(0, t.m):
                        if i != 0:
                            tout.write(" + ")
                        tout.write("a.%s%s*b.%s%s" % (DIMS[r], DIMS[i], DIMS[i], DIMS[c]))
            tout.write("""
        };
    }
    """)
        
            if k == t.m:
                t.args['t2'] = T2
                tout.write("""
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    %(tensor)s<T>& operator*=(%(tensor)s<T>&a, const %(t2)s<U>& b)
    {
        a = a * b;
        return a;
    }
""" % t.args)
    
        f.write("""
        
    template <typename T, typename U>
    constexpr %(colv)s<product_t<T,U>> operator*(const %(tensor)s<T>&a, const %(rowv)s<U>&b)
    {
        return {""" % t.args)
        for r in range(0, t.n):
            if r != 0:
                f.write(',')
            f.write("\n            ")
            for i in range(0, t.m):
                if i != 0:
                    f.write(" + ")
                f.write("a.%s%s*b.%s" % (DIMS[r], DIMS[i], DIMS[i]))
        
        f.write("""
        };
    }

    template <typename T, typename U>
    constexpr %(rowv)s<product_t<T,U>> operator*(const %(colv)s<T>&a, const %(tensor)s<U>&b)
    {
        return {""" % t.args)

        for c in range(0, t.m):
            if c != 0:
                f.write(',')
            f.write("\n            ")
            for i in range(0, t.n):
                if i != 0:
                    f.write(" + ")
                f.write("a.%s*b.%s%s" % (DIMS[i], DIMS[i], DIMS[c]))
        
        f.write("""
        };
    }
""")

        if t.m == t.n:
            f.write("""
    template <typename T, typename U>
    requires trait::self_mul_v<T,U>
    %(rowv)s<T>& operator*=(%(rowv)s<T>&a, const %(tensor)s<U>& b)
    {
        a = a * b;
        return a;
    }
""" % t.args)


        f.write("""
//  --------------------------------------------------------
//  DIVISION


    template <typename T, typename U>
    requires std::is_arithmetic_v<U>
    constexpr %(tensor)s<quotient_t<T,U>>  operator/(const %(tensor)s<T>& a, U b)
    {
        return {""" % t.args)
        
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n            ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write("a.%s%s/b" % (DIMS[r], DIMS[c]))

        f.write("""
        };
    }
    
    template <typename T, typename U>
    requires (std::is_arithmetic_v<U> && trait::self_div_v<T,U>)
    %(tensor)s<quotient_t<T,U>>  operator/=(const %(tensor)s<T>& a, U b)
    {""" % t.args)

        for r in range(0, t.n):
            f.write("\n        ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(" ")
                b = "%s%s" % (DIMS[r], DIMS[c])
                f.write("a.%s/=b;" % b)

        f.write("""        
        return a;
    }

//  --------------------------------------------------------
//  OTIMES PRODUCT

    template <typename T, typename U>
    constexpr %(tensor)s<product_t<T,U>> operator OTIMES(const %(colv)s<T>&a, const %(rowv)s<U>&b)
    {
        return {""" % t.args)
        
        for r in range(0, t.n):
            if r != 0:
                f.write(",")
            f.write("\n            ")
                
            for c in range(0, t.m):
                if c != 0:
                    f.write(', ')
                f.write("a.%s+b.%s" % (DIMS[r], DIMS[c]))

        f.write("""
        };
    }

//  --------------------------------------------------------
//  ADVANCED FUNCTIONS
""")

        if t.n == t.m:
            f.write("""
    /*! \\brief Trace of the %(N)d x %(M)d tensor
    
        \\param[in] a    Tensor to take the trace of
    */
    template <typename T>
    constexpr T     trace(const %(tensor)s<T>& a)
    {
        return """ % t.args)
        
            for i in range(0, t.n):
                if i != 0:
                    f.write("+")
                f.write("a.%s%s" % (DIMS[i], DIMS[i]))
            f.write(""";
    }
""")
        
        f.write("""
}

""")

        for d in t.data:
            f.write("YQ_TYPE_DECLARE(yq::%(name)s)\n" % d.args)
        f.write('\n')
        

    ############################################
    ##  TYPE INSTANTIATIONS

with open('types_python.cpp', 'w') as f:
    f.write(YQUILL)
    f.write(WARNING)
    f.write("""
#include <math/preamble.hpp>

""")
    for t in TENSORS:
        f.write("#include <math/vec/%(header)s>\n" % t.args)
    f.write('\n')
    for v in VECTORS:
        f.write("#include <math/vec/%(header)s>\n" % v.args)

    f.write("""

#include <basic/DelayInit.hpp>
#include <basic/meta/Init.hpp>

using namespace yq;
    
""")
    
    for t in TENSORS:
        for d in t.data:
            f.write("YQ_TYPE_IMPLEMENT(yq::%(name)s)\n" % d.args)
        f.write('\n')

    for v in VECTORS:
        for d in v.data:
            f.write("YQ_TYPE_IMPLEMENT(yq::%(name)s)\n" % d.args)
        f.write('\n')
    
    f.write("""

YQ_INVOKE(
""")

                
    for t in TENSORS:
        for d in t.data:
            f.write("    auto %(lower)s = writer<%(name)s>();\n" % d.args)
            for b in t.bits:
                f.write("""    %(lower)s.property("%(bit)s", &%(name)s::%(bit)s);\n""" % (b.args|d.args))
            f.write('\n')
        f.write('\n')

    for v in VECTORS:
        for d in v.data:
            f.write("    auto %(lower)s = writer<%(name)s>();\n" % d.args)
            for b in v.bits:
                f.write("""    %(lower)s.property("%(bit)s", &%(name)s::%(bit)s);\n""" % (b.args|d.args))
            f.write('\n')
        f.write('\n')

    f.write("""
)
""")


vmath.write("}\n")
vmath.close()

tmath.write("}\n")
tmath.close()

