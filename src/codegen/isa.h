/* LOAD INSTRUCTIONS */
char* ILOAD = "iload";
char* ILOADN = "iloadn";
char* ILOADC = "iloadc";
char* ISTORE = "istore";

char* FLOAD = "fload";
char* FLOADN = "floadn";
char* FLOADC = "floadc";
char* FSTORE = "fstore";

char* BLOAD = "bload";
char* BLOADN = "bloadn";
char* BLOADC = "bloadc";
char* BSTORE = "bstore";

char* ILOADG = "iloadg";
char* FLOADG = "floadg";
char* BLOADG = "bloadg";

char* ISTOREG = "istoreg";
char* FSTOREG = "fstoreg";
char* BSTOREG = "bstoreg";

/* ARITHMETIC INSTRUCTIONS */
char* IADD = "iadd";
char* ISUB = "isub";
char* IMUL = "imul";
char* IDIV = "idiv";
char* IREM = "irem";

char* FADD = "fadd";
char* FSUB = "fsub";
char* FMUL = "fmul";
char* FDIV = "fdiv";

char* BNOT = "bnot";
char* INEG = "ineg";
char* FNEG = "fneg";

char* BMUL = "bmul";
char* BADD = "badd";

/* CONVERSION INSTRUCTIONS */

char* I2F = "i2f";
char* F2I = "f2i";

/* CONDITIONALS */
char* ILT = "ilt";
char* ILE = "ile"; 
char* IGT = "igt";
char* IGE = "ige";
char* IEQ = "ieq";
char* INE = "ine";

char* FLT = "flt";
char* FLE = "fle"; 
char* FGT = "fgt";
char* FGE = "fge";
char* FEQ = "feq";
char* FNE = "fne";

char* BEQ = "beq";
char* BNE = "bne";

char* BRANCH_F = "branch_f";
char* BRANCH_T = "branch_t";
char* JMP = "jump";

char* IINC = "iinc";


/* PSEUDO INSTRUCTIONS */
char* CONST_TABLE = ".const";
char* GLOBAL_TABLE = ".global";
char* EXPORT_FUN = ".exportfun";
char* IMPORT_FUN = ".importfun";
char* IMPORT_VAR = ".importvar";
char* EXPORT_VAR = ".exportvar";


char* ESR = "esr";
char* IRETURN = "ireturn";
char* BRETURN = "breturn";
char* FRETURN = "freturn";
char* RETURN = "return";

char* ISRN = "isrn";
char* ISRL = "isrl";
char* ISR = "isr";
char* JSR = "jsr";
char* JSRE = "jsre";
