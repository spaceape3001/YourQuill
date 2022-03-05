////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/preamble.hpp>


namespace yq {

    //! The reasonable maximum for a raw null terminated string... anything else should be in a string view (at least)
    static constexpr const uint64_t         MAX_NULL_TERM       = 8192;
    
    //! Concatenates the given 32-bit UTF character on to the string as UTF-8
    std::string&  operator+=(std::string&, char32_t);
    
    //! Checks to see if ANY string in the left starts with the right
    bool    any_starts(const std::vector<std::string>&haystack, const std::string_view&pattern);
    
    //! Checks to see if ANY string in the left starts with the right
    bool    any_starts(const std::vector<std::string_view>&haystack, const std::string_view&pattern);
    
    //! Checks to see if ANY string in the left starts with the right
    bool    any_starts(const std::initializer_list<std::string_view>&haystack, const std::string_view&pattern);

    //! Checks to see if ANY string in the left starts with the right
    bool    any_starts_igCase(const std::vector<std::string>&haystack, const std::string_view&pattern);
    
    //! Checks to see if ANY string in the left starts with the right
    bool    any_starts_igCase(const std::vector<std::string_view>&haystack, const std::string_view&pattern);
    
    //! Checks to see if ANY string in the left starts with the right
    bool    any_starts_igCase(const std::initializer_list<std::string_view>&haystack, const std::string_view&pattern);

    
    /*! \brief Blanks all C++ comments in the given string 
    
        This goes over the string, all C++ comments are turned into spaces
    */
    void  blank_cpp_comments(std::string&);
    
    /*! \brief Capitalizes each word
    
        This capitalizes each word in the string, the first character going to upper case, the rest 
        converted to lower case.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
    */
    std::string  capitalize(const char*s, size_t n);

    /*! \brief Capitalizes each word
    
        This capitalizes each word in the string, the first character going to upper case, the rest 
        converted to lower case.

        \param[in] s    String view (which is auto-gen from std::string if need be.
    */
    std::string  capitalize(const std::string_view&s);

    /*! \brief Checks if the haystack has the needle
    */
    bool    contains(const char* haystack, size_t nHaystack, const char* needle, size_t nNeedle);

    /*! \brief Checks if the haystack has the needle
    */
    bool    contains(const std::string_view& haystack, const std::string_view& needle);

    /*! \brief Checks if the haystack has the needle (ignoring case)
    */
    bool    contains_igCase(const char* haystack, size_t nHaystack, const char* needle, size_t nNeedle);

    /*! \brief Checks if the haystack has the needle (ignoring case)
    */
    bool    contains_igCase(const std::string_view& haystack, const std::string_view& needle);

    /*! \brief Copies the string
    
        This doesn't return a reference, but actually makes a copy.
    */
    inline std::string  copy(const std::string_view&s)
    {
        return std::string(s);
    }

    /*! \brief Copies the vector of string
    
        This doesn't return a reference, but actually makes a copy.
    */
    std::vector<std::string>  copy(const std::vector<std::string>&);

    /*! \brief Copies the vector of string-views
    
        This doesn't return a reference, but actually makes a copy.
    */
    std::vector<std::string>  copy(const std::vector<std::string_view>&);
    
    /* !\brief Counts the number of charcters
    */
    size_t  count_characters(const char* s, size_t n);

    /* !\brief Counts the number of charcters
    */
    size_t  count_characters(const std::string_view&s);

    /* !\brief Counts the number of charcters
    */
    size_t  count_characters(const char* s, size_t n, char ch);

    /* !\brief Counts the number of charcters
    */
    size_t  count_characters(const std::string_view&s, char ch);

    /* !\brief Counts the number of charcters
    */
    size_t  count_characters(const char* s, size_t n, char32_t ch);

    /* !\brief Counts the number of charcters
    */
    size_t  count_characters(const std::string_view&s, char32_t ch);

    /* !\brief Counts the number of charcters
    */
    size_t  count_characters_igCase(const char* s, size_t n, char ch);

    /* !\brief Counts the number of charcters
    */
    size_t  count_characters_igCase(const std::string_view&s, char ch);

    /* !\brief Counts the number of charcters
    */
    size_t  count_characters_igCase(const char* s, size_t n, char32_t ch);

    /* !\brief Counts the number of charcters
    */
    size_t  count_characters_igCase(const std::string_view&s, char32_t ch);

    /*! \brief Counts the opening number of space characters
    */
    size_t  count_start_spaces(const std::string_view&);

    /*! \brief First non-blank character
        This returns the first non-blank character seen, or NULL if none found
    */
    char32_t  first_non_blank_char(const std::string_view&sv);

    /*! \brief First argument that isn't empty
    
        This returns the first argument that isn't empty.  If all area empty, a reference to the last one is returned
    */
    std::string_view  first_non_empty(const std::string_view&, const std::string_view&);

    /*! \brief First argument that isn't empty
    
        This returns the first argument that isn't empty.  If all area empty, a reference to the last one is returned
    */
    std::string_view  first_non_empty(const std::string_view&, const std::string_view&, const std::string_view&);

    /*! \brief First argument that isn't empty
    
        This returns the first argument that isn't empty.  If all area empty, a reference to the last one is returned
    */
    std::string_view  first_non_empty(const std::string_view&, const std::string_view&, const std::string_view&, const std::string_view&);

