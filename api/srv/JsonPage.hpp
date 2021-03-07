////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Page.hpp"

class QJsonArray;
class QJsonDocument;
class QJsonObject;

Page::Writer        reg_page(HttpOp, const String&, void (*)(QJsonArray&));
Page::Writer        reg_page(HttpOp, const String&, QJsonArray (*)());
Page::Writer        reg_page(HttpOp, const String&, void (*)(QJsonDocument&));
Page::Writer        reg_page(HttpOp, const String&, QJsonDocument (*)());
Page::Writer        reg_page(HttpOp, const String&, void (*)(QJsonObject&));
Page::Writer        reg_page(HttpOp, const String&, QJsonObject (*)());
