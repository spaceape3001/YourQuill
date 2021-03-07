////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MarkdownWriter.hpp"
#include "Page.hpp"

Page::Writer        reg_page(HttpOp, const String&, void (*)(MarkdownWriter&));
Page::Writer        reg_dispatcher(HttpOp, const String&, void(*)(MarkdownWriter&, const String&));