    /*! \brief First argument that isn't empty
    
        This returns the first argument that isn't empty.  If all area empty, a reference to the last one is returned
    */
    std::string_view  first_non_empty(std::initializer_list<std::string_view>);

    /*! \brief Formats the given number as hexadecimal
    
        \note string_view points to internal (thread-local) buffer, it'll be overwritten on next call!
    
        \param[in] n    Value to format
        \param[in] f    Fill character for leading characters
        \result Formatted string
    */
    std::string_view  fmt_hex(uint8_t n, char f='0');

    /*! \brief Formats the given number as hexadecimal
    
        \note string_view points to internal (thread-local) buffer, it'll be overwritten on next call!

        \param[in] n    Value to format
        \param[in] f    Fill character for leading characters
        \result Formatted string
    */
    std::string_view  fmt_hex(uint16_t n, char f='0');

    /*! \brief Formats the given number as hexadecimal
    
        \note string_view points to internal (thread-local) buffer, it'll be overwritten on next call!

        \param[in] n    Value to format
        \param[in] f    Fill character for leading characters
        \result Formatted string
    */
    std::string_view  fmt_hex(uint32_t n, char f='0');

    /*! \brief Formats the given number as hexadecimal
    
        \note string_view points to internal (thread-local) buffer, it'll be overwritten on next call!

        \param[in] n    Value to format
        \param[in] f    Fill character for leading characters
        \result Formatted string
    */
    std::string_view  fmt_hex(uint64_t n, char f='0');
    
    /*! \brief Formats the given number in scientifica notation
        
        \param[in] v        Value to format
        \param[in] digits   Prescision
        \result Formatted string
    */
    std::string  fmt_scientific(double v, int digits=-1);

    /*! \brief Formats the given number in scientifica notation
        
        \param[in] v        Value to format
        \param[in] digits   Prescision
        \result Formatted string
    */
    std::string  fmt_scientific(float v, int digits=-1);
    
    /*! \brief Hard wrap at the given size
    
        A hard wrap will break mid-word as necessary, and is based on the number of UTF-8 
        character code points.  (ie, entries might have different byte counts, but the character
        counts will remain identical until the last one.)
    
        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s pointer to string
        \param[in] n length of the string
        \param[in] width    number of characters to wrap at.
    */
    std::vector<std::string_view>  hard_wrap(const char*s, size_t n, size_t width);
    
    /*! \brief Hard wrap at the given size
    
        A hard wrap will break mid-word as necessary, and is based on the number of UTF-8 
        character code points.  (ie, entries might have different byte counts, but the character
        counts will remain identical until the last one.)
    
        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        \param[in] width    number of characters to wrap at.
    */
    std::vector<std::string_view>  hard_wrap(const std::string_view&s, size_t width);

    /*! \brief Tests for an alphanumeric character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_alnum(char ch)
    {
        return ::isalnum(ch);
    }
    
    /*! \brief Tests for an alphanumeric character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_alnum(char32_t ch)
    {
        return ::iswalnum(ch);
    }

    /*! \brief Tests for an alphabetical character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_alpha(char ch)
    {
        return ::isalpha(ch);
    }

    /*! \brief Tests for an alphabetical character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_alpha(char32_t ch)
    {
        return ::iswalpha(ch);
    }

    /*! \brief Tests entire string for alpha
    
        \param[in] s pointer to string
        \param[in] n length of the string
        \return TRUE if the ENTIRE string is alpha characters
    */
    bool is_alpha(const char*s, size_t n);
    
    /*! \brief Tests entire string for alpha
    
        \param[in] s    String view (which is auto-gen from std::string if need be.
        \return TRUE if the ENTIRE string is alpha characters
    */
    bool is_alpha(const std::string_view&);

    /*! \brief Tests for blank character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_blank(char ch)
    {
        return ::isblank(ch);
    }

    /*! \brief Tests for blank character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_blank(char32_t ch)
    {
        return ::iswblank(ch);
    }

    /*! \brief Tests for blank string
        \param[in] s pointer to string
        \param[in] n length of the string
    */
    bool  is_blank(const char*s, size_t n);

    /*! \brief Tests for blank string
        \param[in] s    String view (which is auto-gen from std::string if need be.
    */
    bool  is_blank(const std::string_view&s);
    
    /*! \brief Tests for control character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_control(char ch)
    {   
        return ::iscntrl(ch);
    }
    
    /*! \brief Tests for control character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_control(char32_t ch)
    {
        return ::iswcntrl(ch);
    }
    
    /*! \brief Tests for numeric digit
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_digit(char ch)
    {
        return ::isdigit(ch);
    }
    
    /*! \brief Tests for numeric digit
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_digit(char32_t ch)
    {
        return ::iswdigit(ch);
    }
    
    /*! \brief Tests for emptiness
    
        \return TRUE if BOTH first & second are empty
    */
    inline bool is_empty(const string_view_pair_t& p)
    {
        return p.first.empty() && p.second.empty();
    }

    /*! \brief Tests for graphical character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_graph(char ch)
    {
        return ::isgraph(ch);
    }
    
    /*! \brief Tests for graphical character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_graph(char32_t ch)
    {
        return ::iswgraph(ch);
    }

    /*! \brief Tests if b > a ignoring case
    
        This ignores case differences as it compares strings a & b.
    */
    bool  is_greater_igCase(const std::string_view&a, const std::string_view&b);
    
    
    /*! \brief Test to see if string matches the pattern
    
        (this one is here so it signature matches the other
    */
    bool  is_in(const std::string_view& str, const std::string_view& pat);

