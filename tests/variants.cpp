#include <boost/ut.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/type/Variant.hpp>

namespace ut = boost::ut;
using namespace ut;
using namespace yq;

bool    string_test(const char* z)
{
    return Variant(z).value<std::string>().value == z;
}

int    parse_int(int n)
{
    std::string_view     z   = to_string(n);
    Variant v   = Variant::parse_me(meta<int>(), z);
    return v.value<int>();
}

suite tests = []{
    "blank variant"_test = [](){
        Variant     v;
        expect(0_u == v.type().id());
        expect(false == (v != v));
        expect(true == (v == v));
    };
    
    "strings"_test = [](){
        expect(true == string_test("foo"));
        expect(true == string_test("foobar"));
    };
    
    "parse int64"_test = [](){
        expect( 0_i == parse_int(0));
        expect( 42_i == parse_int(42));
        expect( -42_i == parse_int(-42));
    };
};


int main(){
    Meta::freeze();
    return ut::cfg<>.run();
}

