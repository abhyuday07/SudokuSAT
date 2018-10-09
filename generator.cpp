//This code is entire copyright of Pandey A. and Kumar S. 2018
//Dated 2nd October 2018
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<vector>
#include<cstring>
#include<ctime>
#include<thread>
#include<chrono>
using namespace std;

//Maps (i,j,k) to all the integers from 1 to 729
int val(int i, int j, int k){
	return 81*i + 9*j + k + 1;
}
void invoke(){
    FILE* fp;
    fp = fopen("input.txt", "w");
    for( int i=0 ; i<9 ; i++ ) fprintf(fp,". . . . . . . . .\n");
    fclose(fp);
    std::system("g++ -std=c++11 sudoku.cpp");
    std::system("./a.out");
}
//isunique check if problem[][] has a unique solution[][] or not
int isunique(int problem[][9], int solution[][9], int len){
	FILE *fp1;
	FILE *fp2;
	int values[9][9], nonzero = 0;
    //Stores fixed elements in a vector
    //First row for val(i,j,k)
    //Second row for colour [1,2,3...9]->[0,1,2,3...,8]
	vector<int> fixed[2];
	fp2 = fopen( "sat.txt" , "w" );
	for( int i=0 ; i<9 ; i++ ){
		for(int j=0 ; j<9 ; j++ ){
			values[i][j] = problem[i][j];
			if ( values[i][j] != 0 ){
				fixed[0].push_back( val(i, j, values[i][j]-1) );
				fixed[1].push_back( values[i][j] - 1 );
				nonzero++;
			}		
		}
	}
    //Number of variables and clauses fed to minisat
	int variables = 9*9*9 ;
	int clauses = (9*9) + (9*9) + (9*9) + (36*81) + nonzero +(9*2)+ (len>0) ;
    fprintf(fp2,"p cnf %d %d\n", variables, clauses);
	//Checking for rows (9x9)
    for( int i=0 ; i<9 ; i++ ){
    	for( int k=0; k<9 ; k++ ){
    		for( int j=0; j<9; j++){
    			fprintf(fp2,"%d ",val(i,j,k));
    		}
    		fprintf(fp2,"0\n");
    	}
    }
    //Checking for columns (9x9)
    for( int j=0 ; j<9 ; j++ ){
    	for( int k=0; k<9 ; k++ ){
    		for( int i=0; i<9; i++){
    			fprintf(fp2,"%d ",val(i,j,k));
    		}
    		fprintf(fp2,"0\n");
    	}
    }
    //Checking for blocks (9x9)
    for( int k=0 ; k<9 ; k++ ){
    	for( int block=0 ; block<9 ; block++ ){
    		int right = 3 * (block%3);
    		int down = (block>2)*3;
    		for( int i=right; i<right+3; i++){
				for( int j=down; j<down+3; j++) fprintf(fp2,"%d ",val(i,j,k));
			}
			fprintf(fp2,"0\n");
    	}
    }
    //Checking if no two have same colour (81 x 9 choose 2 = 81x36 )
    for(int i=0 ; i<9 ; i++){
    	for( int j=0 ; j<9 ; j++){
    		for(int k=0; k<9; k++){
    			for(int l=k+1; l<9; l++){
    				fprintf(fp2,"-%d -%d 0\n",val(i,j,k),val(i,j,l));
    			}
    		}
    	}
    }
    //Checking fixed values (nonzero)
    for( int i=0 ; i<nonzero ; i++ ){
    	fprintf(fp2,"%d 0\n", fixed[0][i]);
    }
    //Checking main diagonals (9x2)
    for( int k=0 ; k<9 ; k++ ){
        for( int i=0 ; i<9 ; i++ ) fprintf(fp2,"%d ",val(i,i,k));
        fprintf(fp2,"0\n");
    }
    for( int k=0 ; k<9 ; k++ ){
        for( int i=0 ; i<9 ; i++ ) fprintf(fp2,"%d ",val(i,8-i,k));
        fprintf(fp2,"0\n");
    }
    //Ensuring if solution is not solution[][]
    for( int i=0 ; i<9 ; i++ ){
        for( int j=0 ; j<9 ; j++ ){
            if( problem[i][j]==0 ) fprintf(fp2,"-%d ",val(i,j,solution[i][j]-1));
        }
    }
    if(len>0) fprintf(fp2,"0\n");

    fclose(fp2);
    fp1=fopen("ans.txt","r");

    //Execute minisat
    std::system("minisat sat.txt ans.txt > garbage.txt");
    //Check if UNSAT
    char s[10];
    fscanf(fp1,"%s",s);
    fclose(fp1);
    if(s[0]=='U') return 1;
    return 0;
}
int main(){
    //Clock Start
    clock_t start=clock();
    //invokes sudoku.cpp to give a random sudoku in o.txt
    invoke();
	FILE* fpo;
	fpo=fopen("o.txt","r");
	int solution[9][9];
	int problem[9][9];
    int fix[9][9];
	int temp,tempx,tempy,x,y,len=81;
    //Takes input from "o.txt" i.e. a solved sudoku
	for( int i=0 ; i<9 ;i++ ){
		for( int j=0 ; j<9 ; j++ ){
			fscanf(fpo,"%d",&solution[i][j]);
			problem[i][j]=0;
		}
	}
    //Seed value set to CPU time
    srand(time(NULL));
    //This loop invokes randomization to form a tentative solution
    //in which the first n-1 elements do not give a unique solution
    //but the addition of n-th element gives a unique solution
    //NOTE: This set of clues may not be minimal. Call this S.
	while(1){
		do{
			x=rand()%9;
			y=rand()%9;
		}while(problem[x][y]!=0);
		problem[x][y]=solution[x][y];
        len--;
        if(isunique(problem,solution,len)==1){ break; }
	}
    //It is quite evident that if (n-1) do not give a unique solution
    //then n-th element must belong to S', minimal set which is a subset of S
    //This loop segregates two types of point, one which must be in S'
    //the other which can be removed.
    //For a proof to this algorithm refer to readme.txt
    for( int i=0 ; i<9 ; i++ ){
        for( int j=0 ; j<9 ; j++ ){
            if( problem[i][j]!=0 ){
                int temp=problem[i][j];
                problem[i][j]=0;
                len++;
                if( isunique(problem,solution,len)==0 ){
                    len--;
                    problem[i][j]=temp;
                }
            }
        }
    }
    //The minimal sudoku is delivered
	printf("Minimal clue problem for solved sudoku in o.txt:\n");
	for(int i=0;i<9;i++){ 
        for(int j=0;j<9;j++){
            if(problem[i][j]==0) printf(". ");
            else printf("%d ",problem[i][j]); 
        }
        printf("\n");
    }
	fclose(fpo);
    //Clock over
    printf("---------%f seconds--------\n",(double)(clock()-start)/CLOCKS_PER_SEC); 

	return 0;
}