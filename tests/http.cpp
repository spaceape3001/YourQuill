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
    
    "versions match"_test = [](){
        expect( match(http11(), http10()) == false );
        expect( match(http10(), http10()) == true );
        expect( match(http09(), http09()) == true );
        expect( match(parse_version("HtTP/1.0"), http10()) == true);
    };
    
    "method line"_test = [](){
        expect( MethodUriVersion{ "get", "http://www.google.com", "" } == parse_method_uri( "get http://www.google.com" ));
        expect( MethodUriVersion{ "delete", "http://www.google.com", "http/1.2" } == parse_method_uri( "delete http://www.google.com  http/1.2 " ));
    };
    
    "headers"_test = [](){
        expect( HttpHeaderView{ "User-Agent", "Dummy" } == parse_header_line("  User-Agent :  Dummy "));
    };
    
    "uri_parse"_test = [](){
    
        expect( UriView{ "http", "", "", "www.google.com",  "/test.php", "", "", 0 } == parse_uri("http://www.google.com/test.php"));
        expect( UriView{ "ftp", "nobody", "",  "www.google.com", "/test.php", "", "", 0 } == parse_uri("ftp://nobody@www.google.com/test.php"));
        expect( UriView{ "ftp", "nobody", "secret",  "www.google.com", "/test.php", "foobar=true", "hello", 8080 } == parse_uri("ftp://nobody:secret@www.google.com:8080/test.php?foobar=true#hello"));
        expect( UriView{ "https", "", "",  "www.google.com", "/test.php", "foobar=true", "hello", 8080 } == parse_uri("https://www.google.com:8080/test.php?foobar=true#hello"));
        expect( UriView{ "file", "", "",  "", "/test.php", "foobar=true", "hello", 0 } == parse_uri("file:///test.php?foobar=true#hello"));
        expect( UriView{ "", "", "",  "", "/test.php", "foobar=true", "hello", 0 } == parse_uri("///test.php?foobar=true#hello"));
        expect( UriView{ "", "", "",  "", "/test.php", "foobar=true", "hello", 0 } == parse_uri("/test.php?foobar=true#hello"));
        expect( UriView{ "", "", "",  "", "./test.php", "foobar=true", "hello", 0 } == parse_uri("./test.php?foobar=true#hello"));
        
        auto p  = parse_uri("/");
        
        expect( UriView{ "", "", "",  "", "/", "", "", 0 } == p);
    };
    
    "uri_bad"_test = [](){
        expect( false == parse_uri("foobar.txt").good );
        expect( false == parse_uri("http[]//hello/foobar.txt").good );
    };
    
    "hostport parse"_test = [](){
        expect( UriView("", "", "", "localhost", "", "", "", 12345) == parse_hostport("localhost:12345"));
        expect( UriView("", "", "", "localhost", "", "", "", 0) == parse_hostport("localhost"));
    };
};


int main(){
    return ut::cfg<>.run();
}
