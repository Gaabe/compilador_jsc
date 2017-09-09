#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale>

#define Nodes 200

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define LT 4
#define LET 5
#define GT 6
#define GET 7
#define EQ 8
#define NEQ 9
#define AND 10
#define OR 11
#define NOT 12

#define DECVAR 13
#define ASSIGN 14
#define FUNCCALL 15
#define IF 16
#define WHILE 17
#define RETURN 18
#define BREAK 19
#define CONTINUE 20



FILE *out;


struct node{
    char* oper;
    struct node *ops[Nodes];
};

struct node* params;
int leave = -1, position, global = 0, label = 0;
void genblock (struct node* root);
void genstruct (char* tree, struct node** root);
void globlaVariableRename (struct node** root, char* variable, char* newVariable, int n);
void newTree (struct node** root);
void declarations(struct node* root);
void pop();
void push();
void codegen_add();
void codegen_sub(struct node* op);
void codegen_mul();
void codegen_div();
void codegen_lt();
void codegen_let();
void codegen_gt();
void codegen_get();
void codegen_eq();
void codegen_neq();
void codegen_and();
void codegen_or();
void codegen_not();
int keyfromstring(char *key);
void codegen_exp (struct node* root);
void codegen_break (struct node* root);
void codegen_continue (struct node* root);
void codegen_assign (struct node* root);
void codegen_funccall (struct node* root);
void condegen_if (struct node* root);
void codegen_while (struct node* root);
void codegen_return (struct node* root);
void genblock(struct node* root);
int codegen_function (struct node* root);


void pop(){
    fprintf(out, "\taddiu $sp, $sp, 4\n");
}

void push(){
    fprintf(out, "\tsw $a0, 0($sp)\n"
    			 "\taddiu $sp, $sp, -4\n");
}

void codegen_add(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tsubu $a0, $t0, $t1\n");
    push();
}

void codegen_sub(struct node* op){
	if (op->ops[1] == NULL){
        fprintf(out, "\tlw $t0, 4($sp)\n");
        pop();
        fprintf(out, "\tnegu $a0, $t0\n");
        push();  
    }else {
        codegen_exp (op->ops[1]);
        fprintf(out, "\tlw $t1, 4($sp)\n");
        pop();
        fprintf(out, "\tlw $t0, 4($sp)\n");
        pop();
        fprintf(out, "\tsubu $a0, $t0, $t1\n");
        push();
    }
}

void codegen_mul(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tmult $t0, $t1\n"
    			 "\tmflo $a0\n");
    push();
}

void codegen_div(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tdiv $t0, $t1\n"
                  "\tmflo $a0\n");
    push();
}

void codegen_lt(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tslt $a0, $t0, $t1\n");
    push();
}

void codegen_gt(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tslt $a0, $t1, $t0\n");
    push();
}


void codegen_eq(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tbeq $t0, $t1, true%d\n"
    			 "\tori $a0, $0, 0\n"
    			 "\tb end%d\n"
                 "\ttrue%d:\n"
                 "\tori $a0, $0, 1\n"
                 "\tend%d:\n", label,label,label,label);
    push();
}

void codegen_neq(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tbeq $t0, $t1, false%d\n"
                 "\tori $a0, $0, 1\n"
                 "\tb end%d\n"
                 "\false%d:\n"
                 "\tori $a0, $0, 0\n"
                 "\tend%d:\n", label,label,label,label);
    push();
}

void codegen_let(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tslt $t2, $t0, $t1\n"
                 "\tbeq $t0, $t1, true%d\n"
                 "\tori $t3, $0, 0\n"
                 "\tb end%d\n",
                 "\ttrue%d:\n", 
                 "\tori $t3, $0, 1\n"
                 "\tend%d:\n", 
                 "\tor $a0, $t2, $t3\n",label,label,label,label);
    push();
}


