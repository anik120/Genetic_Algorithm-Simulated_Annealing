#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define cross_prob 0.8
#define mute_prob 0.5
#define size 500
#define acceptance_prob 0.4

int n,a[size][size];
struct data{
    int colour[size];
    int fitness;
    int used_colours[size];
  } *chromosomes[30];

void adjacencyMatrix(int a[500][500], int n, int p1, int p2, FILE * inputF);
void check(struct data **chrom);
void fitness(struct data **chrom);
void swap(struct data **chrom1, struct data **chrom2);
void sort_chroms(int x);
void crossover(struct data **chrom1,struct data **chrom2,struct data **chrom11,struct data **chrom12);
void mutation();
void special_mutation();

const char * files[]=
{"DSJC125.1.col","DSJC125.5.col","DSJC125.9.col","DSJC250.1.col","DSJC250.5.col","DSJC250.9.col","DSJC500.1.col","DSJC500.5.col","DSJC500.9.col","DSJR500.1.col","DSJR500.1c.col","DSJR500.5.col","flat300_20_0.col","flat300_26_0.col","flat300_28_0.col","fpsol2.i.1.col","fpsol2.i.2.col","fpsol2.i.3.col","inithx.i.1.col","inithx.i.2.col","inithx.i.3.col","le450_15a.col","le450_15b.col","le450_15c.col","le450_15d.col","le450_25a.col","le450_25b.col","le450_25c.col","le450_25d.col","le450_5a.col","le450_5b.col","le450_5c.col","le450_5d.col","mulsol.i.1.col","mulsol.i.2.col","mulsol.i.3.col","mulsol.i.4.col","mulsol.i.5.col","school1.col","school1_nsh.col","zeroin.i.1.col","zeroin.i.2.col","zeroin.i.3.col","anna.col","david.col","homer.col","huck.col","jean.col","games120.col","miles1000.col","miles1500.col","miles250.col","miles500.col","miles750.col","queen10_10.col","queen11_11.col","queen12_12.col","queen13_13.col","queen14_14.col","queen15_15.col","queen16_16.col","queen5_5.col","queen6_6.col","queen7_7.col","queen8_12.col","queen8_8.col","queen9_9.col","myciel3.col","myciel4.col","myciel5.col","myciel6.col","myciel7.col"};

int main(){
  int y,i,j,temp_colour,ctr=1,min_no,probability;
  FILE * inputFile;
  int entries,x;
  clock_t begin, end;
  double time_spent;
  begin = clock();  
  inputFile = fopen("mulsol.i.1.col","r");
  int p1=0, p2 =0;
  if(inputFile==NULL){
           printf("File failed to open.\n");
           exit(EXIT_FAILURE);
      
         }
  fscanf(inputFile,"%d", &entries);
  adjacencyMatrix(a,entries,p1,p2,inputFile);
   n=entries;
    for(i=1;i<=30;i++){
       chromosomes[i]=(struct data *)malloc(sizeof(struct data));
    }
    //initiallizing used colour array with -1
    for(i=1;i<=30;i++){
	   for(j=1;j<=n;j++){
		    chromosomes[i]->used_colours[j-1]=-1;
	   }
	}
    //initialising population
    srand((unsigned)time(NULL));
    for(i=1;i<=30;i++){
	for(j=1;j<=n;j++){
	    temp_colour=rand()%n;
	    chromosomes[i]->colour[j]=temp_colour;
	    chromosomes[i]->used_colours[temp_colour]=10; 
	    
	}
    }
       
    //validating and calculating fitness	
	for(i=1;i<=30;i++){
	   check(&chromosomes[i]);
	   fitness(&chromosomes[i]);
	}
   min_no=chromosomes[1]->fitness;
   while(ctr<=2000){
       //sorting chromosomes
           sort_chroms(10);
       //performing crossover    
    	crossover(&chromosomes[1],&chromosomes[2],&chromosomes[11],&chromosomes[12]);
       //performing mutation
       probability=(rand()%10)/10;
       if(probability<acceptance_prob)
           special_mutation();
       else   
    	    mutation();
       if(chromosomes[1]->fitness==min_no)
          ctr++;
       else 
    	ctr=1;
       min_no=chromosomes[1]->fitness;
    }	
   end = clock();
   time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
   printf("%d(%f)\n",chromosomes[1]->fitness,time_spent);
return 0;
}


void adjacencyMatrix(int a[500][500], int n, int p1, int p2, FILE * inputF){
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

}

