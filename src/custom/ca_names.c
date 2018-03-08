/*****************************************************************************
 *
 * Module: ca_names
 *
 * Prefix: CAN
 *
 * Description:
 *
 * This module implements a demo traversal of the abstract syntax tree that
 * sums up all integer constants and prints the result at the end of the traversal.
 *
 *****************************************************************************/


#include "ca_names.h"

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#include "memory.h"
#include "ctinfo.h"


/*
 * INFO structure
 */

struct INFO {
    node* table_stack[32];
    int index;
};

#define ERROR_RE_DEC_VAR "Variable is already declared"
#define ERROR_UN_DEC_VAR "Variable is used but not yet declared"

/*
 * INFO macros
 */

// #define INFO_STACK(n)  ((n)->table_stack)
#define INFO_INDEX(n)  ((n)->index)
#define INFO_GET_TABLE(n, i) ((n)->table_stack[i])
#define INFO_GLOBAL_TABLE(n) ((n)->table_stack[0])
#define INFO_TABLE_STACK(n) ((n)->table_stack)
#define INFO_CURRENT_TABLE(n) ((n)->table_stack[(n)->index])


/*
 * INFO functions
 */

static info *MakeInfo(void)
{
    info *result;

    DBUG_ENTER( "MakeInfo");

    result = (info *)MEMmalloc(sizeof(info));

    INFO_INDEX(result) = 0;

    DBUG_RETURN( result);
}

static info *FreeInfo( info *info)
{
    DBUG_ENTER ("FreeInfo");

    info = MEMfree( info);

    DBUG_RETURN( info);
}

void throwError(char *msg) {
    printf("ERROR: %s\n", msg);
}

// @todo Not sure about cctype here
bool addSymbolTableEntry(node* symbol_table, char* name, cctype type) {
    if (NODE_TYPE (symbol_table) != N_symboltable) {
        // @todo terminate 
        return 0;
    }

    while (SYMBOLTABLE_NEXT(symbol_table) != NULL) {
        symbol_table = SYMBOLTABLE_NEXT(symbol_table);
    }
    // @todo Not sure if this works, node_basic.c as guideline

    // Allocate a new symboltable node
    /*
    node* table = MakeEmptyNode ();
    NODE_TYPE (table) = N_symboltable;
    table->sons.N_symboltable = MEMmalloc (sizeof (struct SONS_N_SYMBOLTABLE));
    
    // Allocate a new symboltableentry node
    node* entry = MakeEmptyNode ();
    NODE_TYPE (entry) = N_symboltableentry;
    table->sons.N_symboltableentry = MEMmalloc (sizeof (struct SONS_N_SYMBOLTABLEENTRY));
    
    // Add a new entry to the symbol table
    SYMBOLTABLE_NEXT(symbol_table) = table;
    SYMBOLTABLE_SYMBOLTABLEENTRY(table) = entry;
    SYMBOLTABLEENTRY_NAME(entry) = name;
    SYMBOLTABLEENTRY_TYPE(entry) = type;
     */

    return 1;
}

bool searchSymbolTable(node* symbol_table, char* name, node** symbol_entry) {
    /* @todo Work your magic jan erik */
    if (symbol_entry != NULL) {
        // *symbol_entry = ... ;
    }

    return 0;
}


// @todo info get table boundary checking
bool searchSymbolTables(info* arg_info, char* name, node** symbol_entry) {
    for (int i = 0; i <= INFO_INDEX(arg_info); i++) {
        if (searchSymbolTable(INFO_GET_TABLE(arg_info, i), name, symbol_entry)){
            return 1;
        }
    }
    return 0;
}

/*
 * Traversal functions
 */

