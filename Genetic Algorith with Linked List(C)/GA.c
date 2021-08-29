#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
struct Node{ 
  int data; 
  struct Node*right ,*down,*up; 
};
struct Node* newNode(int d){ 
    struct Node* temp=(struct Node*)malloc(sizeof(struct Node)); 
    temp->data = d;
    temp->right = temp->down = temp->up= NULL; 
    return temp; 
}
long int power(int base, int exponent){
    if(exponent !=0 )
        return(base*power(base,exponent-1));
    else 
        return 1;
}
long int fitnesscalculate(struct Node* head,int maxgen){
    long int fitness=0;
    int pow=1;
    struct Node* current = head;  // Initialize current 
    if (current == NULL) 
        return 0; 
    while (current != NULL){ 
        if (current) {
            fitness+=power(2,maxgen-pow)*current->data; 
            pow++;
        }
        current = current->right; 
    }  
    return fitness;   
} 
void swap(struct Node *node1, struct Node *node2){
    struct Node *ptr1, *ptr2;
    ptr1 = node1 == node2->down ? node2 : node2->down;
    ptr2 = node2 == node1->down ? node1 : node1->down;
    node1->down = ptr1;
    node2->down = ptr2;
    ptr1 = node1 == node2->up ? node2 : node2->up;
    ptr2 = node2 == node1->up ? node1 : node1->up;
    node1->up = ptr1;
    node2->up = ptr2;
    if (node1->down) node1->down->up = node1;
    if (node1->up) node1->up->down = node1;
    if (node2->down) node2->down->up = node2;
    if (node2->up) node2->up->down = node2;
}
void sort(struct Node **recordsHead,int maxgen){
    int swapped;
    struct Node *ptr1;
    struct Node *lptr = NULL;
    if (*recordsHead == NULL)
        return;
    do{
        swapped = 0;
        ptr1 = *recordsHead;
        while (ptr1->down != lptr){
            if (fitnesscalculate(ptr1,maxgen) > fitnesscalculate(ptr1->down,maxgen)){                
                swap(ptr1, ptr1->down);
                if (ptr1 == *recordsHead){
                    (*recordsHead) = ptr1->up;
                }
                swapped = 1;
            }else{
                ptr1 = ptr1->down;
            }
        }
        lptr = ptr1;
    }
    while (swapped);
}
int bestchromosome(struct Node* head,int gensize){
    struct Node* current = head;  // Initialize current 
    printf("Best chromosome found so far: ");    
    while (current) {  
        printf("%d",current->data); 
        current = current->right; 
        if(current) printf(":"); 
    }
    printf(" --> %ld\n",fitnesscalculate(head,gensize)); 
    return 0;      
}
struct Node* CopyList(struct Node* head){
    struct Node* current = head;    // used to iterate over original list
    struct Node* newList = NULL; // head of the new list
    struct Node* tail = NULL;   // point to last node in new list

