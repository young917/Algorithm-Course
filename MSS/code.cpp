#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;

struct answer{
	int left_index, right_index;
	int total_num, sum, algo_idx;
	double spend_time;
	answer(){};
};
vector<int> list; //store number list
answer ans;

void take_input(string filename, int algo_index);
void algo_n_square(int n);
int algo_nlogn(int l, int r);
void algo_n(int n);
void make_output(string inputfilename);

int main(int argc, char *argv[]){
	string inputfilename;
	int algo_index;
	if(argc != 3){
		// if argument is less than 2, error
		cout<<"Input Error"<<endl;
		return 0;
	}
	inputfilename=argv[1];
	algo_index=(int)(argv[2][0]-'0');
	take_input(inputfilename,algo_index);
	clock_t start=clock(); //start to measure time
	switch(algo_index){
		case 1: algo_n_square(ans.total_num);break;
		case 2: algo_nlogn(0,ans.total_num-1);break;
		case 3: algo_n(ans.total_num);break;
	}
	clock_t end=clock(); // finish measuring time
	ans.spend_time=(double)(end-start)/((double)(CLOCKS_PER_SEC)/1000);
	make_output(inputfilename);
	return 0;
}

void take_input(string filename, int algo_index){
	//read input file and initialize 'answer ans'
	ifstream inFile;
	inFile.open(filename.c_str());
	inFile>>ans.total_num;
	list.resize(ans.total_num);
	for(int i=0;i<ans.total_num;i++){
		inFile>>list[i];
	}
	inFile.close();
	ans.algo_idx = algo_index;
	ans.sum = list[0];
	ans.left_index = ans.right_index = 0;
}
void algo_n_square(int n){
	int l, r;
	int tmp_sum; //store sum of numbers [l,r]
	for(l = 0; l < n; l++){
		tmp_sum=0;
		for(r = l; r < n ; r++){
			tmp_sum += list[r];
			if(ans.sum < tmp_sum){ //renew answer
				ans.left_index = l;
				ans.right_index = r;
				ans.sum = tmp_sum;
			}
		}
	}
}
int algo_nlogn(int l, int r){
	// return maximum subsequence sum in part [l,r]
	int entire_part_maxsum;
	int left_part_maxsum;
	int right_part_maxsum;
	int mid_maxsum;
	int mid_maxsum_left;
	int mid_maxsum_right;
	int mid_left_index, mid_right_index;
	int tmp_sum,tmp_l,tmp_r;
	int m;
	m=(l+r)/2;
	if(l<r){
		left_part_maxsum = algo_nlogn(l,m);
		right_part_maxsum = algo_nlogn(m+1,r);

		/* mid_maxsum is max sum of sequence that must includes list[m] and list[m+1]
		   mid_max_sum_left is max sum of sequence from mid_left_index to m
		   mid_max_sum_right is max sum of sequence from (m+1) to mid_right_index*/

		//find mid_max_sum_left
		tmp_sum = mid_maxsum_left = list[m];
		mid_left_index = m;
		tmp_l = m-1;
		while(l <= tmp_l){
			tmp_sum += list[tmp_l];
			if(tmp_sum >= mid_maxsum_left){
				/* renew answer
				   '=' is necessary to find the smallest answer subsequence starting index
				   and we look from large index('m-1') to small index('l')*/
				mid_maxsum_left = tmp_sum;
				mid_left_index = tmp_l;
			}
			tmp_l--;
		}

		//find mid_max_sum_right
		tmp_sum = mid_maxsum_right = list[m+1];
		mid_right_index = m+1;
		tmp_r = m+2;
		while(tmp_r <= r){
			tmp_sum += list[tmp_r];
			if(tmp_sum > mid_maxsum_right){
				/* renew answer
				   '=' doesn't necessary because we look from small index('m+2') to large index('r')*/
				mid_maxsum_right = tmp_sum;
				mid_right_index = tmp_r;
			}
			tmp_r++;
		}
		mid_maxsum = mid_maxsum_left + mid_maxsum_right;
		entire_part_maxsum = max(mid_maxsum, max(left_part_maxsum,right_part_maxsum));
		if(entire_part_maxsum == mid_maxsum){
			if(ans.sum == mid_maxsum){
				if(ans.left_index > mid_left_index){ 
					/* if mid_maxsum is equall to answer value(until now)
					   but stored subsequence left index is larger than mid_left_index
					   => renew answer */ 
					ans.left_index = mid_left_index;
					ans.right_index = mid_right_index;
				}
			}
			else if(ans.sum < mid_maxsum){
				// renew answer
				ans.sum= mid_maxsum;
				ans.left_index = mid_left_index;
				ans.right_index = mid_right_index;
			}
		}
		return entire_part_maxsum;
	}
	else{
		// when l==r. looking one number list[l]
		if(ans.sum < list[l]){
			//renew answer
			ans.sum = list[l];
			ans.left_index = l;
			ans.right_index = r;
		}
		return list[l];
	}	
}
void algo_n(int n){
	int tmp_sum=0;
	int l=0,r;
	for(r=0;r<n;r++){
		tmp_sum+=list[r]; //tmp_sum stores max sum subsequence that ends with list[r]
		if(ans.sum < tmp_sum){
			//renew answer
			ans.sum = tmp_sum;
			ans.left_index = l;
			ans.right_index = r;
		}
		if(tmp_sum < 0){
			/*When max sum subsequence that ends with list[r] is less than zero,
			  I find all possible max sum subsequence that starts with list[l].
			  So, update tmp_sum to starts with list[r+1]*/
			tmp_sum = 0;
			l = r+1;
		}
	}
}
void make_output(string inputfilename){
	ofstream outFile;
	string outputfilename;

	outputfilename="result_"+inputfilename;
	outFile.open(outputfilename.c_str());

	outFile<<inputfilename<<endl;
	outFile<<ans.algo_idx<<endl;
	outFile<<ans.total_num<<endl;
	outFile<<ans.left_index<<endl;
	outFile<<ans.right_index<<endl;
	outFile<<ans.sum<<endl;
	outFile<<ans.spend_time<<endl;
	outFile.close();
}