    /*! \brief Test to see if string matches one of the patterns
    */
    bool  is_in(const std::string_view& str, const std::initializer_list<std::string_view>& pat);
    
    /*! \brief Test to see if string matches one of the patterns
    */
    bool  is_in(const std::string_view& str, const string_view_set_t& pat);

    /*! \brief Test to see if string matches one of the patterns
    */
    bool  is_in(const std::string_view& str, const std::vector<std::string_view>& pat);

    /*! \brief Tests if b < a ignoring case
    
        This ignores case differences as it compares strings a & b.
    */
    bool  is_less_igCase(const std::string_view&a, const std::string_view&b);

    /*! \brief Tests for lower-case alphabetical character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_lower(char ch)
    {
        return ::islower(ch);
    }

    /*! \brief Tests for lower-case alphabetical character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_lower(char32_t ch)
    {
        return ::islower(ch);
    }

    /*! \brief Tests for printable character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_print(char ch)
    {
        return ::isprint(ch);
    }
    
    /*! \brief Tests for printable character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_print(char32_t ch)
    {
        return ::iswprint(ch);
    }
    
    /*! \brief Tests for punctuation character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_punct(char ch)
    {
        return ::ispunct(ch);
    }
    
    /*! \brief Tests for punctuation character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_punct(char32_t ch)
    {
        return ::iswpunct(ch);
    }

    /*! \brief Tests for equality while ignoring case
    */
    bool  is_similar(char, char);

    /*! \brief Tests for equality while ignoring case
    */
    bool  is_similar(char32_t, char32_t);


    /*! \brief Tests for equality while ignoring case
    */
    bool  is_similar(const std::string_view&, const std::string_view&);

    /*! \brief Tests for white-space character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_space(char ch)
    {
        return ::isspace(ch);
    }

    /*! \brief Tests for white-space character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_space(char32_t ch)
    {
        return ::iswspace(ch);
    }
    
    /*! \brief Tests for white-space characters
        \return TRUE if ALL characters in the string view are white space
    */
    bool  is_space(const std::string_view&);
    
    /*! \brief Tests for upper-case alphabetical character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_upper(char ch)
    {
        return ::isupper(ch);
    }

    /*! \brief Tests for upper-case alphabetical character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_upper(char32_t ch)
    {
        return ::iswupper(ch);
    }
    
    /*! \brief Tests for hexadecimal digit character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_xdigit(char ch)
    {
        return ::isxdigit(ch);
    }
    
    /*! \brief Tests for hexadecimal digit character
        \note   This is here for reasons of better spelling and to harmonize with overloads.
    */
    inline bool  is_xdigit(char32_t ch)
    {
        return ::iswxdigit(ch);
    }
    
    /*! \brief Joins the collection into a string
        \param[in] collection  The collection
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::list<std::string>& collection, const std::string_view& separator);

    /*! \brief Joins the collection into a string
        \param[in] collection  The collection
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::list<std::string_view>& collection, const std::string_view& separator);

    /*! \brief Joins the collection into a string
        \param[in] collection  The collection
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::set<std::string>& collection, const std::string_view& separator);

    /*! \brief Joins the collection into a string
        \param[in] collection  The collection
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::set<std::string,IgCase>& collection, const std::string_view& separator);

    /*! \brief Joins the collection into a string
        \param[in] collection  The collection
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::set<std::string_view>& collection, const std::string_view& separator);

    /*! \brief Joins the collection into a string
        \param[in] collection  The collection
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::set<std::string_view,IgCase>& collection, const std::string_view& separator);

    /*! \brief Joins the collection into a string
        \param[in] collection  The collection, each element will be converted into a string
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::set<uint16_t>& collection, const std::string_view& separator);

    /*! \brief Joins the collection into a string
        \param[in] collection  The collection, each element will be converted into a string
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::set<uint32_t>& collection, const std::string_view& separator);

    /*! \brief Joins the collection into a string
        \param[in] collection  The collection
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::vector<std::string>& collection, const std::string_view& separator);

    /*! \brief Joins the collection into a string
        \param[in] collection  The collection
        \param[in] separator   The separator between elements.
        \return The joined string
    */
    std::string     join(const std::vector<std::string_view>& collection, const std::string_view& separator);
    


    /*! \brief Checks to see if the haystack matches the pattern at position
    
        \note Check will end at the length pattern, so "matches_at("foobar rocks", 3, "bar")" will be TRUE
    */
    bool        matches_at(const std::string_view& haystack, size_t pos, const std::string_view& pattern);

    /*! \brief Checks to see if the haystack matches the pattern at position
    
        \note Check will end at the length pattern, so "matches_at("foobar rocks", 3, "bar")" will be TRUE
    */
    bool        matches_at_igCase(const std::string_view& haystack, size_t pos, const std::string_view& pattern);
    
    /*! \brief Left pads the result until the character count is met */
    std::string     pad_left(const std::string_view& data, size_t minSize, char ch=' ');
    
    /*! \brief Left pads the result until the character count is met */
    std::string     pad_left(const std::string_view& data, size_t minSize, char32_t ch);

