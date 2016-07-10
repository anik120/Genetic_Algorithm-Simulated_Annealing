#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>
#define cross_prob 0.8
#define mute_prob 0.5
#define SIZE 1000
#define GA_ITERATION 100
#define SA_ITERATION 500
void check2(int chrom[SIZE],int n,int a[SIZE][SIZE],int used_col[SIZE]);
void crossover(int n,int a[SIZE][SIZE]);
void colourArray(int a[SIZE][SIZE],int n);
void adjacencyMatrix(int a[SIZE][SIZE], int n, int p1, int p2, FILE * inputF);
int calc(int chrom[SIZE],int n);
void mutation(int a[SIZE][SIZE],int n);
int colour[SIZE][SIZE],temp1[SIZE],temp2[SIZE],unique[11],used_colours[SIZE][SIZE];
void fitness(int a[SIZE][SIZE],int colour[SIZE][SIZE],int n);
void check(int n,int a[SIZE][SIZE]);
void simulated_annealing(int a[SIZE][SIZE],int n);
void check_SA(int a[SIZE][SIZE],int n);
int fitness_SA(int a[SIZE][SIZE],int n);

int colour_SA[SIZE], used_colours_SA[SIZE];

const char * files[]=
{"DSJC1000.1.col"};


int main()
{
  int a[SIZE][SIZE],y;
  FILE * inputFile;
  int entries, i;
  for (i = 0; i < 1; i++) {
      inputFile = fopen(files[i],"r");
      printf("%s: ",files[i]);
      int p1=0, p2 =0,j;

       if(inputFile==NULL){
           printf("File failed to open.");
           exit(EXIT_FAILURE);
      
         }
        fscanf(inputFile, "%d", &entries);
        adjacencyMatrix(a,entries,p1,p2,inputFile);
   }
   exit(0);
}

void adjacencyMatrix(int a[SIZE][SIZE], int n, int p1, int p2, FILE * inputF){
int i,j;

	for(i=1;i<=n;i++)
	  for(j=1;j<=n;j++)
		a[i][j]=0;
 
do{
    for(i = 1;i <=n; i++)
    {
        for(j = 1;j<=n; j++)
        {   if(i==p1 && j == p2){
                 a[i][j] = 1;
                 a[j][i] = 1;
            }
        }
        a[i][i] = 0;
    }
}while(fscanf(inputF, "%d %d", &p1, &p2) !=EOF);
    colourArray(a,n);

}

void check(int n,int a[SIZE][SIZE])
{
int i,j,k,l,flag,temp,index,m,rep_flag,g;
srand((unsigned)time(NULL));
    for(i=1;i<=10;i++)
	{//1
	 for(j=1;j<=n;j++)
	    {//2
	     for(k=1;k<=n;k++)
		{//3
		if(j!=k && a[j][k]==1 && colour[i][j]==colour[i][k]){//4
			    rep_flag=0;
			    g=1;
			 while(g<=n){//5
				if(j!=g && a[j][g]==0 && colour[i][j]!=colour[i][g]){//6
					temp=colour[i][g];
				        flag=0;
			                for(l=1;l<=n;l++){//7
					     if(a[i][l]==1 && colour[i][l]==temp){//8
						flag=1;
						break;
					      }//8
				        }//7
				       if(flag==0){//9
					    colour[i][j]=temp;
					    g=n+1;
					    rep_flag=1;
				        }//9
				       else
				          g++;
  				}//6
                              g++;
			      }//5
			

			}//4

				if(rep_flag==0){//10
				for(m=0;m<n;m++){//11
					
					if(used_colours[i][m]==-1){//12
						colour[i][j]=m;
						used_colours[i][m]=10;
						g=n+1;                                   							      break;	
						}//12
					   }//11
	 	
			    }//10

		}//3
	    }//2	
	}//1
}
	
	

