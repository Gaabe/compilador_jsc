#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define Nos 100

int posicao;
int rotulo = 0;
FILE *saida;
int sairloop = -1;

struct nodo{
    char* elemento;
    struct nodo *operando[Nos];
};

struct nodo* parametros;
int geral = 0;

static const char *bloco[] = {
    "decvar",
    "assign",
    "funccall",
    "if",
    "while",
    "return",
    "break",
    "continue",
};

static const char *operadores[] = {
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

void tratarBloco (struct nodo* raiz);
void tratarFunccall (struct nodo* raiz);

void gerarEstrutura (char* arvore, struct nodo** raiz) {
    
    struct nodo* p = (*raiz);
    
    int comeco = 0;
    int indice = 0;
    char palavra[200];
    strcpy(palavra, "\0");
    char aux[2];
    aux[1] = '\0';
        
    while (1) {
        if (arvore[posicao] == '['){
            if (comeco){
                p->operando[indice] = (nodo*) calloc (1, sizeof(struct nodo));
                gerarEstrutura(arvore, &p->operando[indice]);
                indice++;
            }
            else {
                comeco = 1;
                posicao++;
            }
        }
        else if (arvore[posicao] == ']'){
            posicao++;
            p->elemento = strdup(palavra);
            return;
        }
        else if (arvore[posicao] == ' ' || arvore[posicao] == '\t' || arvore[posicao] == '\n' || arvore[posicao] == '\v' || arvore[posicao] == '\f' || arvore[posicao] == '\r')
            posicao++;
        else {
            aux[0] = arvore[posicao];
            strcat(palavra, aux);       
            posicao++;
        }
    }
    
}

void renomear (struct nodo** raiz, int nivel, char* elementoOriginal, char* elementoSubstituto){
    
    struct nodo* p = (*raiz);
    
    while (p->operando[nivel]!=NULL) {
        renomear(&p->operando[nivel], 0, elementoOriginal, elementoSubstituto);
        nivel ++;
    }
    
    if (!strcmp(p->elemento, elementoOriginal))
        p->elemento = strdup(elementoSubstituto);
    
}

void corrigirArvore (struct nodo** raiz) {
    
    struct nodo* p = (*raiz);
    
    if (p == NULL)
        return;
    
    int i = 0;
    int j;
    char numVariavel[100];
    char nomeVariavel[101];
    
    while (p->operando[i] != NULL) {
        i++;
    }
    
    for (j = i-1; j >= 0; j--) {
        corrigirArvore(&p->operando[j]);
    }
    
    j = 0;
    
    if (!strcmp(p->elemento, "block")){
        while(p->operando[j] != NULL){
            while (p->operando[j] != NULL) {
                if (!strcmp(p->operando[j]->elemento, "decvar"))
                    break;
                j++;
            }
            if (p->operando[j] != NULL) {
                nomeVariavel[0] = '_';
                nomeVariavel[1] = 'a';
                nomeVariavel[2] = '\0';
                sprintf(numVariavel,"%d",geral);
                strcat(nomeVariavel, numVariavel);
                renomear(&p, j,p->operando[j]->operando[0]->elemento,nomeVariavel); 
                geral++;
                j++;
            }
        }
    }
    else if (!strcmp(p->elemento, "decfunc")){
        
        while (p->operando[1]->operando[j] != NULL) {
            nomeVariavel[0] = '_';
            nomeVariavel[1] = 'a';
            nomeVariavel[2] = '\0';
            sprintf(numVariavel,"%d",geral);
            strcat(nomeVariavel, numVariavel);
            renomear(&p, 1, p->operando[1]->operando[j]->elemento,nomeVariavel);
            geral++;
            j++;
        }
    }
    
    return;
    
}

void procurarDeclaracoes(struct nodo* raiz){
    
    if (raiz == NULL)
        return;
    
    int i = 0;
    
    if (!strcmp(raiz->elemento, "decvar")){
        fprintf(saida, "\t%s:       .word   0\n", raiz->operando[0]->elemento); 
        
    }
    else {
        while (raiz->operando[i] != NULL) {
            procurarDeclaracoes(raiz->operando[i]);
            i++;
        }   
    }
    
    return;
}

void tratarExp (struct nodo* raiz){
    
    int i;
    
    for (i = 0; i < 13 ; i++){
        if (!strcmp(raiz->elemento, operadores[i])){
          break;
        }
    }
    
    switch (i){
            
            case 0:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\taddu $a0, $t0, $t1\n");
                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 1:
            
                tratarExp (raiz->operando[0]);
                if (raiz->operando[1] != NULL){
                
                    tratarExp (raiz->operando[1]);
                    
                    fprintf(saida, "\tlw $t1, 4($sp)\n");
                    fprintf(saida, "\taddiu $sp, $sp, 4\n");
                    
                    fprintf(saida, "\tlw $t0, 4($sp)\n");
                    fprintf(saida, "\taddiu $sp, $sp, 4\n");
                    
                    fprintf(saida, "\tsubu $a0, $t0, $t1\n");
                    
                    fprintf(saida, "\tsw $a0, 0($sp)\n");
                    fprintf(saida, "\taddiu $sp, $sp, -4\n");
                
                }
                else {
                    
                    fprintf(saida, "\tlw $t0, 4($sp)\n");
                    fprintf(saida, "\taddiu $sp, $sp, 4\n");
                    
                    fprintf(saida, "\tnegu $a0, $t0\n");
                    
                    fprintf(saida, "\tsw $a0, 0($sp)\n");
                    fprintf(saida, "\taddiu $sp, $sp, -4\n");
                }
                
                break;

            case 2:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tmult $t0, $t1\n");
                fprintf(saida, "\tmflo $a0\n");
                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 3:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tdiv $t0, $t1\n");
                fprintf(saida, "\tmflo $a0\n");
                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 4:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tslt $a0, $t0, $t1\n");
                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 5:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tslt $t2, $t0, $t1\n");
                fprintf(saida, "\tbeq $t0, $t1, verdadeiro%d\n",rotulo);
                
                fprintf(saida, "\tori $t3, $0, 0\n");
                fprintf(saida, "\tb end%d\n", rotulo);
                
                fprintf(saida, "\tverdadeiro%d:\n", rotulo);
                fprintf(saida, "\tori $t3, $0, 1\n");
                fprintf(saida, "\tend%d:\n", rotulo);
                
                fprintf(saida, "\tor $a0, $t2, $t3\n");
                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                rotulo++;
                
                break;

            case 6:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tslt $a0, $t1, $t0\n");
                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                
                break;

            case 7:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tslt $t2, $t1, $t0\n");
                fprintf(saida, "\tbeq $t0, $t1, verdadeiro%d\n",rotulo);
                
                fprintf(saida, "\tori $t3, $0, 0\n");
                fprintf(saida, "\tb end%d\n", rotulo);
                
                fprintf(saida, "\tverdadeiro%d:\n", rotulo);
                fprintf(saida, "\tori $t3, $0, 1\n");
                fprintf(saida, "\tend%d:\n", rotulo);
                
                fprintf(saida, "\tor $a0, $t2, $t3\n");
                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                rotulo++;
                
                break;  
                
            case 8:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tbeq $t0, $t1, verdadeiro%d\n",rotulo);
                
                fprintf(saida, "\tori $a0, $0, 0\n");
                fprintf(saida, "\tb end%d\n", rotulo);
                
                fprintf(saida, "\tverdadeiro%d:\n", rotulo);
                fprintf(saida, "\tori $a0, $0, 1\n");
                fprintf(saida, "\tend%d:\n", rotulo);
                                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                rotulo++;
                
                break;  
                
            case 9:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tbeq $t0, $t1, falso%d\n",rotulo);
                
                fprintf(saida, "\tori $a0, $0, 1\n");
                fprintf(saida, "\tb end%d\n", rotulo);
                
                fprintf(saida, "\tfalso%d:\n", rotulo);
                fprintf(saida, "\tori $a0, $0, 0\n");
                fprintf(saida, "\tend%d:\n", rotulo);
                                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                rotulo++;
                
                break;  
                
            case 10:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tbeq $t0, $0, falso%d\n",rotulo);
                fprintf(saida, "\tbeq $t1, $0, falso%d\n",rotulo);
                
                fprintf(saida, "\tori $a0, $0, 1\n");
                fprintf(saida, "\tb end%d\n", rotulo);
                
                fprintf(saida, "\tfalso%d:\n", rotulo);
                fprintf(saida, "\tori $a0, $0, 0\n");
                fprintf(saida, "\tend%d:\n", rotulo);
                                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                rotulo++;
                
                break;  
                
            case 11:
            
                tratarExp (raiz->operando[0]);
                tratarExp (raiz->operando[1]);
                
                fprintf(saida, "\tlw $t1, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tbne $t0, $0, verdadeiro%d\n",rotulo);
                fprintf(saida, "\tbne $t1, $0, verdadeiro%d\n",rotulo);
                
                fprintf(saida, "\tori $a0, $0, 0\n");
                fprintf(saida, "\tb end%d\n", rotulo);
                
                fprintf(saida, "\tverdadeiro%d:\n", rotulo);
                fprintf(saida, "\tori $a0, $0, 1\n");
                fprintf(saida, "\tend%d:\n", rotulo);
                                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                rotulo++;
                
                break;  
                
            case 12:
            
                tratarExp (raiz->operando[0]);
                
                fprintf(saida, "\tlw $t0, 4($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, 4\n");
                
                fprintf(saida, "\tbne $t0, $0, falso%d\n",rotulo);
                
                fprintf(saida, "\tori $a0, $0, 1\n");
                fprintf(saida, "\tb end%d\n", rotulo);
                
                fprintf(saida, "\tfalso%d:\n", rotulo);
                fprintf(saida, "\tori $a0, $0, 0\n");
                fprintf(saida, "\tend%d:\n", rotulo);
                                
                fprintf(saida, "\tsw $a0, 0($sp)\n");
                fprintf(saida, "\taddiu $sp, $sp, -4\n");
                rotulo++;
                
                break;  

            default:
            
                if (!strcmp(raiz->elemento, "funccall")){
                    tratarFunccall(raiz);
                    fprintf(saida, "\tsw $v0, 0($sp)\n");
                    fprintf(saida, "\taddiu $sp, $sp, -4\n");
                }
                else{
                    int tamanho = strlen(raiz->elemento);
                    for (i = 0; i < tamanho; i++){
                        if (!isdigit(raiz->elemento[i]))
                            break;
                    }
                    if (i == tamanho){
                        fprintf(saida, "\tli $a0, %s\n",raiz->elemento);
                        fprintf(saida, "\tsw $a0, 0($sp)\n");
                        fprintf(saida, "\taddiu $sp, $sp, -4\n");
                    }
                    else{
                        i = 0;
                        while (parametros->operando[i] != NULL){
                            if (!strcmp(parametros->operando[i]->elemento,raiz->elemento)){
                                break;
                            }
                            i++;
                        }
                        if (parametros->operando[i] != NULL){
                            fprintf(saida, "\tlw $a0, %d($fp)\n",4+i*4);
                        }
                        else {
                            fprintf(saida, "\tlw $a0, %s\n",raiz->elemento);    
                        }
                        fprintf(saida, "\tsw $a0, 0($sp)\n");
                        fprintf(saida, "\taddiu $sp, $sp, -4\n");
                    }
                    
                }
        }
        
        return; 
}

void tratarBreak (struct nodo* raiz){
    
    if (sairloop != -1){
        fprintf(saida, "\tb falsowhile%d\n", sairloop);         
    }
    
}

void tratarContinue (struct nodo* raiz){
    
    if (sairloop != -1){
        fprintf(saida, "\tb verdadeiro%d\n", sairloop);         
    }
    
}

void tratarAssign (struct nodo* raiz){
    
    int i = 0;

    if (raiz->operando[1] != NULL){

        tratarExp(raiz->operando[1]);
    
        fprintf(saida, "\tlw $a0, 4($sp)\n");
        
        while (parametros->operando[i] != NULL){
            if (!strcmp(parametros->operando[i]->elemento,raiz->operando[0]->elemento)){
                break;
            }
            i++;
        }


        if (parametros->operando[i] != NULL){
            fprintf(saida, "\tsw $a0, %d($fp)\n",4+i*4);
        }
        else {
            fprintf(saida, "\tsw $a0, %s\n",raiz->operando[0]->elemento );  
        }

        fprintf(saida, "\taddiu $sp, $sp, 4\n");
    }
    
    return;
    
}

void tratarFunccall (struct nodo* raiz){
    
    int i = 0;
    int j = 0;
    
    while (raiz->operando[1]->operando[i] != NULL){
        i++;
    }

    for (j = i - 1; j >= 0; j--){
        tratarExp(raiz->operando[1]->operando[j]);
    }
    
    if (!strcmp(raiz->operando[0]->elemento,"print")) {
        fprintf(saida, "\tjal _f_print\n");
    }
    else {

        fprintf(saida, "\tsw $t4, 0($sp)\n");
        fprintf(saida, "\tmove $fp, $sp\n");
        fprintf(saida, "\taddiu $sp, $sp, -4\n");
        
        fprintf(saida, "\tjal _f_%s\n", raiz->operando[0]->elemento);
        
        fprintf(saida, "\tlw $t4, 0($fp)\n");
        fprintf(saida, "\taddiu $sp, $fp, %d\n", 4*i);
        fprintf(saida, "\tlw $fp, 0($t4)\n");
    
    }
    
    return;
    
}

void tratarIf (struct nodo* raiz){
    
    int contador = rotulo;
    rotulo ++;
    
    tratarExp (raiz->operando[0]);
    
    fprintf(saida, "\tlw $a0, 4($sp)\n");
    fprintf(saida, "\taddiu $sp, $sp, 4\n");
    fprintf(saida, "\tbeqz $a0, falso%d\n", contador);  
    
    tratarBloco (raiz->operando[1]);
    
    fprintf(saida, "\tb end%d\n", contador);
    fprintf(saida, "\tfalso%d:\n", contador);
    
    if (raiz->operando[2] != NULL){
        
        tratarBloco(raiz->operando[2]);
        
    }
    
    fprintf(saida, "\tend%d:\n", contador);
    
    return;
}

void tratarWhile (struct nodo* raiz){
    
    int antigoSair = sairloop; 
    int contador = rotulo;
    sairloop = contador;
    rotulo ++;
        
    fprintf(saida, "\tverdadeiro%d:\n",contador);
    
    tratarExp (raiz->operando[0]);
    
    fprintf(saida, "\tlw $a0, 4($sp)\n");
    fprintf(saida, "\taddiu $sp, $sp, 4\n");
    fprintf(saida, "\tbeqz $a0, falsowhile%d\n", contador); 
    
    tratarBloco (raiz->operando[1]);
    
    sairloop = antigoSair;
    fprintf(saida, "\tb verdadeiro%d\n", contador);
    fprintf(saida, "\tfalsowhile%d:\n", contador);
    
    return;
    
}

void tratarReturn (struct nodo* raiz){
    
    if (raiz->operando[0] != NULL){
        tratarExp (raiz->operando[0]);
        fprintf(saida, "\tlw $v0, 4($sp)\n");
    }
    else {
        fprintf(saida, "\tli $v0, 0\n");
    }

    fprintf(saida, "\tlw $ra, 4($t4)\n");
    fprintf(saida, "\tjr $ra\n");

    return; 
}

void tratarBloco(struct nodo* raiz) {
    
    int i, j;
    
    j = 0;
    
    while (raiz->operando[j] != NULL){
    
        for (i = 0; i < 8 ; i++){
            if (!strcmp(raiz->operando[j]->elemento, bloco[i])){
              break;
            }
        }
                
        switch (i){
            
            case 0:
                
            case 1:
                tratarAssign(raiz->operando[j]);
                break;

            case 2:
                tratarFunccall(raiz->operando[j]);
                break;

            case 3:
                tratarIf(raiz->operando[j]);
                break;

            case 4:
                tratarWhile(raiz->operando[j]);
                break;

            case 5:
                tratarReturn(raiz->operando[j]);
                break;

            case 6:
                tratarBreak(raiz->operando[j]);
                break;

            case 7:
                tratarContinue (raiz->operando[j]);
                break;  

            default:
                fprintf(saida, "Erro na arvore. GeradorCodigo");
                exit(1);
        }
        j++;
    
    }
    
    return;
}

int geradorFuncao (struct nodo* raiz){
    
    if (raiz == NULL){
        fprintf(saida, "Erro na arvore. GeradorCodigo");
        exit(1);
    }
    
    fprintf(saida, ".data\n");

    procurarDeclaracoes(raiz);
        
    fprintf(saida, "\n");
    fprintf(saida, ".text\n");
    fprintf(saida, "\n");
    
    fprintf(saida, "_f_print:\n");
    fprintf(saida, "\tlw $a0, 4($sp)\n");
    fprintf(saida, "\taddiu $sp, $sp, 4\n");
    fprintf(saida, "\tli $v0, 1\n");
    fprintf(saida, "\tsyscall\n");
    fprintf(saida, "\tli $v0, 11\n");
    fprintf(saida, "\tli $a0, 0x0a\n");
    fprintf(saida, "\tsyscall\n");
    fprintf(saida, "\tjr $ra\n");

    int aux;
    int i,j;
    
    struct nodo* funcao;
    
    i = 0;
        
    while (raiz->operando[i] != NULL) {
        
        funcao = raiz->operando[i];
        j = 0;
        aux=-4;
                
        if (!strcmp(funcao->elemento, "decfunc")){
            
            if (funcao->operando[0] == NULL){
                fprintf(saida, "Erro na arvore. GeradorCodigo");
                exit(1);
            }
            fprintf(saida, "\n");
            fprintf(saida, "_f_%s:\n", funcao->operando[0]->elemento);  
            fprintf(saida, "\tsw $ra, 0($sp)\n");
            fprintf(saida, "\taddiu $sp, $sp, -4\n");

            if (funcao->operando[1] == NULL){
                fprintf(saida, "Erro na arvore. GeradorCodigo");
                exit(1);
            } 
            
            parametros = funcao->operando[1];

            fprintf(saida, "\tsw $fp, 0($sp)\n");
            fprintf(saida, "\tmove $t4, $sp\n");
            fprintf(saida, "\taddiu $sp, $sp, -4\n");
            
            if (funcao->operando[2] == NULL){
                fprintf(saida, "Erro na arvore. GeradorCodigo");
                exit(1);
            }

            tratarBloco(funcao->operando[2]);
            
            fprintf(saida, "\tli $v0, 0\n");
            fprintf(saida, "\tlw $ra, 4($t4)\n");
            fprintf(saida, "\tjr $ra\n");   
        }
        i++;
        
    }
    
    fprintf(saida, "\n");
    fprintf(saida, "main:\n");
    
    i = 0;

    while (raiz->operando[i] != NULL) {
        funcao = raiz->operando[i];
        if (!strcmp(funcao->elemento, "decvar")) {
            tratarAssign(funcao);
        }
        i++;
    }
    
    fprintf(saida, "\tjal _f_main\n");
    fprintf(saida, "\tli $v0, 10\n");
    fprintf(saida, "\tsyscall\n");
    
}

/* teste (struct nodo* raiz) {
    
    int i = 0;
    fprintf (saida, "[");
    fprintf (saida, raiz->elemento);
    
    while (raiz->operando[i] != NULL) {
        teste(raiz->operando[i]);
        i++;
    }
    
    fprintf (saida, "]");
    
} */

// int main () {
    
//     struct nodo* raiz = calloc (1, sizeof(struct nodo));
    
//     int tarvore;
    
//     if ((saida=fopen("arvoreSintatica","r"))==NULL){
//         fprintf(stderr, "Arquivo não pode ser aberto\n");
//         exit(1);  
//     }
        
//     fseek(saida, 0, SEEK_END);
//     tarvore=ftell(saida);
//     fseek(saida, 0, SEEK_SET);
                
//     char* arvore = (char*)calloc(tarvore+1, sizeof(char));
//     fread(arvore, 1, tarvore, saida);

//     fclose(saida);
    
//     posicao = 0;
        
//     gerarEstrutura (arvore, &raiz);
//     corrigirArvore(&raiz);
        
//     if ((saida=fopen("saida.asm","w"))==NULL){
//          fprintf(stderr, "Arquivo não pode ser aberto\n");
//          exit(1);  
//     }   
    
//     geradorFuncao(raiz);
//     fclose(saida);  

//     return 0;
// }