    /*! \brief Right pads the result until the character count is met */
    std::string     pad_right(const std::string_view& data, size_t minSize, char ch=' ');

    /*! \brief Right pads the result until the character count is met */
    std::string     pad_right(const std::string_view& data, size_t minSize, char32_t ch);
    
    /*! \brief Replaces all instances of old character with new character */
    size_t          replace_all(std::string&, char what, char with);
    
    /*! \brief Replaces all instances of old character with new character ignoring case */
    size_t          replace_all_igCase(std::string&, char what, char with);

    /*! \brief Set left to right if left is empty OR override is true
    
        \note if B is empty, this is a NOP
    */
    std::string&    set_if_empty(std::string&, const std::string_view&, bool fOverride=false);
    
    /*! \brief Simplifies the given string
    
        Simplifying a string removes start & end whitespace, replaces interior spaces with a single space character.
    */
    std::string     simplify(const char*s, size_t n, char sp=' ');

    /*! \brief Simplifies the given string
    */
    std::string     simplify(const std::string_view&, char sp=' ');

    /*! \brief Splits the string up into the specified equal number of chunks
    
        This routine will split the incoming string into the given number of
        chunks as evenly as possible.  (ie, adjacent items might be +- 1 in size)
        
        \note This returns REFERENCES to the original string, so it must live.
        \note If n<number, some of the returned entries will be empty.

        \param[in] s        pointer to string
        \param[in] n        length of the string
        \param[in] number   Number of entries
    */
    Vector<std::string_view>  split(const char* s, size_t n, size_t number); 

    /*! \brief Splits the string up into the specified equal number of chunks
    
        This routine will split the incoming string into the given number of
        chunks as evenly as possible.  (ie, adjacent items might be +- 1 in size)

        \note This returns REFERENCES to the original string, so it must live.
        \note If s.size()<number, some of the returned entries will be empty.

        \param[in] s        String view
        \param[in] number   Number of entries
    */
    Vector<std::string_view>  split(const std::string_view& s, size_t number); 

    /*! \brief Splits the string
    
        This routine splits the incoming string at the specified character.
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        pointer to string
        \param[in] n        length of the string
        \param[in] ch       Character being split on
    */
    Vector<std::string_view>  split(const char*s, size_t n, char ch); 

    /*! \brief Splits the string
    
        This routine splits the incoming string at the specified character.
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        String view
        \param[in] ch       Character being split on
    */
    Vector<std::string_view>  split(const std::string_view&s, char ch); 

    /*! \brief Splits the string, up to number entries
    
        This routine splits the incoming string at the specified character.
        Repeated instances of the character will result in empty entries in 
        the result.
        
        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        pointer to string
        \param[in] n        length of the string
        \param[in] ch       Character being split on
        \param[in] number   MAXIMUM Number of entries, more are treated as part of the last entry
    */
    Vector<std::string_view>  split(const char* s, size_t n, char ch, size_t number); 

    /*! \brief Splits the string, up to number entries
    
        This routine splits the incoming string at the specified character.
        Repeated instances of the character will result in empty entries in 
        the result.
        
        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        String view
        \param[in] ch       Character being split on
        \param[in] number   MAXIMUM Number of entries, more are treated as part of the last entry
    */
    Vector<std::string_view>  split(const std::string_view&s, char ch, size_t number); 

    /*! \brief Splits the string
    
        This routine splits the incoming string at the specified character.
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        pointer to string
        \param[in] n        length of the string
        \param[in] ch       Character being split on
    */
    Vector<std::string_view>  split(const char*s , size_t n, char32_t ch); 

    /*! \brief Splits the string
    
        This routine splits the incoming string at the specified character.
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        String view
        \param[in] ch       Character being split on
    */
    Vector<std::string_view>  split(const std::string_view&s, char32_t ch); 
    
    /*! \brief Splits the string
    
        This routine splits the incoming string at the specified character.
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        pointer to string
        \param[in] n        length of the string
        \param[in] ch       Character being split on
        \param[in] number   MAXIMUM Number of entries, more are treated as part of the last entry
    */
    Vector<std::string_view>  split(const char* s, size_t n, char32_t cn, size_t number); 

    /*! \brief Splits the string
    
        This routine splits the incoming string at the specified character.
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        String view
        \param[in] ch       Character being split on
        \param[in] number   MAXIMUM Number of entries, more are treated as part of the last entry
    */
    Vector<std::string_view>  split(const std::string_view&s, char32_t ch, size_t number); 

    /*! \brief Splits the string
    
        This routine splits the incoming string at the specified string
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        pointer to string
        \param[in] n        length of the string
        \param[in] p        pointer to pattern string
        \param[in] pn       length of the pattern string
    */
    Vector<std::string_view>  split(const char* s, size_t n, const char* p, size_t pn);

    /*! \brief Splits the string
    
        This routine splits the incoming string at the specified string
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        String view
        \param[in] p        pattern string
    */
    Vector<std::string_view>  split(const std::string_view&s, const std::string_view&p);

    /*! \brief Splits the string ignoring case
    
        This routine splits the incoming string at the specified character (case insensitive).
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        pointer to string
        \param[in] n        length of the string
        \param[in] ch       Character being split on
    */
    Vector<std::string_view>  split_igCase(const char*s , size_t n, char ch); 