void sort_chroms(int n){
	int i,j,index[10],temp_chrom[SIZE][SIZE],temp,ith;
	
	for(i=1;i<=10;i++){
		index[i]=i;  //Calculating the unquie colours of each chromosome and storing in an array
	}
	for(i=1;i<10;i++){
		for(j=i+1;j<=10;j++){
			if(unique[i]>unique[j]){
				temp=unique[i];
				unique[i]=unique[j];     //Sorting the unique array and book-keeping with the index array 
				unique[j]=temp;
				temp=index[i];
				index[i]=index[j];
				index[j]=temp;
			}
		}
	}
        /*printf("\n");
	for(i=1;i<=10;i++){
		printf("%d\n",unique[i]);
	}*/
	for(i=1;i<=10;i++){
		for(j=1;j<=n;j++){
			ith=index[i];
			temp_chrom[i][j]=colour[ith][j]; //Creating the temp matrix according to the sorted unique array
		}
	}
	for(i=1;i<=10;i++){
		for(j=1;j<=n;j++){
			colour[i][j]=temp_chrom[i][j];   //copying temp matrix to global matrix a
		}
	}
}

void fitness(int a[SIZE][SIZE],int colour[SIZE][SIZE],int n){

int i,j,k,temp;
for(i=1;i<=10;i++){
   temp=0;
   for(j=0;j<n;j++){
       for(k=1;k<=n;k++){   
	   if(colour[i][k]==j){
	      temp++;
              break;
             }
	   }  
       }
   unique[i]=temp;
   //printf("%d\n",unique[i]);
  }
}

void colourArray(int a[SIZE][SIZE],int n)
{
      int i,j,k,ctr=0,rand_prob,flag1=0;
      clock_t begin, end;
      double time_spent;

      begin = clock();

    //printing a for testing 
    /*for(i = 1;i <=n; i++)
    {
        for(j = 1;j<=n; j++)
        {   
	 printf("%d ",a[i][j]);            
        } 	
	printf("\n");
    }*/
    //Initiallising pops
    srand((unsigned)time(NULL));
    for(i=1;i<=10;i++){
	for(j=1;j<=n;j++){
	    colour[i][j]=rand()%n; 
	}
    }
	for(i=1;i<=10;i++)
	for(j=1;j<=n;j++)
	used_colours[i][j-1]=-1;

    	for(i=1;i<=n;i++)
	for(j=1;j<=n;j++)
	used_colours[i][colour[i][j]]=10;
    	check(n,a);

	

   
    //printing colour
    /*printf("\n");
    for(i = 1;i <=10; i++)
    {
        for(j = 1;j<=n; j++)
        {   
	 printf("%d ",colour[i][j]);            
        } 	
	printf("\n");
    }*/
    fitness(a,colour,n);  
    sort_chroms(n);
    //printing colour
    /*printf("\n");
    for(i = 1;i <=10; i++)
    {
        for(j =1;j<=n; j++)
        {   
	 printf("%d ",colour[i][j]);            
        } 	
	printf("\n");
    }*/
	srand((unsigned)time(NULL));
	while(ctr<=GA_ITERATION){
				
				fitness(a,colour,n);
    				crossover(n,a);
    				fitness(a,colour,n);
    				sort_chroms(n);
				rand_prob=(rand()%10)/10;
				if(rand_prob<=mute_prob)
					{
						mutation(a,n);
						
						fitness(a,colour,n);
    						sort_chroms(n);
						
					}
					else
					continue;
	
	 			//printing colour
    				/*printf("\n");
     				for(i = 1;i <=10; i++)
    				 {
        				for(j =1;j<=n; j++)
        					{   
	 						printf("%d ",colour[i][j]);            
        					} 	
					printf("\n");
   				 }*/
				ctr++;
	
		}
	printf("%d",unique[1]);
	end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("(%f)\n",time_spent);

}

