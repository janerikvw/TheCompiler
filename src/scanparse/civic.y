%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

#include "types.h"
#include "tree_basic.h"
#include "str.h"
#include "dbug.h"
#include "ctinfo.h"
#include "free.h"
#include "globals.h"

static node *parseresult = NULL;
extern int yylex();
static int yyerror( char *errname);

%}

%union {
 nodetype            nodetype;
 char               *id;
 int                 cint;
 double               cflt;
 binop               cbinop;
 monop               cmonop;
 cctype             ccctype;
 node               *node;
}

%token BRACKET_L BRACKET_R COMMA SEMICOLON C_BRACKET_L C_BRACKET_R S_BRACKET_L S_BRACKET_R
%token MINUS PLUS STAR SLASH PERCENT LE LT GE GT EQ NE OR AND EXCL_MARK
%token TRUEVAL FALSEVAL LET
%token TBOOL TVOID TINT TFLOAT EXTERN EXPORT
%token KIF KELSE KWHILE KDO KFOR KRETURN

%token <cint> NUM
%token <cflt> FLOAT
%token <id> ID

//%type <node> stmts stmt assign varlet program

//%type <cbinop> binop
%type <cbinop> binop1
%type <cbinop> binop2
%type <cbinop> binop3
%type <cbinop> binop4
%type <cbinop> binop5
%type <cbinop> binop6
%type <cmonop> monop
%type <ccctype> rettype
%type <ccctype> vartype

%type <node> ident intval floatval boolval constant exprs
%type <node> expr expr2 expr3 expr4 expr5 expr6 expr7 expr8 arrexpr
%type <node> stmts stmt assign ifelsestmt whilestmt dowhilestmt forstmt returnstmt block funcall
%type <node> vardec vardecs arrayindex
%type <node> program declarations declaration start
%type <node> globaldef globaldec fundef fundefs funheader funbody params param

%right "then" KIF

%start start

%%
// ------- PROGRAM DEFINITIONS ---------
start:
    program
    {
        parseresult = $1;
    }
    ;

program:
    declarations
    {
        $$ = TBmakeProgram($1, NULL, NULL);
    }
    ;

declarations:
    declaration declarations
    {
        $$ = TBmakeDeclarations($1, $2);
    }
    | declaration
    {
        $$ = TBmakeDeclarations($1, NULL);
    }
    ;

declaration:
    globaldef { $$ = $1; }
    | globaldec { $$ = $1; }
    | fundef { $$ = $1; }
    ;

// ----------- GLOBAL VARIABLE DEFINITIONS -----------
globaldec:
    EXTERN vartype ident SEMICOLON
    {
        $$ = TBmakeGlobaldec($2, $3, NULL);
    }
    | EXTERN vartype S_BRACKET_L ident S_BRACKET_R ident SEMICOLON
    {
        $$ = TBmakeGlobaldec($2, $6, $4);
    }
    ;

globaldef:
    vartype ident LET expr SEMICOLON
    {
        $$ = TBmakeGlobaldef($1, FALSE, $2, TBmakeExprs($4, NULL), NULL);
    }
    | EXPORT vartype ident LET expr SEMICOLON
    {
        $$ = TBmakeGlobaldef($2, TRUE, $3, TBmakeExprs($5, NULL), NULL);
    }
    | vartype ident SEMICOLON
    {
        $$ = TBmakeGlobaldef($1, FALSE, $2, NULL, NULL);
    }
    | EXPORT vartype ident SEMICOLON
    {
        $$ = TBmakeGlobaldef($2, TRUE, $3, NULL, NULL);
    }
    | vartype  S_BRACKET_L expr S_BRACKET_R ident SEMICOLON
    {
        $$ = TBmakeGlobaldef($1, FALSE, $5, NULL, $3);
    }
    | EXPORT vartype  S_BRACKET_L expr S_BRACKET_R ident SEMICOLON
    {
        $$ = TBmakeGlobaldef($2, TRUE, $6, NULL, $4);
    }
    | vartype  S_BRACKET_L expr S_BRACKET_R ident LET arrexpr SEMICOLON 
    {
        $$ = TBmakeGlobaldef($1, FALSE, $5, $7, $3);
    }
    | EXPORT vartype  S_BRACKET_L expr S_BRACKET_R ident LET arrexpr SEMICOLON{}
    {
        $$ = TBmakeGlobaldef($2, TRUE, $6, $8, $4);
    }
    | vartype  S_BRACKET_L expr S_BRACKET_R ident LET expr SEMICOLON 
    {
        $$ = TBmakeGlobaldef($1, FALSE, $5, TBmakeExprs($7, NULL), $3);
    }
    | EXPORT vartype  S_BRACKET_L expr S_BRACKET_R ident LET expr SEMICOLON{}
    {
        $$ = TBmakeGlobaldef($2, TRUE, $6, TBmakeExprs($8, NULL), $4);
    }
    ;