    /*! \brief Splits the string ignoring case
    
        This routine splits the incoming string at the specified character (case insensitive).
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        string
        \param[in] ch       Character being split on
    */
    Vector<std::string_view>  split_igCase(const std::string_view& s, char ch); 

    /*! \brief Splits the string ignoring case
    
        This routine splits the incoming string at the specified character (case insensitive).
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        pointer to string
        \param[in] n        length of the string
        \param[in] ch       Character being split on
    */
    Vector<std::string_view>  split_igCase(const char* s, size_t n, char32_t ch); 

    /*! \brief Splits the string ignoring case
    
        This routine splits the incoming string at the specified character (case insensitive).
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        string
        \param[in] ch       Character being split on
    */
    Vector<std::string_view>  split_igCase(const std::string_view&s, char32_t ch); 
    
    /*! \brief Splits the string ignoring case
    
        This routine splits the incoming string at the specified string
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        pointer to string
        \param[in] n        length of the string
        \param[in] p        pointer to pattern string
        \param[in] pn       length of the pattern string
    */
    Vector<std::string_view>  split_igCase(const char*s, size_t n, const char* p, size_t pn);

    /*! \brief Splits the string ignoring case
    
        This routine splits the incoming string at the specified string
        Repeated instances of the character will result in empty entries in 
        the result.

        \note This returns REFERENCES to the original string, so it must live.

        \param[in] s        String view
        \param[in] p        pattern string
    */
    Vector<std::string_view>  split_igCase(const std::string_view&s, const std::string_view&p);

    /*! \brief Checks the start of string
    
        This checks to see if the start of the given string matches the given pattern.
    */
    bool  starts(const char* s, size_t n, const char* pat, size_t nPath);

    /*! \brief Checks the start of string
    
        This checks to see if the start of the given string matches the given pattern.
    */
    bool  starts(const std::string_view&s, const std::string_view& pat);

    /*! \brief Checks the start of string
    
        This checks to see if the start of the given string matches the given pattern, ignoring case.
    */
    bool  starts_igCase(const char* s, size_t n, const char* pat, size_t nPath);

    /*! \brief Checks the start of string
    
        This checks to see if the start of the given string matches the given pattern, ignoring case.
    */
    bool  starts_igCase(const std::string_view&s, const std::string_view& pat);

    /*! \brief Strips the LAST extension
    
        This returns the input MINUS the last extension, call repeatedly to remove more than one.
    */
    std::string_view    strip_extension(const std::string_view&);

    /*! \brief Strips ALL spaces from the input
    */
    std::string  strip_spaces(const char* s, size_t n);

    /*! \brief Strips ALL spaces from the input
    */
    std::string  strip_spaces(const std::string_view&);

    /*! \brief Finds the next character 
    
        \param[in] s    Pointer to string
        \param[in] n    Number of bytes to the string.
        \param[in] ch   Character being searched for
        \return pointer to character if found, NULL if not.
    */
    const char*  strnchr(const char*s, size_t n, char ch);

    /*! \brief Finds the next character 
    
        \param[in] s    Pointer to string
        \param[in] n    Number of bytes to the string.
        \param[in] ch   Character being searched for
        \return pointer to character if found, NULL if not.
    */
    const char*  strnchr(const char*s, size_t n, char32_t ch);

    /*! \brief Finds the next character (ignoring case)
    
        \param[in] s    Pointer to string
        \param[in] n    Number of bytes to the string.
        \param[in] ch   Character being searched for
        \return pointer to character if found, NULL if not.
    */
    const char*  strnchr_igCase(const char*s, size_t n, char ch);

    /*! \brief Finds the next character (ignoring case)
    
        \param[in] s    Pointer to string
        \param[in] n    Number of bytes to the string.
        \param[in] ch   Character being searched for
        \return pointer to character if found, NULL if not.
    */
    const char*  strnchr_igCase(const char*s, size_t n, char32_t ch);


    #ifdef WIN32
            //  until MSVC decides to grow up and get with the times....
    inline size_t  strnlen(const char* z, size_t cb)
    {
        return strlen_s(z, cb);
    }
    #else
    using ::strnlen;
    #endif
    
    size_t          strnlen(const char32_t* z, size_t cb);
    
    
    /*! \brief Finds the first needle
    
        \param[in]  haystack    The bigger string
        \param[in]  nHay        Size of the haystack
        \param[in]  needle      The search term
        \param[in]  nNeedle     Size of the needle
        
        \return pointer to characte rif ound, nullptr otherwise
    */
    const char* strnstr(const char* haystack, size_t nHay, const char* needle, size_t nNeedle);

    /*! \brief Finds the first needle (ignoring case)
    
        \param[in]  haystack    The bigger string
        \param[in]  nHay        Size of the haystack
        \param[in]  needle      The search term
        \param[in]  nNeedle     Size of the needle
        
        \return pointer to characte rif ound, nullptr otherwise
    */
    const char* strnstr_igCase(const char* haystack, size_t nHay, const char* needle, size_t nNeedle);

