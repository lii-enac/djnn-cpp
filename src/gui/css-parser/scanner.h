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

#ifndef CSS_SCANNER_H
#define CSS_SCANNER_H

#ifndef YY_DECL

#define	YY_DECL						\
  css::Parser::token_type				\
  css::Scanner::lex(				\
  css::Parser::semantic_type* yylval,		\
  css::Parser::location_type* yylloc)
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer CssFlexLexer
#include "FlexLexer.h"
#undef yyFlexLexer
#endif

#include "parser.h"

namespace css
{
  class Scanner : public CssFlexLexer
  {
  public:
    Scanner (std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);

    virtual ~Scanner ();

    virtual Parser::token_type
    lex (Parser::semantic_type* yylval, Parser::location_type* yylloc);

    void set_debug (bool b);
  };

}

#endif
