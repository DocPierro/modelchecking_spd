// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

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
#define yylex   evallex



#include "Eval-parser.hh"


// Unqualified %code blocks.
#line 38 "src/ModelGenerator/Eval/Eval-parser.yy"

#include "Eval.hpp"
#include <set>
#include <vector>


#line 54 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"


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
#if YYDEBUG

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

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace eval {
#line 145 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Eval_parser::yytnamerr_ (const char *yystr)
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
  Eval_parser::Eval_parser (Eval& Evaluate_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      Evaluate (Evaluate_yyarg)
  {}

  Eval_parser::~Eval_parser ()
  {}

  Eval_parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  Eval_parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value (std::move (that.value))
    , location (std::move (that.location))
  {}
#endif

  template <typename Base>
  Eval_parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  Eval_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  Eval_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (semantic_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}

  template <typename Base>
  bool
  Eval_parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  Eval_parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_type.
  Eval_parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  Eval_parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  Eval_parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  Eval_parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  Eval_parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  Eval_parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  Eval_parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  Eval_parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Eval_parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Eval_parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Eval_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Eval_parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Eval_parser::symbol_number_type
  Eval_parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  Eval_parser::stack_symbol_type::stack_symbol_type ()
  {}

  Eval_parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Eval_parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  Eval_parser::stack_symbol_type&
  Eval_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }

  Eval_parser::stack_symbol_type&
  Eval_parser::stack_symbol_type::operator= (stack_symbol_type& that)
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
  Eval_parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.type_get ())
    {
      case 3: // str
#line 69 "src/ModelGenerator/Eval/Eval-parser.yy"
                    { delete (yysym.value.name); }
#line 389 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
        break;

      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
  Eval_parser::yy_print_ (std::ostream& yyo,
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
    switch (yytype)
    {
      case 3: // str
#line 68 "src/ModelGenerator/Eval/Eval-parser.yy"
                    { debug_stream () << *(yysym.value.name); }
#line 420 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
        break;

      case 4: // rval
#line 72 "src/ModelGenerator/Eval/Eval-parser.yy"
                    { debug_stream () << (yysym.value.RealVal); }
#line 426 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
        break;

      case 5: // ival
#line 71 "src/ModelGenerator/Eval/Eval-parser.yy"
                    { debug_stream () << (yysym.value.IntVal); }
#line 432 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
        break;

      case 21: // exp
#line 72 "src/ModelGenerator/Eval/Eval-parser.yy"
                    { debug_stream () << (yysym.value.RealVal); }
#line 438 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
        break;

      default:
        break;
    }
    yyo << ')';
  }
#endif

  void
  Eval_parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Eval_parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Eval_parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Eval_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Eval_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Eval_parser::debug_level_type
  Eval_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Eval_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Eval_parser::state_type
  Eval_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  Eval_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Eval_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Eval_parser::operator() ()
  {
    return parse ();
  }

  int
  Eval_parser::parse ()
  {
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
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
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
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, Evaluate));
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
      {
        goto yydefault;
      }

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
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
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
  case 2:
#line 82 "src/ModelGenerator/Eval/Eval-parser.yy"
                   {Evaluate.IntResult=(int) (yystack_[0].value.RealVal);Evaluate.RealResult=(yystack_[0].value.RealVal);}
#line 679 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 3:
#line 84 "src/ModelGenerator/Eval/Eval-parser.yy"
             {(yylhs.value.RealVal)=(yystack_[0].value.IntVal);}
#line 685 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 4:
#line 85 "src/ModelGenerator/Eval/Eval-parser.yy"
              {(yylhs.value.RealVal)=(yystack_[0].value.RealVal);}
#line 691 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 5:
#line 86 "src/ModelGenerator/Eval/Eval-parser.yy"
                   {(yylhs.value.RealVal)=(yystack_[1].value.RealVal);}
#line 697 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 6:
#line 87 "src/ModelGenerator/Eval/Eval-parser.yy"
                             {(yylhs.value.RealVal)=-(yystack_[0].value.RealVal);}
#line 703 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 7:
#line 88 "src/ModelGenerator/Eval/Eval-parser.yy"
                     {(yylhs.value.RealVal)=(yystack_[2].value.RealVal)+(yystack_[0].value.RealVal);}
#line 709 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 8:
#line 89 "src/ModelGenerator/Eval/Eval-parser.yy"
                    {(yylhs.value.RealVal)=(yystack_[2].value.RealVal)*(yystack_[0].value.RealVal);}
#line 715 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 9:
#line 90 "src/ModelGenerator/Eval/Eval-parser.yy"
                      {(yylhs.value.RealVal)=(yystack_[2].value.RealVal)-(yystack_[0].value.RealVal);}
#line 721 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 10:
#line 91 "src/ModelGenerator/Eval/Eval-parser.yy"
                    {(yylhs.value.RealVal)=double((yystack_[0].value.RealVal));}
#line 727 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 11:
#line 92 "src/ModelGenerator/Eval/Eval-parser.yy"
                        {if((yystack_[0].value.RealVal)==0)        {cout<<"Division by zero !";YYABORT;}
                                         else (yylhs.value.RealVal)=(yystack_[2].value.RealVal)/double (yystack_[0].value.RealVal);
                                        }
#line 735 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 12:
#line 95 "src/ModelGenerator/Eval/Eval-parser.yy"
                       {(yylhs.value.RealVal)=pow((yystack_[2].value.RealVal),(yystack_[0].value.RealVal));}
