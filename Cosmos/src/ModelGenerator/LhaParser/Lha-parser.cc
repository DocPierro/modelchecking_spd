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
#define yylex   lhalex



#include "Lha-parser.hh"


// Unqualified %code blocks.
#line 42 "src/ModelGenerator/LhaParser/Lha-parser.yy"

        #include "../HaslFormula.hpp"
        #include "Lha-Reader.hpp"
        #include <set>
        #include <vector>

        Eval LhaEvaluate;



        vector<string> FuncFlowVector;

        vector<string> FuncUpdateVector;
    vector<string> FuncUpdateVectorIndex;

        set <string> PetriTransitions;
        set <string> SubSet;



        vector < string > CoeffsVector;
        vector < vector <string> > CoeffsMatrix;
        vector <string> CST;
        vector <string> comp;







#line 79 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"


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

namespace lha {
#line 170 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Lha_parser::yytnamerr_ (const char *yystr)
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
  Lha_parser::Lha_parser (Lha_Reader& Reader_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      Reader (Reader_yyarg)
  {}

  Lha_parser::~Lha_parser ()
  {}

  Lha_parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  Lha_parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value (std::move (that.value))
    , location (std::move (that.location))
  {}
#endif

  template <typename Base>
  Lha_parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
    , location (that.location)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  Lha_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_MOVE_REF (location_type) l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  Lha_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (semantic_type) v, YY_RVREF (location_type) l)
    : Base (t)
    , value (YY_MOVE (v))
    , location (YY_MOVE (l))
  {}

  template <typename Base>
  bool
  Lha_parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  Lha_parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
    location = YY_MOVE (s.location);
  }

  // by_type.
  Lha_parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  Lha_parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  Lha_parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  Lha_parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  Lha_parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  Lha_parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  Lha_parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  Lha_parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Lha_parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Lha_parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Lha_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Lha_parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Lha_parser::symbol_number_type
  Lha_parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  Lha_parser::stack_symbol_type::stack_symbol_type ()
  {}

  Lha_parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value), YY_MOVE (that.location))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Lha_parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value), YY_MOVE (that.location))
  {
    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  Lha_parser::stack_symbol_type&
  Lha_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }

  Lha_parser::stack_symbol_type&
  Lha_parser::stack_symbol_type::operator= (stack_symbol_type& that)
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
  Lha_parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    switch (yysym.type_get ())
    {
      case 3: // str
#line 171 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                    { delete (yysym.value.name); }
#line 414 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
        break;

      default:
        break;
    }
  }

#if YYDEBUG
  template <typename Base>
  void
  Lha_parser::yy_print_ (std::ostream& yyo,
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
#line 170 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                    { debug_stream () << *(yysym.value.name); }
#line 445 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
        break;

      case 4: // rval
#line 174 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                    { debug_stream () << (yysym.value.RealVal); }
#line 451 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
        break;

      case 5: // ival
#line 173 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                    { debug_stream () << (yysym.value.IntVal); }
#line 457 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
        break;

      case 113: // rorival
#line 174 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                    { debug_stream () << (yysym.value.RealVal); }
#line 463 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
        break;

      default:
        break;
    }
    yyo << ')';
  }
#endif

  void
  Lha_parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Lha_parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Lha_parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Lha_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Lha_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Lha_parser::debug_level_type
  Lha_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Lha_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Lha_parser::state_type
  Lha_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  Lha_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Lha_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Lha_parser::operator() ()
  {
    return parse ();
  }

  int
  Lha_parser::parse ()
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
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, Reader));
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
  case 3:
#line 191 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                            {Reader.MyLha.isDeterministic=false;}
#line 704 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 11:
#line 205 "src/ModelGenerator/LhaParser/Lha-parser.yy"
     {snprintf((yylhs.value.expression),BUFF_SIZE,"%i",(yystack_[0].value.IntVal));}
#line 710 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 12:
#line 206 "src/ModelGenerator/LhaParser/Lha-parser.yy"
      {
        if(Reader.MyLha.PlaceIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s; s<<" Marking.P->_PL_"<< (yystack_[0].value.name)->c_str()<<" ";
                snprintf((yylhs.value.expression),BUFF_SIZE, "%s",(s.str()).c_str());
        }
        else if(Reader.MyLha.LhaIntConstant.find(*(yystack_[0].value.name))!=Reader.MyLha.LhaIntConstant.end())
        {std::ostringstream s; s<<Reader.MyLha.LhaIntConstant[*(yystack_[0].value.name)];
                snprintf((yylhs.value.expression),BUFF_SIZE, "%s",(s.str()).c_str());
        } else {
        size_t vararray = Reader.MyLha.Vars.find(*(yystack_[0].value.name));
        if(vararray!= Reader.MyLha.NbVar && Reader.MyLha.Vars.type[vararray]==DISCRETE_VARIABLE){
            snprintf((yylhs.value.expression),BUFF_SIZE, " floor(Vars->%s)", Reader.MyLha.Vars.label[vararray].c_str() );
        } else {cout<<"'"<<*(yystack_[0].value.name)<<"' is not a place label or a defined constant"<<endl;YYABORT;}
    }
}
#line 730 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 13:
#line 221 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                             {
    auto pi = Reader.MyLha.PlaceIndex.find(*(yystack_[3].value.name));
        if( pi !=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s;
         s<<" Marking.P->_PL_"<< (yystack_[3].value.name)->c_str()<< ".mult";
     if(P.is_domain_impl_set){
         size_t coldom = Reader.MyLha.MyGspn->placeStruct[pi->second].colorDom;
         s << "("<< Reader.MyLha.MyGspn->colDoms[coldom].tokname()<< "(" << (yystack_[1].value.expression) << "))";
     } else s << (yystack_[1].value.expression);
                snprintf((yylhs.value.expression),BUFF_SIZE, "%s",(s.str()).c_str());
        }else{cout<<"'"<<*(yystack_[3].value.name)<<"' is not a place label "<<endl;YYABORT;}}
#line 746 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 14:
#line 232 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                {
    size_t vararray = Reader.MyLha.Vars.find(*(yystack_[3].value.name));
        if(vararray!= Reader.MyLha.NbVar && Reader.MyLha.Vars.type[vararray]==INT_INDEXED_DISC_ARRAY)
        {std::ostringstream s;
        s<<" Vars.P->" << Reader.MyLha.Vars.label[vararray] << "["<< *(yystack_[1].value.expression) << "]";
                snprintf((yylhs.value.expression),BUFF_SIZE, "%s",(s.str()).c_str());
        }else{cout<<"'"<<*(yystack_[3].value.name)<<"' is not a discrete array name "<<endl;YYABORT;}}
#line 758 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 15:
#line 239 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                         {snprintf((yylhs.value.expression),BUFF_SIZE,"( %s )", (yystack_[1].value.expression));       }
#line 764 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 16:
#line 240 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                    {snprintf((yylhs.value.expression),BUFF_SIZE,"-%s",(yystack_[0].value.expression));}
#line 770 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 17:
#line 241 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                              {snprintf((yylhs.value.expression),BUFF_SIZE,"%s + %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 776 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 18:
#line 242 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                              {snprintf((yylhs.value.expression),BUFF_SIZE,"%s - %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 782 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 19:
#line 243 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                              {snprintf((yylhs.value.expression),BUFF_SIZE,"%s * %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 788 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 20:
#line 244 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                              {snprintf((yylhs.value.expression),BUFF_SIZE,"pow(%s , %s)", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 794 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 21:
#line 245 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                {snprintf((yylhs.value.expression),BUFF_SIZE,"floor( %s )", (yystack_[1].value.expression));  }
#line 800 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 22:
#line 246 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                      {snprintf((yylhs.value.expression),BUFF_SIZE,"floor( %s /double(%s))", (yystack_[3].value.expression),(yystack_[1].value.expression));  }
#line 806 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 23:
#line 247 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                      {snprintf((yylhs.value.expression),BUFF_SIZE,"min(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 812 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 24:
#line 248 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                      {snprintf((yylhs.value.expression),BUFF_SIZE,"max(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 818 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 25:
#line 249 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                      {snprintf((yylhs.value.expression),BUFF_SIZE,"((int)%s %% (int)%s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 824 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 26:
#line 252 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                          {snprintf((yylhs.value.expression),BUFF_SIZE, "%f",(yystack_[0].value.RealVal));}
#line 830 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 27:
#line 253 "src/ModelGenerator/LhaParser/Lha-parser.yy"
       {snprintf((yylhs.value.expression),BUFF_SIZE,"%d",(yystack_[0].value.IntVal));}
#line 836 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 28:
#line 254 "src/ModelGenerator/LhaParser/Lha-parser.yy"
            {
    if(Reader.MyLha.PlaceIndex.find(*(yystack_[1].value.name))!=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s; s<<" Marking.P->_PL_"<<(yystack_[1].value.name)->c_str();
        if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*(yystack_[1].value.name)]].colorDom !=0 )s <<".value()";
        snprintf((yylhs.value.expression),BUFF_SIZE, "%s ",(s.str()).c_str());
    }
    else {cout<<"'"<<*(yystack_[1].value.name)<<"' is not a place label"<<endl;YYABORT;}}
#line 848 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 29:
#line 261 "src/ModelGenerator/LhaParser/Lha-parser.yy"
      {
        if(Reader.MyLha.PlaceIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s; s<<" Marking.P->_PL_"<<(yystack_[0].value.name)->c_str();
                if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*(yystack_[0].value.name)]].colorDom !=0 )s <<".card()";
                snprintf((yylhs.value.expression),BUFF_SIZE, "%s ",(s.str()).c_str());
        }
        else if(Reader.MyLha.LhaRealConstant.find(*(yystack_[0].value.name))!=Reader.MyLha.LhaRealConstant.end())
        {std::ostringstream s; s<<Reader.MyLha.LhaRealConstant[*(yystack_[0].value.name)];
                snprintf((yylhs.value.expression), BUFF_SIZE,"%s",(s.str()).c_str());
        }

        else{cout<<"'"<<*(yystack_[0].value.name)<<"' is not a place label or a defined constant"<<endl;YYABORT;}}
#line 865 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 30:
#line 273 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                             {
    auto pi = Reader.MyLha.PlaceIndex.find(*(yystack_[3].value.name));
    if( pi !=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s;
        s<<" Marking.P->_PL_"<< (yystack_[3].value.name)->c_str()<< ".mult";
        if(P.is_domain_impl_set){
            size_t coldom = Reader.MyLha.MyGspn->placeStruct[pi->second].colorDom;
            s << "("<< Reader.MyLha.MyGspn->colDoms[coldom].tokname()<< "(" << (yystack_[1].value.expression) << "))";
     } else s << (yystack_[1].value.expression);
                snprintf((yylhs.value.expression),BUFF_SIZE, "%s",(s.str()).c_str());
        }else{cout<<"'"<<*(yystack_[3].value.name)<<"' is not a place label "<<endl;YYABORT;}}
