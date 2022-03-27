////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/net/Http.hpp>
#include <yq/net/VersionSpec.hpp>
#include <yq/stream/Stream.hpp>
#include <yq/type/Ref.hpp>

#include "HttpData.hpp"

namespace yq {

    class HttpConnection;

    class HttpResponse : public RefCount {
    public:
    
        using ContentVec        = std::vector<HttpDataPtr>;
        
        HttpResponse(VersionSpec vs=http10());
        ~HttpResponse();
        
        ContentVec&     content(ContentType ct = ContentType());
        void            content(const std::string_view&);
        void            content_type(ContentType);

        //  set INTENDED status & version first BEFORE a header
        //  Errors will override....
        void            header(const std::string_view&, const std::string_view&);

        //! Sets intended status & version
        void            status(HttpStatus);
        HttpStatus      status() const { return m_status; }
        
        void            redirect(const std::string_view&, bool permanent=false);
        const VersionSpec&     version() const { return m_version; }
        void                reset();
        
    private:    
        friend class HttpConnection;
        HttpDataPtr         m_reply;   // start of the reply....
        HttpDataPtr         m_header;
        HttpStatus          m_status;
        VersionSpec         m_version;
        ContentVec          m_content;
        ContentType         m_contentType;
    };

}
