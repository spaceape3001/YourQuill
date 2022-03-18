#include <boost/ut.hpp>
#include <io/http/HttpParser.hpp>
#include <util/text/Utils.hpp>

namespace ut = boost::ut;
using namespace ut;
using namespace yq;

ut::suite tests = []{
    "versions"_test = [](){
        expect( VersionSpec{ "http", 0, 9 } == parse_version("http/0.9") );
        expect( VersionSpec{ "https", 0, 9 } != parse_version("http/0.9") );
        expect( VersionSpec{ "http", 1, 1 } == parse_version("http/1.1") );
        expect( VersionSpec{ "sftp", 0, 9 } == parse_version("sftp/0.9") );
    };
    
    "method line"_test = [](){
        expect( MethodUriVersion{ "get", "http://www.google.com", "" } == parse_method_uri( "get http://www.google.com" ));
        expect( MethodUriVersion{ "delete", "http://www.google.com", "http/1.2" } == parse_method_uri( "delete http://www.google.com  http/1.2 " ));
    };
    
    "headers"_test = [](){
        expect( HttpHeaderView{ "User-Agent", "Dummy" } == parse_header_line("  User-Agent :  Dummy "));
    };
};


int main(){
    return ut::cfg<>.run();
}