#line 881 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 31:
#line 284 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                        {snprintf((yylhs.value.expression),BUFF_SIZE,"( %s )", (yystack_[1].value.expression));       }
#line 887 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 32:
#line 285 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                     {snprintf((yylhs.value.expression),BUFF_SIZE,"-%s",(yystack_[0].value.expression));}
#line 893 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 33:
#line 286 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                               {snprintf((yylhs.value.expression),BUFF_SIZE,"%s /double(%s)", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 899 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 34:
#line 287 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                {snprintf((yylhs.value.expression),BUFF_SIZE,"%s + %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 905 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 35:
#line 288 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                {snprintf((yylhs.value.expression),BUFF_SIZE,"%s - %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 911 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 36:
#line 289 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                {snprintf((yylhs.value.expression),BUFF_SIZE,"%s * %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 917 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 37:
#line 290 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                {snprintf((yylhs.value.expression),BUFF_SIZE,"pow(%s , %s)", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 923 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 38:
#line 291 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                 {snprintf((yylhs.value.expression),BUFF_SIZE,"floor( %s )", (yystack_[1].value.expression));  }
#line 929 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 39:
#line 292 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                        {snprintf((yylhs.value.expression),BUFF_SIZE,"min(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 935 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 40:
#line 293 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                        {snprintf((yylhs.value.expression),BUFF_SIZE,"max(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 941 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 41:
#line 295 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                             {snprintf((yylhs.value.expression),BUFF_SIZE, "%f",(yystack_[0].value.RealVal));}
#line 947 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 42:
#line 296 "src/ModelGenerator/LhaParser/Lha-parser.yy"
       {snprintf((yylhs.value.expression),BUFF_SIZE,"%d",(yystack_[0].value.IntVal));}
#line 953 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 43:
#line 297 "src/ModelGenerator/LhaParser/Lha-parser.yy"
            {
    if(Reader.MyLha.PlaceIndex.find(*(yystack_[1].value.name))!=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s; s<<" Marking.P->_PL_"<<(yystack_[1].value.name)->c_str();
        if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*(yystack_[1].value.name)]].colorDom !=0 )s <<".value()";
        snprintf((yylhs.value.expression),BUFF_SIZE, "%s ",(s.str()).c_str());
    }
    else {cout<<"'"<<*(yystack_[1].value.name)<<"' is not a place label"<<endl;YYABORT;}}
#line 965 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 44:
#line 304 "src/ModelGenerator/LhaParser/Lha-parser.yy"
      {
        if(Reader.MyLha.PlaceIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s; s<<" Marking.P->_PL_"<<(yystack_[0].value.name)->c_str();
                if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*(yystack_[0].value.name)]].colorDom !=0 )s <<".card()";
                snprintf((yylhs.value.expression),BUFF_SIZE, "%s ",(s.str()).c_str());
        }
        else{ if(Reader.MyLha.LhaRealConstant.find(*(yystack_[0].value.name))!=Reader.MyLha.LhaRealConstant.end())
                {std::ostringstream s; s<<Reader.MyLha.LhaRealConstant[*(yystack_[0].value.name)];
                        snprintf((yylhs.value.expression),BUFF_SIZE, "%s",(s.str()).c_str());
                }
                else {if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
                        {
                                std::ostringstream s; s<<"Vars->"<< (yystack_[0].value.name)->c_str();
                                snprintf((yylhs.value.expression),BUFF_SIZE, "%s",(s.str()).c_str());
                        }
                        else {cout<<"'"<<*(yystack_[0].value.name)<<"' is not a place label, a lha variable or a defined constant"<<endl;YYABORT;}
                }
        }
}
#line 989 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 45:
#line 323 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                {
    size_t varin = Reader.MyLha.Vars.find(*(yystack_[3].value.name));
    if(varin != Reader.MyLha.Vars.label.size()){
        snprintf((yylhs.value.expression),BUFF_SIZE, "Vars->%s[%s]", (yystack_[3].value.name)->c_str(),(yystack_[1].value.expression));
    }
        else {cout<<"'"<<*(yystack_[3].value.name)<<"' is not an lha array variable"<<endl;YYABORT;}
    }
#line 1001 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 46:
#line 330 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                             {
    auto pi = Reader.MyLha.PlaceIndex.find(*(yystack_[3].value.name));
    if( pi !=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s;
        s<<" Marking.P->_PL_"<< (yystack_[3].value.name)->c_str()<< ".mult";
        if(P.is_domain_impl_set){
            size_t coldom = Reader.MyLha.MyGspn->placeStruct[pi->second].colorDom;
            s << "("<< Reader.MyLha.MyGspn->colDoms[coldom].tokname()<< "(" << (yystack_[1].value.expression) << "))";
     } else s << (yystack_[1].value.expression);
                snprintf((yylhs.value.expression),BUFF_SIZE, "%s",(s.str()).c_str());
        }else{cout<<"'"<<*(yystack_[3].value.name)<<"' is not a place label "<<endl;YYABORT;}}
#line 1017 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 47:
#line 341 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                           {snprintf((yylhs.value.expression),BUFF_SIZE,"( %s )", (yystack_[1].value.expression));       }
#line 1023 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 48:
#line 342 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                        {snprintf((yylhs.value.expression),BUFF_SIZE,"-%s",(yystack_[0].value.expression));}
#line 1029 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 49:
#line 343 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                      {snprintf((yylhs.value.expression),BUFF_SIZE,"%s /double(%s)", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 1035 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 50:
#line 344 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                      {snprintf((yylhs.value.expression),BUFF_SIZE,"%s + %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 1041 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 51:
#line 345 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                      {snprintf((yylhs.value.expression),BUFF_SIZE,"%s - %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 1047 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 52:
#line 346 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                      {snprintf((yylhs.value.expression),BUFF_SIZE,"%s * %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 1053 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 53:
#line 347 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                      {snprintf((yylhs.value.expression),BUFF_SIZE,"pow(%s , %s)", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 1059 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 54:
#line 348 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                    {snprintf((yylhs.value.expression),BUFF_SIZE,"floor( %s )", (yystack_[1].value.expression));  }
#line 1065 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 55:
#line 349 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                              {snprintf((yylhs.value.expression),BUFF_SIZE,"min(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 1071 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 56:
#line 350 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                              {snprintf((yylhs.value.expression),BUFF_SIZE,"max(%s , %s)", (yystack_[3].value.expression), (yystack_[1].value.expression));  }
#line 1077 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 57:
#line 352 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                          {if(P.is_domain_impl_set){
                                snprintf((yylhs.value.expression),BUFF_SIZE,"%s", (yystack_[1].value.name)->c_str()); }
                     else snprintf((yylhs.value.expression),BUFF_SIZE,"[%s]", (yystack_[1].value.name)->c_str()); }
#line 1085 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 58:
#line 355 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                           {if(P.is_domain_impl_set){
    snprintf((yylhs.value.expression),BUFF_SIZE,"%s,%s", (yystack_[2].value.name)->c_str(), (yystack_[0].value.expression)); }
else
    snprintf((yylhs.value.expression),BUFF_SIZE,"[%s]%s", (yystack_[2].value.name)->c_str(), (yystack_[0].value.expression)); }
#line 1094 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 61:
#line 362 "src/ModelGenerator/LhaParser/Lha-parser.yy"
             {Reader.MyLha.NbVar=0;}
#line 1100 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 62:
#line 363 "src/ModelGenerator/LhaParser/Lha-parser.yy"
             {Reader.MyLha.NbLoc=0;}
#line 1106 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 63:
#line 364 "src/ModelGenerator/LhaParser/Lha-parser.yy"
  {Reader.MyLha.NbVar=0;Reader.MyLha.NbLoc=0;}