    while (current != NULL)
    {
        // special case for the first new Node
        if (newList == NULL)
        {
            newList = (struct Node*)malloc(sizeof(struct Node));
            newList->data = current->data;
            newList->right = NULL;
            tail = newList;
        }
        else
        {
            tail->right = (struct Node*)malloc(sizeof(struct Node));
            tail = tail->right;
            tail->data = current->data;
            tail->right = NULL;
        }
        current = current->right;
    }
    return newList;
}
void display(struct Node* head,int probsize,int gensize) { 
    struct Node *rp, *dp = head;   
    while (dp) { 
        rp = dp; 
        while (rp) { 
            printf("%d",rp->data); 
            rp = rp->right; 
            if(rp) printf(":"); 
        } 
        printf(" --> %ld\n",fitnesscalculate(dp,gensize)); 
        dp = dp->down; 
    } 
} 
int intswap(int *a,int *b){
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
    return 0;
}
int xover(struct Node* head,int select1,int select2,int xover1,int xover2,int popsize,int maxgen){  
    int lastindex=0,stopselect1=0,stopselect2=0;
    struct Node* select1head=head;
    struct Node* select2head=head;
    struct Node* select1arr=NULL;               
    struct Node* select2arr=NULL;
    int a=((select1) >= (select2)) ? (select1) : (select2);    
    while (lastindex!=a){          
        int lastxindex=0;
        int b=((xover1) >= (xover2)) ? (xover1) : (xover2);
        if (lastindex+1==select1){   
            struct Node* selectright1=select1head;
            while (lastxindex<b){
                if (lastxindex+1==xover1){
                    select1arr=selectright1;
                }if (lastxindex+1<xover1){  
                    selectright1=selectright1->right;
                }
                lastxindex++;                
            }
        }else if (lastindex+1==select2){  
            struct Node* selectright2=select2head;
            while (lastxindex<b){
                if (lastxindex+1==xover1){
                    select2arr=selectright2;
                }if (lastxindex+1<xover1){  
                    selectright2=selectright2->right;
                }
                lastxindex++;                
            }
        }if(lastindex+1<select1){ 
            select1head=select1head->down;
        }if(lastindex+1<select2){
            select2head=select2head->down;
        }
        lastindex++;               
        if(lastindex == a){   
            int i  ;       
            for (i = 0; i <= xover2-xover1; i++)
            {
               intswap(&select1arr->data,&select2arr->data);
               select1arr=select1arr->right;
               select2arr=select2arr->right;
            } 
        } 
    }
    return 0;
}
int runMutate(struct Node* head,int mutindex){ 
    struct Node* current = head;
    struct Node* currentright = NULL;
    while (current!=NULL){
        int index=0; 
        currentright = current;
        while(index+1!=mutindex){
            currentright=currentright->right;
            index++;
        }
        int a = 1;
        int b = 0;
        if (currentright->data==1){
            currentright->data=0;
        }else{
            currentright->data=1;
        } 
        current=current->down;
    }
    return 0;
}
struct Node* AddLL(int **arr, int m, int n){ 
    struct Node* mainhead = NULL; 
    struct Node* head[m]; 
    struct Node *righttemp, *newptr;  
    int i ;  
    for ( i= 0; i < m; i++) { 
        head[i] = NULL; 
        int j;
        for (j = 0; j < n; j++) { 
            newptr = newNode(arr[i][j]);
            if (!mainhead) 
                mainhead = newptr;   
            if (!head[i]) 
                head[i] = newptr; 
            else
                righttemp->right = newptr;   
            righttemp = newptr; 
        } 
    }
    int k;     
    for (k = 0; k < m - 1; k++) {   
        struct Node *temp1 = head[k], *temp2 = head[k + 1];  
        temp1->down = temp2;       
        temp2->up= temp1;  
    } 
    return mainhead; 
}
int** CreateLL(char * population, int gensize,int row,int column){
    int rowin=0 ;
    int **arr = (int **) malloc(row*sizeof(int*));  
    int x;
    for(x = 0; x < row; x++){
        arr[x] = (int*)calloc(column, sizeof(int));
    }
    int aindex=0;
    char *aline[row];
    char *line = strtok(population,"\r\n");
    while (line!=NULL){ 
        aline[aindex]=line;           
        line = strtok(NULL,"\r\n");
        aindex++;
    }  
    int i=0;
    for (int i = 0; i < row; i++){ 
        int colin=0;
        char *genbygen = strtok(aline[i],":");
        while (genbygen!=NULL){    
            arr[i][colin]=atoi(genbygen);      
            genbygen = strtok(NULL,":");
            colin++;
        }
    }
    return arr;
}
int runcommand(struct Node* head,char *selectbyselect,char *xoverline,int popsize,int maxgen){
    int s[2],sind=0;
    char *sindex = strtok(selectbyselect,":");
    while (sindex!=NULL){ 
        s[sind]=atoi(sindex);           
        sindex = strtok(NULL,":");
        sind++;
    }
    int x[2],xind=0;
    char *xindex = strtok(xoverline,":");
    while (xindex!=NULL){ 
        x[xind]=atoi(xindex);           
        xindex = strtok(NULL,":");
        xind++;
    }
	xover(head,s[0],s[1],x[0],x[1],popsize,maxgen);    
	return 0;
}
int command(struct Node* head,char * xover,char * selection,char * Mutate,int probsize,int gensize,int popsize){
    int xindex=0,seindex=0,Mindex=0;
    char *xoverarr[probsize];
    char *Mutatearr[probsize];
    char *selectionarr[probsize];
    char *probline1 = strtok(xover,"\r\n");
    while (probline1!=NULL){ 
        xoverarr[xindex]=probline1;           
        probline1 = strtok(NULL,"\r\n");
        xindex++;
    }
    char *probline2 = strtok(selection,"\r\n");
    while (probline2!=NULL){ 
        selectionarr[seindex]=probline2;           
        probline2 = strtok(NULL,"\r\n");
        seindex++;
    }
    char *probline3 = strtok(Mutate,"\r\n");
    while (probline3!=NULL){ 
        Mutatearr[Mindex]=probline3;           
        probline3 = strtok(NULL,"\r\n");
        Mindex++;
    } 
    printf("GENERATION: 0\n");
    int sindex=0;
    sort(&head,gensize);
    display(head,probsize,gensize);
    struct Node* temp=head; 
    struct Node* bestchroma=CopyList(temp);    
    bestchromosome(bestchroma,gensize);  
    int i;
    for (i = 0; i < seindex; i++){ 
        int ind=0; 
        char *sbsarr[popsize/2];
    	char*selectbyselect = strtok(selectionarr[sindex]," ");
        printf("GENERATION: %d\n",sindex+1);
	    while (selectbyselect!=NULL){   
            sbsarr[ind]=selectbyselect;
	        selectbyselect = strtok(NULL," ");
            ind++;            
	    }
        int j;	
        for (j = 0; j < popsize/2; j++){
            runcommand(head,sbsarr[j],xoverarr[sindex],popsize,gensize);   // 6:4 selectten , 8:9 xoverdan , 5 de Mutateden         
        }
        int M=atoi(Mutatearr[sindex]);
        runMutate(head,M);
        sort(&head,gensize); 
        display(head,probsize,gensize);  
        long int old=fitnesscalculate(bestchroma,gensize);
        long int new=fitnesscalculate(head,gensize);     
        if(old<new){
            bestchromosome(bestchroma,gensize);
        }else {
            bestchromosome(head,gensize);
            bestchroma=CopyList(head);
        }                
        sindex++;    
    }
	return 0;
}
char *readingfile(FILE *f){
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
    FILE *pf = fopen("population","r");
    FILE *xf = fopen("xover","r");
    FILE *sf = fopen("selection","r");   
    FILE *Mf = fopen("mutate","r");
    int probsize = atoi(argv[1]);
    int popsize = atoi(argv[2]);
    int maxgen = atoi(argv[3]);      
    char * pout ;
    char * xout ;
    char * sout ;
    char * Mout ;
    pout = readingfile(pf);
    xout = readingfile(xf); 
    sout = readingfile(sf); 
    Mout = readingfile(Mf); 
    int **population=CreateLL(pout,maxgen,popsize,maxgen);    
    struct Node* head=NULL;        
    head=AddLL(population,popsize,maxgen);   
    command(head,xout,sout,Mout,probsize,maxgen,popsize); 
    fclose(pf);
    fclose(xf); 
    fclose(sf);     
    fclose(Mf);     
    return 0;
}