////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Signal.hpp>
#include <engine/Widget.hpp>
#include <math/Vector2.hpp>
#include <functional>
#include <math/RGBA.hpp>
#include <array>
#include <span>
#include <deque>

namespace yq {
    class Stream;

    namespace engine {
        class Undo;
    };

    namespace widget {
    
        /*! \brief Text edit
        
            This is a text edit 
        *//*
            This is loosely based on https://github.com/BalazsJako/ImGuiColorTextEdit, but needed too much cleanup 
            to do anything except rewrite it.
        */
        class TextEdit : public engine::Widget {
            YQ_OBJECT_DECLARE(TextEdit, engine::Widget)
        public:
        
            static constexpr const uint8_t  MAX_TAB = 32;
        
            enum class TabMode : uint8_t {
                TAB     = 0,
                SPACES
            };
            
            enum class WrapMode : uint8_t {
                NONE = 0,
                SOFT,
                HARD
            };
            
            enum class SelectionMode : uint8_t {
                NORMAL = 0,
                WORD,
                LINE,
                VERTICAL
            };
        
            /*! Text edit coordinate
                
                Anything BEYOND 4 billion is out of scope for this widget.  (Even 16-bit is kinda debatable)
            */
            struct Coord {
                uint32_t    line    = 0;
                uint32_t    column  = 0;

                constexpr auto operator<=>(const Coord&) const noexcept = default;
            };

            struct Glyph {
                char32_t    character;
                uint8_t     style     = 0;
            };
            
            struct Line {
                std::vector<Glyph>  glyphs;
                uint8_t             lines = 0;
                
                void            add(std::string_view);
                void            stream(Stream&) const;
                std::string     text() const;
                std::u32string  utf32() const;
            };
            
            //  common styles
            enum class Style : uint8_t {
                Default     = 0,
                Background,
                Caret,
                Selection,
                LineNumber,
                Keyword,
                Number,
                String,
                CharLiteral,
                Punctuation,
                Preprocessor,
                Identifier,
                KnownIdentifier,
                PreprocIdentifier,
                Comment,
                MultiLineComment,
                ErrorMarker,
                Breakpoint,
                CurrentLineFill,
                CurrentLineFillInactive,
                CurrentLineEdge
            };
            
            struct PaletteEntry {
                uint32_t        color   = 0;
                //  bold, italic, etc to go here
            };
            
            struct Palette {
                std::array<PaletteEntry, 256>   entries;
            };


            static const Palette&   dark_palette();
            //static const Palette&   light_palette();
            //static const Palette&   retro_blue_palette();

            TextEdit();
            ~TextEdit();

            

            //! Builds the text
            std::string         build_text() const;

            //! Gets the specified character
            char32_t            character(const Coord&) const;

            //! Number of glyphs (not bytes) including new-lines
            uint64_t            character_count() const;
            
            uint32_t            color(Style) const;

            //! Clears the text
            void                clear_text();
            
            //! Number of lines
            uint32_t            line_count() const { return m_lines.size(); }

            //! Number of characters (glyphs) on line
            uint32_t            line_characters_count(uint32_t) const;
            
            std::string         line_text(uint32_t) const;

            bool                overwrite() const { return m_overwrite; }

            bool                readonly() const { return m_readonly; }

            //! Resets the configuration
            void                reset_settings();
            
            //! Sets an EXISTING line
            //! \note newlines will be dropped!
            void                set_line_text(uint32_t, std::string_view);

            //! Sets the color palette
            void                set_palette(const Palette&);
            
            void                set_readonly(bool);

            //! Sets the number of spaces per tab
            void                set_tab_count(uint8_t);
            
            //! Sets the tab mode
            void                set_tab_mode(TabMode);
            
            //! Sets the text
            void                set_text(std::string_view);
            
            //! Sets the text as series of lines
            void                set_text_lines(std::span<std::string_view>);
            
            //! Streams the specified line out (no new-line)
            void                stream_line(Stream&, uint32_t) const;
            
            //! Streams the contents out
            void                stream_text(Stream&) const;
            
            //  Draws the widget
            void                draw() override;
            
            void                undo();
            void                redo();
          
        private:
        
            struct Settings {
                TabMode             tabMode             = TabMode::TAB;
                uint8_t             tabCount            = 4;                // zero disables tabs
                WrapMode            wrapMode            = WrapMode::NONE;
                bool                keepWords           = true;             // will keep workds
                uint16_t            vertLine            = 0;                // zero disable
                bool                lineNumbers         = false;            // show line numbers (if enabled)
                bool                showWitespace       = false;
                float               lineSpacing         = 1.f;
                bool                syntaxHighlighting  = false;
                uint32_t            undoLimit           = UINT32_MAX;
            };
            
            struct State {
                Coord           sstart = {}, send = {};
                Coord           caret = {};
                SelectionMode   smode = {};
            };
            
            struct Action {
                std::string     text;
                Coord           start, end;
                State           state;
            };
            
            struct Undo {
                Action      before, after;
            };
            
            
            Palette                 m_palette;
            std::vector<Line>       m_lines;
            Settings                m_settings;
            State                   m_state;
            std::deque<Undo>        m_undo;
            std::string             m_title             = "Untitled";
            Vector2F                m_size              = {};
            bool                    m_readonly          = false;
            bool                    m_overwrite         = false;
            bool                    m_handleKeyboard    = true;
            bool                    m_handleMouse       = true;
            bool                    m_handleShortcuts   = true;
            bool                    m_imguiChild        = true;
            bool                    m_border            = false;
            bool                    m_withinRender      = false;
            
            //! Renders the content
            void        render_content();
            
            void        handle_keyboard();
            void        handle_mouse();
            void        colorize();
            void        reset_colors();
            void        apply_add(const Action&);
            void        apply_remove(const Action&);
        };
        
    }
}