#line 1112 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 68:
#line 373 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{if(Reader.MyLha.LhaRealConstant.find(*(yystack_[3].value.name))!=Reader.MyLha.LhaRealConstant.end())
        {cout<<"Constant "<<*(yystack_[3].value.name)<<" already defined."<<endl; YYABORT;}
        else {string st=(yystack_[1].value.expression);
                if (Reader.P.constants.count(*(yystack_[3].value.name))>0)st = Reader.P.constants[*(yystack_[3].value.name)].to_string();

                LhaEvaluate.parse(st);
                Reader.MyLha.LhaIntConstant[*(yystack_[3].value.name)]=LhaEvaluate.IntResult;Reader.MyLha.LhaRealConstant[*(yystack_[3].value.name)]=LhaEvaluate.RealResult;}
}
#line 1125 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 69:
#line 382 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{if(Reader.MyLha.LhaRealConstant.find(*(yystack_[3].value.name))!=Reader.MyLha.LhaRealConstant.end())
        {cout<<"Constant "<<*(yystack_[3].value.name)<<" already defined."<<endl; YYABORT;}
        else {string st=(yystack_[1].value.expression);
                if (Reader.P.constants.count(*(yystack_[3].value.name))>0)st = Reader.P.constants[*(yystack_[3].value.name)].to_string();
                LhaEvaluate.parse(st);
                Reader.MyLha.LhaRealConstant[*(yystack_[3].value.name)]=LhaEvaluate.RealResult;}
}
#line 1137 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 70:
#line 391 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{if(Reader.MyLha.LhaRealHybrid.find(*(yystack_[1].value.name))!=Reader.MyLha.LhaRealHybrid.end())
        {cout<<"Hybrid Variable "<<*(yystack_[1].value.name)<<" defined twice in the LHA."<<endl; YYABORT;}
        else {
                Reader.MyLha.LhaIntHybrid[*(yystack_[1].value.name)]=1;
                Reader.MyLha.LhaRealHybrid[*(yystack_[1].value.name)]=1.0;
        }
}
#line 1149 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 71:
#line 399 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{if(Reader.MyLha.LhaRealHybrid.find(*(yystack_[1].value.name))!=Reader.MyLha.LhaRealHybrid.end())
    {cout<<"Hybrid Variable "<<*(yystack_[1].value.name)<<" defined twice in the LHA."<<endl; YYABORT;}
    else {
        Reader.MyLha.LhaRealHybrid[*(yystack_[1].value.name)]=1.0;
    }
}
#line 1160 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 72:
#line 408 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                     {Reader.MyLha.NbVar=(yystack_[1].value.IntVal);}
#line 1166 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 73:
#line 410 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                     {Reader.MyLha.NbLoc=(yystack_[1].value.IntVal);}
#line 1172 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 76:
#line 415 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                  {
        if(Reader.MyLha.NbVar==0)Reader.MyLha.NbVar = Reader.MyLha.Vars.label.size();
        if(Reader.MyLha.Vars.label.size()!=Reader.MyLha.NbVar){
                std::cout<<"Variable label missing or redeclared, please check your variables list"<<std::endl;
                YYABORT;
        }

        FuncFlowVector.resize(Reader.MyLha.NbVar);
        FuncUpdateVector.resize(Reader.MyLha.NbVar);
    FuncUpdateVectorIndex.resize(Reader.MyLha.NbVar);
        CoeffsVector.resize(Reader.MyLha.NbVar);

        for(const auto &it : Reader.MyLha.TransitionIndex)
                PetriTransitions.insert(it.first);
}
#line 1192 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 77:
#line 431 "src/ModelGenerator/LhaParser/Lha-parser.yy"
              {
        Reader.MyLha.Vars.label.push_back(*(yystack_[0].value.name));
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(CONTINIOUS_VARIABLE);
        Reader.MyLha.Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
    Reader.MyLha.Vars.isTraced.push_back(true);
        //Reader.MyLha.VarIndex[*$1]=Reader.MyLha.VarLabel.size()-1;
}
#line 1205 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 78:
#line 439 "src/ModelGenerator/LhaParser/Lha-parser.yy"
           {

        Reader.MyLha.Vars.label.push_back(*(yystack_[0].value.name));
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(DISCRETE_VARIABLE);
        Reader.MyLha.Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
    Reader.MyLha.Vars.isTraced.push_back(true);
    //Reader.MyLha.VarIndex[*$1]=Reader.MyLha.VarLabel.size()-1;

}
#line 1220 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 79:
#line 449 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                        {

        Reader.MyLha.Vars.label.push_back(*(yystack_[3].value.name));
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(INT_INDEXED_DISC_ARRAY);
        Reader.MyLha.Vars.colorDomain.push_back((yystack_[1].value.IntVal));
    Reader.MyLha.Vars.isTraced.push_back(true);
        //Reader.MyLha.VarIndex[*$1]=Reader.MyLha.VarLabel.size()-1;

}
#line 1235 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 80:
#line 460 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                   {
        Reader.MyLha.Vars.label.push_back(*(yystack_[0].value.name));
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(CONTINIOUS_VARIABLE);
        Reader.MyLha.Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
    Reader.MyLha.Vars.isTraced.push_back(true);
        //Reader.MyLha.VarIndex[*$3]=Reader.MyLha.VarLabel.size()-1;
}
#line 1248 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 81:
#line 468 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                        {
        Reader.MyLha.Vars.label.push_back(*(yystack_[0].value.name));
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(DISCRETE_VARIABLE);
        Reader.MyLha.Vars.colorDomain.push_back(UNCOLORED_DOMAIN);
    Reader.MyLha.Vars.isTraced.push_back(true);
        //Reader.MyLha.VarIndex[*$3]=Reader.MyLha.VarLabel.size()-1;
}
#line 1261 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 82:
#line 476 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                     {
    Reader.MyLha.Vars.label.push_back(*(yystack_[3].value.name));
        Reader.MyLha.Vars.initialValue.push_back(0.0);
        Reader.MyLha.Vars.type.push_back(INT_INDEXED_DISC_ARRAY);
        Reader.MyLha.Vars.colorDomain.push_back((yystack_[1].value.IntVal));
    Reader.MyLha.Vars.isTraced.push_back(true);

}
#line 1274 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 83:
#line 485 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                  {
        if(Reader.MyLha.NbLoc ==0)Reader.MyLha.NbLoc = Reader.MyLha.LocIndex.size();
        if(Reader.MyLha.LocIndex.size()!=Reader.MyLha.NbLoc){
        std::cout<<"Location label missing or redeclared, please check your locations list"<<std::endl;
        YYABORT;
        }
    Reader.MyLha.FuncLocProperty=vector<string>(Reader.MyLha.NbLoc,"");
    Reader.MyLha.FuncFlow=vector<vector<string> >(Reader.MyLha.NbLoc,vector<string>(Reader.MyLha.NbVar,""));
}
#line 1288 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 84:
#line 495 "src/ModelGenerator/LhaParser/Lha-parser.yy"
              {
        Reader.MyLha.LocLabel.push_back(*(yystack_[0].value.name));
        Reader.MyLha.LocIndex[*(yystack_[0].value.name)]=Reader.MyLha.LocLabel.size()-1;
}
#line 1297 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 85:
#line 499 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                   {Reader.MyLha.LocLabel.push_back(*(yystack_[0].value.name));
        Reader.MyLha.LocIndex[*(yystack_[0].value.name)]=Reader.MyLha.LocLabel.size()-1;
}
#line 1305 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 89:
#line 509 "src/ModelGenerator/LhaParser/Lha-parser.yy"
               {

        if(Reader.MyLha.LocIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.LocIndex.end())
        Reader.MyLha.InitLoc.insert(Reader.MyLha.LocIndex[*(yystack_[0].value.name)]);
        else cout<<"Unknown location:" << *(yystack_[0].value.name) <<endl;


}
#line 1318 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 90:
#line 517 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                    {if(Reader.MyLha.LocIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.LocIndex.end())
        Reader.MyLha.InitLoc.insert(Reader.MyLha.LocIndex[*(yystack_[0].value.name)]);
        else cout<<"Unknown location:"<< *(yystack_[0].value.name) << endl;
}
#line 1327 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 92:
#line 524 "src/ModelGenerator/LhaParser/Lha-parser.yy"
               {

        if(Reader.MyLha.LocIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.LocIndex.end())
        Reader.MyLha.FinalLoc.insert(Reader.MyLha.LocIndex[*(yystack_[0].value.name)]);
        else cout<<"Unknown location: "<< *(yystack_[0].value.name) <<endl;


}
#line 1340 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 93:
#line 532 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                    {if(Reader.MyLha.LocIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.LocIndex.end())
        Reader.MyLha.FinalLoc.insert(Reader.MyLha.LocIndex[*(yystack_[0].value.name)]);
        else {cout<<"Unknown location: "<< *(yystack_[0].value.name) <<endl;YYABORT;}
}
#line 1349 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 96:
#line 546 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                       {
        for(size_t l=0; l<Reader.MyLha.FuncLocProperty.size();l++)
        if(Reader.MyLha.FuncLocProperty[l]=="")
        {cout<<"Some locations are not definded. Please define all the declared locations."<<endl;
                YYABORT;}

        Reader.MyLha.Out_S_Edges.resize(Reader.MyLha.NbLoc);
        Reader.MyLha.Out_A_Edges.resize(Reader.MyLha.NbLoc);
}
#line 1363 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 99:
#line 560 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{
    auto loc = Reader.MyLha.LocIndex.find(*(yystack_[8].value.name));
        if(loc !=Reader.MyLha.LocIndex.end()){
                Reader.MyLha.FuncLocProperty[loc->second]= (yystack_[6].value.expression);
                Reader.MyLha.FuncFlow[loc->second] = FuncFlowVector;
        FuncFlowVector=vector<string>(Reader.MyLha.NbVar,"");
        }
        else {cout<<"Unknown location: "<< *(yystack_[8].value.name) <<endl;YYABORT;}

}
#line 1378 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 100:
#line 573 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{
    auto loc = Reader.MyLha.LocIndex.find(*(yystack_[4].value.name));
        if(loc != Reader.MyLha.LocIndex.end()){
                Reader.MyLha.FuncLocProperty[loc->second]= (yystack_[2].value.expression);
                Reader.MyLha.FuncFlow[loc->second] = FuncFlowVector;
        }
        else {cout<<"Unknown location: "<< *(yystack_[4].value.name) <<endl;YYABORT;}


}
#line 1393 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 103:
#line 586 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                   {
    if(Reader.MyLha.Vars.find(*(yystack_[2].value.name))!=Reader.MyLha.Vars.label.size())
        FuncFlowVector[Reader.MyLha.Vars.find(*(yystack_[2].value.name))]=(yystack_[0].value.expression);
        else{ cout<<"'"<<*(yystack_[2].value.name)<<"' is not an Lha variable"<<endl;
                YYABORT;}
}
#line 1404 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 104:
#line 593 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                          {snprintf((yylhs.value.expression),BUFF_SIZE,"true");}
#line 1410 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 105:
#line 594 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                  {snprintf((yylhs.value.expression),BUFF_SIZE,"false");}
#line 1416 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 106:
#line 595 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                  {snprintf((yylhs.value.expression),BUFF_SIZE,"%s", (yystack_[0].value.expression));}
#line 1422 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 107:
#line 596 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                     {snprintf((yylhs.value.expression),BUFF_SIZE,"%s && %s", (yystack_[2].value.expression), (yystack_[0].value.expression));}
#line 1428 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 108:
#line 597 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                     {snprintf((yylhs.value.expression),BUFF_SIZE,"%s || %s", (yystack_[2].value.expression), (yystack_[0].value.expression));}
#line 1434 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 109:
#line 598 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                     {snprintf((yylhs.value.expression),BUFF_SIZE,"( %s )", (yystack_[1].value.expression));}
#line 1440 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 110:
#line 599 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                     {snprintf((yylhs.value.expression),BUFF_SIZE,"! %s", (yystack_[0].value.expression));}
#line 1446 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 111:
#line 601 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                    {snprintf((yylhs.value.expression),BUFF_SIZE,"%s == %s", (yystack_[2].value.expression), (yystack_[0].value.expression));}
#line 1452 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 112:
#line 602 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                           {snprintf((yylhs.value.expression),BUFF_SIZE,"%s <= %s", (yystack_[2].value.expression), (yystack_[0].value.expression));}
#line 1458 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 113:
#line 603 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                           {snprintf((yylhs.value.expression),BUFF_SIZE,"%s >= %s", (yystack_[2].value.expression), (yystack_[0].value.expression));}
#line 1464 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 114:
#line 604 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                           {snprintf((yylhs.value.expression),BUFF_SIZE,"%s < %s", (yystack_[2].value.expression), (yystack_[0].value.expression));}
#line 1470 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 115:
#line 605 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                           {snprintf((yylhs.value.expression),BUFF_SIZE,"%s > %s", (yystack_[2].value.expression), (yystack_[0].value.expression));}
#line 1476 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 116:
#line 610 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                          {

}
#line 1484 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 119:
#line 617 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                                                       {

        if(Reader.MyLha.LocIndex.find(*(yystack_[11].value.name))!=Reader.MyLha.LocIndex.end())
        if(Reader.MyLha.LocIndex.find(*(yystack_[9].value.name))!=Reader.MyLha.LocIndex.end())
        { int ne=Reader.MyLha.Edge.size();
                Reader.MyLha.AnEdge.Index=ne;
                Reader.MyLha.AnEdge.Source=Reader.MyLha.LocIndex[*(yystack_[11].value.name)];
                Reader.MyLha.AnEdge.Target=Reader.MyLha.LocIndex[*(yystack_[9].value.name)];
                Reader.MyLha.Edge.push_back(Reader.MyLha.AnEdge);
                Reader.MyLha.EdgeActions.push_back(SubSet);
                if(SubSet.size()>0) Reader.MyLha.Out_S_Edges[Reader.MyLha.AnEdge.Source].insert(ne);
                else Reader.MyLha.Out_A_Edges[Reader.MyLha.AnEdge.Source].insert(ne);
                SubSet.erase(SubSet.begin(),SubSet.end());
                Reader.MyLha.ConstraintsCoeffs.push_back(CoeffsMatrix);
        Reader.MyLha.ConstraintsConstants.push_back(CST);
                Reader.MyLha.ConstraintsRelOp.push_back(comp);
                Reader.MyLha.unTimeEdgeConstraints.push_back("true");
                vector<string> vs;comp=vs;CST=vs;
                vector <vector <string> > ms;CoeffsMatrix=ms;

        }
        else {cout<<*(yystack_[9].value.name)<<" is not a location label"<<endl;YYABORT;}
        else  {cout<<*(yystack_[11].value.name)<<" is not a location label"<<endl;YYABORT;}
}
#line 1513 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 121:
#line 643 "src/ModelGenerator/LhaParser/Lha-parser.yy"
      {SubSet=PetriTransitions;}
#line 1519 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 122:
#line 644 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                             {set<string> temp=PetriTransitions;
        for(set<string>::iterator it=SubSet.begin();it!=SubSet.end();it++)
        temp.erase((*it));
        SubSet=temp;}
#line 1528 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 125:
#line 652 "src/ModelGenerator/LhaParser/Lha-parser.yy"
             {if(Reader.MyLha.TransitionIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.TransitionIndex.end())
        SubSet.insert(*(yystack_[0].value.name));
        else {cout<<*(yystack_[0].value.name)<<" is not a Petri-net transition "<<endl;YYABORT;}
}
#line 1537 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 126:
#line 656 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                   {if(Reader.MyLha.TransitionIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.TransitionIndex.end())
        SubSet.insert(*(yystack_[0].value.name));
        else {cout<<*(yystack_[0].value.name)<<" is not a Petri-net transition "<<endl;YYABORT;}}
