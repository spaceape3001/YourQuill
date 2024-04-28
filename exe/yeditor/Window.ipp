////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Window.hpp"
#include "DreamMW.hpp"

Window::Window(QWidget*parent) : SubWindow(parent) 
{
}

Window::~Window()
{
}

DreamMW*    Window::dream() const
{
    return static_cast<DreamMW*>(DreamMW::mainWinFor(const_cast<Window*>(this)));
}