// ----------- FUNCTION DEFINITIONS ---------------
fundef:
    funheader funbody
    {
        $$ = TBmakeFundef(FALSE, $1, $2, NULL);
    }
    | EXPORT funheader funbody
    {
        $$ = TBmakeFundef(TRUE, $2, $3, NULL);
    }
    |
    EXTERN funheader SEMICOLON
    {
        $$ = TBmakeFundef(FALSE, $2, NULL, NULL);
    }
    ;

// @todo How to nest vartypes en rettypes
funheader:
    vartype ident BRACKET_L params BRACKET_R
    {
        $$ = TBmakeFunheader($1, $2, $4);
    }
    | vartype ident BRACKET_L BRACKET_R
    {
        $$ = TBmakeFunheader($1, $2, NULL);
    }
    |
    rettype ident BRACKET_L params BRACKET_R
    {
        $$ = TBmakeFunheader($1, $2, $4);
    }
    | rettype ident BRACKET_L BRACKET_R
    {
        $$ = TBmakeFunheader($1, $2, NULL);
    }
    ;

params:
    param COMMA params
    {
        $$ = TBmakeParams($1, $3);
    }
    | param
    {
        $$ = TBmakeParams($1, NULL);
    }
    ;

param:
    vartype ident
    {
        $$ = TBmakeParam($1, $2, NULL);
    }
    | vartype S_BRACKET_L ident S_BRACKET_R ident
    {
        $$ = TBmakeParam($1, $5, $3);
    }
    ;

funbody:
    C_BRACKET_L vardecs stmts C_BRACKET_R
    {
        $$ = TBmakeFunbody($2, NULL, $3);
    }
    | C_BRACKET_L vardecs C_BRACKET_R
    {
        $$ = TBmakeFunbody($2, NULL, NULL);
    }
    | C_BRACKET_L stmts C_BRACKET_R
    {
        $$ = TBmakeFunbody(NULL, NULL, $2);
    }
    | C_BRACKET_L vardecs fundefs stmts C_BRACKET_R
    {
        $$ = TBmakeFunbody($2, $3, $4);
    }
    | C_BRACKET_L vardecs fundefs C_BRACKET_R
    {
        $$ = TBmakeFunbody($2, $3, NULL);
    }
    | C_BRACKET_L fundefs stmts C_BRACKET_R
    {
        $$ = TBmakeFunbody(NULL, $2, $3);
    }
    | C_BRACKET_L C_BRACKET_R
      {
          $$ = TBmakeFunbody(NULL, NULL, NULL);
      }
    ;

fundefs:
    fundef fundefs
    {
        $$ = TBmakeFundefs($1, $2);
    }
    | fundef
    {
        $$ = TBmakeFundefs($1, NULL);
    }
    ;

// ----------- STATEMENT DEFINITIONS --------------
vardecs:
    vardec vardecs 
    {
        $$ = TBmakeVardecs($1, $2);
    }
    | vardec
    {
        $$ = TBmakeVardecs($1, NULL);
    }
    ;