void codegen_get(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tslt $t2, $t1, $t0\n"
                 "\tbeq $t0, $t1, true%d\n"
                 "\tori $t3, $0, 0\n"
                 "\tb end%d\n"
                 "\ttrue%d:\n"
                 "\tori $t3, $0, 1\n"
                 "\tend%d:\n"
                 "\tor $a0, $t2, $t3\n",label,label,label,label);
    push();
}



void codegen_and(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tbeq $t0, $0, false%d\n"
                 "\tbeq $t1, $0, false%d\n"
                 "\tori $a0, $0, 1\n"
                 "\tb end%d\n", 
                 "\false%d:\n", 
                 "\tori $a0, $0, 0\n"
                 "\tend%d:\n", label,label,label,label,label);
    push();
}

void codegen_or(){
    fprintf(out, "\tlw $t1, 4($sp)\n");
    pop();
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tbne $t0, $0, true%d\n"
                 "\tbne $t1, $0, true%d\n"
                 "\tori $a0, $0, 0\n"
                 "\tb end%d\n"
                 "\ttrue%d:\n"
                 "\tori $a0, $0, 1\n"
                 "\tend%d:\n", label,label,label,label,label);
    push();
}

void codegen_not(){
    fprintf(out, "\tlw $t0, 4($sp)\n");
    pop();
    fprintf(out, "\tbne $t0, $0, false%d\n"
                 "\tori $a0, $0, 1\n"
                 "\tb end%d\n"
                 "\false%d:\n"
                 "\tori $a0, $0, 0\n"
                 "\tend%d:\n", label,label,label,label);
    
    push();
}


void declarations(struct node* root){
    
    if (root != NULL){
        
        if (strcmp(root->oper, "decvar")==0){
            fprintf(out, "\t%s:		.word	0\n", root->ops[0]->oper);
            
        }else{
        	int i = 0;
            while (root->ops[i] != NULL) {
                declarations(root->ops[i]);
                i++;
            }
        }
        
        return;
        
    }else{
        return;
    }
}

void genstruct (char* tree, struct node** root) {
    
    struct node* auxRoot = (*root);
    
    char string[256];
    strcpy(string, "\0");
    char aux[2];
   
    int i,isBeginning = 0;
    i =0;
    
    while (1) {
        if (tree[position] == '['){
            if (isBeginning){
                auxRoot->ops[i] = (node*) calloc(1, sizeof(struct node));
                genstruct(tree, &auxRoot->ops[i]);
                i++;
            }
            else {
                isBeginning = 1;
                position++;
            }
        }
        else if (tree[position] == ']'){
            position++;
            auxRoot->oper = strdup(string);
            return;
        }
        else if (tree[position] == ' ' || tree[position] == '\t' || tree[position] == '\n' || tree[position] == '\v' || tree[position] == '\f' || tree[position] == '\r')
            position++;
        else {
            aux[0] = tree[position];
            aux[1] = '\0';
            strcat(string, aux);
            position++;
        }
    }
    
}

void globlaVariableRename (struct node** root, char* variable, char* newVariable, int n){
    
    struct node* auxRoot = (*root);
    
    while (auxRoot->ops[n]!=NULL) {
        globlaVariableRename(&auxRoot->ops[n], variable, newVariable,0);
        n ++;
    }
    
    if (strcmp(auxRoot->oper, variable)==0)
        auxRoot->oper = strdup(newVariable);
    
}