#line 1545 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 127:
#line 661 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                        {}
#line 1551 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 128:
#line 662 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                            {}
#line 1557 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 130:
#line 666 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{CoeffsMatrix.push_back(CoeffsVector);
        comp.push_back("==");
        CST.push_back((yystack_[0].value.expression));
        vector<string> vs(Reader.MyLha.NbVar,"");CoeffsVector=vs;}
#line 1566 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 131:
#line 671 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{CoeffsMatrix.push_back(CoeffsVector);
        comp.push_back("<=");
        CST.push_back((yystack_[0].value.expression));
        vector<string> vs(Reader.MyLha.NbVar,"");CoeffsVector=vs;}
#line 1575 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 132:
#line 676 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{CoeffsMatrix.push_back(CoeffsVector);
        comp.push_back(">=");
        CST.push_back((yystack_[0].value.expression));
        vector<string> vs(Reader.MyLha.NbVar,"");CoeffsVector=vs;}
#line 1584 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 136:
#line 686 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]="1";}
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 1592 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 137:
#line 690 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<"("<<(yystack_[3].value.expression)<<")";
                CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();}
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 1601 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 138:
#line 695 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<(yystack_[2].value.IntVal);
                CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();}
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 1610 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 139:
#line 700 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<(yystack_[2].value.RealVal);
                CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();}
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 1619 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 140:
#line 705 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {if(Reader.MyLha.LhaRealConstant.find(*(yystack_[2].value.name))!=Reader.MyLha.LhaRealConstant.end())
                {std::ostringstream s; s<<Reader.MyLha.LhaRealConstant[*(yystack_[2].value.name)];
                        CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();
                }
                else
                {
                        if(Reader.MyLha.PlaceIndex.find(*(yystack_[2].value.name))!=Reader.MyLha.PlaceIndex.end())
                        {std::ostringstream s; s<<"Marking.P->_PL_"<<(yystack_[2].value.name)->c_str()<<" ";
                                CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();
                        }
                        else
                        {
                                cout<<*(yystack_[2].value.name)<<" is not Petri-net Place or a defined constant "<<endl;
                                YYABORT;
                        }
                }
        }
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}
}
#line 1644 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 141:
#line 726 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]="-1";}
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 1652 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 142:
#line 730 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<"-("<<(yystack_[3].value.expression)<<")";
                CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();}
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 1661 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 143:
#line 735 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<"-"<<(yystack_[2].value.IntVal);
                CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();}
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 1670 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 144:
#line 740 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {std::ostringstream s; s<<"-"<<(yystack_[2].value.RealVal);
                CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();}
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 1679 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 145:
#line 745 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {if(Reader.MyLha.LhaRealConstant.find(*(yystack_[2].value.name))!=Reader.MyLha.LhaRealConstant.end())
                {std::ostringstream s; s<<"-"<<Reader.MyLha.LhaRealConstant[*(yystack_[2].value.name)];
                        CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();
                }
                else
                {
                        if(Reader.MyLha.PlaceIndex.find(*(yystack_[2].value.name))!=Reader.MyLha.PlaceIndex.end())
                        {std::ostringstream s; s<<"-Marking.P->_PL_"<<(yystack_[2].value.name)->c_str()<<" ";
                                CoeffsVector[Reader.MyLha.Vars.find(*(yystack_[0].value.name))]=s.str();
                        }
                        else
                        {
                                cout<<*(yystack_[2].value.name)<<" is not Petri-net Place or a defined constant "<<endl;
                                YYABORT;
                        }
                }
        }
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}
}
#line 1704 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 146:
#line 767 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                         {
    Reader.MyLha.FuncEdgeUpdates.push_back(FuncUpdateVector);
    Reader.MyLha.FuncEdgeUpdatesIndex.push_back(FuncUpdateVectorIndex);
    FuncUpdateVector=vector<string>(Reader.MyLha.NbVar,"");
    FuncUpdateVectorIndex=vector<string>(Reader.MyLha.NbVar,"");
}
#line 1715 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 147:
#line 773 "src/ModelGenerator/LhaParser/Lha-parser.yy"
        {
    Reader.MyLha.FuncEdgeUpdates.push_back(FuncUpdateVector);
    Reader.MyLha.FuncEdgeUpdatesIndex.push_back(FuncUpdateVectorIndex);
}
#line 1724 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 150:
#line 781 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                     {
        if(Reader.MyLha.Vars.find(*(yystack_[2].value.name))!=Reader.MyLha.Vars.label.size())
    {FuncUpdateVector[Reader.MyLha.Vars.find(*(yystack_[2].value.name))]= (yystack_[0].value.expression);}
        else{cout<<*(yystack_[2].value.name)<<" is not  variable label"<<endl;YYABORT;}
}
#line 1734 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 151:
#line 786 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                         {
    size_t varin = Reader.MyLha.Vars.find(*(yystack_[5].value.name));
        if(varin != Reader.MyLha.Vars.label.size()){
        FuncUpdateVector[varin]= (yystack_[0].value.expression);
        FuncUpdateVectorIndex[varin]= (yystack_[3].value.expression);
        //cout << "var:" << *$1 << "\tindex:" << $3 << "\tupdate:" << $6<< endl;
    }else{cout<<*(yystack_[5].value.name)<<" is not  variable label"<<endl;YYABORT;}
}
#line 1747 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 154:
#line 800 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                     {
        if((yystack_[1].value.TOPHASL) != NULL){
                Reader.MyLha.HASLname.push_back(*(yystack_[3].value.name));
                Reader.MyLha.HASLtop.push_back((yystack_[1].value.TOPHASL));
        }else{
                for(vector<string>::iterator it = Reader.MyLha.HASLname.begin(); it <Reader.MyLha.HASLname.end() ; it++){
                        if( it->find("$_$") == 0)
                                it->replace(0,3,*(yystack_[3].value.name));
                }
        }
}
#line 1763 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 155:
#line 811 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                       {
        if((yystack_[1].value.TOPHASL) != NULL){
                Reader.MyLha.HASLname.push_back("");
                Reader.MyLha.HASLtop.push_back((yystack_[1].value.TOPHASL));
        }else{
                for(vector<string>::iterator it = Reader.MyLha.HASLname.begin(); it <Reader.MyLha.HASLname.end() ; it++){
                        if( it->find("$_$") == 0)
                                it->replace(0,3,"");
                }
        }
}
#line 1779 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 156:
#line 824 "src/ModelGenerator/LhaParser/Lha-parser.yy"
     {(yylhs.value.RealVal)=(yystack_[0].value.RealVal);}
#line 1785 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 157:
#line 825 "src/ModelGenerator/LhaParser/Lha-parser.yy"
       {(yylhs.value.RealVal)=(double)(yystack_[0].value.IntVal);}
#line 1791 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 158:
#line 826 "src/ModelGenerator/LhaParser/Lha-parser.yy"
      { if(Reader.MyLha.LhaRealConstant.find(*(yystack_[0].value.name))!=Reader.MyLha.LhaRealConstant.end())
                        (yylhs.value.RealVal) = Reader.MyLha.LhaRealConstant[*(yystack_[0].value.name)];
                else { if(Reader.MyLha.LhaIntConstant.find(*(yystack_[0].value.name))!=Reader.MyLha.LhaIntConstant.end())
                        (yylhs.value.RealVal) = (double)Reader.MyLha.LhaIntConstant[*(yystack_[0].value.name)];
                else {
                        cout<<*(yystack_[0].value.name)<<" is not a definded constant "<<endl;
                        YYABORT;
                }}}
#line 1804 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 159:
#line 836 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                  {
        Reader.MyLha.Algebraic.push_back((yystack_[1].value.expression));
        (yylhs.value.TOPHASL) = new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-1);
}
#line 1813 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 160:
#line 840 "src/ModelGenerator/LhaParser/Lha-parser.yy"
       {
        (yylhs.value.TOPHASL) = new HaslFormulasTop(PROBABILITY);
}
#line 1821 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 161:
#line 843 "src/ModelGenerator/LhaParser/Lha-parser.yy"
             {
    (yylhs.value.TOPHASL) = new HaslFormulasTop(PROBABILITY);
}
#line 1829 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 162:
#line 846 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                 {
    Reader.MyLha.FinalStateCond.push_back(*(yystack_[1].value.name));
    (yylhs.value.TOPHASL) = new HaslFormulasTop(PROBCOND,(size_t)Reader.MyLha.FinalStateCond.size()-1);
}
#line 1838 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 163:
#line 850 "src/ModelGenerator/LhaParser/Lha-parser.yy"
            {
    (yylhs.value.TOPHASL) = new HaslFormulasTop(EXISTS);
}
#line 1846 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 164:
#line 853 "src/ModelGenerator/LhaParser/Lha-parser.yy"
             {
    (yylhs.value.TOPHASL) = new HaslFormulasTop(NOTALLS);
}
#line 1854 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 165:
#line 856 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                             {
        (yylhs.value.TOPHASL) = new HaslFormulasTop((yystack_[3].value.RealVal),(yystack_[1].value.RealVal));
}
#line 1862 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 166:
#line 859 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                              {

        for(double bucket = (yystack_[3].value.RealVal) ; bucket < (yystack_[1].value.RealVal) ; bucket+= (yystack_[5].value.RealVal)){
                std::ostringstream algPDF;
                algPDF << "(("<<(yystack_[7].value.expression)<<" >= "<<bucket<<"&& "<<(yystack_[7].value.expression)<<"<"<<bucket+(yystack_[5].value.RealVal)<<") ? 1:0)";

                Reader.MyLha.Algebraic.push_back(algPDF.str());
                Reader.MyLha.HASLtop.push_back(
                        new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-1));
                Reader.MyLha.HASLtop.back()->TypeOp = PDF_PART;
                std::ostringstream s; s<<"$_$: Value in ["<< bucket<< " , "<<bucket+(yystack_[5].value.RealVal)<<"]";
                Reader.MyLha.HASLname.push_back(s.str());
        }
        (yylhs.value.TOPHASL) = NULL;
}
#line 1882 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 167:
#line 874 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                                              {

        for(double bucket = (yystack_[3].value.RealVal) ; bucket < (yystack_[1].value.RealVal) ; bucket+= (yystack_[5].value.RealVal)){
                std::ostringstream algCDF;
                algCDF << "(("<<(yystack_[7].value.expression)<<" <= "<<bucket<<") ? 1:0)";

                Reader.MyLha.Algebraic.push_back(algCDF.str());
                Reader.MyLha.HASLtop.push_back(
                new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-1));
                Reader.MyLha.HASLtop.back()->TypeOp = CDF_PART;
                std::ostringstream s; s<<"$_$: Value in [-infinity , "<< bucket<<"]";
                Reader.MyLha.HASLname.push_back(s.str());
        }
        (yylhs.value.TOPHASL) = NULL;
}
#line 1902 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 168:
#line 889 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                    {
        Reader.MyLha.Algebraic.push_back((yystack_[1].value.expression));
        char tmp[5000];
        snprintf(tmp,BUFF_SIZE,"(%s * %s)", (yystack_[1].value.expression),(yystack_[1].value.expression));
        Reader.MyLha.Algebraic.push_back(tmp);
        (yylhs.value.TOPHASL) = new HaslFormulasTop(HASL_MINUS,
                new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-1),
                new HaslFormulasTop(HASL_TIME,
                        new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-2),
                        new HaslFormulasTop((size_t)Reader.MyLha.Algebraic.size()-2)));
}
#line 1918 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 169:
#line 900 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                   {
        (yylhs.value.TOPHASL) = (yystack_[1].value.TOPHASL);
}
#line 1926 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 170:
#line 903 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                             {
        (yylhs.value.TOPHASL) = new HaslFormulasTop(HASL_PLUS, (yystack_[2].value.TOPHASL),(yystack_[0].value.TOPHASL));
}
#line 1934 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 171:
#line 906 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                              {
        (yylhs.value.TOPHASL) = new HaslFormulasTop(HASL_MINUS, (yystack_[2].value.TOPHASL),(yystack_[0].value.TOPHASL));
}
#line 1942 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 172:
#line 909 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                            {
        (yylhs.value.TOPHASL) = new HaslFormulasTop(HASL_TIME, (yystack_[2].value.TOPHASL), (yystack_[0].value.TOPHASL));
}
#line 1950 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 173:
#line 912 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                            {
        (yylhs.value.TOPHASL) = new HaslFormulasTop(HASL_DIV, (yystack_[2].value.TOPHASL),(yystack_[0].value.TOPHASL));
}
#line 1958 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 174:
#line 915 "src/ModelGenerator/LhaParser/Lha-parser.yy"
       {
        (yylhs.value.TOPHASL) = new HaslFormulasTop((double)(yystack_[0].value.IntVal),0.0,1.0);
}
#line 1966 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 175:
#line 918 "src/ModelGenerator/LhaParser/Lha-parser.yy"
       {
        (yylhs.value.TOPHASL) = new HaslFormulasTop((double)(yystack_[0].value.RealVal),0.0,1.0);
}
#line 1974 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 176:
#line 922 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                {string ss=(yystack_[0].value.expression);
        snprintf((yylhs.value.expression),BUFF_SIZE,"LhaFunc[%d]", Reader.MyLha.LhaFunction[ss]);
}
#line 1982 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 177:
#line 925 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                  {snprintf((yylhs.value.expression),BUFF_SIZE,"min(%s,%s)", (yystack_[3].value.expression),(yystack_[1].value.expression));}
#line 1988 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 178:
#line 926 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                  {snprintf((yylhs.value.expression),BUFF_SIZE,"max(%s,%s)", (yystack_[3].value.expression),(yystack_[1].value.expression));}
#line 1994 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 179:
#line 927 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                         {snprintf((yylhs.value.expression),BUFF_SIZE,"-%s", (yystack_[0].value.expression));}
#line 2000 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 180:
#line 928 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                     {snprintf((yylhs.value.expression),BUFF_SIZE,"floor(%s)", (yystack_[1].value.expression));}
#line 2006 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 181:
#line 929 "src/ModelGenerator/LhaParser/Lha-parser.yy"
               {snprintf((yylhs.value.expression),BUFF_SIZE,"(%s)", (yystack_[1].value.expression));}
