#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {//passenger struct 
    char *soldclass;
    char *wantedclass;
    char *flight;
    char *name;
    char *prior;
} passenger;
typedef struct {//Stack for Seat class
    int top; 
    unsigned capacity; 
    passenger*array; 
} Stack;  
typedef struct { //queue for ticket que
    int front;
    int bus,eco,std;
    unsigned capacity; 
    passenger *array; 
} Queue; 
typedef struct { //Flight struct for any info 
    char * flightname;
    int closed;
    Queue *fque;
    Stack *buss;
    Stack *eco;
    Stack *std;
} flight; 
int isFull(Stack* stack) { //chacking stack is full or not 

    return stack->top == stack->capacity - 1;
}
int isEmpty(Stack* stack){ //chacking stack is empty or not

    return stack->top != stack->capacity - 1; 
}
//finding passenger in passenger array getiing index
int indexpass(passenger*p,char* passname,int size){ 
    if(!p) return -1;
    int i;
    for (i=0; i < size; i++){
        if (!p[i].name) return -1;
        if (strcmp(p[i].name,passname)==0) return i;
    }
    return -1;
}
//finding flight if there is not exist create flight şn flight array
int addindexFlight(flight *f,char * flightname,int size){
    if(!f) return -1; 
    int i; 
    for (i = 0; i < size; i++){
        if (!f[i].flightname){
            f[i].flightname=flightname;
            f[i].closed=0;
            f[i].fque = (Queue*)malloc(sizeof(Queue));
            f[i].fque->capacity=0; 
            f[i].fque->bus=0;
            f[i].fque->eco=0;
            f[i].fque->std=0; 
            f[i].fque->front=0;      
            f[i].buss = (Stack*)malloc(sizeof(Stack));
            f[i].buss->capacity=0;
            f[i].buss->top=(-1);
            f[i].eco = (Stack*)malloc(sizeof(Stack)); 
            f[i].eco->capacity=0;
            f[i].eco->top=(-1);
            f[i].std = (Stack*)malloc(sizeof(Stack));
            f[i].std->capacity=0; 
            f[i].std->top=(-1);
            return i;
        }else if (strcmp(f[i].flightname,flightname)==0){
            return i;           
        }
    }
    return -1;
}
//If flight is exist , getting index.
int indexflight(flight *f,char * flightname,int size){
    if(!f) return -1; 
    int i;      
    for (i = 0; i < size; i++){
        if (!f[i].flightname)   return -1;
        else if (strcmp(f[i].flightname,flightname)==0){
            return i;           
        }
    }
    return -1;
}
//Arranges the numbers for each class in the queue.
int whicone(char*a ,flight*f,int fi){ 
    if (strcmp(a,"business") == 0 )  return f[fi].fque->bus;
    else if (strcmp(a,"economy") == 0 )  return f[fi].fque->eco;
    else    return f[fi].fque->std;
}
//Adding passenger in passenger array.
int addpassarray(passenger*p,char *flightname,char* wclass,char*soldclass,char *passname,char*priority,int size){
    if(!p) return -1; 
    int i;   
    for (i = 0; i < size; i++){
        if (!p[i].name){
            p[i].flight=flightname;
            p[i].name=passname;
            p[i].prior=priority;
            p[i].wantedclass=wclass;
            p[i].soldclass=soldclass;
            return i;
        }
        else if (strcmp(p[i].name,passname)==0){            
            return -1;           
        }
    }
    return -1;
}
//Painting the remaining people. 
int printque(flight *f,int fi,FILE *fp){
    int i;
    for (i = f[fi].fque->front; i < f[fi].fque->capacity; i++){
        fprintf(fp,"waiting %s\n",f[fi].fque->array[i].name ); 
    }
    return 0;
} 
//This is the swap function used when setting a priority queue.
int swap(passenger *a, passenger *b){  
    passenger temp;
    temp=*a;
    *a = *b;
    *b = temp;  
    return 0;
}
//This is the function that places the next ticket holder on the seat.
int push(Stack* stack, passenger item){ 
    if (isFull(stack))         
        return -1; 
    stack->array[stack->top+1] = item;
    stack->top+=1;   
    return 0; 
}
//Priority queue function. Arranges the arriving passengers according to their order of tickets.
int enqueue(flight*f,passenger*p,int fi,char*flightname,char*class,char*passname,char* prior,int normal,int size){ 
    int en = addpassarray(p,flightname,class,"none",passname,prior,size);
    if(en<0&&normal==1) return en;    
    if (strcmp(class,"business") == 0 )  f[fi].fque->bus++;
    else if (strcmp(class,"economy") == 0 )  f[fi].fque->eco++;
    else f[fi].fque->std++; 
    if (f[fi].fque->capacity==0&&normal==1){
        f[fi].fque->capacity+=1; 
        f[fi].fque->array = (passenger*)malloc(f[fi].fque->capacity * sizeof(passenger));
        f[fi].fque->array[f[fi].fque->capacity-1] = p[en];  
        return en;        
    }
    else if(f[fi].fque->capacity!=0&&normal==1){
        passenger a[f[fi].fque->capacity+1];        
        f[fi].fque->capacity+=1; 
        int j;
        for (j = 0; j < f[fi].fque->capacity-1; j++){                
            a[j]=f[fi].fque->array[j];
        }
        a[f[fi].fque->capacity-1]=p[en];
        int i;
        int cap=f[fi].fque->capacity-1;        
        for (i = cap ;i > f[fi].fque->front; i--){ 
            if (strcmp(a[i].wantedclass,"standard")==0) {
                break;
            }else if (strcmp(a[i].wantedclass,"economy")==0){  
                if(strcmp(a[i].prior,"npy")==0){                    
                    if (strcmp(a[i-1].wantedclass,"economy")==0||strcmp(a[i-1].wantedclass,"business")==0)  break;
                }else if (strcmp(a[i].prior,"veteran")==0){
                    if (strcmp(a[i-1].prior,"veteran")==0||strcmp(a[i-1].wantedclass,"business")==0)    break;
                }
                swap(&a[i-1],&a[i]);                 
            }else if (strcmp(a[i].wantedclass,"business")==0){ 
                if(strcmp(a[i].prior,"npy")==0){
                    if (strcmp(a[i-1].wantedclass,"business")==0)  break;
                }else if (strcmp(a[i].prior,"diplomat")==0){
                    if (strcmp(a[i-1].prior,"diplomat")==0)    break;
                }                
                swap(&a[i-1],&a[i]);
            }            
        }
        free(f[fi].fque->array);           
        f[fi].fque->array = (passenger*)malloc(f[fi].fque->capacity * sizeof(passenger));
        int k;
        for (k= 0; k < f[fi].fque->capacity; k++){                
            f[fi].fque->array[k]=a[k];
        }         
        return en;   
    }else {
        int ip= indexpass(p,passname,size);
        passenger a[f[fi].fque->capacity+1];        
        f[fi].fque->capacity+=1; 
        int i;
        for (i = 0; i < f[fi].fque->capacity-1; i++){                
            a[i]=f[fi].fque->array[i];
        }
        a[f[fi].fque->capacity-1]=p[ip];     
        free(f[fi].fque->array);           
        f[fi].fque->array = (passenger*)malloc(f[fi].fque->capacity * sizeof(passenger));
        for (i = 0; i < f[fi].fque->capacity; i++){                
            f[fi].fque->array[i]=a[i];
        }
        return ip;
    }
}
/* If there is a seat on the flight, it will throw the next one into the seat. 
	If the desired seat is not available, it sends to the back of the queue.*/