vardec:
    vartype ident SEMICOLON
    {
        $$ = TBmakeVardec($1, $2, NULL, NULL);
    }
    | vartype S_BRACKET_L expr S_BRACKET_R ident SEMICOLON
    {
        $$ = TBmakeVardec($1, $5, NULL, $3);
    } 
    | vartype ident LET exprs SEMICOLON 
    {
        $$ = TBmakeVardec($1, $2, $4, NULL);
    } 
    |  vartype S_BRACKET_L expr S_BRACKET_R ident LET arrexpr SEMICOLON 
    {
        $$ = TBmakeVardec($1, $5, $7, $3);
    }
    ;

arrayindex:
    ident S_BRACKET_L expr S_BRACKET_R
    {
        $$ = TBmakeArrayindex($1, $3);
    }
    ;

stmts:
    stmt stmts
    {
        $$ = TBmakeStmts($1, $2);
    }
    | stmt
    {
        $$ = TBmakeStmts($1, NULL);
    }
    ;

stmt:
    assign { $$ = $1; }
    | ifelsestmt { $$ = $1; }
    | funcall SEMICOLON { $$ = $1; }
    | whilestmt { $$ = $1; }
    | dowhilestmt { $$ = $1; }
    | forstmt { $$ = $1; }
    | returnstmt { $$ = $1; }
    ;

// ------------ STATEMENT FUNCTIONS ---------------
assign:
    ident LET expr SEMICOLON
    {
        $$ = TBmakeAssign( $1, $3, NULL);
    }
    | ident S_BRACKET_L expr S_BRACKET_R LET expr SEMICOLON
    {
        $$ = TBmakeAssign( $1, $6, $3);
    }
    ;



ifelsestmt:
    KIF BRACKET_L expr BRACKET_R block KELSE block
    {
        $$ = TBmakeIfelsestmt($3, $5, $7);
    } |
    KIF BRACKET_L expr BRACKET_R block
    {
        $$ = TBmakeIfelsestmt($3, $5, NULL);
    }
    ;

whilestmt:
    KWHILE BRACKET_L expr BRACKET_R block
    {
        $$ = TBmakeWhilestmt($3, $5);
    }

dowhilestmt:
    KDO block KWHILE BRACKET_L expr BRACKET_R SEMICOLON
    {
        $$ = TBmakeDowhilestmt($5, $2);
    }
    ;

// @todo Update expression also assign stmt
forstmt:
    KFOR BRACKET_L TINT ident LET expr COMMA expr BRACKET_R block
    {
        $$ = TBmakeForstmt($4, $6, $8, NULL, $10);
    }
    |
    KFOR BRACKET_L TINT ident LET expr COMMA expr COMMA expr BRACKET_R block
    {
        $$ = TBmakeForstmt($4, $6, $8, $10, $12);
    }
    ;

returnstmt:
    KRETURN expr SEMICOLON 
    {
        $$ = TBmakeReturnstmt($2);
    }
    | KRETURN SEMICOLON
    {
        $$ = TBmakeReturnstmt(NULL);
    }
    ;

block:
    C_BRACKET_L stmts C_BRACKET_R
    {
        $$ = TBmakeBlock($2);
    }
    | C_BRACKET_L C_BRACKET_R
    {
        $$ = TBmakeBlock(NULL);
    }
    |
    stmt
    {
        $$ = TBmakeBlock(TBmakeStmts($1, NULL));
    }
    ;

funcall:
    ident BRACKET_L exprs BRACKET_R
    {
        $$ = TBmakeFuncall($1, $3);
    }
    | ident BRACKET_L BRACKET_R
    {
        $$ = TBmakeFuncall($1, NULL);
    }
    ;

// ----------- EXPRESSION DEFINITIONS -----------
exprs:
    expr COMMA exprs
    {
        $$ = TBmakeExprs($1, $3);
    }
    | expr
    {
        $$ = TBmakeExprs($1, NULL);
    }
    ;

