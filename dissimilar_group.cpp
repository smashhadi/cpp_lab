"""
Identify dissimilar group of users
"""

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
using namespace std;



void combinationUtil_r(int arr[], int data[], int start, int end, int index, int r, int val);
void combinationUtil_c(int arr[], int data[], int start, int end, int index, int r, int val);
//Functions to print row wise relations
void printCombination_r(int arr[], int n, int r,int val)
{
    
    int data[10];
    combinationUtil_r(arr, data, 0, n-1, 0, r, val);
}

void combinationUtil_r(int arr[], int data[], int start, int end,
                     int index, int r, int val)
{
    // Current combination is ready to be printed
    if (index == r && r ==1)
    {
    	cout<<"{"<<val<<"}"<<" "<<"{"<<data[0]<<"}";
        cout<<"\n";
        return;
    }
    if (index == r && r>1)
        {
    	    cout<<"{"<<val<<"}"<<" "<<"{";
    		for (int j=0; j<r; j++)
            {
                cout<< data[j];
                if (j<r-1)
                	cout<<",";
            }
            cout<<"}"<<"\n";
            return;
        }


    for (int i=start; i<=end && end-i+1 >= r-index; i++)
    {
        data[index] = arr[i];
        combinationUtil_r(arr, data, i+1, end, index+1, r, val);
    }
}

//Functions to print column wise relations
void printCombination_c(int arr[], int n, int r,int val)
{

    int data[10];

    combinationUtil_c(arr, data, 0, n-1, 0, r, val);
}

void combinationUtil_c(int arr[], int data[], int start, int end,
                     int index, int r, int val)
{
    if (index == r && r>1)
        {
    	    cout<<"{";
    		for (int j=0; j<r; j++)
            {
                cout<< data[j];
                if (j<r-1)
                	cout<<",";
            }
            cout<<"}"<<" "<<"{"<<val<<"}"<<"\n";
            return;
        }

    for (int i=start; i<=end && end-i+1 >= r-index; i++)
    {
        data[index] = arr[i];
        combinationUtil_c(arr, data, i+1, end, index+1, r, val);
    }
}

void main()
{
	vector<int> pairF, pairW, pairL;
	ifstream in("input.txt",ios::in);
	int numU, numC, num;
	char x;
	while (in >> x) {
		switch(x){
			case 'U':
				in >> numU;
				break;
			case 'C':
				in >> numC;
				break;
			case 'F':
				while (in >> x){
					if(x=='#')
						break;
					else {
						num = x;
						pairF.push_back(num-48);
					}
				}
				break;
			case 'W':
				while (in >> x){
					if(x=='#')
						break;
					else{
						num = x;
						pairW.push_back(num-48);
					}
				}
				break;
			case 'L':
				while (in >> x){
					if(x=='#')
						break;
					else{
						num = x;
						pairL.push_back(num-48);
					}
				}
				break;
			default:
				break;
			}
		}

		//Close the file stream
	in.close();

	//Display file content
	cout<<"Number of users: "<<numU<<"\n";
	cout<<"Number of content: "<<numC<<"\n";
	cout<<"\nF\n";
	for (int i=0; i<pairF.size(); i++) {
			cout << pairF[i] << '\n';
	}
	cout<<"\nW\n";
	for (int i=0; i<pairW.size(); i++) {
		cout << pairW[i] << '\n';
	}
	cout<<"\nL\n";
	for (int i=0; i<pairL.size(); i++) {
			cout << pairL[i] << '\n';
	}
	//Find User likes
	int like[10][10] = {0};
	int tmp=0;
	int col = 0;

	for (int n=0; n<numU; n++){
		like[n][n] = 1;
		col = 0;
		tmp = 0;
		//Finding friends - reflexive
		for (int j = 0; j<pairF.size(); j++) {
				tmp = pairF[j];
				if((j%2==0) && (tmp == n+1)){
					j=j+1;
					col = pairF[j];
					like[n][col-1] = 1;
					like[col-1][n] = 1;
					for(int s=0; s<pairW.size();s++){

					}
				}
			}
		col = 0;
		//Check for likes and find owners
		for (int k = 0; k < pairL.size(); k++){
			if((k%2==0) && (pairL[k] == n+1)){
				k+=1;
				for(int b=0; b<pairW.size();b++){
					if((b%2 == 1) && (pairW[b] == pairL[k])){
						col = pairW[b-1];
						like[n][col-1] = 1;
						break;
					}
				}
			}
		}

	}

	int width = numU, height = numU;
	cout<<"\n\n";
    for (int p = 0; p < height; ++p)
    {
        for (int q = 0; q < width; ++q)
        {
            cout << like[p][q] << ' ';
        }
        cout << endl;
    }
	//Print dissimilar sets
    int set[10] = {0};
    int count;
    bool flag = 0;
	for (int p=0; p<numU; p++){
		count=0;
		flag = 0;
		fill( begin( set ), end( set ), 0 );
		for (int q=0; q<numU; q++){
			if (like[p][q] ==0){
				flag = 1;
				set[count] = q+1;
				count+=1;
			}
		}
		if (flag ==1){
			for (int r=1; r<count+1; r++){
				printCombination_r(set, count, r, p+1);
			}
		}
	}
	for (int p=0; p<numU; p++){
			count=0;
			flag = 0;
			fill( begin( set ), end( set ), 0 );
			for (int q=0; q<numU; q++){
				if (like[q][p] ==0){
					flag = 1;
					set[count] = q+1;
					count+=1;
				}
			}
			if (flag ==1){
				for (int r=2; r<count+1; r++){
					printCombination_c(set, count, r, p+1);
				}
			}
		}
}