void newTree (struct node** root) {
    
    struct node* auxRoot2 = (*root);
    
  	if (auxRoot2 == NULL){
        return;
  	}
 
    int i = 0,j;
   
    while (auxRoot2->ops[i] != NULL) {
        i++;
    }
    
    char variavles[255],names[256];
    for (j = i-1; j >= 0; j--) {
        newTree(&auxRoot2->ops[j]);
    }
    
    j = 0;
    
    if (strcmp(auxRoot2->oper, "block")==0){
        while(auxRoot2->ops[j] != NULL){
            while (auxRoot2->ops[j] != NULL) {
                if (!strcmp(auxRoot2->ops[j]->oper, "decvar")){
                    break;
                }
                j++;
            }
            if (auxRoot2->ops[j] != NULL) {
                names[0] = '_';
                names[1] = 'a';
                names[2] = '\0';
                sprintf(variavles,"%d",global);
                strcat(names, variavles);
                globlaVariableRename(&auxRoot2,auxRoot2->ops[j]->ops[0]->oper,names, j);
                global++;
                j++;
            }
        }
    }
    else if (strcmp(auxRoot2->oper, "decfunc")==0){
        
	        while (auxRoot2->ops[1]->ops[j] != NULL) {
	            names[0] = '_';
	            names[1] = 'a';
	            names[2] = '\0';
	            sprintf(variavles,"%d",global);
	            strcat(names, variavles);
	            globlaVariableRename(&auxRoot2, auxRoot2->ops[1]->ops[j]->oper,names, 1);
	            global++;
	            j++;
	        }
    }
    
    return;
    
}

void codegen_break (struct node* root){
    if (leave != -1){
        fprintf(out, "\tb falsewhile%d\n", leave);
    }
}

void codegen_continue (struct node* root){
    if (leave != -1){
        fprintf(out, "\tb true%d\n", leave);
    }
}

void codegen_assign (struct node* root){
    
    int i = 0;
    if (root->ops[1] != NULL){
        codegen_exp(root->ops[1]);
        
        fprintf(out, "\tlw $a0, 4($sp)\n");
        while (params->ops[i] != NULL){
            if (!strcmp(params->ops[i]->oper,root->ops[0]->oper)){
                break;
            }
            i++;
        }
        
        
        if (params->ops[i] == NULL){
        	fprintf(out, "\tsw $a0, %s\n",root->ops[0]->oper );

        }
        else {
        	fprintf(out, "\tsw $a0, %d($fp)\n",4+i*4);
        }
        
        pop();
    }
    
    return;
    
}

void condegen_if (struct node* root){
    
    int contador = label;
    label ++;
    codegen_exp (root->ops[0]);
    fprintf(out, "\tlw $a0, 4($sp)\n");
    pop();
    fprintf(out, "\tbeqz $a0, false%d\n", contador);
    genblock (root->ops[1]);
    fprintf(out, "\tb end%d\n"
    			 "\false%d:\n", contador,contador);
    
    if (root->ops[2] != NULL){
        genblock(root->ops[2]);
    }
    
    fprintf(out, "\tend%d:\n", contador);
    return;
}

void codegen_while (struct node* root){
        int aux = label;

    int quit = leave;
    label ++;
        leave = aux;

    fprintf(out, "\ttrue%d:\n",aux);
    codegen_exp (root->ops[0]);
    fprintf(out, "\tlw $a0, 4($sp)\n");
    pop();
    fprintf(out, "\tbeqz $a0, falsewhile%d\n", aux);
    genblock (root->ops[1]);
    fprintf(out, "\tb true%d\n"
    			 "\tfalsewhile%d:\n", aux, aux);
    leave = quit;

    return;
    
}

void codegen_return (struct node* root){
    
    if (root->ops[0] == NULL){
        fprintf(out, "\tli $v0, 0\n");
    }
    else {
    	        codegen_exp (root->ops[0]);
        fprintf(out, "\tlw $v0, 4($sp)\n");
    }
    
    fprintf(out, "\tlw $ra, 4($t4)\n"
    			 "\tjr $ra\n");
    
    return;
}