    /*! \brief Decodes the given string into a boolean
    
    
        This trims whitespace from both ends & decodes the non-whitespace text.  
        For false, it accepts 0, n, no, f, false.
        For true, it accepts 1, y, yes, t, true.
        All other inputs incur an invalid result (false/false).
        
        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    boolean_r  to_boolean(const char*s, size_t n);

    /*! \brief Decodes the given string into a boolean
    
        For false, it accepts 0, n, no, f, false.
        For true, it accepts 1, y, yes, t, true.
        All other inputs incur an invalid result.
        All other inputs incur an invalid result (false/false).
        
        \param[in] s    String view (which is auto-gen from std::string if need be.
        \return The result
    */
    boolean_r  to_boolean(const std::string_view& s);

    /*! \brief Decodes the given string into a dobule
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        \return The result
    */
    double_r  to_double(const char*s, size_t n);

    /*! \brief Decodes the given string into a dobule
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        \return The result
    */
    double_r  to_double(const std::string_view&s);

    /*! \brief Decodes the given string into a float
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        \return The result
    */
    float_r   to_float(const char*s, size_t n);

    /*! \brief Decodes the given string into a float
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    float_r   to_float(const std::string_view&);


    /*! \brief Decodes the given hex string into an unsigned
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        \return The result
    */
    unsigned_r to_hex(const char*s, size_t n);

    /*! \brief Decodes the given hex string into an unsigned
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        \return The result
    */
    unsigned_r to_hex(const std::string_view&s);

    /*! \brief Decodes the given hex string into an unsigned 8-bit intetger
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        \return The result
    */
    uint8_r  to_hex8(const char*s, size_t n);

    /*! \brief Decodes the given hex string into an unsigned 8-bit intetger
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        \return The result
    */
    uint8_r  to_hex8(const std::string_view&s);

    /*! \brief Decodes the given hex string into an unsigned 16-bit intetger
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        \return The result
    */
    uint16_r  to_hex16(const char*s, size_t n);

    /*! \brief Decodes the given hex string into an unsigned 16-bit intetger
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        \return The result
    */
    uint16_r  to_hex16(const std::string_view&s);

    /*! \brief Decodes the given hex string into an unsigned 32-bit intetger
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        \return The result
    */
    uint32_r  to_hex32(const char*s, size_t n);

    /*! \brief Decodes the given hex string into an unsigned 32-bit intetger
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        \return The result
    */
    uint32_r  to_hex32(const std::string_view&s);

    /*! \brief Decodes the given hex string into an unsigned 64-bit intetger
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        \return The result
    */
    uint64_r  to_hex64(const char*s, size_t n);

    /*! \brief Decodes the given hex string into an unsigned 64-bit intetger
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        \return The result
    */
    uint64_r  to_hex64(const std::string_view&s);
    
        /*! \brief Decodes the given string into an integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    integer_r  to_int(const char*s, size_t n);

    /*! \brief Decodes the given string into an integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    integer_r  to_int(const std::string_view&s);

    /*! \brief Decodes the given string into an 8-bit integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    int8_r  to_int8(const char*s, size_t n);

    /*! \brief Decodes the given string into an 8-bit integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    int8_r  to_int8(const std::string_view&s);

    /*! \brief Decodes the given string into a 16-bit integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    int16_r  to_int16(const char*s, size_t n);

    /*! \brief Decodes the given string into a 16-bit integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    int16_r  to_int16(const std::string_view&s);

    /*! \brief Decodes the given string into a 32-bit integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    int32_r  to_int32(const char*s, size_t n);

    /*! \brief Decodes the given string into a 32-bit integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    int32_r  to_int32(const std::string_view&s);

    /*! \brief Decodes the given string into a 64-bit integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    int64_r  to_int64(const char*s, size_t n);

    /*! \brief Decodes the given string into a 64-bit integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    int64_r  to_int64(const std::string_view&s);

    /*! \brief Decodes the given string into an integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    integer_r  to_integer(const char*s, size_t n);

    /*! \brief Decodes the given string into an integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    integer_r  to_integer(const std::string_view&s);

    /*! \brief Converts to lower case
    
        Harmonized overload (for name consistency) to convert a character to lower case. 
        (If it's not upper case, then the input argument is returned.)
    */
    inline char  to_lower(char ch)
    {
        return ::tolower(ch);
    }

    /*! \brief Converts to lower case
    
        Harmonized overload (for name consistency) to convert a character to lower case. 
        (If it's not upper case, then the input argument is returned.)
    */
    inline char32_t  to_lower(char32_t ch)
    {
        return ::towlower(ch);
    }
    
    /*! \brief Converts to lower case
    
        Converts the given string to lowercase (as unicode characters)
    */
    std::string  to_lower(const std::string_view&s);

    /*! \brief Converts to lower case
        Converts the given string to lowercase.
    */
    std::u32string  to_lower(const std::u32string_view&s);

    /*! \brief Decodes the given string into a short integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    short_r  to_short(const char*s, size_t n);

    /*! \brief Decodes the given string into a short integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    short_r  to_short(const std::string_view&s);

    /*! \brief Returns the boolean as a string (view)
    */
    inline std::string_view     to_string(bool f, const std::string_view& kFalse = "false", const std::string_view& kTrue="true")
    {
        return f ? kTrue : kFalse;
    }
    
    /*! \brief Returns the character as a string.
    */
    inline std::string  to_string(char ch)
    {
        return std::string(1,ch);
    }
    
    inline std::string  to_string(char8_t ch)
    {
        return std::string(1,(char) ch);
    }

