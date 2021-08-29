#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  	int rows;
  	int cols;
  	int **data;
  	char *name;
} matrix;
typedef struct {
	int len;
	int *data;	
	char *name;
} vector;
/* Initialization */
int zeromatrix(matrix * m,int rows, int cols,char *name1) { //Creates a new matrix of given rows, cols and name ,filled with zeros
	if (rows <= 0 || cols <= 0) return -1;
	int i;
	for (i = 0; i <= sizeof(m); i++){
		
		if (!m[i].name){		
			m[i].rows = rows;
			m[i].cols = cols;
			m[i].name = name1;
			m[i].data = (int **) malloc(rows*sizeof(int*));	
			int x;
			for(x = 0; x < rows; x++){
        		m[i].data[x] = (int*)calloc(cols, sizeof(int));
    		}
			int l,k;
			for (l = 0; l < rows; l++) {
				for (k = 0; k < cols; k++) {
					m[i].data[l][k] = 0;
				}
			}
			return i;
		}else if(strcmp(m[i].name,name1) == 0){
			return -1;
		}

	}			  
}
int zerovector(vector * v,char *name1,int len1) { //Creates a new vector of given length and name, filled with zeros.
  	if (len1 <= 0) return -1;
	int i;
  	for (i = 0; i <= sizeof(v); i++){
		if (!v[i].name){
			v[i].len = len1;
			v[i].name = name1;
			v[i].data = (int *) malloc(len1*sizeof(int));
			int l;
			for (l = 0; l < len1; l++) v[i].data[l] = 0;
			return i;
		}else if(strcmp(v[i].name,name1) == 0){
			return -1;
		}
	}	
}
int readmatrix(char inp4[], char* file1 , matrix* m ){ //Reads the matrix from the file with the given filename	
    FILE *Read1 = fopen(inp4, "r");  
    char *output2 = NULL;
    int cf = 0;
    int c = 0;
    do {
        c = 0;
        c = fgetc (Read1);
        if (c != EOF) {
            output2 = (char *) realloc (output2, (cf + 1) * sizeof(char));
            output2[cf] = c;
        }
        cf++;
    } while (c != EOF); 
	int j=0;
	char *aline[cf];
	char *line = strtok(output2,"\n");
	while (line!=NULL){  	
		aline[j]=line;
		line = strtok(NULL,"\n");
		j++;
	}	
	char *to1 = malloc(strlen(aline[j-1]));
    strcpy(to1,aline[j-1]);	
	int k=0;
	char *line9 = strtok(to1," ");	
	while (line9!=NULL){
		line9 = strtok(NULL," ");
		k++;
	} 	
	char *matname=strtok(file1,".");
	int indexmat= zeromatrix(m,j,k,matname);
	int r;
	for (r = 0; r < j; r++){
		int p=0;
	    char *line2 = strtok(aline[r]," ");
	    while (line2!=NULL){
	    	m[indexmat].data[r][p]=atoi(line2); 
	        line2 = strtok(NULL," ");
	        p++;
	    } 	   				
	}
	free(to1);
	free(output2); 
    fclose(Read1);
	return indexmat;
}
int readvector(char inp3[], char* file2 , vector* v){  //Reads the vector from the file with the given filename
	FILE *Read2 = fopen(inp3, "r");
    char *output4 = NULL;
    int cf = 0;
    int c = 0;
    do {
        c = 0;
        c = fgetc (Read2);
        if (c != EOF) {
            output4 = (char *) realloc (output4, (cf+1) * sizeof(char));
            output4[cf] = c;
        }
        cf++;
    } while (c != EOF);  
    char *to = malloc(strlen(output4));
    strcpy(to,output4);
    int k=0;
	char *line6 = strtok(to," ");
	while (line6!=NULL){
		line6 = strtok(NULL," ");
		k++;
	} 
	char *filename = strtok(file2,".");
	int vecindex =zerovector(v,filename,k);
	int eleindex=0;
	char *line7 = strtok(output4," ");
	while (line7!=NULL){
		v[vecindex].data[eleindex]=atoi(line7);
		line7 = strtok(NULL," ");
		eleindex++;
	} 
	free(to);
	free(output4);
	fclose(Read2);
	return vecindex;
}
/* Concatenation */
int vecstack(char*vec1,char*vec2,char*direction,char*matname,matrix*m,vector*v){ //Combines “vector1”and “vector2” and creates a matrix
	int vec1index = indevec(v,vec1);
	int vec2index = indevec(v,vec2);		
	if(v[vec1index].len==v[vec2index].len){		
		if(strcmp(direction,"row") == 0){
			int matindex=zeromatrix(m,2,v[vec1index].len,matname);	
			int j;
			for (j = 0; j < v[vec1index].len; j++){
				m[matindex].data[0][j]=v[vec1index].data[j];
				m[matindex].data[1][j]=v[vec2index].data[j];
			}	
			return matindex;	
		}else if(strcmp(direction,"column") == 0){
			int matindex=zeromatrix(m,v[vec1index].len,2,matname);				
			int j;
			for (j = 0; j < v[vec1index].len ; j++){
				m[matindex].data[j][0]=v[vec1index].data[j];
				m[matindex].data[j][1]=v[vec2index].data[j];
			}
			return matindex;
		}else{
			return 0;
		}
	}else{
		return 0;
	}
	return 0;
}
int matstack(char*mat1,char*mat2,char*where,matrix*m){ //Combines “matrix1” and “matrix2”.Overwrite "matrix1".
	int mat1index = indemat(m,mat1);
	int mat2index = indemat(m,mat2);	
	int row=m[mat1index].rows+m[mat2index].rows;
	int col=m[mat1index].cols+m[mat2index].cols;
	int newdata[row+1][col+1];	
	int i;
	for (i = 0; i < m[mat1index].rows; i++) {
		int j;
		for (j = 0; j < m[mat1index].cols; j++) {
			newdata[i][j]=m[mat1index].data[i][j];
		}		
	}
	if(strcmp(where,"r") == 0){
		if(m[mat1index].rows==m[mat2index].rows){
			free1matrix(m,mat1index);
			int i;
			for (i = 0; i < m[mat1index].rows; i++) {
				int j;
				for (j = 0; j < m[mat2index].cols; j++) {	
					newdata[i][m[mat1index].cols+j]=m[mat2index].data[i][j];							
				}								
			}
			m[mat1index].cols=col;	
		}else{
			return -1;
		} 
	}else if(strcmp(where,"d") == 0){
		if(m[mat1index].cols==m[mat2index].cols){
			free1matrix(m,mat1index);
			int i;
			for (i = 0; i < m[mat2index].rows; i++) {
				int j;
				for (j = 0; j < m[mat1index].cols; j++) {	
					newdata[m[mat1index].rows+i][j]=m[mat2index].data[i][j];							
				}								
			}
			m[mat1index].rows=row;
		}else{
			return -1;
		}
	}else{
		return -1;
	}	
	m[mat1index].data = (int **) malloc(m[mat1index].rows*sizeof(int*));
	int x;
	for(x = 0; x < m[mat1index].rows; x++){
    	m[mat1index].data[x] = (int*)calloc(m[mat1index].cols, sizeof(int));
    }
	int h;
	for (h = 0; h < m[mat1index].rows; h++) {
		int g;
		for (g = 0; g < m[mat1index].cols; g++) {
			m[mat1index].data[h][g]=newdata[h][g];
		}
	}
	return mat1index;
}
int mvstack(char*mat1,char*vec1,char*where,matrix*m,vector*v){ //Combines “matrix” and “vector”.Overwrite "matrix".
	int mat1index = indemat(m,mat1);
	int vec1index = indevec(v,vec1);	
	int newdata[m[mat1index].rows+1][m[mat1index].cols+1];
	int i;
	for (i = 0; i < m[mat1index].rows; i++) {
		int j;
		for (j = 0; j < m[mat1index].cols; j++) {
			newdata[i][j]=m[mat1index].data[i][j];
		}
	}
	if(strcmp(where,"r") == 0){
		if(m[mat1index].rows==v[vec1index].len){
			free1matrix(m,mat1index);
			int i;
			for (i = 0; i < m[mat1index].rows; i++) {		
				newdata[i][m[mat1index].cols]=v[vec1index].data[i];							
												
			}
			m[mat1index].cols+=1;	
		}else{
			return -1;
		} 
	}else if(strcmp(where,"d") == 0){
		if(m[mat1index].cols==v[vec1index].len){
			free1matrix(m,mat1index);
			int i;
			for (i = 0; i < m[mat1index].cols; i++) {	
				newdata[m[mat1index].rows][i]=v[vec1index].data[i];									
			}
			m[mat1index].rows+=1;
		}else{
			return -1;
		}
	}else{
		return -1;
	}	
	m[mat1index].data = (int **) malloc(m[mat1index].rows*sizeof(int*));
	int x;
	for(x = 0; x < m[mat1index].rows; x++){
    	m[mat1index].data[x] = (int*)calloc(m[mat1index].cols, sizeof(int));
    }
	int h;
	for (h = 0; h < m[mat1index].rows; h++) {
		int g;
		for (g = 0; g < m[mat1index].cols; g++) {
			m[mat1index].data[h][g]=newdata[h][g];

		}
	}
	return mat1index;
}
/* Slicing */
int vecslice(char*vec1,int start,int stop,char*vecname,vector*v){ //Takes a part from the given vector.And create given name of the vector.
	int vec1index = indevec(v,vec1);
	if (stop > v[vec1index].len) return -1;
	int newindex = zerovector(v,vecname,stop-start);
	int i;
	for (i = 0; i < stop-start; i++){
		v[newindex].data[i]=v[vec1index].data[start+i];
	}	
	return newindex;
}
int matslice(char*mat1,int y1,int y2,int x1,int x2,char*matname,matrix*m){ //Takes a part from the given matrix.And create given name of the matrix. 
	int mat1index = indemat(m,mat1);
	if (x2 > m[mat1index].rows || y2 > m[mat1index].cols) return -1;
	int newindex = zeromatrix(m,x2-x1,y2-y1,matname);
	int i;
	for (i = 0; i < y2-y1; i++){
		int j;
		for (j = 0; j < x2-x1; j++){
			m[newindex].data[j][i]=m[mat1index].data[x1+j][y1+i];
		}
	}
	if (mat1index<0)	return mat1index;	
	return newindex;
}
int matslicerow(char*mat1,int index,int start,int stop,char*vecname,matrix*m,vector*v){ //Takes a part of vector from the given matrix.
	int mat1index = indemat(m,mat1);
	if (stop > m[mat1index].cols) return -1;
	int newindex = zerovector(v,vecname,stop-start);
	int i;
	for (i = 0; i < stop-start; i++){
		v[newindex].data[i]=m[mat1index].data[index][start+i];
	}	
	return newindex;
}
int matslicecol(char*mat1,int index,int start,int stop,char*vecname,matrix*m,vector*v){ //Takes a part of vector from the given matrix.
	int mat1index = indemat(m,mat1);
	if (stop > m[mat1index].rows) return -1;
	int newindex = zerovector(v,vecname,stop-start);
		int i;
		for (i = 0; i < stop-start; i++){
			v[newindex].data[i]=m[mat1index].data[start+i][index];
		}	
	return newindex;
}
/* Math */
int add(char * mat1,char* mat2,matrix*m){ //calculates the element-wise sum of two matrices and overwrites matrix1.	
	int mat1index = indemat(m,mat1);
	int mat2index = indemat(m,mat2);
	if (m[mat1index].rows != m[mat2index].rows || m[mat1index].cols != m[mat2index].cols) return -1;
	int i;
	for (i = 0; i < m[mat1index].rows; i++){
		int j;
		for (j = 0; j < m[mat1index].cols; j++){
			m[mat1index].data[i][j]+=m[mat2index].data[i][j];
		}
	}	
	if (mat1index<0)	return mat1index;
	if (mat2index<0)	return mat2index;
	return mat1index;
}
int substract(char * mat1,char* mat2,matrix*m){ //calculates the element-wise subtraction of two matrices and overwrites matrix1.
	int mat1index = indemat(m,mat1);
	int mat2index = indemat(m,mat2);
	if (m[mat1index].rows != m[mat2index].rows || m[mat1index].cols != m[mat2index].cols) return -1;
	int i;
	for (i = 0; i < m[mat1index].rows; i++){
		int j;
		for (j = 0; j < m[mat1index].cols; j++){
			m[mat1index].data[i][j]-=m[mat2index].data[i][j];
		}
	}
	if (mat1index<0)	return mat1index;
	if (mat2index<0)	return mat2index;
	return mat1index;
}
int multiply(char * mat1,char* mat2,matrix*m){//calculates the element-wise product of two matrices and overwrites matrix1.
	int mat1index = indemat(m,mat1);
	int mat2index = indemat(m,mat2);
	if (m[mat1index].rows != m[mat2index].rows||m[mat1index].cols != m[mat2index].cols) return -1;	
	if (m[mat1index].rows != m[mat2index].rows || m[mat1index].cols != m[mat2index].cols) return -1;
	int i;
	for (i = 0; i < m[mat1index].rows; i++){
		int j;
		for (j = 0; j < m[mat1index].cols; j++){
			m[mat1index].data[i][j]*=m[mat2index].data[i][j];
		}
	}
	if (mat1index<0)	return mat1index;
	if (mat2index<0)	return mat2index;
	return mat1index;
}
/* Padding */
int bigone(int first,int second){ //returns the largest of the two numbers.
	if (second>=first) return second;
	else return first;
}
int smallone(int first,int second){  //returns the smallest of the two numbers.
	if (second>=first) return first;
	else return second;
}
int pad(char * mat1,int x,int y,char* mode,matrix*m){	//Enlarges the size of the matrix and Pads with the maximum or minimum value of all or part of the vector along each axis.
	int mat1index = indemat(m,mat1);	
	int row=m[mat1index].rows+x;
	int col=m[mat1index].cols+y;	
	int oldrow=m[mat1index].rows;
	int oldcol=m[mat1index].cols;
	int newdata[row+1][col+1];	
	int i;
	for (i = 0; i < m[mat1index].rows; i++) {
		int j;
		for (j = 0; j < m[mat1index].cols; j++) {
			newdata[i][j]=m[mat1index].data[i][j];
		}
	}
	if(strcmp(mode,"maximum") == 0){
		free1matrix(m,mat1index);
		int l;
		for (l = 0; l < oldrow+x; l++) { 	
			if (l>=oldrow){ 
				int k;
				for (k = 0; k < oldcol+y; k++) { 					
					if (k>=oldcol){
						int value=newdata[0][k];
						int j;
						for (j = 0; j < l; j++) {	
							value=bigone(value,newdata[j][k]);
						}
						int m;
						for (m = 0; m < k; m++) {	
							value=bigone(value,newdata[l][m]);
						}
						newdata[l][k]=value;
					}else{
						int value=newdata[0][k];
						int j;
						for (j = 0; j < l; j++) {	
							value=bigone(value,newdata[j][k]);
						}
						newdata[l][k]=value;
					}				
				}
			}else {
				int k;
				for (k = oldcol; k < oldcol+y; k++) {	
					int value=newdata[l][0];
					int m;
					for (m = 0; m < k; m++) {	
						value=bigone(value,newdata[l][m]);
					}	
					newdata[l][k]=value;
				}
			}				
		} 
	}else if(strcmp(mode,"minimum") == 0){
		free1matrix(m,mat1index);
		int l;
		for (l = 0; l < oldrow+x; l++) { 	
			if (l>=oldrow){  
				int k;
				for (k = 0; k < oldcol+y; k++) { 					
					if (k>=oldcol){
						int value=newdata[0][k];
						int j;
						for (j = 0; j < l; j++) {	
							value=smallone(value,newdata[j][k]);
						}
						int m;
						for (m = 0; m < k; m++) {	
							value=smallone(value,newdata[l][m]);
						}
						newdata[l][k]=value;
					}else{
						int value=newdata[0][k];
						int j;
						for (j = 0; j < l; j++) {	
							value=smallone(value,newdata[j][k]);
						}
						newdata[l][k]=value;
					}				
				}
			}else {		 	
				int k;
				for (k = oldcol; k < oldcol+y; k++) {	
					int value=newdata[l][0];
					int m;
					for (m = 0; m < k; m++) {	
						value=smallone(value,newdata[l][m]);
					}	
					newdata[l][k]=value;
				}
			}				
		}
	}else{
		return -1;
	}
	m[mat1index].rows=row;	
	m[mat1index].cols=col;		
	m[mat1index].data = (int **) malloc(m[mat1index].rows*sizeof(int*));
	int x1;
	for (x1 = 0; x1 < m[mat1index].rows; x1++){
    	m[mat1index].data[x1] = (int*)calloc(m[mat1index].cols, sizeof(int));
    }
	int h;
	for (h = 0; h < m[mat1index].rows; h++) {
		int g;
		for (g = 0; g < m[mat1index].cols; g++) {
			m[mat1index].data[h][g]=newdata[h][g];

		}
	}
	return mat1index;
}
int padval(char * mat1,int x,int y,int val,matrix*m){  //Enlarges the size of the matrix and sets the values of the new elements.
	int mat1index=indemat(m,mat1);
	int oldrow=m[mat1index].rows;
	int oldcol=m[mat1index].cols;
	int newdata[oldrow+x+1][oldrow+y+1];
	int i;
	for (i = 0; i < oldrow; i++) {
		int j;
		for (j = 0; j < oldcol; j++) {
			newdata[i][j]=m[mat1index].data[i][j];
		}
	}
	free1matrix(m,mat1index);	
	int l;
	for (l = 0; l < oldrow+x; l++) {		
		if (l>=oldrow){
			int m;
			for (m = 0; m < oldcol+y; m++) {	
				newdata[l][m]=val;				
			}
		}else {
			int k;
			for (k = oldcol; k < oldcol+y; k++) {	
				newdata[l][k]=val;
			}
		}				
	}		
	m[mat1index].rows+=x;
	m[mat1index].cols+=y;	
	m[mat1index].data = (int **) malloc(m[mat1index].rows*sizeof(int*));			
	int x1;
	for (x1 = 0; x1 < m[mat1index].rows; x1++){
    	m[mat1index].data[x1] = (int*)calloc(m[mat1index].cols, sizeof(int));
    }	
	int h;
	for (h = 0; h < m[mat1index].rows; h++) {
		int g;
		for (g = 0; g < m[mat1index].cols; g++) {
			m[mat1index].data[h][g]=newdata[h][g];

		}
	}
	return mat1index;
} 
/* Printing */
int printVector(vector * v,int i,FILE *fp) { //Finding given vector index in array and print that. 
	if (!v) return -1;
	int len1;
	for (len1 = 1; len1 <= v[i].len; len1++) {
	    fprintf(fp, "%d ", v[i].data[len1-1]);  		      		
	}
	fprintf(fp, "\n");
	return 0;	
}
int printMatrix(matrix * m,int i,FILE *fp) { //Finding given matrix index in array and print that.
	if (!m) return -1;	
	int row, col;
	for (row = 1; row <= m[i].rows; row++) {
	    for (col = 1; col <= m[i].cols; col++) {
	      	fprintf(fp, "%d ", m[i].data[row-1][col-1]);
	    }
	    fprintf(fp, "\n");	      
	}
}
/* Finding the index of the vector in the vector array. */
int indevec(vector * v,char *name) {	
	if (!v) return -1;
	int i;
	for (i = 0; i < sizeof(v); i++){
	  	if(strcmp(v[i].name,name) == 0){
	    	return i;
	    }  
	}
	return -1;
}
/* Finding the index of the Matrix in the matrix array */
int indemat(matrix * m,char *name){
	if (!m) return -1;
	int i;
	for (i = 0; i < sizeof(m); i++){
	  	if(strcmp(m[i].name,name)==0){	  		
	    	return i;
	    }  
	}
	return -1;
}
/* Free Functions */
int freematrix(matrix * mtx) {
  	if (!mtx) return 0;
	int i;
  	for (i = 0; i < sizeof(mtx); i++){
  		if (!mtx[i].data) return 0;
		int k;
  		for (k = 0; k < mtx[i].rows; k++){
  			free(mtx[i].data[k]);
  		}
  		free(mtx[i].data);
   	}    		
  	return 0;	 
}
int freevector(vector * vec) {
  	if (!vec) return -1;
	int i;
  	for (i = 0; i < sizeof(vec); i++){
  		if (!vec[i].data) return 0;
	  	free(vec[i].data);
  	}
  	return 0;
}
int free1matrix(matrix*m,int mat1index){
	int i;
	for (i = 0; i < m[mat1index].rows; i++) {		
		free(m[mat1index].data[i]);
	}
	free(m[mat1index].data);
}
/* Function that checks commands and executes functions. */
int command(matrix*m,vector*v,int CharInFile,int j,char * command,char *in,FILE *fp){ 
	int k=0;
    char *bline[CharInFile];
    char *spaceline = strtok(command," ");
    while (spaceline!=NULL){
        bline[k]=spaceline;
        spaceline = strtok(NULL," ");
        k++;
    }       
    if (strcmp(bline[0],"veczeros") == 0){ 	
    	int vi= zerovector(v,bline[1],atoi(bline[2]));
    	if (vi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "created vector %s %d\n",v[vi].name,v[vi].len);
    		printVector(v,vi,fp);
    	} 
    	return 0;    	   	
    } else if (strcmp(bline[0],"matzeros")==0){    	
        int mi= zeromatrix(m,atoi(bline[2]),atoi(bline[3]),bline[1]);
        if (mi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "created matrix %s %d %d\n",m[mi].name,m[mi].rows,m[mi].cols);
    		printMatrix(m,mi,fp);
    	} 
    	return 0;        
    } else if (strcmp(bline[0],"vecread")==0){	     
    	char inp1[strlen(in)];
    	char inp2[strlen(bline[1])];
		strcpy(inp1,in);
    	strcat(inp1,bline[1]); 
    	strcpy(inp2,bline[1]); 
    	int vi = readvector(inp1,bline[1],v);

    	if (vi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "read vector %s %d\n",inp2,v[vi].len);
    		printVector(v,vi,fp);
    	}   
    	return 0;   	
    } else if (strcmp(bline[0],"matread")==0){	    	
    	char inp1[strlen(in)];
    	char inp2[strlen(bline[1])];
		strcpy(inp1,in);
    	strcat(inp1,bline[1]); 
    	strcpy(inp2,bline[1]);   	
    	int mi = readmatrix(inp1,bline[1],m);
    	if (mi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "read matrix %s %d %d\n",inp2,m[mi].rows,m[mi].cols);
    		printMatrix(m,mi,fp);
    	} 
    	return 0;     	
    } else if (strcmp(bline[0],"vecstack")==0){	    	
		int vecresult=vecstack(bline[1],bline[2],bline[3],bline[4],m,v);
	    if (vecresult<0){
	    	fprintf(fp, "%s\n", "error");
	    }else{	    	
	    	fprintf(fp, "vectors concatenated %s %d %d\n",m[vecresult].name,m[vecresult].rows,m[vecresult].cols); 
	    	printMatrix(m,vecresult,fp);
	    }
	    return 0;		        
    } else if (strcmp(bline[0],"matstack")==0){    	
    	int matresult=matstack(bline[1],bline[2],bline[3],m);
	    if (matresult<0){
	    	fprintf(fp, "%s\n", "error");
	    }else{	    	
	    	fprintf(fp, "matrices concatenated %s %d %d\n",m[matresult].name,m[matresult].rows,m[matresult].cols); 
	    	printMatrix(m,matresult,fp);
	    }
	    return 0; 
    } else if (strcmp(bline[0],"mvstack")==0){   
    	int matresult=mvstack(bline[1],bline[2],bline[3],m,v);
	    if (matresult<0){
	    	fprintf(fp, "%s\n", "error");
	    }else{	    	
	    	fprintf(fp, "matrix and vector concatenated %s %d %d\n",m[matresult].name,m[matresult].rows,m[matresult].cols); 
	    	printMatrix(m,matresult,fp);
	    }
	    return 0;
    } else if (strcmp(bline[0],"pad")==0){
    	int mi=pad(bline[1],atoi(bline[2]),atoi(bline[3]),bline[4],m);
    	if (mi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "%s %s %d %d\n", "matrix paded",m[mi].name,m[mi].rows,m[mi].cols);
    		printMatrix(m,mi,fp);
    	}
    	return 0;
    } else if (strcmp(bline[0],"padval")==0){
    	int mi=padval(bline[1],atoi(bline[2]),atoi(bline[3]),atoi(bline[4]),m);
    	if (mi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "%s %s %d %d\n", "matrix paded",m[mi].name,m[mi].rows,m[mi].cols);
    		printMatrix(m,mi,fp);
    	}
    	return 0;
    } else if (strcmp(bline[0],"vecslice")==0){
    	int vi= vecslice(bline[1],atoi(bline[2]),atoi(bline[3]),bline[4],v);
    	if (vi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "%s %s %d\n", "vector sliced",v[vi].name,v[vi].len);
    		printVector(v,vi,fp);
    	}
    	return 0;  
    } else if (strcmp(bline[0],"matslicecol")==0){ 
        int vi= matslicecol(bline[1],atoi(bline[2]),atoi(bline[3]),atoi(bline[4]),bline[5],m,v);
    	if (vi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "%s %s %d\n", "vector sliced",v[vi].name,v[vi].len);
    		printVector(v,vi,fp);
    	}
    	return 0; 
    } else if (strcmp(bline[0],"matslicerow")==0){ 
        int vi= matslicerow(bline[1],atoi(bline[2]),atoi(bline[3]),atoi(bline[4]),bline[5],m,v);
        if (vi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "%s %s %d\n", "vector sliced",v[vi].name,v[vi].len);
    		printVector(v,vi,fp);
    	}
    	return 0;  
    } else if (strcmp(bline[0],"matslice")==0){ 
    	int mi=matslice(bline[1],atoi(bline[2]),atoi(bline[3]),atoi(bline[4]),atoi(bline[5]),bline[6],m);
        if (mi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "%s %s %d %d\n", "matrix sliced",m[mi].name,m[mi].rows,m[mi].cols);
    		printMatrix(m,mi,fp);
    	}
    	return 0;  
    } else if (strcmp(bline[0],"add")==0){ 
       	int mi=add(bline[1],bline[2],m);
        if (mi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "add %s %s\n",bline[1],bline[2]);
    		printMatrix(m,mi,fp);
    	}
        return 0;  
    } else if (strcmp(bline[0],"multiply")==0){
        int mi=multiply(bline[1],bline[2],m);
        if (mi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "multiply %s %s\n",bline[1],bline[2]);
    		printMatrix(m,mi,fp);
    	}
        return 0;  
    } else if (strcmp(bline[0],"subtract")==0){
        int mi=substract(bline[1],bline[2],m);
        if (mi<0){
    		fprintf(fp, "%s\n", "error");
    	}else{
    		fprintf(fp, "subtract %s %s\n",bline[1],bline[2]);
    		printMatrix(m,mi,fp);
    	}
        return 0;    
    } else if (bline[0]!=NULL){
    	fprintf(fp, "%s\n", "error");
    	return 0;
    }
}
int main(int argc, char *argv[]){
  	FILE *ds = fopen(argv[2],"r"); // "r" for read
  	FILE *fp = fopen(argv[3],"a");   	
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
    int j=0;
    char *aline[CharInFile];
    char *line = strtok(output,"\r\n");//reading in output by line by line
    while (line!=NULL){
        aline[j]=line;      
        line = strtok(NULL,"\r\n");
        j++;
    }	
    matrix * m = (matrix *) malloc((j+2)*sizeof(matrix));
    vector * v = (vector *) malloc((j+2)*sizeof(vector));
	int i;
    for (i = 0; i < j; ++i){       	
        command(m,v,CharInFile,j,aline[i],argv[1],fp);
	}  	
	freematrix(m);	
	freevector(v);
    free(output);
    free(m);
    free(v);
  	fclose(ds);
  	fclose(fp);  	
  	return 0;
}