int keyfromstring(char *key)
{
    
    if (strcmp("+", key) == 0)
        return ADD;
    
    if (strcmp("-", key) == 0)
        return SUB;
    
    if (strcmp("*", key) == 0)
        return MUL;
    
    if (strcmp("/", key) == 0)
        return DIV;
    
    if (strcmp("<", key) == 0)
        return LT;
    
    if (strcmp("<=", key) == 0)
        return LET;
    
    if (strcmp(">", key) == 0)
        return GT;
    
    if (strcmp(">=", key) == 0)
        return GET;
    
    if (strcmp("==", key) == 0)
        return EQ;
    
    if (strcmp("!=", key) == 0)
        return NEQ;
    
    
    if (strcmp("&&", key) == 0)
        return AND;
    
    if (strcmp("||", key) == 0)
        return OR;
    
    if (strcmp("!", key) == 0)
        return NOT;
    
    if (strcmp("decvar", key) == 0)
        return DECVAR;
    
    if (strcmp("assign", key) == 0)
        return ASSIGN;
    
    if (strcmp("funccall", key) == 0)
        return FUNCCALL;
    
    if (strcmp("if", key) == 0)
        return IF;
    
    if (strcmp("while", key) == 0)
        return WHILE;
    
    if (strcmp("return", key) == 0)
        return RETURN;
    
    if (strcmp("break", key) == 0)
        return BREAK;
    
    if (strcmp("continue", key) == 0)
        return CONTINUE;
    
 			return 21;
}



void codegen_exp (struct node* root){
    
    int pos=0;
    
    switch (keyfromstring(root->oper)){
            
        case 0:
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_add();
            
            break;
            
        case 1:
            
            codegen_exp (root->ops[0]);
            codegen_sub(root);
            break;
            
        case 2:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_mul();
            
            break;
            
        case 3:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_div();
            
            break;
            
        case 4:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_lt();
            
            break;
            
        case 5:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_let();
            
            label++;
            
            break;
            
        case 6:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_gt();
            
            
            break;
            
        case 7:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_get();
            
            label++;
            
            break;
            
        case 8:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_eq();
            
            label++;
            
            break;
            
        case 9:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_neq();
            
            label++;
            
            break;
            
        case 10:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_and();
            
            label++;
            
            break;
            
        case 11:
            
            codegen_exp (root->ops[0]);
            codegen_exp (root->ops[1]);
            codegen_or();
            
            label++;
            
            break;
            
        case 12:
            
            codegen_exp (root->ops[0]);
            codegen_not();
            
            label++;
            
            break;

        case 15:

            codegen_funccall(root);
            fprintf(out, "\tsw $v0, 0($sp)\n"
            			 "\taddiu $sp, $sp, -4\n");
            
            break;
            
        default:
                for (int i = 0; i < strlen(root->oper); i++){
                    if (isdigit(root->oper[i])==0){
                    	if (i != strlen(root->oper)){
	                    i = 0;
	                    while (params->ops[i] != NULL){
	                        if (strcmp(params->ops[i]->oper,root->oper)==0){
	                            break;
	                        }
	                        i++;
	                    }
	                    if (params->ops[i] != NULL){
	                        fprintf(out, "\tlw $a0, %d($fp)\n",4+i*4);
	                    }
	                    else {
	                        fprintf(out, "\tlw $a0, %s\n",root->oper);
	                    }
	                    push();

	                	}else{
	                    	fprintf(out, "\tli $a0, %s\n",root->oper);
	                    	push();
	                	}

						break;

                    }
                                  
                }          
            
    }
    
    return;
}


void codegen_funccall (struct node* root){
    
    int i = 0;
    int j = 0;
    
    while (root->ops[1]->ops[i] != NULL){
        i++;
    }
    
    for (j = i - 1; j >= 0; j--){
        codegen_exp(root->ops[1]->ops[j]);
    }
    
    if (strcmp(root->ops[0]->oper,"print")==1) {

            fprintf(out, "\tsw $t4, 0($sp)\n"
   						 "\tmove $fp, $sp\n"
    					 "\taddiu $sp, $sp, -4\n"  
    					 "\tjal _function_%s\n"   
    					 "\tlw $t4, 0($fp)\n"
    					 "\taddiu $sp, $fp, %d\n"
    					 "\tlw $fp, 0($t4)\n",root->ops[0]->oper,4*i);
        return;
    }
   
   fprintf(out, "\tjal _function_print\n");
   
    return;
    
}