    /*! \brief Returns the character as a string.
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view  to_string(char32_t);

    /*! \brief Returns the character-array as a string.
    */
    inline std::string_view  to_string(const char*z)
    {
        return std::string_view(z);
    }
    
    /*! \brief Returns the character-array as a string.
    */
    std::string   to_string(const char32_t*);
    

    /*! \brief Formats the double into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view   to_string(double);

    /*! \brief Formats the float into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view   to_string(float);

    /*! \brief Formats the integer into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view   to_string(int8_t);

    /*! \brief Formats the integer into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view   to_string(int16_t);

    /*! \brief Formats the integer into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view   to_string(int32_t);

    /*! \brief Formats the integer into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view  to_string(int64_t);
    
    /*! Converts the string view to string string view
    
        Yes, there's a constructor, however, this is for the template (below);  Copy 
    */
    inline std::string_view  to_string(const std::string_view& s)
    {
        return s;
    }

    /*! No-op string to string, for templates
    
        Yes, there's a constructor, however, this is for the template (below);
    */
    inline const std::string&  to_string(const std::string& s)
    {
        return s;
    }
    
    /*! \brief Converts to std::string
    */
    std::string_view  to_string(const std::u8string_view&s);

    /*! \brief Converts to std::string
    */
    std::string  to_string(const std::u32string_view&s);

    std::string to_string(const std::wstring_view&);

    /*! \brief Formats the unsigned integer into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view  to_string(uint8_t);

    /*! \brief Formats the unsigned integer into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view  to_string(uint16_t);

    /*! \brief Formats the unsigned integer into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view  to_string(uint32_t);

    /*! \brief Formats the unsigned integer into a string
        \note Returns a REFERENCE to a thread-local buffer, copy off before next call, if retention is important.
    */
    std::string_view  to_string(uint64_t);

    template <typename T>
    auto  to_string(const std::vector<T>& input)
    {
        using ret_t  = decltype(to_string(T()));
        Vector<ret_t>   ret;
        ret.reserve(input.size());
        for(auto& i : input)
            ret << to_string(i);
        return ret;
    }

    /*! \brief Decodes the given string into an unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    unsigned_r  to_uint(const char*s, size_t n);

    /*! \brief Decodes the given string into an unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    unsigned_r  to_uint(const std::string_view&s);

    /*! \brief Decodes the given string into an 8-bit unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    uint8_r  to_uint8(const char*s, size_t n);

    /*! \brief Decodes the given string into an 8-bit unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    uint8_r  to_uint8(const std::string_view&s);

    /*! \brief Decodes the given string into a 16-bit unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    uint16_r  to_uint16(const char*s, size_t n);

    /*! \brief Decodes the given string into a 16-bit unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    uint16_r  to_uint16(const std::string_view&s); 

    /*! \brief Decodes the given string into a 32-bit unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    uint32_r  to_uint32(const char*s, size_t n);

    /*! \brief Decodes the given string into a 32-bit unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    uint32_r  to_uint32(const std::string_view&s);

    /*! \brief Decodes the given string into a 64-bit unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    uint64_r  to_uint64(const char*s, size_t n);

    /*! \brief Decodes the given string into a 64-bit unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    uint64_r  to_uint64(const std::string_view&s);

    /*! \brief Decodes the given string into an unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    unsigned_r  to_uinteger(const char*s, size_t n);

    /*! \brief Decodes the given string into an unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    unsigned_r  to_uinteger(const std::string_view&s);

    /*! \brief Decodes the given string into an unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    unsigned_r  to_unsigned(const char*s, size_t n);

    /*! \brief Decodes the given string into an unsigned integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    unsigned_r  to_unsigned(const std::string_view&s);

    /*! \brief Converts to upper case
    
        Converts the given character to upper case
    */
    inline char  to_upper(char ch)
    {
        return ::toupper(ch);
    }
    
    /*! \brief Converts to upper case
    
        Converts the given character to upper case
    */
    inline char32_t  to_upper(char32_t ch)
    {
        return ::towupper(ch);
    }
    
    /*! \brief Converts to upper case
    
        Converts the given string to upper case (as unicode characters)
    */
    std::string  to_upper(const std::string_view&);

    /*! \brief Converts to upper case
    
        Converts the given string to upper case (as unicode characters)
    */
    std::u32string  to_upper(const std::u32string_view&);

    /*! \brief Decodes the given string into an unsigned short integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    pointer to char string
        \param[in] n    number of characters
        
        \return The result
    */
    ushort_r  to_ushort(const char*s, size_t n);
    
    /*! \brief Decodes the given string into an unsigned short integer
    
        This trims whitespace from both ends & decodes the non-whitespace text.

        \param[in] s    String view (which is auto-gen from std::string if need be.
        
        \return The result
    */
    ushort_r  to_ushort(const std::string_view&s);


    /*! \brief String not starting/ending with spaces
    */
    std::string_view    trimmed(const std::string_view&);
    
    /*! \brief String not ending with whitespaces
    */
    std::string_view    trimmed_end(const std::string_view&);
    
    /*! \brief String not starting with whitespace
    */
    std::string_view    trimmed_start(const std::string_view&);