void crossover(int n,int a[SIZE][SIZE]){

float index,rand_prob;
int i,j,temp1[SIZE],temp2[SIZE],k,flag=0,temp,fit_new1,fit_new2,flag1=0,used_col[SIZE];
for(i=0;i<n;i++)
used_col[i]=-1;
srand((unsigned)time(NULL));
rand_prob=(rand()%10)/10;
if(rand_prob<cross_prob){                    //creating new chromosomes via crossover 
	index=(rand()%n)+1;
	//printf("\n %f \n",index);
	for(i=1;i<=index;i++){
		temp1[i]=colour[1][i];
		temp2[i]=colour[2][i];
        }
	i=index+1;
	while(i<=n){
        temp1[i]=colour[2][i];
	temp2[i]=colour[1][i];
	i++;
	}

        //checking the validity of the new chromosmes
	check2(temp1,n,a,used_col);
	check2(temp2,n,a,used_col);

	

        //comparing fitness of the new chromosomes and the worst two chromosomes
        fit_new1=calc(temp1,n);
	fit_new2=calc(temp2,n);
	if(fit_new1<=fit_new2)
	{
		
		if(fit_new1<unique[10])
			{
		       for(i=1;i<=n;i++)
			  colour[10][i]=temp1[i];
			}
		if(fit_new2<unique[9])
			{
				for(i=1;i<10;i++)
					colour[9][i]=temp2[i];
				
			}
	
	}
	else
	{
		if(fit_new2<unique[10])
			{
						
				for(i=1;i<=n;i++)
				
					colour[10][i]=temp2[i];

			}

		else if(fit_new1<unique[9])
			{
				for(i=1;i<=n;i++)
				
					colour[9][i]=temp1[i];
			}
	}
	/*printf("\n");
	for(i=1;i<=10;i++)
		{
			for(j=1;j<=n;j++)
				{
					printf("%d",colour[i][j]);
				}
		printf("\n");
		}*/
		
	fitness(a,colour,n);
	sort_chroms(n);

}
//else
//continue;
}
void check2(int chrom[SIZE],int n,int a[SIZE][SIZE],int used_col[SIZE]){
		
		int i,j,k,l,flag,temp,index,m,rep_flag;
		for(i=1;i<=n;i++)
			used_col[i-1]=-1;

    for(i=1;i<=n;i++)
	used_col[chrom[i]]=10;
	//printf("Entered check\n");
	for(i=1;i<=n;i++){//1
		for(j=1;j<=n;j++){//2
			if(i!=j && a[i][j]==1 && chrom[i]==chrom[j]){//3
			    rep_flag=0;
			    k=1;
			    while(k<=n){//4
				if(i!=k && a[i][k]==0 && chrom[i]!=chrom[k]){//5
					temp=chrom[k];
				        flag=0;
			                for(l=1;l<=n;l++){//6
					     if(a[i][l]==1 && chrom[l]==temp){//7
						flag=1;
						break;
					      }//7
				        }//6
				       if(flag==0){//8
					    chrom[i]=temp;
					    k=n+1;
					    rep_flag=1;
				        }//8
				       else
				          k++;
  				}//5
                              k++;
			      }//4
                            
			    if(rep_flag==0){//9
				for(m=0;m<n;m++){
					if(used_col[m]==-1){
						chrom[i]=m;
						used_col[m]=10;
						k=n+1;                                   							      break;	
						}
					   }
	 	
			    }//9
			}//3
		}//2
	}//1 
     
}
		
int calc(int chrom[SIZE],int n){
   int i,j,temp=0; 
	for(i=0;i<n;i++){
          for(j=1;j<=n;j++){   
	      if(chrom[j]==i){
	       temp++;
               break;
              }
	   }  
        }    
    return temp;
} 

void mutation(int a[SIZE][SIZE],int n){
	int index,vertex,zero,flag=0,c_temp,i,j,k,temp_colour[SIZE],fit1,fit2,flag1=0,used_cols[SIZE];                        	index=(rand()%10)+1;
	for(j=1;j<=n;j++)
       	     temp_colour[j]=colour[index][j];
	simulated_annealing(a,n);
	fit1=calc(temp_colour,n);
        fit2=fitness_SA(a,n);
        if(fit2<fit1){
           for(i=1;i<=n;i++)
	       colour[index][i]=colour_SA[i];
         }
	
	fitness(a,colour,n);
	sort_chroms(n);
}


