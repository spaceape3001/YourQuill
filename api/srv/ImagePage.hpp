////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Page.hpp"

class QImage;

Page::Writer        reg_page(HttpOp, const String&, void (*)(QImage&));
Page::Writer        reg_page(HttpOp, const String&, QImage (*)());
