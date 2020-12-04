/*
 *  djnn v2
 *
 *  The copyright holders for the contents of this file are:
 *      Ecole Nationale de l'Aviation Civile, France (2020)
 *  See file "license.terms" for the rights and conditions
 *  defined by copyright holders.
 *
 *
 *  Contributors:
 *      Mathieu Magnaudet <mathieu.magnaudet@enac.fr>
 *
 */

/*
 * this code is an adaptation from https://www.w3.org/Style/CSS/validator/
 */

%{

// #include <string> // pch

#include "scanner.h"

typedef css::Parser::token token;
typedef css::Parser::token_type token_type;

#define MM 1.0
#define IN (25.4 * MM)
#define PT (IN/72.0)
#define CM (10 * MM)
#define PC (12 * PT)
#define PX (IN/96.0) 

#define yyterminate() return token::END

#define YY_NO_UNISTD_H

%}
%option c++
%option prefix="Css"
%option batch
%option debug
%option yywrap nounput 
%option stack

unicode     \\[0-9a-f]{1,4}
latin1      [¡-ÿ]
escape      {unicode}|\\[ -~¡-ÿ]
stringchar  {escape}|{latin1}|[ !#$%&(-~]
nmstrt      [a-z]|{latin1}|{escape}
nmchar      [-a-zA-Z0-9]|{latin1}|{escape}
ident       {nmstrt}{nmchar}*
name        {nmchar}+
d           [0-9]
notnm       [^-a-z0-9\\]|{latin1}
w           [ \t\n]*
num         {d}+|{d}*\.{d}+
string      \"({stringchar}|\')*\"|\'({stringchar}|\")*\'

%x COMMENT
%s AFTER_IDENT

/* The following paragraph suffices to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position. */
%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

%% /*** Regular Expressions Part ***/

 /* code to place at the beginning of yylex() */
%{
    // reset location
    yylloc->step();
%}

 /*** BEGIN EXAMPLE - Change the example lexer rules below ***/

"/*"            {BEGIN(COMMENT);}
<COMMENT>"*/"   {BEGIN(0);}
<COMMENT>\n     {/* ignore */}
<COMMENT>.      {/* ignore */}
@import         {BEGIN(0); return token::IMPORT_SYM;}
"!"{w}important {BEGIN(0); return token::IMPORTANT_SYM;}
{ident}         {BEGIN(AFTER_IDENT); yylval->stringVal = new std::string(yytext, yyleng); return token::IDENT;}
{string}        {BEGIN(0); yylval->stringVal = new std::string(yytext, yyleng); return token::STRING;}

{num}             {BEGIN(0); yylval->doubleVal = atof(yytext); return token::NUMBER;}
{num}"%"          {BEGIN(0); yylval->doubleVal = atof(yytext); return token::PERCENTAGE;}
{num}pt/{notnm}   {BEGIN(0); yylval->doubleVal = atof(yytext) * PT; return token::LENGTH;}
{num}mm/{notnm}   {BEGIN(0); yylval->doubleVal = atof(yytext) * MM; return token::LENGTH;}
{num}cm/{notnm}   {BEGIN(0); yylval->doubleVal = atof(yytext) * CM; return token::LENGTH;}
{num}pc/{notnm}   {BEGIN(0); yylval->doubleVal = atof(yytext) * PC; return token::LENGTH;}
{num}in/{notnm}   {BEGIN(0); yylval->doubleVal = atof(yytext) * IN; return token::LENGTH;}
{num}px/{notnm}   {BEGIN(0); yylval->doubleVal = atof(yytext) * PX; return token::LENGTH;}
{num}em/{notnm}   {BEGIN(0); yylval->doubleVal = atof(yytext); return token::EMS;}
{num}ex/{notnm}   {BEGIN(0); yylval->doubleVal = atof(yytext); return token::EXS;}

<AFTER_IDENT>":"link            {return token::LINK_PSCLASS_AFTER_IDENT;}
<AFTER_IDENT>":"visited         {return token::VISITED_PSCLASS_AFTER_IDENT;}
<AFTER_IDENT>":"active          {return token::ACTIVE_PSCLASS_AFTER_IDENT;}
<AFTER_IDENT>":"first-line      {return token::FIRST_LINE_AFTER_IDENT;}
<AFTER_IDENT>":"first-letter    {return token::FIRST_LETTER_AFTER_IDENT;}
<AFTER_IDENT>"#"{name}          {yylval->integerVal = std::strtol((std::string(yytext)).replace(0,1,"0x").c_str(), nullptr, 16); return token::HASH_AFTER_IDENT;}
<AFTER_IDENT>"."{name}          {return token::CLASS_AFTER_IDENT;}

":"link         {BEGIN(AFTER_IDENT); return token::LINK_PSCLASS;}
":"visited      {BEGIN(AFTER_IDENT); return token::VISITED_PSCLASS;}
":"active       {BEGIN(AFTER_IDENT); return token::ACTIVE_PSCLASS;}
":"first-line   {BEGIN(AFTER_IDENT); return token::FIRST_LINE;}
":"first-letter {BEGIN(AFTER_IDENT); return token::FIRST_LETTER;}
"#"{name}       {BEGIN(AFTER_IDENT); yylval->integerVal = std::strtol((std::string(yytext)).replace(0,1,"0x").c_str(), nullptr, 16); return token::HASH;}
"."{name}       {BEGIN(AFTER_IDENT); yylval->stringVal = new std::string(yytext, yyleng); return token::CLASS;}

url\({w}{string}{w}\)            |
url\({w}([^ \n\'\")]|\\\ |\\\'|\\\"|\\\))+{w}\)    {BEGIN(0); return token::URL;}
rgb\({w}{num}%?{w}\,{w}{num}%?{w}\,{w}{num}%?{w}\) {BEGIN(0); yylval->stringVal = new std::string(yytext, yyleng); return token::RGB;}

[\n]            {BEGIN(0);}
\<\!\-\-        {BEGIN(0); return token::CDO;}
\-\-\>          {BEGIN(0); return token::CDC;}

[ \t\r]+        {BEGIN(0); yylloc->step();}

 /* pass all other characters up to bison */
. {
    return static_cast<token_type>(*yytext);
  }
%% /*** Additional Code ***/

namespace css {

Scanner::Scanner(std::istream* in,
		 std::ostream* out)
    : CssFlexLexer(in, out)
{
}

Scanner::~Scanner()
{
}

void Scanner::set_debug(bool b)
{
    yy_flex_debug = b;
}

}

#ifdef yylex
#undef yylex
#endif

int CssFlexLexer::yylex()
{
    std::cerr << "in CssFlexLexer::yylex() !" << std::endl;
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int CssFlexLexer::yywrap()
{
    return 1;
}
