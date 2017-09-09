#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define maxNodes 100

int position;
int label = 0;
FILE *out;
int leaveloop = -1;

struct Node2{
    char* elemento;
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

void genBlock (struct Node2* raiz);
void genFuncCall (struct Node2* raiz);

void genStruct (char* arvore, struct Node2** raiz) {
    
    struct Node2* p = (*raiz);
    
    int start = 0;
    int x = 0;
    char word[250];
    strcpy(word, "\0");
    char aux[2];
    aux[1] = '\0';
        
    while (1) {
        if (arvore[position] == '['){
            if (start){
                p->operands[x] = (Node2*) calloc (1, sizeof(struct Node2));
                genStruct(arvore, &p->operands[x]);
                x++;
            }
            else {
                start = 1;
                position++;
            }
        }
        else if (arvore[position] == ']'){
            position++;
            p->elemento = strdup(word);
            return;
        }
        else if (arvore[position] == ' ' || arvore[position] == '\t' || arvore[position] == '\n' || arvore[position] == '\v' || arvore[position] == '\f' || arvore[position] == '\r')
            position++;
        else {
            aux[0] = arvore[position];
            strcat(word, aux);       
            position++;
        }
    }
    
}

void rename (struct Node2** raiz, int nivel, char* elementoOriginal, char* elementoSubstituto){
    
    struct Node2* p = (*raiz);
    
    while (p->operands[nivel]!=NULL) {
        rename(&p->operands[nivel], 0, elementoOriginal, elementoSubstituto);
        nivel ++;
    }
    
    if (!strcmp(p->elemento, elementoOriginal))
        p->elemento = strdup(elementoSubstituto);
    
}

void fixTree (struct Node2** raiz) {
    
    struct Node2* p = (*raiz);
    
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
    
    if (!strcmp(p->elemento, "block")){
        while(p->operands[j] != NULL){
            while (p->operands[j] != NULL) {
                if (!strcmp(p->operands[j]->elemento, "decvar"))
                    break;
                j++;
            }
            if (p->operands[j] != NULL) {
                nomeVariavel[0] = '_';
                nomeVariavel[1] = 'a';
                nomeVariavel[2] = '\0';
                sprintf(numVariavel,"%d",global_var);
                strcat(nomeVariavel, numVariavel);
                rename(&p, j,p->operands[j]->operands[0]->elemento,nomeVariavel); 
                global_var++;
                j++;
            }
        }
    }
    else if (!strcmp(p->elemento, "decfunc")){
        
        while (p->operands[1]->operands[j] != NULL) {
            nomeVariavel[0] = '_';
            nomeVariavel[1] = 'a';
            nomeVariavel[2] = '\0';
            sprintf(numVariavel,"%d",global_var);
            strcat(nomeVariavel, numVariavel);
            rename(&p, 1, p->operands[1]->operands[j]->elemento,nomeVariavel);
            global_var++;
            j++;
        }
    }
    
    return;
    
}

void findDecs(struct Node2* raiz){
    
    if (raiz == NULL)
        return;
    
    int i = 0;
    
    if (!strcmp(raiz->elemento, "decvar")){
        fprintf(out, "\t%s:       .word   0\n", raiz->operands[0]->elemento); 
        
    }
    else {
        while (raiz->operands[i] != NULL) {
            findDecs(raiz->operands[i]);
            i++;
        }   
    }
    
    return;
}

void genExp (struct Node2* raiz){
    
    int i;
    
    for (i = 0; i < 13 ; i++){
        if (!strcmp(raiz->elemento, operators[i])){
          break;
        }
    }
    
    switch (i){
            
            case 0:
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\taddu $a0, $t0, $t1\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 1:
            
                genExp (raiz->operands[0]);
                if (raiz->operands[1] != NULL){
                
                    genExp (raiz->operands[1]);
                    
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
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
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
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
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
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tslt $a0, $t0, $t1\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 5:
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tslt $t2, $t0, $t1\n");
                fprintf(out, "\tbeq $t0, $t1, verdadeiro%d\n",label);
                
                fprintf(out, "\tori $t3, $0, 0\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\tverdadeiro%d:\n", label);
                fprintf(out, "\tori $t3, $0, 1\n");
                fprintf(out, "\tend%d:\n", label);
                
                fprintf(out, "\tor $a0, $t2, $t3\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;

            case 6:
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tslt $a0, $t1, $t0\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 7:
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tslt $t2, $t1, $t0\n");
                fprintf(out, "\tbeq $t0, $t1, verdadeiro%d\n",label);
                
                fprintf(out, "\tori $t3, $0, 0\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\tverdadeiro%d:\n", label);
                fprintf(out, "\tori $t3, $0, 1\n");
                fprintf(out, "\tend%d:\n", label);
                
                fprintf(out, "\tor $a0, $t2, $t3\n");
                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;  
                
            case 8:
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tbeq $t0, $t1, verdadeiro%d\n",label);
                
                fprintf(out, "\tori $a0, $0, 0\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\tverdadeiro%d:\n", label);
                fprintf(out, "\tori $a0, $0, 1\n");
                fprintf(out, "\tend%d:\n", label);
                                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;  
                
            case 9:
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
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
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
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
            
                genExp (raiz->operands[0]);
                genExp (raiz->operands[1]);
                
                fprintf(out, "\tlw $t1, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tlw $t0, 4($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, 4\n");
                
                fprintf(out, "\tbne $t0, $0, verdadeiro%d\n",label);
                fprintf(out, "\tbne $t1, $0, verdadeiro%d\n",label);
                
                fprintf(out, "\tori $a0, $0, 0\n");
                fprintf(out, "\tb end%d\n", label);
                
                fprintf(out, "\tverdadeiro%d:\n", label);
                fprintf(out, "\tori $a0, $0, 1\n");
                fprintf(out, "\tend%d:\n", label);
                                
                fprintf(out, "\tsw $a0, 0($sp)\n");
                fprintf(out, "\taddiu $sp, $sp, -4\n");
                label++;
                
                break;  
                
            case 12:
            
                genExp (raiz->operands[0]);
                
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
            
                if (!strcmp(raiz->elemento, "funccall")){
                    genFuncCall(raiz);
                    fprintf(out, "\tsw $v0, 0($sp)\n");
                    fprintf(out, "\taddiu $sp, $sp, -4\n");
                }
                else{
                    int tamanho = strlen(raiz->elemento);
                    for (i = 0; i < tamanho; i++){
                        if (!isdigit(raiz->elemento[i]))
                            break;
                    }
                    if (i == tamanho){
                        fprintf(out, "\tli $a0, %s\n",raiz->elemento);
                        fprintf(out, "\tsw $a0, 0($sp)\n");
                        fprintf(out, "\taddiu $sp, $sp, -4\n");
                    }
                    else{
                        i = 0;
                        while (params->operands[i] != NULL){
                            if (!strcmp(params->operands[i]->elemento,raiz->elemento)){
                                break;
                            }
                            i++;
                        }
                        if (params->operands[i] != NULL){
                            fprintf(out, "\tlw $a0, %d($fp)\n",4+i*4);
                        }
                        else {
                            fprintf(out, "\tlw $a0, %s\n",raiz->elemento);    
                        }
                        fprintf(out, "\tsw $a0, 0($sp)\n");
                        fprintf(out, "\taddiu $sp, $sp, -4\n");
                    }
                    
                }
        }
        
        return; 
}

void genBreak (struct Node2* raiz){
    
    if (leaveloop != -1){
        fprintf(out, "\tb falsowhile%d\n", leaveloop);         
    }
    
}

void genContinue (struct Node2* raiz){
    
    if (leaveloop != -1){
        fprintf(out, "\tb verdadeiro%d\n", leaveloop);         
    }
    
}

void genAssign (struct Node2* raiz){
    
    int i = 0;

    if (raiz->operands[1] != NULL){

        genExp(raiz->operands[1]);
    
        fprintf(out, "\tlw $a0, 4($sp)\n");
        
        while (params->operands[i] != NULL){
            if (!strcmp(params->operands[i]->elemento,raiz->operands[0]->elemento)){
                break;
            }
            i++;
        }


        if (params->operands[i] != NULL){
            fprintf(out, "\tsw $a0, %d($fp)\n",4+i*4);
        }
        else {
            fprintf(out, "\tsw $a0, %s\n",raiz->operands[0]->elemento );  
        }

        fprintf(out, "\taddiu $sp, $sp, 4\n");
    }
    
    return;
    
}

void genFuncCall (struct Node2* raiz){
    
    int i = 0;
    int j = 0;
    
    while (raiz->operands[1]->operands[i] != NULL){
        i++;
    }

    for (j = i - 1; j >= 0; j--){
        genExp(raiz->operands[1]->operands[j]);
    }
    
    if (!strcmp(raiz->operands[0]->elemento,"print")) {
        fprintf(out, "\tjal _f_print\n");
    }
    else {

        fprintf(out, "\tsw $t4, 0($sp)\n");
        fprintf(out, "\tmove $fp, $sp\n");
        fprintf(out, "\taddiu $sp, $sp, -4\n");
        
        fprintf(out, "\tjal _f_%s\n", raiz->operands[0]->elemento);
        
        fprintf(out, "\tlw $t4, 0($fp)\n");
        fprintf(out, "\taddiu $sp, $fp, %d\n", 4*i);
        fprintf(out, "\tlw $fp, 0($t4)\n");
    
    }
    
    return;
    
}

void genIf (struct Node2* raiz){
    
    int contador = label;
    label ++;
    
    genExp (raiz->operands[0]);
    
    fprintf(out, "\tlw $a0, 4($sp)\n");
    fprintf(out, "\taddiu $sp, $sp, 4\n");
    fprintf(out, "\tbeqz $a0, falso%d\n", contador);  
    
    genBlock (raiz->operands[1]);
    
    fprintf(out, "\tb end%d\n", contador);
    fprintf(out, "\tfalso%d:\n", contador);
    
    if (raiz->operands[2] != NULL){
        
        genBlock(raiz->operands[2]);
        
    }
    
    fprintf(out, "\tend%d:\n", contador);
    
    return;
}

void genWhile (struct Node2* raiz){
    
    int antigoSair = leaveloop; 
    int contador = label;
    leaveloop = contador;
    label ++;
        
    fprintf(out, "\tverdadeiro%d:\n",contador);
    
    genExp (raiz->operands[0]);
    
    fprintf(out, "\tlw $a0, 4($sp)\n");
    fprintf(out, "\taddiu $sp, $sp, 4\n");
    fprintf(out, "\tbeqz $a0, falsowhile%d\n", contador); 
    
    genBlock (raiz->operands[1]);
    
    leaveloop = antigoSair;
    fprintf(out, "\tb verdadeiro%d\n", contador);
    fprintf(out, "\tfalsowhile%d:\n", contador);
    
    return;
    
}

void genReturn (struct Node2* raiz){
    
    if (raiz->operands[0] != NULL){
        genExp (raiz->operands[0]);
        fprintf(out, "\tlw $v0, 4($sp)\n");
    }
    else {
        fprintf(out, "\tli $v0, 0\n");
    }

    fprintf(out, "\tlw $ra, 4($t4)\n");
    fprintf(out, "\tjr $ra\n");

    return; 
}

void genBlock(struct Node2* raiz) {
    
    int i, j;
    
    j = 0;
    
    while (raiz->operands[j] != NULL){
    
        for (i = 0; i < 8 ; i++){
            if (!strcmp(raiz->operands[j]->elemento, block[i])){
              break;
            }
        }
                
        switch (i){
            
            case 0:
                
            case 1:
                genAssign(raiz->operands[j]);
                break;

            case 2:
                genFuncCall(raiz->operands[j]);
                break;

            case 3:
                genIf(raiz->operands[j]);
                break;

            case 4:
                genWhile(raiz->operands[j]);
                break;

            case 5:
                genReturn(raiz->operands[j]);
                break;

            case 6:
                genBreak(raiz->operands[j]);
                break;

            case 7:
                genContinue (raiz->operands[j]);
                break;  

            default:
                fprintf(out, "Erro na arvore. GeradorCodigo");
                exit(1);
        }
        j++;
    
    }
    
    return;
}

int genFunc (struct Node2* raiz){
    
    if (raiz == NULL){
        fprintf(out, "Erro na arvore. GeradorCodigo");
        exit(1);
    }
    
    fprintf(out, ".data\n");

    findDecs(raiz);
        
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
    
    struct Node2* funcao;
    
    i = 0;
        
    while (raiz->operands[i] != NULL) {
        
        funcao = raiz->operands[i];
        j = 0;
        aux=-4;
                
        if (!strcmp(funcao->elemento, "decfunc")){
            
            if (funcao->operands[0] == NULL){
                fprintf(out, "Erro na arvore. GeradorCodigo");
                exit(1);
            }
            fprintf(out, "\n");
            fprintf(out, "_f_%s:\n", funcao->operands[0]->elemento);  
            fprintf(out, "\tsw $ra, 0($sp)\n");
            fprintf(out, "\taddiu $sp, $sp, -4\n");

            if (funcao->operands[1] == NULL){
                fprintf(out, "Erro na arvore. GeradorCodigo");
                exit(1);
            } 
            
            params = funcao->operands[1];

            fprintf(out, "\tsw $fp, 0($sp)\n");
            fprintf(out, "\tmove $t4, $sp\n");
            fprintf(out, "\taddiu $sp, $sp, -4\n");
            
            if (funcao->operands[2] == NULL){
                fprintf(out, "Erro na arvore. GeradorCodigo");
                exit(1);
            }

            genBlock(funcao->operands[2]);
            
            fprintf(out, "\tli $v0, 0\n");
            fprintf(out, "\tlw $ra, 4($t4)\n");
            fprintf(out, "\tjr $ra\n");   
        }
        i++;
        
    }
    
    fprintf(out, "\n");
    fprintf(out, "main:\n");
    
    i = 0;

    while (raiz->operands[i] != NULL) {
        funcao = raiz->operands[i];
        if (!strcmp(funcao->elemento, "decvar")) {
            genAssign(funcao);
        }
        i++;
    }
    
    fprintf(out, "\tjal _f_main\n");
    fprintf(out, "\tli $v0, 10\n");
    fprintf(out, "\tsyscall\n");
    
}
