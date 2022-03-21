#include <boost/ut.hpp>
#include <yq/collection/Vector.hpp>
#include <yq/text/Utils.hpp>

namespace ut = boost::ut;
using namespace ut;
using namespace yq;

ut::suite tests = []{
    
    "is alnum"_test =  [](){
        expect(true == is_alnum('a'));
        expect(true == is_alnum('A'));
        expect(true == is_alnum('0'));
        expect(false == is_alnum('+'));
    };
    
    "is alpha"_test = [](){
        expect(true == is_alpha('A'));
        expect(false == is_alpha('1'));
        expect(false == is_alpha('+'));
    };
    
    "is digit"_test = [](){
        expect(true == is_digit('1'));
        expect(false == is_digit('A'));
        expect(false == is_digit('+'));
    };
    
    "is lowercase"_test = [](){
        expect(false == is_lower('0'));
        expect(false == is_lower('A'));
        expect(true == is_lower('a'));
        expect(true == is_lower('z'));
        expect(false == is_lower('+'));
    };

    "is uppercase"_test = [](){
        expect(false == is_upper('0'));
        expect(true == is_upper('A'));
        expect(false == is_upper('a'));
        expect(false == is_upper('z'));
        expect(false == is_upper('+'));
    };

    "bad booleans"_test = [](){
        expect(false == to_boolean("bad").good);
        expect(false == to_boolean("falsey").good);
        expect(false == to_boolean("truy").good);
        expect(false == to_boolean("truey").good);
    };
    
    "false booleans"_test = [](){
        expect(to_boolean("0") == boolean_r{ false, true });
        expect(to_boolean("False") == boolean_r{ false, true });
        expect(to_boolean("false") == boolean_r{ false, true });
        expect(to_boolean("fAlSe") == boolean_r{ false, true });
        expect(to_boolean("NO") == boolean_r{ false, true });
        expect(to_boolean("No") == boolean_r{ false, true });
        expect(to_boolean("no") == boolean_r{ false, true });
        expect(to_boolean("nO") == boolean_r{ false, true });
    };
    
    "true booleans"_test = [](){
        expect(to_boolean("1") == boolean_r{ true, true });
        expect(to_boolean("True") == boolean_r{ true, true });
        expect(to_boolean("true") == boolean_r{ true, true });
        expect(to_boolean("truE") == boolean_r{ true, true });
        expect(to_boolean("YES") == boolean_r{ true, true });
        expect(to_boolean("yes") == boolean_r{ true, true });
    };
    
    "is_similar"_test = [](){
        expect(is_similar("foobar", "FooBar") == true);
        expect(is_similar("foodar", "FooBar") == false);
        expect(is_similar("foobar", "Foo Bar") == false);
    };
    
    "split string"_test = [](){
        expect(split("1 2 3", ' ') == Vector<std::string_view>({ "1", "2", "3" }));
        expect(split("1,2,3", ',') == Vector<std::string_view>({ "1", "2", "3" }));
    };
};


int main(){
    return ut::cfg<>.run();
}