void genblock(struct node* root) {
    
    int i = 0;
    
    while (root->ops[i] != NULL){
        
        
        switch (keyfromstring(root->ops[i]->oper)){
                
            case 13:
                
            case 14:
                codegen_assign(root->ops[i]);
                break;
                
            case 15:
                codegen_funccall(root->ops[i]);
                break;
                
            case 16:
                condegen_if(root->ops[i]);
                break;
                
            case 17:
                codegen_while(root->ops[i]);
                break;
                
            case 18:
                codegen_return(root->ops[i]);
                break;
                
            case 19:
                codegen_break(root->ops[i]);
                break;
                
            case 20:
                codegen_continue (root->ops[i]);
                break;	
                
            default:
                fprintf(out, "Error invalid code");
                exit(1);
        }
        i++;
        
    }
    
    return;
}

int codegen_function (struct node* root){
    
    if (root == NULL){
        fprintf(out, "Error on codegen_function");
        exit(1);
    }
    
    fprintf(out, ".data\n");
    declarations(root);
    fprintf(out, "\n.text\n\n"
    			 "_function_print:\n"
    			 "\tlw $a0, 4($sp)\n");
    pop();
    fprintf(out, "\tli $v0, 1\n"
    			 "\tsyscall\n"
    			 "\tli $v0, 11\n"
   			     "\tli $a0, 0x0a\n"
    			 "\tsyscall\n"
    			 "\tjr $ra\n");
    
    int i,j;
    
    struct node* function;
    
    i = 0;
    
    while (root->ops[i] != NULL) {
        
        function = root->ops[i];
        j = 0;
        
        if (strcmp(function->oper, "decfunc")==0){
            
            if (function->ops[0] == NULL){
                fprintf(out, "Error");
                exit(1);
            }
            fprintf(out, "\n"
           			 	 "_function_%s:\n" 
            		 	 "\tsw $ra, 0($sp)\n"
            		 	 "\taddiu $sp, $sp, -4\n",function->ops[0]->oper);
            
            if (function->ops[1] == NULL){
                fprintf(out, "Error");
                exit(1);
            } 
            
            params = function->ops[1];
            
            fprintf(out, "\tsw $fp, 0($sp)\n"
            		 	 "\tmove $t4, $sp\n"
            		 	 "\taddiu $sp, $sp, -4\n");
            
            if (function->ops[2] == NULL){
                fprintf(out, "Error");
                exit(1);
            }
            
            genblock(function->ops[2]);
            
            fprintf(out, "\tli $v0, 0\n"
            		 	 "\tlw $ra, 4($t4)\n"
            		 	 "\tjr $ra\n");	
        }
        i++;
        
    }
    
    fprintf(out, "\nmain:\n");
    
    i = 0;
    
    while (root->ops[i] != NULL) {
        function = root->ops[i];
        if (strcmp(function->oper, "decvar")==0) {
            codegen_assign(function);
        }
        i++;
    }
    
    fprintf(out, "\tjal _function_main\n"
    			 "\tli $v0, 10\n"
    			 "\tsyscall\n");
    
}

// int main(int argc, char *argv[]) {

//     if ((out=fopen(argv[1],"r"))==NULL){
//         fprintf(stderr, "Compiler error\n");
//         exit(1);  
//     }
    
//     struct node* root = (node*) calloc(1, sizeof(struct node));
//     int treeSize;
//     position = 0;

//     fseek(out, 0, SEEK_END);
//     treeSize =ftell(out);
//     fseek(out, 0, SEEK_SET);
//     char* tree = (char*)calloc(treeSize+1, sizeof(char));
//     fread(tree, 1, treeSize, out);
//     fclose(out);
//     genstruct (tree, &root);
//     newTree(&root);
    

//     if ((out=fopen(argv[2],"w"))==NULL){
//         fprintf(stderr, "Compiler error\n");
//         exit(1);  
//     }	
//     codegen_function(root);
//     fclose(out);	
    
//     return 0;
// }