    /*! \brief Split via visitor pattern
    
        This "splits" the string, but uses a lambda visitor instead of return vector.  Note, matched
        character will NOT be echoed.
        
        \param[in]      s    Pointer to the string
        \param[in]      n    Length of the string (in bytes)
        \param[in]      ch   Character to split on
        \param[in,out]  pred Function-like object, taking a std::string_view.
    */
    template <typename Pred>
    void            vsplit(const char* s, size_t n, char ch, Pred pred)
    {
        if(s && n){
            const char* end = s + n;
            const char* i   = nullptr;
            const char* j   = nullptr;
            for(i = s; (j = strnchr(i, end-i, ch)); i = j+1)
                pred(std::string_view(i, j-i));
            pred(std::string_view(i, end-i));
        }
    }

    /*! \brief Split via visitor pattern
    
        This "splits" the string, but uses a lambda visitor instead of return vector.  Note, matched
        character will NOT be echoed.
        
        \param[in]      s    String view to operate on
        \param[in]      ch   Character to split on
        \param[in,out]  pred Function-like object, taking a std::string_view.
    */
    template <typename Pred>
    void            vsplit(const std::string_view& s, char ch, Pred pred)
    {
        vsplit(s.data(), s.size(), pred);
    }
    
    /*! \brief Split via visitor pattern
    
        This "splits" the string, but uses a lambda visitor instead of return vector.  Note, matched
        character will NOT be echoed.
        
        \param[in]      s    Pointer to the string
        \param[in]      n    Length of the string (in bytes)
        \param[in]      p    Pattern to split on
        \param[in]      pn   Length of the pattern (in bytes)
        \param[in,out]  pred Function-like object, taking a std::string_view.
    */
    template <typename Pred>
    void            vsplit(const char* s, size_t n, const char* p, size_t pn, Pred pred)
    {
        if(s && n && p && pn){
            const char* end = s + n;
            const char* i   = nullptr;
            const char* j   = nullptr;
            for(i = s; (j = strnstr(i, end-i, p, pn)); i = j)
                pred(std::string_view(i, j-i));
            pred(std::string_view(i, end-i));
        }
    }
    
    /*! \brief Split via visitor pattern
    
        This "splits" the string, but uses a lambda visitor instead of return vector.  Note, matched
        character will NOT be echoed.
        
        \param[in]      s    String view to operate on
        \param[in]      p    String view to split on
        \param[in,out]  pred Function-like object, taking a std::string_view.
    */
    template <typename Pred>
    void            vsplit(const std::string_view& s, const std::string_view& pattern, Pred pred)    
    {
        vsplit(s.data(), s.size(), pattern.data(), pattern.size(), pred);
    }
    
    /*! \brief Split via visitor pattern (ignoring case)
    
        This "splits" the string, but uses a lambda visitor instead of return vector.  Note, matched
        character will NOT be echoed.
        
        \param[in]      s    Pointer to the string
        \param[in]      n    Length of the string (in bytes)
        \param[in]      ch   Character to split on
        \param[in,out]  pred Function-like object, taking a std::string_view.
    */

    template <typename Pred>
    void            vsplit_igCase(const char* s, size_t n, char ch, Pred pred)
    {
        if(s && n){
            const char* end = s + n;
            const char* i   = nullptr;
            const char* j   = nullptr;
            for(i = s; (j = strnchr_igCase(i, end-i, ch)); i = j+1)
                pred(std::string_view(i, j-i));
            pred(std::string_view(i, end-i));
        }
    }

    /*! \brief Split via visitor pattern (ignoring case)
    
        This "splits" the string, but uses a lambda visitor instead of return vector.  Note, matched
        character will NOT be echoed.
        
        \param[in]      s    String view to operate on
        \param[in]      ch   Character to split on
        \param[in,out]  pred Function-like object, taking a std::string_view.
    */
    template <typename Pred>
    void            vsplit_igCase(const std::string_view& s, char ch, Pred pred)
    {
        vsplit_igCase(s.data(), s.size(), pred);
    }

    /*! \brief Split via visitor pattern (ignoring case)
    
        This "splits" the string, but uses a lambda visitor instead of return vector.  Note, matched
        character will NOT be echoed.
        
        \param[in]      s    Pointer to the string
        \param[in]      n    Length of the string (in bytes)
        \param[in]      p    Pattern to split on
        \param[in]      pn   Length of the pattern (in bytes)
        \param[in,out]  pred Function-like object, taking a std::string_view.
    */
    template <typename Pred>
    void            vsplit_igCase(const char* s, size_t n, const char* p, size_t pn, Pred pred)
    {
        if(s && n && p && pn){
            const char* end = s + n;
            const char* i   = nullptr;
            const char* j   = nullptr;
            for(i = s; (j = strnstr_igCase(i, end-i, p, pn)); i = j)
                pred(std::string_view(i, j-i));
            pred(std::string_view(i, end-i));
        }
    }
    
    /*! \brief Split via visitor pattern (ignoring case)
    
        This "splits" the string, but uses a lambda visitor instead of return vector.  Note, matched
        character will NOT be echoed.
        
        \param[in]      s    String view to operate on
        \param[in]      p    String view to split on
        \param[in,out]  pred Function-like object, taking a std::string_view.
    */
    template <typename Pred>
    void            vsplit_igCase(const std::string_view& s, const std::string_view& pattern, Pred pred)    
    {
        vsplit_igCase(s.data(), s.size(), pattern.data(), pattern.size(), pred);
    }
    
}