node *CANvardec(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CANvardec");
    node* ident = VARDEC_IDENT(arg_node);

    printf("VAR DEC: %s \n", IDENT_NAME(ident));

    if(VARDEC_EXPR(arg_node) != NULL) {
        VARDEC_EXPR(arg_node) = TRAVdo(VARDEC_EXPR(arg_node), arg_info);
    }

    /*
    if (!searchSymbolTables(arg_info, IDENT_NAME(ident), NULL)) {
        addSymbolTableEntry(INFO_CURRENT_TABLE(arg_info),
                            IDENT_NAME(VARDEC_IDENT(arg_node)), VARDEC_TYPE(arg_node));
    }
    else {
        throwError(ERROR_RE_DEC_VAR);
    }
    */
    DBUG_RETURN( arg_node);
}

node *CANglobaldec(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CANglobaldec");
    node* ident = GLOBALDEC_IDENT(arg_node);
    //node* entry;

    printf("GLOBAL DEC: %s \n", IDENT_NAME(ident));

    /*
    if (!searchSymbolTable(INFO_GLOBAL_TABLE(arg_info), IDENT_NAME(ident), &entry)) {
        addSymbolTableEntry(INFO_GLOBAL_TABLE(arg_info),
                            IDENT_NAME(ident), GLOBALDEC_TYPE(arg_node));
    }
    else {
        throwError(ERROR_RE_DEC_VAR);
    }
     */
    
    DBUG_RETURN( arg_node);
}

node *CANglobaldef(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CANglobaldef");
    node* ident = GLOBALDEF_IDENT(arg_node);
    //node* entry;

    printf("GLOBAL DEF: %s \n", IDENT_NAME(ident));
    if(GLOBALDEF_EXPR(arg_node) != NULL) {
        GLOBALDEF_EXPR(arg_node) = TRAVdo(GLOBALDEF_EXPR(arg_node), arg_info);
    }

    /*
    if (searchSymbolTable(INFO_GLOBAL_TABLE(arg_info), IDENT_NAME(ident), &entry)) {
        addSymbolTableEntry(INFO_GLOBAL_TABLE(arg_info),
                            IDENT_NAME(ident), GLOBALDEF_TYPE(arg_node));
    }
    else {
        // @todo not sure if this is the correct error
        throwError(ERROR_RE_DEC_VAR);
    }

     */
    DBUG_RETURN( arg_node);
}

node *CANfundef(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CANfundef");
    node* ident = FUNHEADER_IDENT(FUNDEF_FUNHEADER(arg_node));
    //node* entry;

    printf("FUNCTION DEF: %s -------- \n", IDENT_NAME(ident));
    FUNDEF_FUNHEADER(arg_node) = TRAVdo(FUNDEF_FUNHEADER(arg_node), arg_info);
    if(FUNDEF_FUNBODY(arg_node) != NULL) {
        FUNDEF_FUNBODY(arg_node) = TRAVdo(FUNDEF_FUNBODY(arg_node), arg_info);
    }

    /*
    if (searchSymbolTable(INFO_GLOBAL_TABLE(arg_info), IDENT_NAME(ident), &entry)) {
        addSymbolTableEntry(INFO_GLOBAL_TABLE(arg_info),
                            IDENT_NAME(ident), GLOBALDEF_TYPE(arg_node));
    }
    else {
        // @todo not sure if this is the correct error
        throwError(ERROR_RE_DEC_VAR);
    }

     */
    DBUG_RETURN( arg_node);
}


node *CANvarcall(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CANvarcall");
    node* ident = VARCALL_IDENT(arg_node);
    //node* entry;

    printf("VARIABLE CALL: %s \n", IDENT_NAME(ident));

    /*
    if (searchSymbolTable(INFO_GLOBAL_TABLE(arg_info), IDENT_NAME(ident), &entry)) {
        addSymbolTableEntry(INFO_GLOBAL_TABLE(arg_info),
                            IDENT_NAME(ident), GLOBALDEF_TYPE(arg_node));
    }
    else {
        // @todo not sure if this is the correct error
        throwError(ERROR_RE_DEC_VAR);
    }

     */
    DBUG_RETURN( arg_node);
}