#line 2012 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 182:
#line 930 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                       {snprintf((yylhs.value.expression),BUFF_SIZE,"pow(%s , %s)", (yystack_[2].value.expression),(yystack_[0].value.expression));}
#line 2018 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 183:
#line 931 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                      {snprintf((yylhs.value.expression),BUFF_SIZE,"(%s + %s)", (yystack_[2].value.expression),(yystack_[0].value.expression));}
#line 2024 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 184:
#line 932 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                       {snprintf((yylhs.value.expression),BUFF_SIZE,"(%s - %s)", (yystack_[2].value.expression),(yystack_[0].value.expression));}
#line 2030 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 185:
#line 933 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                     {snprintf((yylhs.value.expression),BUFF_SIZE,"(%s * %s)", (yystack_[2].value.expression),(yystack_[0].value.expression));}
#line 2036 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 186:
#line 934 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                     {snprintf((yylhs.value.expression),BUFF_SIZE,"(%s / %s)", (yystack_[2].value.expression),(yystack_[0].value.expression));}
#line 2042 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 187:
#line 936 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                 {std::ostringstream s; s<<(yystack_[1].value.expression);
        if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
        {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
        Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
        Reader.MyLha.LhaFuncType.push_back("Last");
        string ss="Last("; ss.append(s.str()); ss.append(")");
        if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
        {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
        snprintf((yylhs.value.expression),BUFF_SIZE,"%s", ss.c_str());
}
#line 2057 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 188:
#line 946 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                        {std::ostringstream s; s<<(yystack_[1].value.expression);
        if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
        {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
        Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
        Reader.MyLha.LhaFuncType.push_back("Min");
        string ss="Min("; ss.append(s.str()); ss.append(")");
        if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
        {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
        snprintf((yylhs.value.expression),BUFF_SIZE,"%s", ss.c_str());
}
#line 2072 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 189:
#line 956 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                       {std::ostringstream s; s<<(yystack_[1].value.expression);
        if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
        {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
        Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
        Reader.MyLha.LhaFuncType.push_back("Max");
        string ss="Max("; ss.append(s.str()); ss.append(")");
        if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
        {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
        snprintf((yylhs.value.expression),BUFF_SIZE,"%s", ss.c_str());
}
#line 2087 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 190:
#line 966 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                       {std::ostringstream s; s<<(yystack_[1].value.expression);
        if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
        {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
        Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
        Reader.MyLha.LhaFuncType.push_back("Integral");
        string ss="Integral("; ss.append(s.str()); ss.append(")");
        if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
        {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
        snprintf((yylhs.value.expression),BUFF_SIZE,"%s", ss.c_str());
}
#line 2102 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 191:
#line 976 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                    {std::ostringstream s; s<<(yystack_[1].value.expression);
                if(Reader.MyLha.LinearForm.find(s.str())==Reader.MyLha.LinearForm.end())
                {int i=Reader.MyLha.LinearForm.size();Reader.MyLha.LinearForm[s.str()]=i;}
                Reader.MyLha.LhaFuncArg.push_back(Reader.MyLha.LinearForm[s.str()]);
                Reader.MyLha.LhaFuncType.push_back("Mean");
                string ss="Mean("; ss.append(s.str()); ss.append(")");
                if(Reader.MyLha.LhaFunction.find(ss)==Reader.MyLha.LhaFunction.end())
                {int i=Reader.MyLha.LhaFunction.size();Reader.MyLha.LhaFunction[ss]=i;}
                snprintf((yylhs.value.expression),BUFF_SIZE,"%s", ss.c_str());
}
#line 2117 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 192:
#line 987 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                 {snprintf((yylhs.value.expression),BUFF_SIZE,"%s", (yystack_[0].value.expression));  }
#line 2123 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 193:
#line 988 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                       {snprintf((yylhs.value.expression),BUFF_SIZE,"%s - %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 2129 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 194:
#line 989 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                       {snprintf((yylhs.value.expression),BUFF_SIZE,"%s + %s", (yystack_[2].value.expression), (yystack_[0].value.expression));  }
#line 2135 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 195:
#line 993 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {snprintf((yylhs.value.expression),BUFF_SIZE,"Vars->%s", (yystack_[0].value.name)->c_str());}
    else if(Reader.MyLha.LhaRealHybrid.find(*(yystack_[0].value.name))!=Reader.MyLha.LhaRealHybrid.end())
    {snprintf((yylhs.value.expression),BUFF_SIZE," hybridVar::%s",(yystack_[0].value.name)->c_str());}
        else if(Reader.MyLha.PlaceIndex.find(*(yystack_[0].value.name))!=Reader.MyLha.PlaceIndex.end())
        {std::ostringstream s; s<<" Marking.P->_PL_"<< (yystack_[0].value.name)->c_str();
                        if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*(yystack_[0].value.name)]].colorDom !=0 )s <<".card()";
                snprintf((yylhs.value.expression),BUFF_SIZE, "%s ",(s.str()).c_str());
        } else {cout<<*(yystack_[0].value.name)<<" is not a Hybrid variable, a Lha variable or a place name"<<endl;YYABORT;}
}
#line 2150 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 196:
#line 1003 "src/ModelGenerator/LhaParser/Lha-parser.yy"
            {
    if(Reader.MyLha.PlaceIndex.find(*(yystack_[1].value.name))!=Reader.MyLha.PlaceIndex.end())
    {std::ostringstream s; s<<" Marking.P->_PL_"<<(yystack_[1].value.name)->c_str();
        if(Reader.MyLha.MyGspn->placeStruct[Reader.MyLha.PlaceIndex[*(yystack_[1].value.name)]].colorDom !=0 )s <<".value()";
        snprintf((yylhs.value.expression),BUFF_SIZE, "%s ",(s.str()).c_str());
    }
    else {cout<<"'"<<*(yystack_[1].value.name)<<"' is not a place label"<<endl;YYABORT;}}
#line 2162 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 197:
#line 1010 "src/ModelGenerator/LhaParser/Lha-parser.yy"
                                {
    if(Reader.MyLha.Vars.find(*(yystack_[3].value.name))!=Reader.MyLha.Vars.label.size()){
        snprintf((yylhs.value.expression),BUFF_SIZE,"Vars->%s[%s]", (yystack_[3].value.name)->c_str(), (yystack_[1].value.expression));
        } else {cout<<*(yystack_[3].value.name)<<" is not a Lha array variable "<<endl;YYABORT;}
}
#line 2172 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 198:
#line 1018 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {snprintf((yylhs.value.expression),BUFF_SIZE,"(%s) * Vars->%s", (yystack_[3].value.expression), (yystack_[0].value.name)->c_str());
        }
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 2181 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 199:
#line 1023 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {snprintf((yylhs.value.expression),BUFF_SIZE,"%d * Vars->%s", (yystack_[2].value.IntVal), (yystack_[0].value.name)->c_str());
        }
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 2190 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 200:
#line 1028 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {snprintf((yylhs.value.expression),BUFF_SIZE,"%f * Vars->%s", (yystack_[2].value.RealVal), (yystack_[0].value.name)->c_str());
        }
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 2199 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 201:
#line 1034 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ snprintf((yylhs.value.expression),BUFF_SIZE,"%i ", (yystack_[0].value.IntVal));
}
#line 2206 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 202:
#line 1037 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ snprintf((yylhs.value.expression),BUFF_SIZE,"%f ", (yystack_[0].value.RealVal));}
#line 2212 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 203:
#line 1040 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {if(Reader.MyLha.LhaRealConstant.find(*(yystack_[2].value.name))!=Reader.MyLha.LhaRealConstant.end())
                {snprintf((yylhs.value.expression),BUFF_SIZE,"%f * Vars->%s", Reader.MyLha.LhaRealConstant[*(yystack_[2].value.name)],(yystack_[0].value.name)->c_str());
                }
                else
                {
                        if(Reader.MyLha.PlaceIndex.find(*(yystack_[2].value.name))!=Reader.MyLha.PlaceIndex.end())
                        {snprintf((yylhs.value.expression),BUFF_SIZE,"Marking.P->_PL_%s * Vars->%s", (yystack_[2].value.name)->c_str() ,(yystack_[0].value.name)->c_str());
                        }
                        else
                        {
                                cout<<*(yystack_[2].value.name)<<" is not Petri-net Place or a definded constant "<<endl;
                                YYABORT;
                        }
                }
        }
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}
}
#line 2235 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 204:
#line 1059 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {snprintf((yylhs.value.expression),BUFF_SIZE,"-Vars->%s", (yystack_[0].value.name)->c_str());}
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 2243 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 205:
#line 1063 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {snprintf((yylhs.value.expression),BUFF_SIZE,"-(%s) * Vars->%s", (yystack_[3].value.expression), (yystack_[0].value.name)->c_str());
        }
        else {cout<<*(yystack_[1].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 2252 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 206:
#line 1068 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {snprintf((yylhs.value.expression),BUFF_SIZE,"-%d * Vars->%s", (yystack_[2].value.IntVal), (yystack_[0].value.name)->c_str());
        }
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 2261 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 207:
#line 1073 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {snprintf((yylhs.value.expression),BUFF_SIZE,"-%f * Vars->%s", (yystack_[2].value.RealVal), (yystack_[0].value.name)->c_str());
        }
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}}
#line 2270 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;

  case 208:
