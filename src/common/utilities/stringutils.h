/*****************************************************************************
	Glare

    Copyright (C) 2017  German Molina (germolinal@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#pragma once 

#include <string>
#include <vector>


/*!
@defgroup stringUtils String utilities

This module defines functions that are useful for handling strings
*/

/* @{ */

#define DEFAULT_CHAR '-' //!< The default char to replace all non-ascii characters

//! Checks if a word includes a substring
/*!
@author German Molina
@param[in] word the string to check
@param[in] substring the substring to be found in the word
@return the word contains the substring
*/
bool stringInclude(std::string word, std::string substring);

//! Checks if a word includes a substring
/*!
@author German Molina
@param[in] word the string to check
@param[in] substring the substring to be found in the word
@return the word contains the substring
*/
bool stringInclude(char * word, std::string substring);

//! Checks if a word includes a substring
/*!
@author German Molina
@param[in] word the string to check
@param[in] substring the substring to be found in the word
@return the word contains the substring
*/
bool stringInclude(char * word, char * substring);


//! Checks if a word includes any of the given substring
/*!
@author German Molina
@param[in] word the string to check
@param[in] substringArray the substrings to be found in the word
@param[in] nItems the number of items in the array
@return the word contains one of the substrings
*/
bool stringIncludeAny(std::string word, char ** substringArray, int nItems);


//! Checks if a word includes any of the given substring
/*!
@author German Molina
@param[in] word the string to check
@param[in] substringArray the substrings to be found in the word
@param[in] nItems the number of items in the array
@return the word contains one of the substrings
*/
bool stringIncludeAny(char * word, char ** substringArray, int nItems);


//! Clones a UTF-8 string into an ASCII one. 
/*!
@author German Molina
@param[in] input the UTF-8 string to transform
@param[in] inputLength the length of the input
@param[out] output The string to put the ASCII string
@param[out] outputLength the length of the final output string
*/
void utf8toASCII(char * input, size_t inputLength, char * output, size_t * outputLength);

//! Some strings would be ilegal in Radiance format. This function fixes that
/*!
Replaces spaces and # and | by an underscore, leaves Digits, Capital Letters 
and Small letters as they were, and replaces everything else by the DEFAULT_CHAR

@author German Molina
@param[in] s the input string
@param[in] stringLength the length of the string
@note this transforms the string, does not create a new one.
*/
void fixString(char * s, size_t stringLength);

//! Transforms a size_t into a string
/*!
@author German Molina (based on the internet)
@param[in] sz size_t param
@return the string
*/
std::string size_tToString(size_t sz);

//! Splits a string into a vector of strings, divided by a blank space
/*!
@author German Molina
@param[in] s The string to splot
@param[out] v The vector to populate
*/
void tokenize(std::string * s, std::vector<std::string> * v);

/* @} */