node *CANparam(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CANparam");
    node* ident = PARAM_IDENT(arg_node);
    //node* entry;

    printf("PARAM DEF: %s \n", IDENT_NAME(ident));

    /*
    if (searchSymbolTable(INFO_GLOBAL_TABLE(arg_info), IDENT_NAME(ident), &entry)) {
        addSymbolTableEntry(INFO_GLOBAL_TABLE(arg_info),
                            IDENT_NAME(ident), GLOBALDEF_TYPE(arg_node));
    }
    else {
        // @todo not sure if this is the correct error
        throwError(ERROR_RE_DEC_VAR);
    }

     */
    DBUG_RETURN( arg_node);
}

node *CANfuncall(node *arg_node, info *arg_info)
{
    DBUG_ENTER("CANfuncall");
    node* ident = FUNCALL_IDENT(arg_node);
    //node* entry;

    printf("FUNCTION CALL: %s \n", IDENT_NAME(ident));
    if(FUNCALL_ARGS(arg_node) != NULL) {
        FUNCALL_ARGS(arg_node) = TRAVdo(FUNCALL_ARGS(arg_node), arg_info);
    }

    /*
    if (searchSymbolTable(INFO_GLOBAL_TABLE(arg_info), IDENT_NAME(ident), &entry)) {
        addSymbolTableEntry(INFO_GLOBAL_TABLE(arg_info),
                            IDENT_NAME(ident), GLOBALDEF_TYPE(arg_node));
    }
    else {
        // @todo not sure if this is the correct error
        throwError(ERROR_RE_DEC_VAR);
    }

     */
    DBUG_RETURN( arg_node);
}


node *CANassign(node *arg_node, info *arg_info) {
    DBUG_ENTER("CANassign");
    node* ident = ASSIGN_LET(arg_node);
    //node* entry;

    printf("ASSIGN: %s \n", IDENT_NAME(ident));
    ASSIGN_EXPR(arg_node) = TRAVdo(ASSIGN_EXPR(arg_node), arg_info);

    /*
    if (searchSymbolTables(arg_info, IDENT_NAME(ident), &entry)) {
        IDENT_DECL(ident) = entry;
    } else {
        throwError(ERROR_UN_DEC_VAR);
    }
     */

    DBUG_RETURN( arg_node);
}

node *CANforstmt(node *arg_node, info *arg_info) {
    DBUG_ENTER("CANforstmt");
    node* ident = FORSTMT_ASSIGNVAR(arg_node);
    //node* entry;

    printf("LOOP WITH ASSIGN VAR: %s \n", IDENT_NAME(ident));
    FORSTMT_ASSIGNEXPR(arg_node) = TRAVdo(FORSTMT_ASSIGNEXPR(arg_node), arg_info);

    FORSTMT_COMPAREEXPR(arg_node) = TRAVdo(FORSTMT_COMPAREEXPR(arg_node), arg_info);

    if(FORSTMT_UPDATEEXPR(arg_node) != NULL) {
        FORSTMT_UPDATEEXPR(arg_node) = TRAVdo(FORSTMT_UPDATEEXPR(arg_node), arg_info);
    }

    FORSTMT_BLOCK(arg_node) = TRAVdo(FORSTMT_BLOCK(arg_node), arg_info);

    /*
    if (searchSymbolTables(arg_info, IDENT_NAME(ident), &entry)) {
        IDENT_DECL(ident) = entry;
    } else {
        throwError(ERROR_UN_DEC_VAR);
    }
     */

    DBUG_RETURN( arg_node);
}

/*
 * Traversal start function
 */

node *CANdoNames( node *syntaxtree)
{
    info *arg_info;

    DBUG_ENTER("CANdoNames");

    arg_info = MakeInfo();

    TRAVpush(TR_can);
    syntaxtree = TRAVdo( syntaxtree, arg_info);
    TRAVpop();

    //CTInote( "Sum of integer constants: %d", INFO_SUM( arg_info));

    arg_info = FreeInfo( arg_info);

    DBUG_RETURN( syntaxtree);
}