#line 1078 "src/ModelGenerator/LhaParser/Lha-parser.yy"
{ if(Reader.MyLha.Vars.find(*(yystack_[0].value.name))!=Reader.MyLha.Vars.label.size())
        {if(Reader.MyLha.LhaRealConstant.find(*(yystack_[2].value.name))!=Reader.MyLha.LhaRealConstant.end())
                {snprintf((yylhs.value.expression),BUFF_SIZE,"-%f * Vars->%s", Reader.MyLha.LhaRealConstant[*(yystack_[2].value.name)],(yystack_[0].value.name)->c_str());
                }
                else
                {
                        if(Reader.MyLha.PlaceIndex.find(*(yystack_[2].value.name))!=Reader.MyLha.PlaceIndex.end())
                        {snprintf((yylhs.value.expression),BUFF_SIZE,"-Marking.P->_PL_%s * Vars->%s", (yystack_[2].value.name)->c_str() ,(yystack_[0].value.name)->c_str());
                        }
                        else
                        {
                                cout<<*(yystack_[2].value.name)<<" is not Petri-net Place or a definded constant "<<endl;
                                YYABORT;
                        }
                }
        }
        else {cout<<*(yystack_[0].value.name)<<" is not a Lha variable"<<endl;YYABORT;}
}
#line 2293 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"
    break;