#line 741 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 13:
#line 96 "src/ModelGenerator/Eval/Eval-parser.yy"
                                        {if((yystack_[3].value.RealVal)<=(yystack_[1].value.RealVal)) (yylhs.value.RealVal)=(yystack_[3].value.RealVal); else (yylhs.value.RealVal)=(yystack_[1].value.RealVal);}
#line 747 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 14:
#line 97 "src/ModelGenerator/Eval/Eval-parser.yy"
                                        {if((yystack_[3].value.RealVal)>=(yystack_[1].value.RealVal)) (yylhs.value.RealVal)=(yystack_[3].value.RealVal);       else (yylhs.value.RealVal)=(yystack_[1].value.RealVal);}
#line 753 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;

  case 15:
#line 98 "src/ModelGenerator/Eval/Eval-parser.yy"
                                                {(yylhs.value.RealVal)=floor((yystack_[1].value.RealVal));}
#line 759 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"
    break;


#line 763 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"

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
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
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
      error_token.state = state_type (yyn);
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
  Eval_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  Eval_parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
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
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

        int yyn = yypact_[+yystate];
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
              if (yycheck_[yyx + yyn] == yyx && yyx != yy_error_token_
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
    std::ptrdiff_t yyi = 0;
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


  const signed char Eval_parser::yypact_ninf_ = -12;

  const signed char Eval_parser::yytable_ninf_ = -1;

  const signed char
  Eval_parser::yypact_[] =
  {
      14,   -12,   -12,   -11,   -10,     3,    14,    14,    14,    11,
      67,    14,    14,    14,    -3,    39,   -12,   -12,    14,    14,
      14,    14,    14,    23,    31,    46,   -12,    -3,    -3,    10,
      10,   -12,    14,    14,   -12,    53,    60,   -12,   -12
  };

  const signed char
  Eval_parser::yydefact_[] =
  {
       0,     4,     3,     0,     0,     0,     0,     0,     0,     0,
       2,     0,     0,     0,     6,     0,    10,     1,     0,     0,
       0,     0,     0,     0,     0,     0,     5,     7,     9,     8,
      11,    12,     0,     0,    15,     0,     0,    13,    14
  };

  const signed char
  Eval_parser::yypgoto_[] =
  {
     -12,   -12,    -6
  };

  const signed char
  Eval_parser::yydefgoto_[] =
  {
      -1,     9,    10
  };

  const signed char
  Eval_parser::yytable_[] =
  {
      14,    15,    16,    11,    12,    23,    24,    25,    20,    21,
      22,    17,    27,    28,    29,    30,    31,    13,     1,     2,
       3,     4,     5,    22,     6,     0,    35,    36,     7,     0,
       0,     8,    18,    19,    20,    21,    22,     0,     0,    32,
      18,    19,    20,    21,    22,     0,     0,    33,    18,    19,
      20,    21,    22,     0,    26,    18,    19,    20,    21,    22,
       0,    34,    18,    19,    20,    21,    22,     0,    37,    18,
      19,    20,    21,    22,     0,    38,    18,    19,    20,    21,
      22
  };

  const signed char
  Eval_parser::yycheck_[] =
  {
       6,     7,     8,    14,    14,    11,    12,    13,    11,    12,
      13,     0,    18,    19,    20,    21,    22,    14,     4,     5,
       6,     7,     8,    13,    10,    -1,    32,    33,    14,    -1,
      -1,    17,     9,    10,    11,    12,    13,    -1,    -1,    16,
       9,    10,    11,    12,    13,    -1,    -1,    16,     9,    10,
      11,    12,    13,    -1,    15,     9,    10,    11,    12,    13,
      -1,    15,     9,    10,    11,    12,    13,    -1,    15,     9,
      10,    11,    12,    13,    -1,    15,     9,    10,    11,    12,
      13
  };

  const signed char
  Eval_parser::yystos_[] =
  {
       0,     4,     5,     6,     7,     8,    10,    14,    17,    20,
      21,    14,    14,    14,    21,    21,    21,     0,     9,    10,
      11,    12,    13,    21,    21,    21,    15,    21,    21,    21,
      21,    21,    16,    16,    15,    21,    21,    15,    15
  };

  const signed char
  Eval_parser::yyr1_[] =
  {
       0,    19,    20,    21,    21,    21,    21,    21,    21,    21,
      21,    21,    21,    21,    21,    21
  };

  const signed char
  Eval_parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     3,     2,     3,     3,     3,
       2,     3,     3,     6,     6,     4
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Eval_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "str", "rval", "ival", "MIN",
  "MAX", "FLOOR", "PLUS", "MINUS", "MUL", "DIV", "POWER", "LB", "RB",
  "COMMA", "DOUBLE", "NEG", "$accept", "ArithmethicExp", "exp", YY_NULLPTR
  };

#if YYDEBUG
  const signed char
  Eval_parser::yyrline_[] =
  {
       0,    82,    82,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    95,    96,    97,    98
  };

  // Print the state stack on the debug stream.
  void
  Eval_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Eval_parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Eval_parser::token_number_type
  Eval_parser::yytranslate_ (int t)
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
      15,    16,    17,    18
    };
    const int user_token_number_max_ = 273;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

} // eval
#line 1217 "/home/pierre/Documents/Cosmos/src/ModelGenerator/Eval/Eval-parser.cc"

#line 100 "src/ModelGenerator/Eval/Eval-parser.yy"


void
eval::Eval_parser::error (const eval::Eval_parser::location_type& l,
                          const std::string& m)
{
 Evaluate.error (l, m);
}
