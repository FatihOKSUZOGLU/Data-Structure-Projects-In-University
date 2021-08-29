#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <time.h>
struct Node3{ //struct for 3 child node
  char* name; 
  void *down1 ,*down2, *down3; 
};
struct Node2{ //struct for 2 child node
  char* name; 
  void *down1 ,*down2; 
};
struct Node1{ //struct for 1 child node
  char* name; 
  void * down1; 
};
struct Node0{ //struct for ekstra
    char* name; 
};
int randint(int n) { //returning random int number with modding n  
    return rand()%n;
}
struct Node3* newNode3(char* name){  //creation dynemically new node
    struct Node3* temp=(struct Node3*)malloc(sizeof(struct Node3)); 
    temp->name = name; 
    temp->down1 = temp->down2 =temp->down3 = NULL; 
    return temp;
}
struct Node2* newNode2(char* name){  //creation dynemically new node
    struct Node2* temp=(struct Node2*)malloc(sizeof(struct Node2)); 
    temp->name = name;
    temp->down1 = temp->down2  = NULL; 
    return temp; 
}
struct Node1* newNode1(char* name){  //creation dynemically new node
    struct Node1* temp=(struct Node1*)malloc(sizeof(struct Node1)); 
    temp->name = name;
    temp->down1  = NULL; 
    return temp;
}
struct Node0* newNode0(char* name){  //creation dynemically new node
    struct Node0* temp=(struct Node0*)malloc(sizeof(struct Node0)); 
    temp->name = name;
    return temp; 
}
char * returnerindexinchar(char* ch){ //returning given index op in given op file 
    int i=0;
    char *arr[10];
    char *probline1 = strtok(ch,"\r\n");
    while (probline1!=NULL){ 
        arr[i]=probline1;           
        probline1 = strtok(NULL,"\r\n");
        i++;
    }   
    int raint=randint(i);
    return arr[raint] ;
}
void Createleaf(void **child,char*name,char*op_out,char*pre_out,char*rel_out,char*set_out,char*var_out){ //Creatin Tree with recursively and returning node.
    if (strcmp(name,"op_out") == 0){
        struct Node1* temp=newNode1(returnerindexinchar(op_out));
        (*child)=temp;
        return ;
    }else if (strcmp(name,"pre_out") == 0){
        struct Node1* temp=newNode1(returnerindexinchar(pre_out));
        (*child)=temp;
        return ;
    }else if (strcmp(name,"rel_out") == 0){
        struct Node1* temp=newNode1(returnerindexinchar(rel_out));
        (*child)=temp;
        return ;
    }else if (strcmp(name,"set_out") == 0){
        struct Node1* temp=newNode1(returnerindexinchar(set_out));
        (*child)=temp;
        return ;
    }else if (strcmp(name,"var_out") == 0){
        struct Node1* temp=newNode1(returnerindexinchar(var_out));
        (*child)=temp;
        return ;
    }else if (strcmp(name,"op") == 0){
        struct Node1* temp=newNode1(name);
        Createleaf(&temp->down1,"op_out",op_out,pre_out,rel_out,set_out,var_out);
        (*child)=temp;
        return ;
    }else if (strcmp(name,"pre_op") == 0 ){
        struct Node1* temp=newNode1(name);
        Createleaf(&temp->down1,"pre_out",op_out,pre_out,rel_out,set_out,var_out);
        (*child)=temp;
        return ;
    }else if (strcmp(name,"rel_op") == 0){
        struct Node1* temp=newNode1(name);
        Createleaf(&temp->down1,"rel_out",op_out,pre_out,rel_out,set_out,var_out);
        (*child)=temp;
        return ;
    }else if (strcmp(name,"set_op") == 0){
        struct Node1* temp=newNode1(name);
        Createleaf(&temp->down1,"set_out",op_out,pre_out,rel_out,set_out,var_out);
        (*child)=temp;
        return ; 
    }else if (strcmp(name,"var") == 0){
        struct Node1* temp=newNode1(name);
        Createleaf(&temp->down1,"var_out",op_out,pre_out,rel_out,set_out,var_out);
        (*child)=temp;
        return ;
    }else if (strcmp(name,"cond") == 0){
        int rval=randint(2);
        struct Node3* temp=newNode3(name);     
        if (rval==0){
            Createleaf(&temp->down1,"cond",op_out,pre_out,rel_out,set_out,var_out);
            Createleaf(&temp->down2,"set_op",op_out,pre_out,rel_out,set_out,var_out);
            Createleaf(&temp->down3,"cond",op_out,pre_out,rel_out,set_out,var_out);
        }else{
            Createleaf(&temp->down1,"expr",op_out,pre_out,rel_out,set_out,var_out);
            Createleaf(&temp->down2,"rel_op",op_out,pre_out,rel_out,set_out,var_out);
            Createleaf(&temp->down3,"expr",op_out,pre_out,rel_out,set_out,var_out); 
        }        
        (*child)=temp;
        return ;       
    }else if (strcmp(name,"expr") == 0){
        int rval=randint(3);   
        if (rval==0){
            struct Node3* temp=newNode3(name); 
            Createleaf(&temp->down1,"expr",op_out,pre_out,rel_out,set_out,var_out);
            Createleaf(&temp->down2,"op",op_out,pre_out,rel_out,set_out,var_out);
            Createleaf(&temp->down3,"expr",op_out,pre_out,rel_out,set_out,var_out);  
            (*child)=temp;
        }else if (rval==1){
            struct Node2* temp=newNode2(name);
            Createleaf(&temp->down1,"pre_op",op_out,pre_out,rel_out,set_out,var_out);
            Createleaf(&temp->down2,"expr",op_out,pre_out,rel_out,set_out,var_out);
            (*child)=temp;

        }else{
            struct Node1* temp=newNode1(name);
            Createleaf(&temp->down1,"var",op_out,pre_out,rel_out,set_out,var_out);            
            (*child)=temp;
        }
        return ;
    }else{
        return ;
    }
    return ;
}
void printLeafNodes(void *root){ //printing recursively leaf nodes  
    if (root==NULL){ 
        return;   
    }  
    struct Node3* temp=NULL;
    temp=root;
    if (strcmp(temp->name,"op") == 0){
        struct Node0 * a=NULL;
        a=temp->down1;        
        printLeafNodes(a);
        return ;
    }else if (strcmp(temp->name,"pre_op") == 0){
        struct Node0 * a=NULL;
        a=temp->down1;        
        printLeafNodes(a);
        return ;
    }else if (strcmp(temp->name,"rel_op") == 0){
        struct Node0 * a=NULL;
        a=temp->down1;        
        printLeafNodes(a);
        return ;
    }else if (strcmp(temp->name,"set_op") == 0){
          struct Node0 * a=NULL;
        a=temp->down1;        
        printLeafNodes(a);
        return ;
    }else if (strcmp(temp->name,"var") == 0){
        struct Node0 * a=NULL;
        a=temp->down1;        
        printLeafNodes(a);
        return ;
    }else if (strcmp(temp->name,"cond") == 0){
        printf("(");  
        struct Node3 *a=NULL;
        a=temp->down1;     
        printLeafNodes(a);
        struct Node3 *b=NULL;
        b=temp->down2; 
        printLeafNodes(b);
        struct Node3 *c=NULL;
        c=temp->down3; 
        printLeafNodes(c);
        printf(")");  
        return;       
    }else if (strcmp(temp->name,"expr") == 0){
        printf("(");  
        struct Node3 *a=NULL;
        a=temp->down1;     
        printLeafNodes(a);
        struct Node3 *b=NULL;
        b=temp->down2;         
        if(strcmp(a->name,"var") != 0){
            printLeafNodes(b);
        } else{
            printf(")");
            return;
        }
        struct Node3 *c=NULL;
        c=temp->down3;         
        if(strcmp(b->name,"op") == 0){
            printLeafNodes(c);
        } else{
            printf(")");
            return;
        }
        printf(")");
        return;
    }else {
        printf("%s",temp->name);
        return;
    }
    return;
}
int Run(char*op_out,char*pre_out,char*rel_out,char*set_out,char*var_out){    //like a main function creating a Main Node
    struct Node1* Root=NULL;
    Root=newNode1("alg.");
    Createleaf(&Root->down1,"cond",op_out,pre_out,rel_out,set_out,var_out);
    printf("if ");
    printLeafNodes(Root->down1);
    printf(" {}");
    return 0;
}
char *readingfile(FILE *f){ //Reading dynemically with realloc op File
	char *output = NULL;
    int CharInFile = 0;
    int c = 0;
    do {
        c = 0;
        c = fgetc (f);
        if (c != EOF) {
            output = (char *) realloc (output, (CharInFile + 2) * sizeof(char));
            output[CharInFile] = c;                     
        }
        CharInFile++;
    } while (c != EOF);      
    return output;
}
int main(int argc, char *argv[]){  
    FILE *op = fopen("op","r");
    FILE *pre_op = fopen("pre_op","r");
    FILE *rel_op = fopen("rel_op","r");   
    FILE *set_op = fopen("set_op","r");
    FILE *var = fopen("var","r");   
    char * op_out ;
    char * pre_out ;
    char * rel_out ;
    char * set_out ;
    char * var_out ;    
    op_out = readingfile(op);
    pre_out = readingfile(pre_op); 
    rel_out = readingfile(rel_op); 
    set_out = readingfile(set_op);
    var_out = readingfile(var);  
    srand(time(NULL));     
    Run(op_out,pre_out,rel_out,set_out,var_out); 
    fclose(op);
    fclose(pre_op); 
    fclose(rel_op);     
    fclose(set_op); 
    fclose(var);    
    return 0;
}