#line 2297 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"

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
  Lha_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  Lha_parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
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


  const short Lha_parser::yypact_ninf_ = -342;

  const signed char Lha_parser::yytable_ninf_ = -7;

  const short
  Lha_parser::yypact_[] =
  {
      14,   -12,  -342,  -342,   117,    37,    43,    71,    80,   354,
      96,   101,   137,   166,  -342,  -342,   123,   -34,   470,   208,
     144,  -342,    74,   297,   117,   586,   213,   213,    10,   248,
     -34,   229,   267,   213,   213,  -342,   246,   280,   145,   233,
     269,   272,   295,   348,   358,   508,   238,   301,   349,    74,
     362,   379,  -342,  -342,  -342,  -342,   117,   117,   117,   117,
     323,  -342,   213,   213,   435,   437,   446,   474,   476,   479,
     502,   509,   200,  -342,   398,   397,  -342,   434,   145,   441,
     480,   302,   312,   417,   457,   510,   518,  -342,   499,   468,
    -342,  -342,   556,   560,   562,   564,   524,   525,   238,    74,
      74,  -342,  -342,  -342,  -342,  -342,  -342,  -342,   334,   334,
    -342,   421,   161,   213,   213,   213,   157,   157,   157,   157,
     157,  -342,   213,   213,   213,   213,   213,  -342,  -342,   596,
    -342,  -342,  -342,   265,   265,   587,   594,   539,   565,  -342,
    -342,   588,   611,   622,   636,    13,   638,    74,  -342,  -342,
    -342,   428,   328,   338,    77,   537,   629,   304,   436,    20,
    -342,   114,   143,   153,   257,   627,   627,   161,   161,  -342,
     651,  -342,  -342,  -342,   654,   664,  -342,    -5,  -342,    -2,
     665,   666,   304,   230,  -342,  -342,  -342,   670,     7,  -342,
      16,  -342,  -342,   213,   213,   230,  -342,   673,   674,   675,
      22,  -342,  -342,   304,   304,   671,   672,   676,   433,   650,
     653,   655,   304,  -342,   157,   157,  -342,  -342,  -342,  -342,
    -342,   265,   265,   680,   681,   683,   682,   686,    -1,  -342,
     684,   625,   666,   249,   685,  -342,   230,   230,   688,   689,
     690,   691,   119,   692,    25,   695,   699,   698,   438,   444,
     589,  -342,  -342,  -342,   702,  -342,   461,   570,   304,   304,
     304,   662,   304,   304,   304,   304,   304,   703,   704,   705,
     467,  -342,  -342,   687,   701,  -342,  -342,  -342,  -342,   706,
     707,  -342,   708,   709,  -342,  -342,   240,   550,    11,   230,
     230,   230,   230,  -342,   230,   230,   230,   230,   711,  -342,
     714,  -342,  -342,  -342,  -342,  -342,  -342,   710,   700,  -342,
     472,   347,   353,   716,   660,   660,   570,   570,  -342,  -342,
    -342,  -342,   693,   265,   265,   281,  -342,   712,  -342,   156,
     593,   713,  -342,   477,   357,   382,   388,   678,    11,    11,
    -342,   715,   717,   702,  -342,  -342,   304,   304,  -342,   718,
     719,   720,   281,   281,  -342,  -342,   635,   232,  -342,   721,
    -342,  -342,  -342,   230,   230,   230,   230,  -342,   726,  -342,
     484,   500,  -342,  -342,  -342,   405,   273,  -342,   304,   304,
     304,   304,   304,   723,   727,   281,   281,   724,   554,   563,
     577,   581,   725,  -342,  -342,  -342,   603,   603,   603,   603,
     603,   732,  -342,  -342,   722,   728,  -342,  -342,  -342,  -342,
    -342,   731,   201,  -342,    23,   304,   732,   729,  -342,   679,
     737,   733,  -342,   603,  -342,   736,   659,  -342,    17,   134,
    -342,  -342,   741,  -342,   697,   730,   734,   304,  -342,   475,
     738,   735,   124,  -342,  -342,   742,   744,   745,   507,   739,
     740,   743,   304,    34,   134,   304,   304,   304,   177,   177,
    -342,  -342,  -342,   746,   747,   748,   749,   512,  -342,   750,
     751,  -342,   603,   603,   603,  -342,  -342,   752,  -342,  -342,
    -342,   753,    15,    21,  -342,   754,  -342,   755,   230,   314,
     750,  -342,  -342,  -342,   614,   174,  -342,  -342,   314,   314,
     756,   757,   758,   608,  -342,   760,   240,  -342,   517,   628,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   618,
     759,  -342,   523,   363,   372,   761,   761,   628,   628,  -342,
     608,  -342,  -342,  -342,   314,   314,   540,   546,  -342,  -342
  };

  const unsigned char
  Lha_parser::yydefact_[] =
  {
      63,     0,   175,   174,     0,     0,     0,   160,     0,    63,
       0,     0,     0,     0,   163,   164,     0,     0,     0,    62,
      61,     5,   152,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,    66,     0,
       0,     0,    60,    59,   153,   155,     0,     0,     0,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   176,     0,     0,   161,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     0,     0,
      86,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    67,    10,    74,    75,   173,   172,   170,   171,
     154,     0,   179,     0,     0,     0,     0,     0,     0,     0,
       0,   159,     0,     0,     0,     0,     0,   168,   162,     0,
       3,    73,    72,     0,     0,     0,     0,     0,     0,    94,
      95,     0,     0,     0,     0,     0,     0,     0,     9,     8,
     181,     0,     0,     0,   195,   202,   201,     0,     0,     0,
     192,     0,     0,     0,     0,   186,   185,   183,   184,   182,
       0,   158,   156,   157,     0,     0,    89,     0,    92,     0,
       0,     0,     0,     0,    71,    70,    77,     0,     0,    84,
       0,     7,   180,     0,     0,     0,   196,     0,     0,     0,
      29,    26,    27,     0,     0,     0,     0,     0,     0,   204,
       0,     0,     0,   188,     0,     0,   189,   187,   190,   191,
     165,     0,     0,     0,     0,     0,     0,     0,     0,    97,
       0,     0,   117,     0,    12,    11,     0,     0,     0,     0,
       0,     0,     0,    78,     0,     0,     0,     0,     0,     0,
       0,   203,   200,   199,     0,    28,     0,    32,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   194,   193,     0,     0,    90,    88,    93,    91,     0,
       0,    98,     0,     0,   118,    69,     0,     0,    16,     0,
       0,     0,     0,    68,     0,     0,     0,     0,     0,    80,
       0,    76,    85,    83,   177,   178,   197,     0,     0,    31,
       0,     0,     0,     0,    33,    36,    34,    35,    37,   208,
     207,   206,     0,     0,     0,     0,    96,     0,   116,    12,
       0,     0,    15,     0,     0,     0,     0,    19,    17,    18,
      20,     0,    81,    57,    30,    38,     0,     0,   198,     0,
       0,     0,     0,     0,   104,   105,     0,     0,   106,     0,
      14,    13,    21,     0,     0,     0,     0,    79,     0,    58,
       0,     0,   205,   166,   167,     0,     0,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    39,    40,   109,   111,   113,   112,   114,
     115,     0,   100,   107,   108,     0,    22,    23,    24,    25,
      82,     0,     0,   101,     0,     0,     0,     0,   120,   121,
       0,     0,   123,   103,   102,     0,     0,   125,     0,     0,
      99,   122,     0,   124,   136,     0,     0,     0,   129,     0,
       0,   127,     0,   133,   126,     0,     0,     0,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     140,   139,   138,     0,     0,     0,     0,     0,   147,     0,
       0,   128,   130,   132,   131,   135,   134,     0,   145,   144,
     143,     0,     0,     0,   148,     0,   137,     0,     0,     0,
       0,   146,   119,   142,     0,    44,    41,    42,     0,     0,
       0,     0,     0,   150,   149,     0,     0,    43,     0,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,     0,     0,     0,    49,    52,    50,    51,    53,
     151,    45,    46,    54,     0,     0,     0,     0,    55,    56
  };

  const short
  Lha_parser::yypgoto_[] =
  {
    -342,  -342,   764,  -195,  -155,  -298,  -285,  -342,   762,   193,
    -342,  -342,   763,   765,   -36,   766,  -342,   768,  -342,   767,
     770,  -342,   769,  -342,   696,   668,  -342,   521,  -342,   346,
    -341,  -342,   771,   522,  -342,  -342,   344,  -342,   310,  -342,
    -342,   -86,  -342,  -342,   287,   -15,  -342,  -129,   148,     5,
    -342,   551,   183
  };

  const short
  Lha_parser::yydefgoto_[] =
  {
      -1,    16,    17,   242,   356,   503,   308,    18,    45,    46,
      47,    48,    19,    20,    49,    50,   188,    51,   190,    38,
      39,   177,    40,   179,    87,    88,   228,   229,   412,   413,
     357,   358,    89,   231,   232,   421,   422,   428,   440,   441,
     442,   443,   470,   483,   484,    21,    22,   174,    23,    72,
      73,   159,   160
  };

  const short
  Lha_parser::yytable_[] =
  {
     250,   331,   208,   223,    24,   175,   225,    54,   227,    99,
     100,   376,   377,    75,    -2,   244,   186,     1,     2,     3,
      76,    36,    37,     4,   246,   432,   488,   233,   299,   490,
     213,   489,    74,   254,   103,   255,   418,   419,    81,    82,
     294,   287,   288,   297,   403,   404,    26,   468,   256,   257,
     214,   215,    27,     5,     6,     7,     8,   270,   369,   187,
     224,     9,   147,   226,   280,    10,    11,   111,   112,    -6,
      -6,   300,   245,    12,    13,    14,    15,     1,     2,     3,
      28,   247,   433,     4,   148,   149,   491,   420,   195,    29,
     196,   330,   273,   274,   333,   334,   335,   336,   469,   337,
     338,   339,   340,   310,   311,   312,   197,   314,   315,   316,
     317,   318,    31,     5,     6,     7,     8,    32,   151,   152,
     153,     2,     3,    35,   216,   293,     4,   165,   166,   167,
     168,   169,   191,    12,    13,    14,    15,   434,   435,   436,
     455,   456,   457,   437,   214,   215,    33,   438,   294,   295,
     296,   297,    25,   217,   458,   459,     5,     6,     7,     8,
     154,   155,   156,   218,   343,   439,   157,   286,   388,   389,
     390,   391,    60,   214,   215,    34,    12,    13,    14,    15,
     434,   435,   436,   214,   215,   506,   437,   507,   158,   122,
     123,   370,   371,   126,   350,   351,    11,   375,   248,   249,
     508,   509,    85,    86,   106,   107,   108,   109,   439,   416,
     121,   417,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   520,    62,   396,   397,   398,   399,   400,   122,   123,
     124,   125,   126,   234,    79,   235,   536,   537,    98,   236,
     383,   102,   384,   329,    63,   235,    64,    65,    66,   236,
      67,    68,    77,   385,   386,   285,    69,    70,    71,    10,
     423,   237,    83,   238,   239,   240,   241,   219,   171,   172,
     173,   237,    80,   238,   239,   240,   241,   262,   263,   264,
     265,   266,   448,   395,   200,   201,   202,   214,   215,    37,
     352,    43,    44,   494,   385,   386,    84,   467,    92,    93,
     472,   473,   474,    55,   353,   354,   355,   200,   201,   202,
     133,   519,   204,   203,   205,   206,   207,   495,   496,   497,
     134,    94,    95,   498,    36,    56,    57,    58,    59,   110,
     122,   123,   124,   125,   126,   204,   193,   205,   206,   207,
     122,   123,   124,   125,   126,   499,   194,   500,   501,   502,
      41,    56,    57,    58,    59,   346,   122,   123,   124,   125,
     126,   347,    56,    57,    96,   364,   122,   123,   124,   125,
     126,   534,   475,   476,    97,   262,   263,   264,   265,   266,
     535,   262,   263,   264,   265,   266,   294,   295,   296,   297,
     365,   513,   514,   515,   516,   517,   366,   271,   272,    42,
     513,   514,   515,   516,   517,    10,    11,   128,   127,    -6,
      -6,   294,   295,   296,   297,   309,    44,   294,   295,   296,
     297,   378,   379,   380,   381,   382,   122,   123,   124,   125,
     126,   150,    43,   262,   263,   264,   265,   266,   192,   209,
     210,   211,   129,   261,   113,   212,   114,   131,   304,   122,
     123,   124,   125,   126,   305,   115,   122,   123,   124,   125,
     126,   262,   263,   264,   265,   266,   122,   123,   124,   125,
     126,   309,   122,   123,   124,   125,   126,   322,   449,   450,
     451,   135,   345,   116,   452,   117,   132,   362,   118,   262,
     263,   264,   265,   266,   393,   262,   263,   264,   265,   266,
     262,   263,   264,   265,   266,   363,   294,   295,   296,   297,
     394,   119,   262,   263,   264,   265,   266,   463,   120,    41,
      42,   136,   481,    43,    44,    85,   137,   521,   262,   263,
     264,   265,   266,   533,   138,   262,   263,   264,   265,   266,
     262,   263,   264,   265,   266,   513,   514,   515,   516,   517,
     538,   513,   514,   515,   516,   517,   539,    86,    42,   141,
     332,    43,    44,   142,   406,   143,   198,   144,   513,   514,
     515,   516,   517,   407,   513,   514,   515,   516,   517,   294,
     295,   296,   297,   294,   295,   296,   297,   408,   145,   146,
     176,   409,   294,   295,   296,   297,    61,   178,   262,   263,
     170,   306,   266,   180,   182,   360,   294,   295,   296,   297,
     294,   295,   296,   297,    56,    57,    58,    59,   294,   295,
     296,   297,   294,   295,   296,   297,   505,   183,   184,   181,
     531,   262,   263,   264,   265,   266,   513,   514,   515,   516,
     517,   189,   185,   294,   295,   296,   297,   294,   295,   296,
     297,   378,   379,   380,   381,   382,   513,   514,   199,   126,
     517,   220,   221,   262,   263,   264,   265,   266,   161,   162,
     163,   164,   222,   243,   227,   230,   251,   252,   253,   267,
     258,   259,   268,   275,   269,   260,   277,   276,   278,   279,
     283,   313,   266,   282,   426,   323,   286,   289,   290,   291,
     292,   301,   302,   298,   303,   307,   319,   320,   321,   324,
     297,   327,   344,   326,   325,   328,   341,   342,   343,   348,
     359,   372,   349,   420,   387,   361,   445,   367,   368,   373,
     374,   392,   401,   402,   405,   411,   414,   410,   415,   425,
     427,   429,   430,   385,   444,   460,   453,   461,   462,   281,
     478,   479,   480,   482,   284,   486,   454,   140,   493,   446,
     492,   485,   424,   447,   471,   510,   511,   512,   464,   465,
     431,   532,   466,    30,   130,   477,   518,   504,     0,     0,
       0,     0,   487,    53,    52,     0,     0,     0,     0,     0,
       0,     0,     0,   517,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    90,   101,
      91,     0,     0,     0,     0,     0,     0,   105,   104,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139
  };

  const short
  Lha_parser::yycheck_[] =
  {
     195,   286,   157,     8,    16,   134,     8,    22,     9,    45,
      46,   352,   353,     3,     0,     8,     3,     3,     4,     5,
      10,    55,    56,     9,     8,     8,    11,   182,     3,     8,
      10,    16,    27,    11,    49,    13,    13,    14,    33,    34,
      29,   236,   237,    32,   385,   386,     9,    13,   203,   204,
      30,    31,     9,    39,    40,    41,    42,   212,   343,    46,
      65,    47,    98,    65,    65,    51,    52,    62,    63,    55,
      56,    46,    65,    59,    60,    61,    62,     3,     4,     5,
       9,    65,    65,     9,    99,   100,    65,    64,    11,     9,
      13,   286,   221,   222,   289,   290,   291,   292,    64,   294,
     295,   296,   297,   258,   259,   260,    29,   262,   263,   264,
     265,   266,    16,    39,    40,    41,    42,    16,   113,   114,
     115,     4,     5,     0,    10,     6,     9,   122,   123,   124,
     125,   126,   147,    59,    60,    61,    62,     3,     4,     5,
      16,    17,    18,     9,    30,    31,     9,    13,    29,    30,
      31,    32,     4,    10,    30,    31,    39,    40,    41,    42,
       3,     4,     5,    10,     8,    31,     9,    11,   363,   364,
     365,   366,    24,    30,    31,     9,    59,    60,    61,    62,
       3,     4,     5,    30,    31,    11,     9,    13,    31,    28,
      29,   346,   347,    32,   323,   324,    52,   352,   193,   194,
     498,   499,    57,    58,    56,    57,    58,    59,    31,     8,
      10,    10,   510,   511,   512,   513,   514,   515,   516,   517,
     518,   506,     9,   378,   379,   380,   381,   382,    28,    29,
      30,    31,    32,     3,     5,     5,   534,   535,    45,     9,
       8,    48,    10,     3,    31,     5,    33,    34,    35,     9,
      37,    38,     4,    21,    22,     6,    43,    44,    45,    51,
     415,    31,    16,    33,    34,    35,    36,    10,     3,     4,
       5,    31,     5,    33,    34,    35,    36,    28,    29,    30,
      31,    32,   437,    10,     3,     4,     5,    30,    31,    56,
       9,    53,    54,   488,    21,    22,    16,   452,    26,    27,
     455,   456,   457,     6,    23,    24,    25,     3,     4,     5,
       8,   506,    31,     9,    33,    34,    35,     3,     4,     5,
       8,    26,    27,     9,    55,    28,    29,    30,    31,     6,
      28,    29,    30,    31,    32,    31,     8,    33,    34,    35,
      28,    29,    30,    31,    32,    31,     8,    33,    34,    35,
      49,    28,    29,    30,    31,     8,    28,    29,    30,    31,
      32,     8,    28,    29,    16,     8,    28,    29,    30,    31,
      32,     8,   458,   459,    16,    28,    29,    30,    31,    32,
       8,    28,    29,    30,    31,    32,    29,    30,    31,    32,
       8,    28,    29,    30,    31,    32,     8,   214,   215,    50,
      28,    29,    30,    31,    32,    51,    52,    10,    10,    55,
      56,    29,    30,    31,    32,    10,    54,    29,    30,    31,
      32,    16,    17,    18,    19,    20,    28,    29,    30,    31,
      32,    10,    53,    28,    29,    30,    31,    32,    10,     3,
       4,     5,     8,    10,     9,     9,     9,     6,    10,    28,
      29,    30,    31,    32,    10,     9,    28,    29,    30,    31,
      32,    28,    29,    30,    31,    32,    28,    29,    30,    31,
      32,    10,    28,    29,    30,    31,    32,    10,     3,     4,
       5,    64,    10,     9,     9,     9,     6,    10,     9,    28,
      29,    30,    31,    32,    10,    28,    29,    30,    31,    32,
      28,    29,    30,    31,    32,    28,    29,    30,    31,    32,
      10,     9,    28,    29,    30,    31,    32,    10,     9,    49,
      50,    64,    10,    53,    54,    57,    16,    10,    28,    29,
      30,    31,    32,    10,    16,    28,    29,    30,    31,    32,
      28,    29,    30,    31,    32,    28,    29,    30,    31,    32,
      10,    28,    29,    30,    31,    32,    10,    58,    50,     3,
      10,    53,    54,     3,    10,     3,    29,     3,    28,    29,
      30,    31,    32,    10,    28,    29,    30,    31,    32,    29,
      30,    31,    32,    29,    30,    31,    32,    10,    64,    64,
       3,    10,    29,    30,    31,    32,    10,     3,    28,    29,
       4,    12,    32,    64,    16,    12,    29,    30,    31,    32,
      29,    30,    31,    32,    28,    29,    30,    31,    29,    30,
      31,    32,    29,    30,    31,    32,    12,    16,     6,    64,
      12,    28,    29,    30,    31,    32,    28,    29,    30,    31,
      32,     3,     6,    29,    30,    31,    32,    29,    30,    31,
      32,    16,    17,    18,    19,    20,    28,    29,    29,    32,
      32,    10,     8,    28,    29,    30,    31,    32,   117,   118,
     119,   120,     8,     3,     9,     9,     3,     3,     3,    29,
       9,     9,    29,     3,    29,     9,     3,     6,     6,     3,
      65,    29,    32,     9,    15,     8,    11,     9,     9,     9,
       9,     6,     3,    11,     6,     3,     3,     3,     3,     8,
      32,     3,    12,     6,     8,     6,     5,     3,     8,     3,
       8,     3,    29,    64,     3,    12,    29,    12,    11,    10,
      10,     5,     9,     6,    10,     3,     8,    12,     7,    10,
       3,     8,     6,    21,     3,     3,     8,     3,     3,   228,
       3,     3,     3,     3,   232,     3,    21,    89,     3,    29,
       6,    10,   416,    29,   454,     9,     9,     9,    29,    29,
     426,    12,    29,     9,    78,    29,    16,   490,    -1,    -1,
      -1,    -1,    29,    20,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    47,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    51,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88
  };

  const signed char
  Lha_parser::yystos_[] =
  {
       0,     3,     4,     5,     9,    39,    40,    41,    42,    47,
      51,    52,    59,    60,    61,    62,    67,    68,    73,    78,
      79,   111,   112,   114,    16,   114,     9,     9,     9,     9,
      68,    16,    16,     9,     9,     0,    55,    56,    85,    86,
      88,    49,    50,    53,    54,    74,    75,    76,    77,    80,
      81,    83,    79,    78,   111,     6,    28,    29,    30,    31,
     114,    10,     9,    31,    33,    34,    35,    37,    38,    43,
      44,    45,   115,   116,   115,     3,    10,     4,    85,     5,
       5,   115,   115,    16,    16,    57,    58,    90,    91,    98,
      88,    86,    26,    27,    26,    27,    16,    16,    75,    80,
      80,    74,    75,   111,    83,    81,   114,   114,   114,   114,
       6,   115,   115,     9,     9,     9,     9,     9,     9,     9,
       9,    10,    28,    29,    30,    31,    32,    10,    10,     8,
      90,     6,     6,     8,     8,    64,    64,    16,    16,    98,
      91,     3,     3,     3,     3,    64,    64,    80,   111,   111,
      10,   115,   115,   115,     3,     4,     5,     9,    31,   117,
     118,   117,   117,   117,   117,   115,   115,   115,   115,   115,
       4,     3,     4,     5,   113,   113,     3,    87,     3,    89,
      64,    64,    16,    16,     6,     6,     3,    46,    82,     3,
      84,   111,    10,     8,     8,    11,    13,    29,    29,    29,
       3,     4,     5,     9,    31,    33,    34,    35,    70,     3,
       4,     5,     9,    10,    30,    31,    10,    10,    10,    10,
      10,     8,     8,     8,    65,     8,    65,     9,    92,    93,
       9,    99,   100,    70,     3,     5,     9,    31,    33,    34,
      35,    36,    69,     3,     8,    65,     8,    65,   115,   115,
      69,     3,     3,     3,    11,    13,    70,    70,     9,     9,
       9,    10,    28,    29,    30,    31,    32,    29,    29,    29,
      70,   118,   118,   113,   113,     3,     6,     3,     6,     3,
      65,    93,     9,    65,    99,     6,    11,    69,    69,     9,
       9,     9,     9,     6,    29,    30,    31,    32,    11,     3,
      46,     6,     3,     6,    10,    10,    12,     3,    72,    10,
      70,    70,    70,    29,    70,    70,    70,    70,    70,     3,
       3,     3,    10,     8,     8,     8,     6,     3,     6,     3,
      69,    72,    10,    69,    69,    69,    69,    69,    69,    69,
      69,     5,     3,     8,    12,    10,     8,     8,     3,    29,
     113,   113,     9,    23,    24,    25,    70,    96,    97,     8,
      12,    12,    10,    28,     8,     8,     8,    12,    11,    72,
      70,    70,     3,    10,    10,    70,    96,    96,    16,    17,
      18,    19,    20,     8,    10,    21,    22,     3,    69,    69,
      69,    69,     5,    10,    10,    10,    70,    70,    70,    70,
      70,     9,     6,    96,    96,    10,    10,    10,    10,    10,
      12,     3,    94,    95,     8,     7,     8,    10,    13,    14,
      64,   101,   102,    70,    95,    10,    15,     3,   103,     8,
       6,   102,     8,    65,     3,     4,     5,     9,    13,    31,
     104,   105,   106,   107,     3,    29,    29,    29,    70,     3,
       4,     5,     9,     8,    21,    16,    17,    18,    30,    31,
       3,     3,     3,    10,    29,    29,    29,    70,    13,    64,
     108,   104,    70,    70,    70,   107,   107,    29,     3,     3,
       3,    10,     3,   109,   110,    10,     3,    29,    11,    16,
       8,    65,     6,     3,    69,     3,     4,     5,     9,    31,
      33,    34,    35,    71,   110,    12,    11,    13,    71,    71,
       9,     9,     9,    28,    29,    30,    31,    32,    16,    69,
      72,    10,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    12,    12,    10,     8,     8,    71,    71,    10,    10
  };

  const signed char
  Lha_parser::yyr1_[] =
  {
       0,    66,    67,    67,    67,    67,    68,    68,    68,    68,
      68,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    72,    72,    73,
      73,    73,    73,    73,    74,    74,    75,    75,    76,    76,
      77,    77,    78,    79,    80,    80,    81,    82,    82,    82,
      82,    82,    82,    83,    84,    84,    85,    85,    86,    87,
      87,    88,    89,    89,    90,    90,    91,    92,    92,    93,
      93,    94,    94,    95,    96,    96,    96,    96,    96,    96,
      96,    97,    97,    97,    97,    97,    98,    99,    99,   100,
     101,   101,   101,   101,   102,   103,   103,   104,   104,   104,
     105,   105,   105,   106,   106,   106,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   108,   108,   109,   109,
     110,   110,   111,   111,   112,   112,   113,   113,   113,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   116,   116,   116,
     116,   116,   117,   117,   117,   118,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118
  };

  const signed char
  Lha_parser::yyr2_[] =
  {
       0,     2,     0,     4,     3,     1,     0,     5,     4,     4,
       3,     1,     1,     4,     4,     3,     2,     3,     3,     3,
       3,     4,     6,     6,     6,     6,     1,     1,     2,     1,
       4,     3,     2,     3,     3,     3,     3,     3,     4,     6,
       6,     1,     1,     2,     1,     4,     4,     3,     2,     3,
       3,     3,     3,     3,     4,     6,     6,     2,     3,     2,
       2,     1,     1,     0,     1,     2,     1,     2,     6,     6,
       4,     4,     4,     4,     2,     2,     6,     1,     2,     5,
       3,     4,     7,     6,     1,     3,     2,     2,     6,     1,
       3,     6,     1,     3,     2,     2,     6,     1,     2,    10,
       6,     1,     3,     3,     1,     1,     1,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     6,     1,     2,    14,
       1,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     3,     1,     3,     3,     1,     5,     3,     3,
       3,     2,     6,     4,     4,     4,     3,     1,     1,     3,
       3,     6,     1,     2,     4,     2,     1,     1,     1,     4,
       1,     3,     4,     1,     1,     6,    10,    10,     4,     3,
       3,     3,     3,     3,     1,     1,     1,     6,     6,     2,
       4,     3,     3,     3,     3,     3,     3,     4,     4,     4,
       4,     4,     1,     3,     3,     1,     2,     4,     5,     3,
       3,     1,     1,     3,     2,     6,     4,     4,     4
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Lha_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "str", "rval", "ival",
  "SEMICOLON", "COLON", "COMMA", "LB", "RB", "LSB", "RSB", "SHARP", "ALL",
  "BackSlash", "EQ", "GEQ", "LEQ", "LL", "GG", "AND", "OR", "NOT", "TRUE",
  "FALSE", "DOUBLE", "INT", "DIV", "MUL", "PLUS", "MINUS", "POWER",
  "FLOOR", "MIN", "MAX", "MOD", "LhaMIN", "LhaMAX", "AVG", "VAR", "PROB",
  "SPRT", "LAST", "INTEGRAL", "MEAN", "DISC", "NOTDET", "LhaName", "Const",
  "Hybrid", "NbLoc", "NbVar", "VList", "LList", "Iloc", "Floc",
  "locations", "edges", "PDF", "CDF", "EXIST_TOK", "NOTALL_TOK", "NEG",
  "'{'", "'}'", "$accept", "LHA", "declarations", "IntMarkingFormula",
  "RealMarkingFormula", "RealVarMarkingFormula", "ColorClassList", "Sizes",
  "Constants", "HybridVars", "Constant", "HybridVar", "NbVariables",
  "NbLocations", "Lists", "VariablesList", "VLabels", "LocationsList",
  "LLabels", "InitFinal", "init", "iLLabels", "final", "fLLabels",
  "definitions", "LocationsDef", "LOCATIONS", "LOCATION", "FLOWS", "FLOW",
  "LogExpr", "CompExpr", "EdgesDef", "EDGES", "EDGE", "ACTIONS",
  "SetOfActions", "Actions", "CONSTRAINTS", "CONSTRAINT", "LinearExp",
  "term", "UPDATES", "Updates", "Update", "HaslExps", "HaslExp", "rorival",
  "TopHaslExp", "AlgExpr", "LhaFunc", "LinForm", "VarTerm", YY_NULLPTR
  };

