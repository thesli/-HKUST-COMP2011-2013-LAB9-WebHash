#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iomanip>

const int HASHTABLESIZE = 50;
const int MAXLENGTH = 30;

using namespace std;

unsigned int hash(const char* str);

unsigned int hash(const char* str){
	unsigned int hash = 1315423911;
	for (int i = 0;*str; str++,i++)
	{
		hash ^=((hash<<5)+(*str)+(hash>>2));
	}
	return hash; 
}

struct Map_Entry{
	char keyword[MAXLENGTH];
	char webaddress[MAXLENGTH];	
};

int add_entry(Map_Entry* hashtable[],int size,const char keyword[],const char webaddress[]){		
	int index = hash(keyword) % HASHTABLESIZE;
	if(hashtable[index]->keyword != NULL){
		return 0;
	}
	hashtable[index] = new Map_Entry;
	strcpy(hashtable[index]->keyword,keyword);
	strcpy(hashtable[index]->webaddress,webaddress);
	return 1;
}
	
void cleanup(Map_Entry* hashtable[],int size){
	hashtable = NULL;
	delete hashtable;
}

int check_entry(Map_Entry* hashtable[],int size,const char keyword[]){
	int index = hash(keyword)%size;		
	if(hashtable[index]!=NULL & strcmp(keyword,hashtable[index]->keyword)==0){
		return index;
	}else{
		return -1;
	}
}

int init_hashtable(Map_Entry* hashtable[], int size, const char file_name[]){

	//initialize entries of hashtable to NULL
	for(int i=0; i<size; i++){
		hashtable[i]=NULL;
	}

	ifstream fin(file_name);
	if(fin.fail()){					// can not open file
			return -1;
	}

	cout << "====Initialization======" << endl;
	cout << "Read mapping from File " << file_name << ":" <<endl;

	char keyword[MAXLENGTH];
	char webaddress[MAXLENGTH];
	int total_count = 0;
	int collision_count=0;
	int mapping_count = 0;

	while(!fin.eof()){
		// read pair from file
		fin>>keyword;
		fin>>webaddress;
		cout<<setiosflags(ios::left)<<setw(15)<<keyword<<setw(20)<<webaddress<<endl;

		// add entry to hash table
		int temp=add_entry(hashtable, size, keyword, webaddress);
		total_count++;
		if(temp == 0){		//collision
			collision_count++;
			cout<<"-- Collision: "<<keyword<<" --"<<endl;
		}
		else
			mapping_count++;

	}
	cout << "There are " << total_count << " mappings in " << file_name << endl;
	cout << mapping_count << " mappings are recorded." << endl;
	cout<<collision_count<<" collisions are discarded."<<endl << endl;
	fin.close();
	return 0;
}

int main(int argc, char const *argv[])
{
	//IMPORTANT NOTE: elements in hashtable are pointers to Map_Entry objects
	//hashtable[] itself is a static object, its memory is automatically allocated and de-allocated
	struct Map_Entry* hashtable[HASHTABLESIZE];

	//Initialize hash table from mapping.txt
	char file[MAXLENGTH]="mapping.txt";
	int init = init_hashtable(hashtable, HASHTABLESIZE,file); //successful initialization returns 0
	
	if(init !=0){
		cout<<"Can not open file: "<<file<<endl;
	}else{
		cout << "=======You can start search keyword now=======:" << endl;
		//query with keyword
		char option='y';
		char keyword[30];
		while(option=='y' || option=='Y'){
			cout<<"Please input the keyword:";
			cin >> keyword;
			int entry_index=check_entry(hashtable, HASHTABLESIZE, keyword);
			if(entry_index != -1)
				cout<<"The corresponding web address is: "<<(*hashtable[entry_index]).webaddress<<endl;
			else{
				cout<<"No corresponding web address is found!"<<endl;
			}

			cout<<"Do you want to continue?(Y/N)";
			cin>>option;
		}

	}

	//deallocate memory
	cleanup(hashtable, HASHTABLESIZE);
	return 0;
}