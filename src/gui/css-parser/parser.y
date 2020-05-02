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
 
%{
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "core/core.h"
#include "base/base.h"
#include "gui/gui-dev.h"

// core/utils/error.h macros clash with Driver::error
#undef error
#undef warning

%}

%require "2.3"

%debug

%start stylesheet

%defines

%skeleton "lalr1.cc"

%define api.prefix {css}

%define parser_class_name { Parser }

%locations
%initial-action
{
    @$.begin.filename = @$.end.filename = &driver.stream;
};

%code
{
  using namespace djnn;

  std::string cur_property;
  FatProcess *cur_parent;

  void
  make_text_property (std::string property, std::string arg)
  {
    if (property == "fill") {
      if (arg == "none") {
        new NoFill (cur_parent, "fill");
        return;
      } else {
        int color = SVG_Utils::djn__get_color_from_name (arg);
        if (color != -1) {
          new FillColor (cur_parent, "fill", color);
        }
      }
    }
    if (property == "stroke") {
      if (arg == "none") {
        new NoOutline (cur_parent, "stroke");
        return;
      } else {
        int color = SVG_Utils::djn__get_color_from_name (arg);
        if (color != -1) {
          new OutlineColor (cur_parent, "stroke", color);
        }
      }
    }
    if (property == "stroke-linecap") {
      djnCapStyle cap = DJN_BUTT_CAP;
      if (arg == "round")
        cap = DJN_ROUND_CAP;
      if (arg == "square")
        cap = DJN_SQUARE_CAP;
      new OutlineCapStyle(cur_parent, "stroke-linecap", cap);
    }
    if (property == "stroke-linejoin") {
      djnJoinStyle join = DJN_MITER_JOIN; /* SVG default value */
      if (arg == "round")
        join = DJN_ROUNDCAP_JOIN;
      if (arg == "bevel")
        join = DJN_BEVEL_JOIN;
      new OutlineJoinStyle(cur_parent, "stroke-linejoin", join);
    }
    if (property == "font-family") {
      new FontFamily (cur_parent, "font-famliy", arg);
    }
    if (property == "font-weight") {
      int val;
      if (arg == "normal")
        val = 50;
      if (arg == "bold")
        val = 75;
      if (arg == "lighter")
        val = -1;
      if (arg == "bolder")
        val = 100;
      new FontWeight (cur_parent, "font-weight", val);
      return;
    }
    if (property == "font-style") {
      djnFontSlope style = DJN_NORMAL_FONT;
      if (arg == "italic")
        style = DJN_ITALIC_FONT;
      if (arg == "oblique")
        style = DJN_OBLIQUE_FONT;
      new FontStyle(cur_parent, "font-style", style);
      return; 
    }
  }

  void
  make_num_property (std::string property, float arg)
  {
    if (property == "fill") {
      new FillColor (cur_parent, "fill", arg);
      return;
    }
    if (property == "stroke") {
      new OutlineColor (cur_parent, "stroke", arg);
      return;
    }
    if (property == "stroke-miterlimit") {
      new OutlineMiterLimit (cur_parent, "stroke-miterlimit", arg);
      return;
    }
    if (property == "stroke-width") {
      new OutlineWidth (cur_parent, "stroke-width", arg);
      return;
    }
    if (property == "opacity") {
      new FillOpacity (cur_parent, "opacity", arg);
      new OutlineOpacity (cur_parent, "stroke-opacity", arg);
      //new Connector (cur_parent, "", cur_parent, "opacity/a", cur_parent, "stroke-opacity/a");
      new SimpleConnector (cur_parent, "", cur_parent->find_child("opacity/a"), cur_parent->find_child("stroke-opacity/a"));
      return;
    }
    if (property == "stroke-opacity") {
      new OutlineOpacity (cur_parent, "stroke-opacity", arg);
      return;
    }
    if (property == "fill-opacity") {
      new FillOpacity (cur_parent, "stroke-opacity", arg);
      return;
    }
    if (property == "font-weight") {
      new FontWeight (cur_parent, "font-weight", arg/10);
      return;
    }
  }

  int
  rgb_to_hex (int r, int g, int b)
  {
    return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
  }


}

%parse-param { class Driver& driver }

%define parse.error verbose

%union {
  int integerVal;
  double doubleVal;
  std::string* stringVal;
}