void check(struct data **chrom){
		
    int i,j,k,l,flag,temp,index,m,rep_flag;
	for(i=0;i<n;i++)
	    (*chrom)->used_colours[i]=-1;
	for(i=1;i<=n;i++)
	    (*chrom)->used_colours[(*chrom)->colour[i]]=10;
	for(i=1;i<=n;i++){//1
		for(j=1;j<=n;j++){//2
			rep_flag=1;
			if(i!=j && a[i][j]==1 && (*chrom)->colour[i]==(*chrom)->colour[j]){//3
			    rep_flag=0;
			    k=1;
			    while(k<=n){//4
				if(i!=k && a[i][k]==0 && (*chrom)->colour[i]!=(*chrom)->colour[k]){//5
					temp=(*chrom)->colour[k];
				        flag=0;
			                for(l=1;l<=n;l++){//6
					     if(a[i][l]==1 && (*chrom)->colour[l]==temp){//7
						flag=1;
						break;
					      }//7
				        }//6
				       if(flag==0){//8	
					    (*chrom)->colour[i]=temp;
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
					if((*chrom)->used_colours[m]==-1){
						(*chrom)->colour[i]=m;
						(*chrom)->used_colours[m]=10;
						k=n+1;                                   							break;	
						}
					   }
	 	
			    }//9
			}//3
		}//2
	}//1 
}

void fitness(struct data **chrom){

int i,j,k,temp=0;
   for(j=0;j<n;j++){
       for(k=1;k<=n;k++){   
	   if((*chrom)->colour[k]==j){
	      temp++;
              break;
             }
	   }  
       }
   (*chrom)->fitness=temp;
  
}

void sort_chroms(int x){
	
  int i,j;

	for(i=1;i<x;i++){
		for(j=i+1;j<=x;j++){
			if(chromosomes[i]->fitness>chromosomes[j]->fitness)
				swap(&chromosomes[i],&chromosomes[j]);
		}
	}
}

void swap(struct data **chrom1, struct data **chrom2){
	struct data *temp=*chrom1;
	*chrom1=*chrom2;
	*chrom2=temp;
}
	


void crossover(struct data **chrom1,struct data **chrom2,struct data **chrom11,struct data **chrom12){

float rand_prob;
int i,index;

srand((unsigned)time(NULL));
rand_prob=(rand()%10)/10;
if(rand_prob<cross_prob){                    //creating new chromosomes via crossover 
	index=(rand()%n)+1;
	//printf("\n %f \n",index);
	for(i=1;i<=index;i++){
		(*chrom11)->colour[i]=(*chrom1)->colour[i];
		(*chrom11)->used_colours[i-1]=(*chrom1)->used_colours[i-1];
		(*chrom12)->colour[i]=(*chrom2)->colour[i];
		(*chrom12)->used_colours[i-1]=(*chrom2)->used_colours[i-1];
        }
	i=index+1;
	while(i<=n){
        (*chrom11)->colour[i]=(*chrom2)->colour[i];
	(*chrom12)->used_colours[i-1]=(*chrom2)->used_colours[i-1];
	(*chrom12)->colour[i]=(*chrom1)->colour[i];
	(*chrom12)->used_colours[i-1]=(*chrom2)->used_colours[i-1];
	i++;
	}
    //checking the validity of the new chromosmes
	check(&chromosomes[11]);
	check(&chromosomes[12]);
	//calculating fitness of new chromosomes.
	fitness(&chromosomes[11]);
	fitness(&chromosomes[12]);
	//comparing fitness of the new chromosomes and the worst two chromosomes
        if((*chrom11)->fitness<=(*chrom12)->fitness)
	{
		
		if(chromosomes[11]->fitness<chromosomes[10]->fitness)
			swap(&chromosomes[10],&chromosomes[11]);
			
		if(chromosomes[12]->fitness<chromosomes[9]->fitness)
			swap(&chromosomes[9],&chromosomes[12]);
	
	}
	else
	{
		if(chromosomes[12]->fitness<chromosomes[10]->fitness)
			swap(&chromosomes[10],&chromosomes[12]);

		else if(chromosomes[11]->fitness<chromosomes[9]->fitness)
			swap(&chromosomes[9],&chromosomes[11]);
	}	
}
//else
//continue;
}

void mutation(){
	
	int index,vertex,rep_vertex,flag=0,i,c_temp;	
	srand((unsigned)time(NULL));
    index=(rand()%10)+1;
    vertex=(rand()%n)+1;
    c_temp=chromosomes[index]->colour[vertex];
    while(flag==0){
    rep_vertex=(rand()%n)+1;
    if(vertex!=rep_vertex && a[vertex][rep_vertex]==0)
        flag=1;
    }  
    chromosomes[index]->colour[vertex]=chromosomes[index]->colour[rep_vertex];
    for(i=1;i<=n;i++){
        if(chromosomes[index]->colour[i]==c_temp)
               break;
    }
    if(i>n)
        chromosomes[index]->used_colours[c_temp]==-1;
}


void special_mutation(){
	
	int index,ctr=1,vertex,zero,flag=0,c_temp,i,j,k,flag1=0;
	struct data *temp;
	float acpt_prob;
	temp=(struct data *)malloc(sizeof(struct data));	
	srand((unsigned)time(NULL));
    index=(rand()%10)+1;
//Copying into new array to generate neighbour
	for(i=1;i<=n;i++){
	temp->colour[i]=chromosomes[index]->colour[i];
	temp->used_colours[i-1]=chromosomes[index]->used_colours[i-1];	
	}
	  
	  temp->fitness=chromosomes[index]->fitness;
  //Emptying one colour class
    vertex=(rand()%n)+1;  
    temp->used_colours[vertex]=-1;
    c_temp=temp->colour[vertex];
    for(i=1;i<=n;i++){
	flag=0;
	if(temp->colour[i]==c_temp){
             while(flag!=1){
		zero=(rand()%n)+1;
		if(a[i][zero]==0 && temp->colour[zero]!=c_temp)
			flag=1;
	     }
	 temp->colour[i]=temp->colour[zero];    
	   }            
    }

	check(&temp);

    fitness(&temp);
    
    srand((unsigned)time(NULL));
    acpt_prob=(rand()%10)/10;
    if(temp->fitness<chromosomes[index]->fitness || acpt_prob<acceptance_prob){
             chromosomes[index]=temp;
     }    	
}
