#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
using namespace std;

vector<int> list;
int list_len;

void swap(int &a, int &b){
	int tmp = a;
	a = b;
	b = tmp;
}
void take_input(string filename);
void insertion_sort(int left_index,int right_index);
void quick_sort(int left_index, int right_index);
void heap_sort(int left_index, int right_index);
void adjust_heap(int base, int n, int root);
void optimal_sort(int n);
void intro_sort(int left_index, int right_index, long long depth);
int median_of_3(int left_index, int right_index);
int make_non_decreasing_sublist(int n);
void make_output(string filename, int algo_index, double spend);

int main(int argc, char *argv[]){
	string inputfilename;
	int algo_index;
	if(argc != 3){
		cout<<"Input Error"<<endl;
		return 0;
	}
	inputfilename = argv[1];
	algo_index = (int)(argv[2][0]-'0');
	take_input(inputfilename);

	clock_t start = clock();
	switch(algo_index){
		case 1: insertion_sort(0,list_len-1); break;
		case 2: quick_sort(0,list_len-1); break;
		case 3: heap_sort(0,list_len-1); break;
		case 4: optimal_sort(list_len); break;
	}
	clock_t end = clock();
	double spend_time = (double)(end-start)/(CLOCKS_PER_SEC);

	make_output(inputfilename,algo_index,spend_time);
	return 0;
}
void take_input(string filename){
	ifstream inFile;
	inFile.open(filename.c_str());
	inFile>>list_len;
	list.resize(list_len);
	for(int i=0;i<list_len;i++){
		inFile>>list[i];
	}
	inFile.close();
}
void insertion_sort(int left_index, int right_index){
	for( int i = left_index+1 ; i <= right_index ; i++ ){
		int tmp = list[i];
		int j = i - 1;
		/* find where to insert list[i] in the sublist that is already sorted*/
		while( j >= left_index && list[j] > tmp ){
			list[j+1] = list[j];
			j--;
		}
		list[j+1] = tmp;
	}
}
void quick_sort(int left_index, int right_index){
	if(left_index >= right_index)
		return;
	int piv = right_index;//pivot value = list[right_index]
	int i = left_index;
	int j = right_index-1;
	/*i starts from left_index and stops when value is larger than pivot
	  j starts from right_index and stops when value is less than pivot
	  until i<j, swap list[i],list[j]. So, when exit while loop left part is
	 less than or equal to pivot and right part is larger than or equal to pivot*/
	while(1){
		while( i<right_index && list[i] <= list[piv])
			i++;
		while( j>=left_index && list[j] >= list[piv])
			j--;
		if(i>=j)
			break;
		swap(list[i],list[j]);
	}
	swap(list[i],list[piv]);
	quick_sort(left_index,j);
	quick_sort(i+1,right_index);
}
void heap_sort(int left_index, int right_index){
	//sort list[left_index]~list[right_index] using heap_sort
	int n = right_index - left_index + 1; // length of list that wants to sort
	
	//make max_heap
	for(int i = n/2 ; i > 0 ; i --)
		adjust_heap(left_index-1, n, i);

	/*swap root and last node of heap. so larger value is located in later index
	  Then adjust heap to keep max heap property */
	for(int j = right_index; j != left_index; j--,n--){
		swap(list[left_index],list[j]);
		adjust_heap(left_index-1,n-1, 1);
	}
}
void adjust_heap(int base, int n, int root){
	/* make sub heap of variable 'root' satisfy max heap.
	   set base to satisfy base+1 = left_index.
	   So, I use index in heap instead of real index in list.
	   And, n is size of heap.
	 */
	int child = 2*root;
	int insert_position = root;
	int item = list[base+root];
	//find where to put item
	while(child <= n){
		if(child < n && (list[base+child] < list[base+child+1]))
			child++;
		if(list[base+child] <= item)
			break;
		list[base+insert_position] = list[base+child];
		insert_position = child;
		child *=2;
	}
	list[base+insert_position] = item;
}
void optimal_sort(int n){
	int judge =	make_non_decreasing_sublist(n); 
	/* return value is number of non-decreasing sub_list
	  so if judge is 1, list is already sorted */
	if( n < 250 && judge > 1){
		heap_sort(0,n-1);
	}
	else if(judge != 1){
		long long depth=floor(log(n))*2;
		intro_sort(0,n-1,depth);
	}
}
void intro_sort(int left_index, int right_index, long long depth){
	//left1, right1 -> left_index_loop, right_index_loop
	//left2, right2 -> left_index_recursive, right_index_recursive
	int left_index_loop, right_index_loop;
	int left_index_recursive, right_index_recursive;

	left_index_loop = left_index;
	right_index_loop = right_index;

	while(left_index_loop < right_index_loop){
		if(depth == 0){
			heap_sort(left_index_loop,right_index_loop);
			return;
		}
		int pivot = median_of_3(left_index_loop,right_index_loop);
		// already list[left_index_loop]<= list[mid] <= list[right_index_loop]
		/* i starts from left and stops when value is larger than pivot
		   j starts from right and stops when value is less than pivot
		   Until i<=j, swap list[i] and list[j]. Then, left part of list is less than or
		   equal to pivot and right part of list is larger than or equal to pivot at the end*/
		int i = left_index_loop + 1;
		int j = right_index_loop - 1;
		while(1){
			while(i < right_index_loop && list[i] <= pivot)
				i++;
			while(j > left_index_loop && list[j] >= pivot)
				j--;
			if(i > j)
				break;
			swap(list[i],list[j]);
			i++;
			j--;
		}
		// after partition, make the larger part sorted by loop and smaller part sorted by recursive
		if( j < (left_index_loop + right_index_loop)/2){
			left_index_recursive = left_index_loop;
			right_index_recursive = j;
			left_index_loop = i;
		}
		else{
			left_index_recursive = i;
			right_index_recursive = right_index_loop;
			right_index_loop = j;
		}
		depth--;
		intro_sort(left_index_recursive,right_index_recursive,depth);
	}
}
int median_of_3(int left_index, int right_index){
	//make list[left_index] <= list[mid] <= list[right_index]
	int mid = (left_index + right_index)/2;
	if(list[left_index] > list[right_index])
		swap(list[left_index],list[right_index]);
	if(list[mid] > list[right_index])
		swap(list[mid],list[right_index]);
	if(list[left_index] > list[mid])
		swap(list[left_index],list[mid]);
	return list[mid];
}
int make_non_decreasing_sublist(int n){
	int pastflag; // 0: same, -1: decrease, 1: increase
	int flag;// 0: same, -1: decrease, 1: increase
	int sublist_num=0;

	/* pastflag represents relation between list[i-1] and list[i]
	   		list[i-1] = list[i], then pastflag = 0
			list[i-1] < list[i], then pastflag = 1
			list[i-1] > list[i], then pastflag = -1
	   flag represents relation between list[i] and list[i+1]
	*/
	int i=0;
	while(i < n){
		pastflag = 0;
		/*In next while loop,
		  find sublist([i]~[j-1]) that is already non-decreasing order or non-increasing order*/
		int j = i+1;
		while(j < n){
			if(list[j-1] < list[j])
				flag = 1;
			else if(list[j-1] > list[j])
				flag = -1;
			else
				flag = 0;
			if((!pastflag) || (!flag) || (pastflag == flag)){
				// condition for satisfying non-decreasing order or non-increasing order
				j++;
				if(flag)
					pastflag = flag;
			}
			else
				break;
		}
		sublist_num++;
		if(pastflag == -1){
			// list[i] ~ list[j-1] is non-increasing -> make non-decreasing sublist by swapping
			int mid = (j-i)/2;
			for(int k=0; k<mid; k++)
				swap(list[i+k],list[j-1-k]);
		}
		i=j;
	}
	return sublist_num;
}
void make_output(string filename, int algo_index, double spend){
	ofstream outFile;
	string outputfilename;

	outputfilename="result_";
	outputfilename+=(char)(algo_index+'0');
	outputfilename+="_"+filename;
	outFile.open(outputfilename.c_str());

	outFile<<filename<<endl;
	outFile<<algo_index<<endl;
	outFile<<list_len<<endl;
	outFile<<spend<<endl;
	for(int i=0;i<list_len;i++)
		outFile<<list[i]<<" ";

	outFile.close();	
}