%token END 0	"end of file"
%token EOL "end of line"
%token CDO
%token CDC
%token IMPORT_SYM
%token URL
%token <stringVal> IDENT
%token LINK_PSCLASS
%token ACTIVE_PSCLASS
%token VISITED_PSCLASS
%token IMPORTANT_SYM
%token <stringVal> STRING
%token <doubleVal> PERCENTAGE
%token <doubleVal> LENGTH
%token <doubleVal> EMS
%token <doubleVal> EXS
%token <stringVal> RGB
%token <doubleVal> NUMBER
%token CLASS_AFTER_IDENT
%token <stringVal> CLASS
%token <integerVal> HASH_AFTER_IDENT
%token <integerVal> HASH
%token FIRST_LETTER_AFTER_IDENT
%token FIRST_LINE_AFTER_IDENT
%token FIRST_LETTER
%token FIRST_LINE LINK_PSCLASS_AFTER_IDENT
%token ACTIVE_PSCLASS_AFTER_IDENT
%token VISITED_PSCLASS_AFTER_IDENT

%type <stringVal> solitary_class
%type <integerVal> hexcolor

%{

#include "driver.h"
#include "scanner.h"

#undef yylex
#define yylex driver._lexer->lex

%}

%%

stylesheet
 : CDO import_list ruleset_list CDC
 | import_list ruleset_list
 ;

import_list
 :
 | import_list import

ruleset_list
 :
 | ruleset_list ruleset

import
 : IMPORT_SYM STRING ';'
 | IMPORT_SYM URL ';'
 ;

unary_operator
 : 
 | '-'
 | '+'
 ;

operator
 : '/'
 | ','
 | /* empty */
 ;

property
 : IDENT { cur_property = std::string($1->data()); }
 ;

ruleset
 : selector_list '{' declaration_list '}'
 ;
 
 selector_list
 : selector
 | selector_list ',' selector

 declaration_list
 : declaration
 | declaration_list ';' declaration

selector
 : simple_selector_list
 | simple_selector_list pseudo_element
 | simple_selector_list solitary_pseudo_element
 | solitary_pseudo_element
 ;

 simple_selector_list
  :
  | simple_selector_list simple_selector

simple_selector
 : element_name id cssclass pseudo_class
 | solitary_id cssclass pseudo_class
 | solitary_class {
   std::string classname = $1->data ();
   if (classname.at(0) == '.')
    classname = classname.substr (1);
   cur_parent = new StyleSheet (driver.get_parent (),  classname);
 }
 | solitary_class pseudo_class
 | solitary_pseudo_class
 ;

element_name
 : IDENT
 ;
pseudo_class
 : LINK_PSCLASS_AFTER_IDENT
 | VISITED_PSCLASS_AFTER_IDENT
 | ACTIVE_PSCLASS_AFTER_IDENT
 ;

solitary_pseudo_class
 : LINK_PSCLASS
 | VISITED_PSCLASS
 | ACTIVE_PSCLASS
 ;

cssclass
 : CLASS_AFTER_IDENT
 ;

solitary_class
 : CLASS { $$ = $1; }
 ;

pseudo_element
 : FIRST_LETTER_AFTER_IDENT
 | FIRST_LINE_AFTER_IDENT
 ;

solitary_pseudo_element
 : FIRST_LETTER
 | FIRST_LINE
 ;

id
 : HASH_AFTER_IDENT
 ;

solitary_id
 : HASH
 ;

declaration
 : property ':' expr
 | property ':' expr prio
 | /* empty */
 ;

prio
 : IMPORTANT_SYM { std::cerr << "WARNING: symbol '!' is not supported"; }
 ;

expr
 : term 
 | term term_list { std::cerr << "WARNING: list of values is not supported for css attribute specification"; }
 ;

term_list
 :
 | term_list operator term

term
 : unary_operator NUMBER { make_num_property (cur_property, $2); };
 | unary_operator STRING {
   // remove quote
   std::string str ($2->data());
   str.erase (std::remove(str.begin(), str.end(), '\''), str.end());
   str.erase (std::remove(str.begin(), str.end(), '\"'), str.end());
   make_text_property (cur_property,str);
 }
 | unary_operator PERCENTAGE { make_num_property (cur_property, $2); }
 | unary_operator LENGTH { make_num_property (cur_property, $2); }
 | unary_operator EMS { make_num_property (cur_property, $2); }
 | unary_operator EXS { make_num_property (cur_property, $2); }
 | unary_operator IDENT { make_text_property (cur_property, $2->data()); }
 | hexcolor { make_num_property (cur_property, $1); }
 | URL
 | RGB
 ;
    /* There is a constraint on the color that it must
    ** have either 3, 6 or 9 hex-digits (i.e., [0-9a-fA-F])
    ** after the "#"; e.g., "#000" is OK, but "#abcd" is not.
    ** (Not checked by this program.)
    */
hexcolor
 : HASH { $$ = $1; }
 | HASH_AFTER_IDENT { $$ = $1; }
 ;



%%

void css::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}