void check_SA(int a[SIZE][SIZE],int n){
	int i,j,k,l,flag,temp,index,m,rep_flag;
	//printf("Entered check\n");
	for(i=1;i<=n;i++){//1
		for(j=1;j<=n;j++){//2
			if(i!=j && a[i][j]==1 && colour_SA[i]==colour_SA[j]){//3
			    rep_flag=0;
			    k=1;
			    while(k<=n){//4
				if(i!=k && a[i][k]==0 && colour_SA[i]!=colour_SA[k]){//5
					temp=colour_SA[k];
				        flag=0;
			                for(l=1;l<=n;l++){//6
					     if(a[i][l]==1 && colour_SA[l]==temp){//7
						flag=1;
						break;
					      }//7
				        }//6
				       if(flag==0){//8
					    colour_SA[i]=temp;
					    k=n+1;
					    rep_flag=1;
				        }//8
				       else
				          k++;
  				}//5
                              k++;
			      }//4
                            
			    if(rep_flag==0){//9
				for(m=0;m<n;m++){
					if(used_colours_SA[m]==-1){
						colour_SA[i]=m;
						used_colours_SA[m]=10;
						k=n+1;                                   							break;	
						}
					   }
	 	
			    }//9
			}//3
		}//2
	}//1
}


int fitness_SA(int a[SIZE][SIZE],int n)
{
int i,j,temp=0;

for(i=0;i<n;i++){
       for(j=1;j<=n;j++){   
	   if(colour_SA[j]==i){
	      temp++;
              break;
             }
	   }  
       }
return temp;
}



void simulated_annealing(int a[SIZE][SIZE],int n)
{
      int ctr=1,i,j,k,c_temp,flag=0,iteration=1,final_col=1000,col_needed,index1,index2,temp_colour[SIZE];
      float T=10;
//Generating initial population
    srand((unsigned)time(NULL));
    
     
    for(i=1;i<=n;i++){
	c_temp=rand()%n;
	colour_SA[i]=c_temp;
     }	
    
  while(iteration<=SA_ITERATION){

    /*index1=(rand()%n)+1;
    c_temp=colour_SA[index1];
    for(i=1;i<=n;i++){
	if(colour_SA[i]==c_temp){
		k=1;flag=0;
		while(k<=n && flag==0){
		   if(a[i][k]!=1){
		     colour_SA[i]=colour[k];
		     flag=1;
		     k=n+1;
		   }
		k++;
		}		
	}
     }*/

for(i=1;i<=n;i++)
	temp_colour[i]=colour_SA[i];

//Emptying one colour class
    index1=(rand()%n)+1;  
    c_temp=colour_SA[index1];
    for(i=1;i<=n;i++){
	if(colour_SA[i]==c_temp){
             while(flag!=1){
		index2=(rand()%n)+1;
		if(a[i][index2]==0 && colour_SA[i]!=colour_SA[index2])
			flag=1;
	     }
	  colour_SA[i]=colour_SA[index2];            
	}
    }
	

    for(i=1;i<=n;i++)
	used_colours_SA[i-1]=-1;

    for(i=1;i<=n;i++)
	used_colours_SA[colour_SA[i]]=10;

    check_SA(a,n);
    
  
    col_needed=fitness_SA(a,n);
    
    if(col_needed<final_col)
        final_col=col_needed;
    //else if(exp(-(col_needed-final_col)/T)<1.15)
	//final_col=col_needed;
    //T=T-0.1;
     else{
	for(i=1;i<=n;i++)
	     colour_SA[i]=temp_colour[i];
     }	
     
    iteration++;
  }	
   //printf("No of colours required:%d\n",fitness(a,n)); 
}
        
