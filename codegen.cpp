#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define maxNodes 100

int position;
int label = 0;
FILE *out;
int leaveloop = -1;

struct Node2{
    char* op;
    struct Node2 *operands[maxNodes];
};

struct Node2* params;
int global_var = 0;

static const char *block[] = {
    "decvar",
    "assign",
    "funccall",
    "if",
    "while",
    "return",
    "break",
    "continue",
};

static const char *operators[] = {
    "+",
    "-",
    "*",
    "/",
    "<",
    "<=",
    ">",
    ">=",
    "==",
    "!=",
    "&&",
    "||",
    "!",
};

void genBlock (struct Node2* root);
void genFuncCall (struct Node2* root);

void genStruct (char* tree, struct Node2** root) {
    
    struct Node2* p = (*root);
    
    int start = 0;
    int x = 0;
    char word[250];
    strcpy(word, "\0");
    char aux[2];
    aux[1] = '\0';
        
    while (1) {
        if (tree[position] == '['){
            if (start){
                p->operands[x] = (Node2*) calloc (1, sizeof(struct Node2));
                genStruct(tree, &p->operands[x]);
                x++;
            }
            else {
                start = 1;
                position++;
            }
        }
        else if (tree[position] == ']'){
            position++;
            p->op = strdup(word);
            return;
        }
        else if (tree[position] == ' ' || tree[position] == '\t' || tree[position] == '\n' || tree[position] == '\v' || tree[position] == '\f' || tree[position] == '\r')
            position++;
        else {
            aux[0] = tree[position];
            strcat(word, aux);       
            position++;
        }
    }
    
}

void rename (struct Node2** root, int level, char* orig, char* sub){
    
    struct Node2* p = (*root);
    
    while (p->operands[level]!=NULL) {
        rename(&p->operands[level], 0, orig, sub);
        level ++;
    }
    
    if (!strcmp(p->op, orig))
        p->op = strdup(sub);
    
}

void fixTree (struct Node2** root) {
    
    struct Node2* p = (*root);
    
    if (p == NULL)
        return;
    
    int i = 0;
    int j;
    char numVariavel[100];
    char nomeVariavel[101];
    
    while (p->operands[i] != NULL) {
        i++;
    }
    
    for (j = i-1; j >= 0; j--) {
        fixTree(&p->operands[j]);
    }
    
    j = 0;
    
    if (!strcmp(p->op, "block")){
        while(p->operands[j] != NULL){
            while (p->operands[j] != NULL) {
                if (!strcmp(p->operands[j]->op, "decvar"))
                    break;
                j++;
            }
            if (p->operands[j] != NULL) {
                nomeVariavel[0] = '_';
                nomeVariavel[1] = 'a';
                nomeVariavel[2] = '\0';
                sprintf(numVariavel,"%d",global_var);
                strcat(nomeVariavel, numVariavel);
                rename(&p, j,p->operands[j]->operands[0]->op,nomeVariavel); 
                global_var++;
                j++;
            }
        }
    }
    else if (!strcmp(p->op, "decfunc")){
        
        while (p->operands[1]->operands[j] != NULL) {
            nomeVariavel[0] = '_';
            nomeVariavel[1] = 'a';
            nomeVariavel[2] = '\0';
            sprintf(numVariavel,"%d",global_var);
            strcat(nomeVariavel, numVariavel);
            rename(&p, 1, p->operands[1]->operands[j]->op,nomeVariavel);
            global_var++;
            j++;
        }
    }
    
    return;
    
}

void findDecs(struct Node2* root){
    
    if (root == NULL)
        return;
    
    int i = 0;
    
    if (!strcmp(root->op, "decvar")){
        fprintf(out, "\t%s:       .word   0\n", root->operands[0]->op); 
        
    }
    else {
        while (root->operands[i] != NULL) {
            findDecs(root->operands[i]);
            i++;
        }   
    }
    
    return;
}

