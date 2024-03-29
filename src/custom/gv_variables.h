#ifndef _GV_VARIABLES_H_
#define _GV_VARIABLES_H_

#include "types.h"
extern node *GVVfundef(node *arg_node, info *arg_info);
extern node *GVVvardec(node *arg_node, info *arg_info);
extern node *GVVdeclarations(node *arg_node, info *arg_info);
extern node *GVVprogram(node *arg_node, info *arg_info);
extern node *GVdoVariableToFunction( node *syntaxtree);

#endif