#if YYDEBUG
  const short
  Lha_parser::yyrline_[] =
  {
       0,   190,   190,   191,   192,   193,   195,   196,   197,   198,
     199,   205,   206,   221,   232,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   252,   253,   254,   261,
     273,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   295,   296,   297,   304,   323,   330,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   352,   355,   360,
     361,   362,   363,   364,   366,   367,   369,   370,   372,   381,
     390,   398,   408,   410,   412,   413,   415,   431,   439,   449,
     460,   468,   476,   485,   495,   499,   504,   505,   507,   509,
     517,   522,   524,   532,   541,   542,   546,   556,   557,   559,
     570,   584,   585,   586,   593,   594,   595,   596,   597,   598,
     599,   601,   602,   603,   604,   605,   610,   614,   615,   617,
     642,   643,   644,   648,   650,   652,   656,   661,   662,   663,
     665,   670,   675,   681,   682,   683,   685,   689,   694,   699,
     704,   725,   729,   734,   739,   744,   767,   773,   778,   779,
     781,   786,   798,   798,   800,   811,   824,   825,   826,   836,
     840,   843,   846,   850,   853,   856,   859,   874,   889,   900,
     903,   906,   909,   912,   915,   918,   922,   925,   926,   927,
     928,   929,   930,   931,   932,   933,   934,   936,   946,   956,
     966,   976,   987,   988,   989,   992,  1003,  1010,  1017,  1022,
    1027,  1033,  1036,  1039,  1058,  1062,  1067,  1072,  1077
  };

  // Print the state stack on the debug stream.
  void
  Lha_parser::yystack_print_ ()
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
  Lha_parser::yy_reduce_print_ (int yyrule)
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

  Lha_parser::token_number_type
  Lha_parser::yytranslate_ (int t)
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
       2,     2,     2,    64,     2,    65,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63
    };
    const int user_token_number_max_ = 318;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

} // lha
#line 3141 "/home/pierre/Documents/Cosmos/src/ModelGenerator/LhaParser/Lha-parser.cc"

#line 1101 "src/ModelGenerator/LhaParser/Lha-parser.yy"


void
lha::Lha_parser::error (const lha::Lha_parser::location_type& l,
const std::string& m)
{
        Reader.error (l, m);
}
