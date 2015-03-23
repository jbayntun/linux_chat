#include "helpers.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: Split
--
-- DATE: January 18, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jeff Bayntun
--
-- PROGRAMMER: Jeff Bayntun
--
-- INTERFACE: void Split( std::string list, std::vector<std::string>& v )
-- list: string with delimiters in it
-- &v: vector to place the parsed tokens into
--
-- RETURNS: void
--
-- NOTES:
----------------------------------------------------------------------------------------------------------------------*/
void Split( std::string list, std::vector<std::string>& v, char DELIM )
{
    size_t pos = 0;
    std::string delimiter(1, DELIM);
    std::string token;
    while ( (pos = list.find( delimiter ) ) != std::string::npos )
    {
        token = list.substr( 0, pos );
        v.emplace_back(token);
        list.erase( 0, pos + delimiter.length() );
        token.clear();
    }

    return;
}