expr: expr binop1 expr2 { $$ = TBmakeBinop( $2, $1, $3, NULL); }
     | expr2;

expr2: expr2 binop2 expr3 { $$ = TBmakeBinop( $2, $1, $3, NULL); }
     | expr3;

expr3: expr3 binop3 expr4 { $$ = TBmakeBinop( $2, $1, $3, NULL); }
     | expr4;

expr4: expr4 binop4 expr5 { $$ = TBmakeBinop( $2, $1, $3, NULL); }
     | expr5;

expr5: expr5 binop5 expr6 { $$ = TBmakeBinop( $2, $1, $3, NULL); }
    | expr6
    ;
expr6: expr6 binop6 expr7 { $$ = TBmakeBinop( $2, $1, $3, NULL); }
     | expr7;
expr7: monop expr7 { $$ = TBmakeMonop($1, $2); }
     | BRACKET_L vartype BRACKET_R expr7 { $$ = TBmakeCastexpr($2, $4, NULL); }
     | expr8;
expr8: BRACKET_L expr BRACKET_R { $$ = $2; }
     | constant { $$ = $1; }
     | funcall { $$ = $1; }
     | arrayindex { $$ = $1; }
     | ID { $$ = TBmakeVarcall(TBmakeIdent( STRcpy( $1))); };

arrexpr:
    S_BRACKET_L S_BRACKET_R 
    {
        $$ = TBmakeExprs(NULL, NULL);
    }
    | S_BRACKET_L exprs S_BRACKET_R 
    {
        $$ = $2;
    }
    ;

constant:
    floatval
    {
        $$ = $1;
    }
    | intval
    {
        $$ = $1;
    }
    | boolval
    {
        $$ = $1;
    }
    ;

floatval:
    MINUS FLOAT {
        node* in = TBmakeFloat(0.0);
        FLOAT_VALUE(in) = -$2;
        $$ = in;
    } |
    FLOAT
    {
        node* in = TBmakeFloat(0.0);
        FLOAT_VALUE(in) = $1;
        $$ = in;
    }
    ;

intval:
    MINUS NUM {
        $$ = TBmakeNum(-$2);
    } |
    NUM
    {
        $$ = TBmakeNum($1);
    }
    ;

boolval:
    TRUEVAL
    {
        $$ = TBmakeBool( TRUE);
    }
    | FALSEVAL
    {
        $$ = TBmakeBool( FALSE);
    }
    ;

binop1: OR        { $$ = BO_or; };

binop2: AND       { $$ = BO_and; };

binop3:
    EQ        { $$ = BO_eq; }
    | NE        { $$ = BO_ne; };

binop4:
    LE        { $$ = BO_le; }
    | LT        { $$ = BO_lt; }
    | GE        { $$ = BO_ge; }
    | GT        { $$ = BO_gt; };

binop5:
    PLUS      { $$ = BO_add; }
    | MINUS     { $$ = BO_sub; };

binop6:
    STAR      { $$ = BO_mul; }
    | SLASH     { $$ = BO_div; }
    | PERCENT   { $$ = BO_mod; };


monop:
    MINUS    { $$ = MO_neg; }
    | EXCL_MARK { $$ = MO_not; }
    ;

vartype:
    TINT   { $$ = T_int; }
    |   TFLOAT { $$ = T_float; }
    |   TBOOL  { $$ = T_bool; };

rettype: TVOID  { $$ = T_void; };

ident:
    ID
    {
        $$ = TBmakeIdent( STRcpy( $1));
    }
    ;

%%

static int yyerror( char *error)
{
  CTIabort( "line %d, col %d\nError parsing source code: %s\n", 
            global.line, global.col, error);

  return( 0);
}

node *YYparseTree( void)
{
  DBUG_ENTER("YYparseTree");

  yyparse();

  DBUG_RETURN( parseresult);
}

