// A Bison parser, made by GNU Bison 3.4.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.


// Take the name prefix into account.
#define yylex   csslex

// First part of user prologue.
#line 15 "parser.y"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "core/core.h"
#include "base/base.h"
#include "gui/gui-dev.h"

using namespace djnn;

#line 54 "parser.cpp"


#include "parser.h"

// Second part of user prologue.
#line 210 "parser.y"


#include "driver.h"
#include "scanner.h"

#undef yylex
#define yylex driver._lexer->lex


#line 70 "parser.cpp"

// Unqualified %code blocks.
#line 48 "parser.y"

  std::string cur_property;
  Process *cur_parent;

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
      new Connector (cur_parent, "", cur_parent, "opacity/a", cur_parent, "stroke-opacity/a");
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



#line 193 "parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if CSSDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !CSSDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !CSSDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace css {
#line 284 "parser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   Parser ::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
   Parser :: Parser  (class Driver& driver_yyarg)
    :
#if CSSDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

   Parser ::~ Parser  ()
  {}

   Parser ::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
   Parser ::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value (std::move (that.value))
    , location (std::move (that.location))
  {}
#endif

  template <typename Base>
   Parser ::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
   Parser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
   Parser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (semantic_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}

  template <typename Base>
  bool
   Parser ::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
   Parser ::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_type.
   Parser ::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
   Parser ::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

   Parser ::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

   Parser ::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
   Parser ::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
   Parser ::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
   Parser ::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
   Parser ::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

   Parser ::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
   Parser ::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
   Parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

   Parser ::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

   Parser ::symbol_number_type
   Parser ::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

   Parser ::stack_symbol_type::stack_symbol_type ()
  {}

   Parser ::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

   Parser ::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
   Parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if CSSDEBUG
  template <typename Base>
  void
   Parser ::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
   Parser ::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
   Parser ::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
   Parser ::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if CSSDEBUG
  std::ostream&
   Parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   Parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   Parser ::debug_level_type
   Parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   Parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // CSSDEBUG

   Parser ::state_type
   Parser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
   Parser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
   Parser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   Parser ::operator() ()
  {
    return parse ();
  }

  int
   Parser ::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    // User initialization code.
#line 43 "parser.y"
{
    yyla.location.begin.filename = yyla.location.end.filename = &driver.stream;
}

#line 653 "parser.cpp"


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 16:
#line 253 "parser.y"
    { cur_property = std::string((yystack_[0].value.stringVal)->data()); }
#line 778 "parser.cpp"
    break;

  case 30:
#line 282 "parser.y"
    {
   std::string classname = (yystack_[0].value.stringVal)->data ();
   if (classname.at(0) == '.')
    classname = classname.substr (1);
   cur_parent = new StyleSheet (driver.get_parent (),  classname);
 }
#line 789 "parser.cpp"
    break;

  case 41:
#line 312 "parser.y"
    { (yylhs.value.stringVal) = (yystack_[0].value.stringVal); }
#line 795 "parser.cpp"
    break;

  case 51:
#line 340 "parser.y"
    { std::cerr << "WARNING: symbol '!' is not supported"; }
#line 801 "parser.cpp"
    break;

  case 53:
#line 345 "parser.y"
    { std::cerr << "WARNING: list of values is not supported for css attribute specification"; }
#line 807 "parser.cpp"
    break;

  case 56:
#line 353 "parser.y"
    { make_num_property (cur_property, (yystack_[0].value.doubleVal)); }
#line 813 "parser.cpp"
    break;

  case 57:
#line 354 "parser.y"
    {
   // remove quote
   std::string str ((yystack_[0].value.stringVal)->data());
   str.erase (std::remove(str.begin(), str.end(), '\''), str.end());
   str.erase (std::remove(str.begin(), str.end(), '\"'), str.end());
   make_text_property (cur_property,str);
 }
#line 825 "parser.cpp"
    break;

  case 58:
#line 361 "parser.y"
    { make_num_property (cur_property, (yystack_[0].value.doubleVal)); }
#line 831 "parser.cpp"
    break;

  case 59:
#line 362 "parser.y"
    { make_num_property (cur_property, (yystack_[0].value.doubleVal)); }