int sell(flight*f,passenger*p,int fi,char*flightname,int size){
    int stdcount;
    int c=f[fi].fque->capacity;
    while(f[fi].fque->front!=f[fi].fque->capacity){     
        passenger item = f[fi].fque->array[f[fi].fque->front];        
        int ip= indexpass(p,item.name,size);
        if (item.name==NULL) break;
        if(strcmp(item.wantedclass,"business")==0&&isFull(f[fi].buss)&&isEmpty(f[fi].std)){
            if(f[fi].fque->front>=c){
                p[ip].soldclass="standard"; 
                push(f[fi].std,item);
                f[fi].fque->front +=1;
                f[fi].fque->bus--;
                continue;
            }else {
                enqueue(f,p,fi,item.flight,item.wantedclass,item.name,"npy",0,size);                
                f[fi].fque->front +=1;
                f[fi].fque->bus--;
                continue;  
            }
        }else if(strcmp(item.wantedclass,"economy")==0&&isFull(f[fi].eco)&&isEmpty(f[fi].std)){
            if(f[fi].fque->front>=c){
                push(f[fi].std,item);
                p[ip].soldclass="standard";    
                f[fi].fque->front +=1;
                f[fi].fque->eco--;
                continue;
            }else {
                enqueue(f,p,fi,item.flight,item.wantedclass,item.name,"npy",0,size); 
                f[fi].fque->front +=1;
                f[fi].fque->eco--;
                continue;  
            }
        }else if(strcmp(item.wantedclass,"business")==0&&isEmpty(f[fi].buss)){
            push(f[fi].buss,item);
            p[ip].soldclass="business";              
            f[fi].fque->front +=1;
            f[fi].fque->bus-=1;
            continue;
        }else if(strcmp(item.wantedclass,"economy")==0&&isEmpty(f[fi].eco)){
            push(f[fi].eco,item); 
            p[ip].soldclass="economy";
            f[fi].fque->front +=1;
            f[fi].fque->eco-=1;
            continue;
        }else if(strcmp(item.wantedclass,"standard")==0&&isEmpty(f[fi].std)){
            push(f[fi].std,item); 
            p[ip].soldclass="standard";   
            f[fi].fque->front +=1;
            f[fi].fque->std-=1;
            continue;         
        }else if (isFull(f[fi].std)){
            break; 
        }else break;
    }
    return 0;
}
//It is the function that adds the desired seat to the flight.	
int addseat(int fi,flight *f,char*classname,int classize){ 
    if (strcmp(classname,"business")==0){
        if (f[fi].buss->capacity==0){
            f[fi].buss->capacity += classize; 
            f[fi].buss->array = (passenger*)malloc(f[fi].buss->capacity * sizeof(passenger)); 
        }
        else{
            passenger a[f[fi].buss->top+1] ;
            f[fi].buss->capacity += classize;
            int i;
            for (i = 0; i < f[fi].buss->top+1; i++){                
                a[i]=f[fi].buss->array[i];
            }
            free(f[fi].buss->array);           
            f[fi].buss->array = (passenger*)malloc(f[fi].buss->capacity * sizeof(passenger));
            for (i = 0; i < f[fi].buss->top+1; i++){                
               f[fi].buss->array[i]=a[i];
            }
        }           
    }
    else if(strcmp(classname,"economy")==0){
        if (f[fi].eco->capacity==0){
            f[fi].eco->capacity += classize; 
            f[fi].eco->array = (passenger*)malloc(f[fi].eco->capacity * sizeof(passenger)); 
        }
        else{
            passenger a[f[fi].eco->top+1] ;
            f[fi].eco->capacity += classize;
            int i;
            for (i = 0; i < f[fi].eco->top+1; i++){                
                a[i]=f[fi].eco->array[i];
            }
            free(f[fi].eco->array);
            f[fi].eco->array = (passenger*)malloc(f[fi].eco->capacity * sizeof(passenger));
            for (i = 0; i < f[fi].eco->top+1; i++){                
               f[fi].eco->array[i]=a[i];
            }
        }
    }  
    else if(strcmp(classname,"standard")==0){
        if (f[fi].std->capacity==0){
            f[fi].std->capacity += classize; 
            f[fi].std->array = (passenger*)malloc(f[fi].std->capacity * sizeof(passenger)); 
        }
        else{
            passenger a[f[fi].std->top+1] ;
            f[fi].std->capacity += classize;
            int i;
            for (i = 0; i < f[fi].std->top+1; i++){                
                a[i]=f[fi].std->array[i];
            }
            free(f[fi].std->array);
            f[fi].std->array = (passenger*)malloc(f[fi].std->capacity * sizeof(passenger));
            for (i = 0; i < f[fi].std->top+1; i++){                
               f[fi].std->array[i]=a[i];
            }
        }
    }
    return 0;
}
//Printing error if I need.
int error(FILE *fp){
    fprintf(fp,"%s\n", "error");  
    return 0;
}
//Printing Flight report. 
int report(flight*f,passenger*p,int fi,FILE *fp){
    fprintf(fp,"business %d\n",f[fi].buss->top+1);
    int i;
    for (i = 0; i < f[fi].buss->top+1; i++){
        fprintf(fp,"%s\n",f[fi].buss->array[i].name);
    }
    fprintf(fp,"economy %d\n",f[fi].eco->top+1);
    int j;
    for (j = 0; j < f[fi].eco->top+1; j++){
        fprintf(fp,"%s\n",f[fi].eco->array[j].name);
    }
    fprintf(fp,"standard %d\n",f[fi].std->top+1);
    int k;
    for (k = 0; k < f[fi].std->top+1; k++){
        fprintf(fp,"%s\n",f[fi].std->array[k].name);
    }
    return 0;
}
// Free to all Dynamically used function data.
int freeall(flight*f,int size){    
    if (!f) return 0;
    int i;
    for (i = 0; i < size; i++){
        if (!f[i].flightname) break;       
        free(f[i].fque->array);       
        free(f[i].buss->array);
        free(f[i].eco->array);
        free(f[i].std->array);
    }        
    for (i = 0; i < size; i++) {  
        free(f[i].buss);       
        free(f[i].eco); 
        free(f[i].std); 
        free(f[i].fque);
    }
}
//the main command function. It is the function that processes incoming input lines and prints the result.
int command(int inputchar,int size,char * command,int flightsize,FILE *fp,flight *f,passenger *p){ 
    int k=0;
    char *bline[inputchar];
    char *spaceline = strtok(command," ");
    while (spaceline!=NULL){  //Spliting word by word
        bline[k]=spaceline;
        spaceline = strtok(NULL," ");
        k++;
    }       
    if (strcmp(bline[0],"addseat") == 0){ //checking first word ,calling create flight function and addseat function. 
        int fi=addindexFlight(f,bline[1],size);
        if (fi<0||f[fi].closed==1){
            error(fp); 
        }else{    
            addseat(fi,f,bline[2],atoi(bline[3]));
            fprintf(fp,"addseats %s %d %d %d\n",f[fi].flightname,f[fi].buss->capacity,f[fi].eco->capacity,f[fi].std->capacity);
        }
        return 0;           
    }else if (strcmp(bline[0],"enqueue")==0){  //checking first word ,calling index flight function and enqueue function. 
        int fi=indexflight(f,bline[1],size);
        if (fi<0){
            error(fp); 
            return 0;  
        }      
        if (k==4&&(strcmp(bline[2],"business")==0||strcmp(bline[2],"economy")==0||strcmp(bline[2],"standard")==0)){
            int en=enqueue(f,p,fi,bline[1],bline[2],bline[3],"npy",1,size);
            if(en<0) error(fp);
            else    fprintf(fp,"queue %s %s %s %d\n", p[en].flight,p[en].name,p[en].wantedclass,whicone(bline[2],f,fi));              
            return 0;
        }else if (strcmp(bline[2],"business") == 0 && strcmp(bline[4],"diplomat") == 0){
            int en=enqueue(f,p,fi,bline[1],bline[2],bline[3],bline[4],1,size);
            if(en<0) error(fp);
            else    fprintf(fp,"queue %s %s %s %d\n", p[en].flight,p[en].name,p[en].wantedclass,whicone(bline[2],f,fi));            
            return 0;
        }else if (strcmp(bline[2],"economy") == 0 && strcmp(bline[4],"veteran") == 0){
            int en=enqueue(f,p,fi,bline[1],bline[2],bline[3],bline[4],1,size);
            if(en<0) error(fp);
            else    fprintf(fp,"queue %s %s %s %d\n", p[en].flight,p[en].name,p[en].wantedclass,whicone(bline[2],f,fi));            
            return 0;
        }else{
            error(fp); 
        }               
        return 0;       
    }else if (strcmp(bline[0],"sell")==0){//checking first word ,calling index flight function and selling ticket function.
        int fi=indexflight(f,bline[1],size) ;  
        if (fi<0||f[fi].closed==1){
            error(fp);
        }else{    
            sell(f,p,fi,bline[1],size);
            fprintf(fp,"sold %s %d %d %d\n",f[fi].flightname,f[fi].buss->top+1,f[fi].eco->top+1,f[fi].std->top+1 );
        }       
        return 0;       
    }else if (strcmp(bline[0],"close")==0){ //checking first word ,calling index flight function and close this flight. 
        int fi=indexflight(f,bline[1],size);   
        if (fi<0){
            error(fp);
        }else{ 
            f[fi].closed=1;   
            int ttt=f[fi].buss->top+f[fi].eco->top+f[fi].std->top+3;
            int wpc=f[fi].fque->capacity-f[fi].fque->front;
            fprintf(fp,"closed %s %d %d\n", f[fi].flightname,ttt,wpc);
            printque(f,fi,fp); 
        }       
        return 0; 
    }else if (strcmp(bline[0],"report")==0){ //checking first word ,calling index flight function and writing report this flight.
        int fi=indexflight(f,bline[1],size);   
        if (fi<0){
            error(fp);
        }else{  
            fprintf(fp,"report %s\n", f[fi].flightname);  
            report(f,p,fi,fp);
            fprintf(fp,"end of report %s\n", f[fi].flightname);
        }       
        return 0;               
    }else if (strcmp(bline[0],"info")==0){//checking first word ,calling index passenger function and writing passenger info. 
        int pi=indexpass(p,bline[1],size);  
        if (pi<0){
            error(fp);
        }else{  
            fprintf(fp,"info %s %s %s %s\n",p[pi].name, p[pi].flight,p[pi].wantedclass,p[pi].soldclass); 
        }       
        return 0;
    }else{ //otherwise calling error fuction.
        error(fp);
        return 0;
    }
}
// Main function for Reading , Writing , getting Command
int main(int argc, char *argv[]){  
    FILE *ds = fopen(argv[1],"r"); // "r" for read
    FILE *fp = fopen(argv[2],"a");      
    char *output = NULL;
    int CharInFile = 0;
    int c = 0;
    do {
        c = 0;
        c = fgetc (ds);
        if (c != EOF) {
            output = (char *) realloc (output, (CharInFile + 2) * sizeof(char));
            output[CharInFile] = c;                     
        }
        CharInFile++;
    } while (c != EOF);
    int j=0,flightsize=0,quesize=0;
    char *aline[CharInFile];
    char *line = strtok(output,"\r\n");//reading in output by line by line
    while (line!=NULL){     
        aline[j]=line;     
        line = strtok(NULL,"\r\n");
        j++;
    }
    flight * f = (flight *) malloc(j*sizeof(flight));    
    passenger * p = (passenger *) malloc(j*sizeof(passenger));
    int i;  
    for (i = 0; i < j; ++i){        
        command(CharInFile,j,aline[i],flightsize,fp,f,p);
    }
    freeall(f,j);
    free(f);
    free(p);
    free(output);
    fclose(ds);
    fclose(fp);     
    return 0;
}