void genExp (struct Node2* root){
    
    int i;
    
    for (i = 0; i < 13 ; i++){
        if (!strcmp(root->op, operators[i])){
          break;
        }
    }
    
    switch (i){
            
            case 0:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\taddu $a0, $t0, $t1\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 1:
            
                genExp (root->operands[0]);
                if (root->operands[1] != NULL){
                
                    genExp (root->operands[1]);
                    
                    fprintf(out, "\tlw $t1, 4($sp)\n");
                    fprintf(out, "\taddiu $sp, $sp, 4\n");
                    
                    fprintf(out, "\tlw $t0, 4($sp)\n");
                    fprintf(out, "\taddiu $sp, $sp, 4\n");
                    
                    fprintf(out, "\tsubu $a0, $t0, $t1\n");
                    
                    fprintf(out, "\tsw $a0, 0($sp)\n");
                    fprintf(out, "\taddiu $sp, $sp, -4\n");
                
                }
                else {
                    
                    fprintf(out, "\tlw $t0, 4($sp)\n");
                    fprintf(out, "\taddiu $sp, $sp, 4\n");
                    
                    fprintf(out, "\tnegu $a0, $t0\n");
                    
                    fprintf(out, "\tsw $a0, 0($sp)\n");
                    fprintf(out, "\taddiu $sp, $sp, -4\n");
                }
                
                break;

            case 2:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tmult $t0, $t1\n");
                fprintf(out, "\tmflo $a0\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 3:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tdiv $t0, $t1\n");
                fprintf(out, "\tmflo $a0\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 4:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tslt $a0, $t0, $t1\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 5:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tslt $t2, $t0, $t1\n");
                fprintf(out, "\tbeq $t0, $t1, trueb%d\n",label);
                
                fprintf(out, "\tori $t3, $0, 0\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\ttrueb%d:\n", label);
                fprintf(out, "\tori $t3, $0, 1\n");
                fprintf(out, "\tend%d:\n", label);
                
                fprintf(out, "\tor $a0, $t2, $t3\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;

            case 6:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tslt $a0, $t1, $t0\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 7:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tslt $t2, $t1, $t0\n");
                fprintf(out, "\tbeq $t0, $t1, trueb%d\n",label);
                
                fprintf(out, "\tori $t3, $0, 0\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\ttrueb%d:\n", label);
                fprintf(out, "\tori $t3, $0, 1\n");
                fprintf(out, "\tend%d:\n", label);
                
                fprintf(out, "\tor $a0, $t2, $t3\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;  
                
            case 8:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tbeq $t0, $t1, trueb%d\n",label);
                
                fprintf(out, "\tori $a0, $0, 0\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\ttrueb%d:\n", label);
                fprintf(out, "\tori $a0, $0, 1\n");
                fprintf(out, "\tend%d:\n", label);
                                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;  
                
            case 9:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tbeq $t0, $t1, falso%d\n",label);
                
                fprintf(out, "\tori $a0, $0, 1\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\tfalso%d:\n", label);
                fprintf(out, "\tori $a0, $0, 0\n");
                fprintf(out, "\tend%d:\n", label);
                                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;  
                
            case 10:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tbeq $t0, $0, falso%d\n",label);
                fprintf(out, "\tbeq $t1, $0, falso%d\n",label);
                
                fprintf(out, "\tori $a0, $0, 1\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\tfalso%d:\n", label);
                fprintf(out, "\tori $a0, $0, 0\n");
                fprintf(out, "\tend%d:\n", label);
                                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;  
                
            case 11:
            
                genExp (root->operands[0]);
                genExp (root->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tbne $t0, $0, trueb%d\n",label);
                fprintf(out, "\tbne $t1, $0, trueb%d\n",label);
                
                fprintf(out, "\tori $a0, $0, 0\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\ttrueb%d:\n", label);
                fprintf(out, "\tori $a0, $0, 1\n");
                fprintf(out, "\tend%d:\n", label);
                                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;  
                
            case 12:
            
                genExp (root->operands[0]);
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tbne $t0, $0, falso%d\n",label);
                
                fprintf(out, "\tori $a0, $0, 1\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\tfalso%d:\n", label);
                fprintf(out, "\tori $a0, $0, 0\n");
                fprintf(out, "\tend%d:\n", label);
                                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;  

            default:
            
                if (!strcmp(root->op, "funccall")){
                    genFuncCall(root);
                    fprintf(out, "\tsw $v0, 0($sp)\n");
                    fprintf(out, "\taddiu $sp, $sp, -4\n");
                }
                else{
                    int tamanho = strlen(root->op);
                    for (i = 0; i < tamanho; i++){
                        if (!isdigit(root->op[i]))
                            break;
                    }
                    if (i == tamanho){
                        fprintf(out, "\tli $a0, %s\n",root->op);
                        fprintf(out, "\tsw $a0, 0($sp)\n");
                        fprintf(out, "\taddiu $sp, $sp, -4\n");
                    }
                    else{
                        i = 0;
                        while (params->operands[i] != NULL){
                            if (!strcmp(params->operands[i]->op,root->op)){
                                break;
                            }
                            i++;
                        }
                        if (params->operands[i] != NULL){
                            fprintf(out, "\tlw $a0, %d($fp)\n",4+i*4);
                        }
                        else {
                            fprintf(out, "\tlw $a0, %s\n",root->op);    
                        }
                        fprintf(out, "\tsw $a0, 0($sp)\n");
                        fprintf(out, "\taddiu $sp, $sp, -4\n");
                    }
                    
                }
        }
        
        return; 
}

void genBreak (){
    
    if (leaveloop != -1){
        fprintf(out, "\tb ww%d\n", leaveloop);         
    }
    
}

void genContinue (){
    
    if (leaveloop != -1){
        fprintf(out, "\tb trueb%d\n", leaveloop);         
    }
    
}

void genAssign (struct Node2* root){
    
    int i = 0;

    if (root->operands[1] != NULL){

        genExp(root->operands[1]);
    
        fprintf(out, "\tlw $a0, 4($sp)\n");
        
        while (params->operands[i] != NULL){
            if (!strcmp(params->operands[i]->op,root->operands[0]->op)){
                break;
            }
            i++;
        }


        if (params->operands[i] != NULL){
            fprintf(out, "\tsw $a0, %d($fp)\n",4+i*4);
        }
        else {
            fprintf(out, "\tsw $a0, %s\n",root->operands[0]->op );  
        }

        fprintf(out, "\taddiu $sp, $sp, 4\n");
    }
    
    return;
    
}

void genFuncCall (struct Node2* root){
    
    int i = 0;
    int j = 0;
    
    while (root->operands[1]->operands[i] != NULL){
        i++;
    }

    for (j = i - 1; j >= 0; j--){
        genExp(root->operands[1]->operands[j]);
    }
    
    if (!strcmp(root->operands[0]->op,"print")) {
        fprintf(out, "\tjal _f_print\n");
    }
    else {

        fprintf(out, "\tsw $t4, 0($sp)\n");
        fprintf(out, "\tmove $fp, $sp\n");
        fprintf(out, "\taddiu $sp, $sp, -4\n");
        
        fprintf(out, "\tjal _f_%s\n", root->operands[0]->op);
        
        fprintf(out, "\tlw $t4, 0($fp)\n");
        fprintf(out, "\taddiu $sp, $fp, %d\n", 4*i);
        fprintf(out, "\tlw $fp, 0($t4)\n");
    
    }
    
    return;
    
}

void genIf (struct Node2* root){
    
    int count = label;
    label ++;
    
    genExp (root->operands[0]);
    
    fprintf(out, "\tlw $a0, 4($sp)\n");
    fprintf(out, "\taddiu $sp, $sp, 4\n");
    fprintf(out, "\tbeqz $a0, falso%d\n", count);  
    
    genBlock (root->operands[1]);
    
    fprintf(out, "\tb end%d\n", count);
    fprintf(out, "\tfalso%d:\n", count);
    
    if (root->operands[2] != NULL){
        
        genBlock(root->operands[2]);
        
    }
    
    fprintf(out, "\tend%d:\n", count);
    
    return;
}

void genWhile (struct Node2* root){
    
    int leaveloopold = leaveloop; 
    int count = label;
    leaveloop = count;
    label ++;
        
    fprintf(out, "\ttrueb%d:\n",count);
    
    genExp (root->operands[0]);
    
    fprintf(out, "\tlw $a0, 4($sp)\n");
    fprintf(out, "\taddiu $sp, $sp, 4\n");
    fprintf(out, "\tbeqz $a0, ww%d\n", count); 
    
    genBlock (root->operands[1]);
    
    leaveloop = leaveloopold;
    fprintf(out, "\tb trueb%d\n", count);
    fprintf(out, "\tww%d:\n", count);
    
    return;
    
}

void genReturn (struct Node2* root){
    
    if (root->operands[0] != NULL){
        genExp (root->operands[0]);
        fprintf(out, "\tlw $v0, 4($sp)\n");
    }
    else {
        fprintf(out, "\tli $v0, 0\n");
    }

    fprintf(out, "\tlw $ra, 4($t4)\n");
    fprintf(out, "\tjr $ra\n");

    return; 
}

void genBlock(struct Node2* root) {
    
    int i, j;
    
    j = 0;
    
    while (root->operands[j] != NULL){
    
        for (i = 0; i < 8 ; i++){
            if (!strcmp(root->operands[j]->op, block[i])){
              break;
            }
        }
                
        switch (i){
            
            case 0:
                
            case 1:
                genAssign(root->operands[j]);
                break;

            case 2:
                genFuncCall(root->operands[j]);
                break;

            case 3:
                genIf(root->operands[j]);
                break;

            case 4:
                genWhile(root->operands[j]);
                break;

            case 5:
                genReturn(root->operands[j]);
                break;

            case 6:
                genBreak();
                break;

            case 7:
                genContinue();
                break;  

            default:
                fprintf(out, "error");
                exit(1);
        }
        j++;
    
    }
    
    return;
}

int genFunc (struct Node2* root){
    
    if (root == NULL){
        fprintf(out, "error");
        exit(1);
    }
    
    fprintf(out, ".data\n");

    findDecs(root);
        
    fprintf(out, "\n");
    fprintf(out, ".text\n");
    fprintf(out, "\n");
    
    fprintf(out, "_f_print:\n");
    fprintf(out, "\tlw $a0, 4($sp)\n");
    fprintf(out, "\taddiu $sp, $sp, 4\n");
    fprintf(out, "\tli $v0, 1\n");
    fprintf(out, "\tsyscall\n");
    fprintf(out, "\tli $v0, 11\n");
    fprintf(out, "\tli $a0, 0x0a\n");
    fprintf(out, "\tsyscall\n");
    fprintf(out, "\tjr $ra\n");

    int aux;
    int i,j;
    
    struct Node2* func;
    
    i = 0;
        
    while (root->operands[i] != NULL) {
        
        func = root->operands[i];
        j = 0;
        aux=-4;
                
        if (!strcmp(func->op, "decfunc")){
            
            if (func->operands[0] == NULL){
                fprintf(out, "error");
                exit(1);
            }
            fprintf(out, "\n");
            fprintf(out, "_f_%s:\n", func->operands[0]->op);  
            fprintf(out, "\tsw $ra, 0($sp)\n");
            fprintf(out, "\taddiu $sp, $sp, -4\n");

            if (func->operands[1] == NULL){
                fprintf(out, "error");
                exit(1);
            } 
            
            params = func->operands[1];

            fprintf(out, "\tsw $fp, 0($sp)\n");
            fprintf(out, "\tmove $t4, $sp\n");
            fprintf(out, "\taddiu $sp, $sp, -4\n");
            
            if (func->operands[2] == NULL){
                fprintf(out, "error");
                exit(1);
            }

            genBlock(func->operands[2]);
            
            fprintf(out, "\tli $v0, 0\n");
            fprintf(out, "\tlw $ra, 4($t4)\n");
            fprintf(out, "\tjr $ra\n");   
        }
        i++;
        
    }
    
    fprintf(out, "\n");
    fprintf(out, "main:\n");
    
    i = 0;

    while (root->operands[i] != NULL) {
        func = root->operands[i];
        if (!strcmp(func->op, "decvar")) {
            genAssign(func);
        }
        i++;
    }
    
    fprintf(out, "\tjal _f_main\n");
    fprintf(out, "\tli $v0, 10\n");
    fprintf(out, "\tsyscall\n");
    
}
