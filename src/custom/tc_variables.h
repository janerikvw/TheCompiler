#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#include "memory.h"
#include "ctinfo.h"

node* TCVglobaldef (node *arg_node, info *arg_info);
node* TCVbool(node *arg_node, info *arg_info);
node* TCVfloat(node *arg_node, info *arg_info);
node* TCVnum(node *arg_node, info *arg_info);
node* TCVbinop(node *arg_node, info *arg_info);
node* TCVvardec(node *arg_node, info *arg_info);
node* TCVassign(node *arg_node, info *arg_info);
node* TCVcastexpr(node *arg_node, info *arg_info);
node *TCVvarcall(node *arg_node, info *arg_info);
node *TCVfuncall(node *arg_node, info *arg_info);
node* TCVmonop(node *arg_node, info *arg_info);
node* TCVdoVariables( node *syntaxtree);

