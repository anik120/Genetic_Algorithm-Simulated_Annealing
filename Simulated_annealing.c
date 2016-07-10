#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include<time.h>
#define ITERATIONS 10000
#define SIZE 1000

void adjacencyMatrix(int a[SIZE][SIZE], int n, int p1, int p2, FILE * inputF);
void simulated_annealing(int a[SIZE][SIZE],int n);
void check(int a[SIZE][SIZE],int n);
int fitness(int a[SIZE][SIZE],int n);

int colour[SIZE], used_colours[SIZE];

const char * files[]=
{"DSJC125.1.col","DSJC125.5.col","DSJC125.9.col","DSJC250.1.col","DSJC250.5.col","DSJC250.9.col","DSJC500.1.col","DSJC500.5.col","DSJC500.9.col","DSJR500.1.col","DSJR500.1c.col","DSJR500.5.col","flat300_20_0.col","flat300_26_0.col","flat300_28_0.col","fpsol2.i.1.col","fpsol2.i.2.col","fpsol2.i.3.col","inithx.i.1.col","inithx.i.2.col","inithx.i.3.col","le450_15a.col","le450_15b.col","le450_15c.col","le450_15d.col","le450_25a.col","le450_25b.col","le450_25c.col","le450_25d.col","le450_5a.col","le450_5b.col","le450_5c.col","le450_5d.col","mulsol.i.1.col","mulsol.i.2.col","mulsol.i.3.col","mulsol.i.4.col","mulsol.i.5.col","school1.col","school1_nsh.col","zeroin.i.1.col","zeroin.i.2.col","zeroin.i.3.col","anna.col","david.col","homer.col","huck.col","jean.col","games120.col","miles1000.col","miles1500.col","miles250.col","miles500.col","miles750.col","queen10_10.col","queen11_11.col","queen12_12.col","queen13_13.col","queen14_14.col","queen15_15.col","queen16_16.col","queen5_5.col","queen6_6.col","queen7_7.col","queen8_12.col","queen8_8.col","queen9_9.col","myciel3.col","myciel4.col","myciel5.col","myciel6.col","myciel7.col"};


int main()
{
  int a[SIZE][SIZE],y;
  FILE * inputFile;
  int entries, i;
  for (i = 0; i < 72; i++) {
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
    for(i=1;i <=n; i++)
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
  simulated_annealing(a,n);
}


void check(int a[SIZE][SIZE],int n){
	int i,j,k,l,flag,temp,index,m,rep_flag;
	//printf("Entered check\n");
	for(i=1;i<=n;i++){//1
		for(j=1;j<=n;j++){//2
			if(i!=j && a[i][j]==1 && colour[i]==colour[j]){//3
			    rep_flag=0;
			    k=1;
			    while(k<=n){//4
				if(i!=k && a[i][k]==0 && colour[i]!=colour[k]){//5
					temp=colour[k];
				        flag=0;
			                for(l=1;l<=n;l++){//6
					     if(a[i][l]==1 && colour[l]==temp){//7
						flag=1;
						break;
					      }//7
				        }//6
				       if(flag==0){//8
					    colour[i]=temp;
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
					if(used_colours[m]==-1){
						colour[i]=m;
						used_colours[m]=10;
						k=n+1;                                   							break;	
						}
					   }
	 	
			    }//9
			}//3
		}//2
	}//1
}



int fitness(int a[SIZE][SIZE],int n)
{
int i,j,temp=0;

for(i=0;i<n;i++){
       for(j=1;j<=n;j++){   
	   if(colour[j]==i){
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
      clock_t begin, end;
      double time_spent;

      begin = clock();
    //printing a for testing 
    //for(i = 1;i <=n; i++)
    //{
      //  for(j = 1;j<=n; j++)
        //{   
	 //printf("%d ",a[i][j]);            
        //} 	
	//printf("\n");
    //}
   //Generating initial population
    srand((unsigned)time(NULL));
    
     
    for(i=1;i<=n;i++){
	c_temp=rand()%n;
	colour[i]=c_temp;
     }	
    
  while(iteration<=ITERATIONS){

    /*index1=(rand()%n)+1;
    c_temp=colour[index1];
    for(i=1;i<=n;i++){
	if(colour[i]==c_temp){
		k=1;flag=0;
		while(k<=n && flag==0){
		   if(a[i][k]!=1){
		     colour[i]=colour[k];
		     flag=1;
		     k=n+1;
		   }
		k++;
		}		
	}
     }*/

for(i=1;i<=n;i++)
	temp_colour[i]=colour[i];

//Emptying one colour class
    index1=(rand()%n)+1;  
    c_temp=colour[index1];
    for(i=1;i<=n;i++){
	if(colour[i]==c_temp){
             while(flag!=1){
		index2=(rand()%n)+1;
		if(a[i][index2]==0 && colour[i]!=colour[index2])
			flag=1;
	     }
	  colour[i]=colour[index2];            
	}
    }
	

    for(i=1;i<=n;i++)
	used_colours[i-1]=-1;

    for(i=1;i<=n;i++)
	used_colours[colour[i]]=10;

    /*for(i=1;i<=(n/2);i++){
        index1=i;
	index2=(n/2)+i;
	if(index2 > n)
		continue;
	else
           colour[index1]=colour[index2];
    }*/
    //printf("About to enter check\n");
    check(a,n);
    //printf("Left check block\n");
  
    col_needed=fitness(a,n);
    
    if(col_needed<final_col)
        final_col=col_needed;
    //else if(exp(-(col_needed-final_col)/T)<1.15)
	//final_col=col_needed;
    //T=T-0.1;
     else{
	for(i=1;i<=n;i++)
	     colour[i]=temp_colour[i];
     }	
     //printf("Colour array:-");  
     //for(i=1;i<=n;i++){
	//printf("%d ",colour[i]); 
	//}
      //printf("\n");
    iteration++;
  }
   //printf("\n");	
   printf("%d",fitness(a,n));
   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
   printf("(%f) \n",time_spent);
   //return 0;
}
