////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    struct TLSVars {
         stefanfrings::HttpRequest*      m_request;
         stefanfrings::HttpResponse*     m_response;
         QDateTime                       m_at;
         QString                         m_time;
         bool                            m_is_local;
         bool                            m_can_edit;
         QByteArray                      m_client;
         HttpOp                          m_op;
         QByteArray                      m_path;
         Session*                        m_session;
         QByteArray                      m_title;
         QByteArray                      m_content;
         Vector<QByteArray>              m_scripts;

         Atom                            m_atom;
         Class                           m_class;
         Directory                       m_directory;
         Document                        m_document;
         Field                           m_field;
         Folder                          m_folder;
         Fragment                        m_fragment;
         Graph                           m_graph;
         Image                           m_icon;
         Leaf                            m_leaf;
         const Page*                     m_page;
         const Root*                     m_root;
         Tag                             m_tag;
         bool                            m_edit_req;
         QString                         m_key;
         unsigned int                    m_columns;
    };
    
    static thread_local TLSVars g_tls{};
}

thread_local stefanfrings::HttpRequest*      &x_request         = g_tls.m_request;
thread_local stefanfrings::HttpResponse*     &x_response        = g_tls.m_response;
thread_local QDateTime                       &x_at              = g_tls.m_at;
thread_local QString                         &x_time            = g_tls.m_time;
thread_local bool                            &x_is_local        = g_tls.m_is_local;
thread_local bool                            &x_can_edit        = g_tls.m_can_edit;
thread_local QByteArray                      &x_client          = g_tls.m_client;
thread_local HttpOp                          &x_op              = g_tls.m_op;
thread_local QByteArray                      &x_path            = g_tls.m_path;
thread_local Session*                        &x_session         = g_tls.m_session;
thread_local QByteArray                      &x_title           = g_tls.m_title;
thread_local QByteArray                      &x_content         = g_tls.m_content;
thread_local Vector<QByteArray>              &x_scripts         = g_tls.m_scripts;

thread_local Atom                            &x_atom            = g_tls.m_atom;
thread_local Class                           &x_class           = g_tls.m_class;
thread_local Directory                       &x_directory       = g_tls.m_directory;
thread_local Document                        &x_document        = g_tls.m_document;
thread_local Field                           &x_field           = g_tls.m_field;
thread_local Folder                          &x_folder          = g_tls.m_folder;
thread_local Fragment                        &x_fragment        = g_tls.m_fragment;
thread_local Graph                           &x_graph           = g_tls.m_graph;
thread_local Image                           &x_icon            = g_tls.m_icon;
thread_local Leaf                            &x_leaf            = g_tls.m_leaf;
thread_local const Page*                     &x_page            = g_tls.m_page;
thread_local const Root*                     &x_root            = g_tls.m_root;
thread_local Tag                             &x_tag             = g_tls.m_tag;
thread_local bool                            &x_edit_req        = g_tls.m_edit_req;
thread_local QString                         &x_key             = g_tls.m_key;
thread_local unsigned int                    &x_columns         = g_tls.m_columns;

void        reset_tls()
{
    g_tls   = TLSVars{};
}