#line 837 "parser.cpp"
    break;

  case 60:
#line 363 "parser.y"
    { make_num_property (cur_property, (yystack_[0].value.doubleVal)); }
#line 843 "parser.cpp"
    break;

  case 61:
#line 364 "parser.y"
    { make_num_property (cur_property, (yystack_[0].value.doubleVal)); }
#line 849 "parser.cpp"
    break;

  case 62:
#line 365 "parser.y"
    { make_text_property (cur_property, (yystack_[0].value.stringVal)->data()); }
#line 855 "parser.cpp"
    break;

  case 63:
#line 366 "parser.y"
    { make_num_property (cur_property, (yystack_[0].value.integerVal)); }
#line 861 "parser.cpp"
    break;

  case 66:
#line 376 "parser.y"
    { (yylhs.value.integerVal) = (yystack_[0].value.integerVal); }
#line 867 "parser.cpp"
    break;

  case 67:
#line 377 "parser.y"
    { (yylhs.value.integerVal) = (yystack_[0].value.integerVal); }
#line 873 "parser.cpp"
    break;


#line 877 "parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
   Parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
   Parser ::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char  Parser ::yypact_ninf_ = -48;

  const signed char  Parser ::yytable_ninf_ = -54;

  const signed char
   Parser ::yypact_[] =
  {
       1,   -48,    10,    26,    26,   -48,     2,     0,   -48,    -3,
       4,    11,   -48,   -48,   -48,   -23,   -48,    29,   -48,   -48,
     -48,   -48,   -10,    33,   -48,   -48,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,    21,   -48,    -8,   -48,   -48,    24,   -48,
     -48,     7,   -30,   -48,   -48,    24,   -48,   -48,   -48,   -48,
     -48,    -8,    -4,    33,   -48,    -8,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,    49,    34,    -6,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,    -1,   -48,
     -48,    -4,   -48
  };

  const unsigned char
   Parser ::yydefact_[] =
  {
       4,     4,     0,     6,     6,     1,     0,    26,     5,    26,
       0,     0,    44,    45,     7,     0,    18,    22,    25,     2,
       9,     8,    26,    50,    33,    37,    39,    38,    41,    47,
      42,    43,    27,     0,    32,    30,    23,    24,     0,    19,
      16,     0,     0,    20,    46,     0,    34,    36,    35,    31,
      40,     0,    10,    50,    17,     0,    29,    64,    65,    67,
      66,    11,    12,     0,    48,    54,    63,    21,    28,    62,
      57,    58,    59,    60,    61,    56,    51,    49,    15,    13,
      14,    10,    55
  };

  const signed char
   Parser ::yypgoto_[] =
  {
     -48,   -48,    46,    44,   -48,   -48,   -48,   -48,   -48,   -48,
     -48,    27,   -48,   -48,   -48,   -47,   -48,     6,   -48,   -48,
      41,   -48,   -48,     8,   -48,   -48,   -48,   -22,   -48
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
      -1,     2,     3,     7,     8,    63,    81,    41,    14,    15,
      42,    16,    17,    32,    33,    49,    34,    51,    35,    36,
      18,    45,    38,    43,    77,    64,    78,    65,    66
  };

  const signed char
   Parser ::yytable_[] =
  {
      -3,    53,    19,    57,    56,     1,   -52,    54,    68,    10,
       5,   -53,    22,    23,    58,    11,    12,    13,    59,    60,
      46,    47,    48,    12,    13,   -52,    12,    13,    61,    62,
     -53,   -52,     6,    79,    80,    20,   -53,    24,    25,    26,
      27,    40,    21,    44,    50,    52,    76,     4,     9,    39,
      28,    55,    29,    30,    31,    12,    13,    69,    37,    82,
       0,    67,    70,    71,    72,    73,    74,     0,    75
  };

  const signed char
   Parser ::yycheck_[] =
  {
       0,    31,     5,     7,    51,     4,    12,    37,    55,     7,
       0,    12,    35,    36,    18,    13,    26,    27,    22,    23,
      28,    29,    30,    26,    27,    31,    26,    27,    32,    33,
      31,    37,     6,    34,    35,    31,    37,     8,     9,    10,
      11,     8,    31,    22,    20,    38,    12,     1,     4,    22,
      21,    45,    23,    24,    25,    26,    27,     8,    17,    81,
      -1,    53,    13,    14,    15,    16,    17,    -1,    19
  };

  const unsigned char
   Parser ::yystos_[] =
  {
       0,     4,    40,    41,    41,     0,     6,    42,    43,    42,
       7,    13,    26,    27,    47,    48,    50,    51,    59,     5,
      31,    31,    35,    36,     8,     9,    10,    11,    21,    23,
      24,    25,    52,    53,    55,    57,    58,    59,    61,    50,
       8,    46,    49,    62,    22,    60,    28,    29,    30,    54,
      20,    56,    38,    31,    37,    56,    54,     7,    18,    22,
      23,    32,    33,    44,    64,    66,    67,    62,    54,     8,
      13,    14,    15,    16,    17,    19,    12,    63,    65,    34,
      35,    45,    66
  };

  const unsigned char
   Parser ::yyr1_[] =
  {
       0,    39,    40,    40,    41,    41,    42,    42,    43,    43,
      44,    44,    44,    45,    45,    45,    46,    47,    48,    48,
      49,    49,    50,    50,    50,    50,    51,    51,    52,    52,
      52,    52,    52,    53,    54,    54,    54,    55,    55,    55,
      56,    57,    58,    58,    59,    59,    60,    61,    62,    62,
      62,    63,    64,    64,    65,    65,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    67,    67
  };

  const unsigned char
   Parser ::yyr2_[] =
  {
       0,     2,     4,     2,     0,     2,     0,     2,     3,     3,
       0,     1,     1,     1,     1,     0,     1,     4,     1,     3,
       1,     3,     1,     2,     2,     1,     0,     2,     4,     3,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       0,     1,     1,     2,     0,     3,     2,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  Parser ::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"end of line\"", "CDO",
  "CDC", "IMPORT_SYM", "URL", "IDENT", "LINK_PSCLASS", "ACTIVE_PSCLASS",
  "VISITED_PSCLASS", "IMPORTANT_SYM", "STRING", "PERCENTAGE", "LENGTH",
  "EMS", "EXS", "RGB", "NUMBER", "CLASS_AFTER_IDENT", "CLASS",
  "HASH_AFTER_IDENT", "HASH", "FIRST_LETTER_AFTER_IDENT",
  "FIRST_LINE_AFTER_IDENT", "FIRST_LETTER", "FIRST_LINE",
  "LINK_PSCLASS_AFTER_IDENT", "ACTIVE_PSCLASS_AFTER_IDENT",
  "VISITED_PSCLASS_AFTER_IDENT", "';'", "'-'", "'+'", "'/'", "','", "'{'",
  "'}'", "':'", "$accept", "stylesheet", "import_list", "ruleset_list",
  "import", "unary_operator", "operator", "property", "ruleset",
  "selector_list", "declaration_list", "selector", "simple_selector_list",
  "simple_selector", "element_name", "pseudo_class",
  "solitary_pseudo_class", "cssclass", "solitary_class", "pseudo_element",
  "solitary_pseudo_element", "id", "solitary_id", "declaration", "prio",
  "expr", "term_list", "term", "hexcolor", YY_NULLPTR
  };

#if CSSDEBUG
  const unsigned short
   Parser ::yyrline_[] =
  {
       0,   223,   223,   224,   228,   229,   232,   233,   236,   237,
     241,   242,   243,   247,   248,   249,   253,   257,   261,   262,
     265,   266,   269,   270,   271,   272,   276,   277,   280,   281,
     282,   288,   289,   293,   296,   297,   298,   302,   303,   304,
     308,   312,   316,   317,   321,   322,   326,   330,   334,   335,
     336,   340,   344,   345,   349,   350,   353,   354,   361,   362,
     363,   364,   365,   366,   367,   368,   376,   377
  };

  // Print the state stack on the debug stream.
  void
   Parser ::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
   Parser ::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // CSSDEBUG

   Parser ::token_number_type
   Parser ::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    33,    35,    32,     2,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    38,    31,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    36,     2,    37,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
    };
    const unsigned user_token_number_max_ = 285;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

} // css
#line 1375 "parser.cpp"

#line 382 "parser.y"


void css::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}
