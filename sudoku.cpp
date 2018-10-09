//This code is entire copyright of Pandey A. and Kumar S. 2018
//Dated 2nd October 2018
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<vector>
#include<ctime>
#include<thread>
#include<chrono>
#include<cstring>
using namespace std;

//Maps (i,j,k) to all the integers from 1 to 729
int val(int i, int j, int k){
    return 81*i + 9*j + k + 1;
}
int main(){
    //clock starts
    clock_t start=clock();
	FILE *fp1;
	FILE *fp2;
	int values[9][9],nonzero=0;
    char ch,ch1;
    //Stores fixed elements in a vector
    //First row for val(i,j,k)
    //Second row for colour [1,2,3...9]->[0,1,2,3...,8]
	vector<int> fixed[2];
    fp1=fopen("input.txt","r");
	fp2=fopen("sat.txt","w");

	for( int i=0 ; i<9 ; i++ ){
		for(int j=0 ; j<9 ; j++ ){
            if(i==8&&j==8) fscanf( fp1, "%c", &ch);
			else fscanf( fp1, "%c%c", &ch,&ch1);
            if(ch=='.') values[i][j]=0;
            else values[i][j]=ch-'0';
			if(values[i][j]!=0){
				fixed[0].push_back(val(i,j,values[i][j]-1));
				fixed[1].push_back(values[i][j]-1);
				nonzero++;
			}		
		}
	}
    //Number of variables and clauses fed to minisat
	int variables = 9*9*9;
	int clauses = (9*9) + (9*9) + (9*9) + (36*81) + nonzero +(9*2);
    fprintf(fp2,"p cnf %d %d\n",variables,clauses);

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
    	for(int block=0; block<9; block++){
    		int right = 3 * (block%3);
    		int down = (block>2)*3;
    		for(int i=right ; i<right+3 ; i++){
				for( int j=down ; j<down+3 ; j++) fprintf(fp2,"%d ",val(i,j,k));
			}
			fprintf(fp2,"0\n");
    	}
    }
    //Checking if no two have same colour (81 x 9 choose 2 = 81x36 )
    for(int i=0 ; i<9 ; i++ ){
    	for( int j=0 ; j<9 ; j++ ){
    		for(int k=0 ; k<9 ; k++ ){
    			for(int l=k+1 ; l<9 ; l++ ){
    				fprintf(fp2,"-%d -%d 0\n",val(i,j,k),val(i,j,l));
    			}
    		}
    	}
    }
    //Checking fixed values (nonzero)
    for( int i=0 ; i<nonzero ; i++){
    	fprintf(fp2,"%d 0\n", fixed[0][i]);
    }
    //Checking main diagonals
    for(int k=0;k<9;k++){
        for(int i=0;i<9;i++) fprintf(fp2,"%d ",val(i,i,k));
        fprintf(fp2,"0\n");
    }
    for(int k=0; k<9 ; k++){
        for(int i=0;i<9;i++) fprintf(fp2,"%d ",val(i,8-i,k));
        fprintf(fp2,"0\n");
    }
    //Close fp1 and fp2
    fclose(fp1);
    fclose(fp2);

    fp1=fopen("ans.txt","r");
    fp2=fopen("o.txt","w");

    //Execute minisat
    srand(time(NULL));
    string str="minisat -rnd-seed="+std::to_string(rand())+".0 -rnd-freq=0."+std::to_string(rand())+" sat.txt ans.txt > garbage.txt";
    const char* cmd=str.c_str();
    std::system(cmd);

    //Process output
    char s[10];
    fscanf(fp1,"%s",s);
    if(strcmp(s,"UNSAT")==0){
        fprintf(fp2,"0");
    }
    else{
        int vals=0;
        for(int i=0 ; i<729; i++){
            int n;
            fscanf(fp1,"%d",&n);
            if(n>0){
                fprintf(fp2,"%d ",(i%9)+1);
                vals++;
                if(vals%9==0) fprintf(fp2,"\n");
            } 
        }
    }
    //Close fp1 and fp2
    fclose(fp1);
    fclose(fp2);
    printf("Solved sudoku:\n");
    std::system("cat o.txt");
    
    //Clock over
    printf("---------%f seconds--------\n",(double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}