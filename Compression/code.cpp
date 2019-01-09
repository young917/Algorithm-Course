#include <cstdio>
#include <iostream>
#include <queue>
#include <string>
#include <stack>
using namespace std;

struct huff_tree_node{
	char key;
	int frequency;
	huff_tree_node *left_child;
	huff_tree_node *right_child;
	huff_tree_node(){
		frequency = 1;
		left_child = NULL;
		right_child = NULL;
	}
	huff_tree_node(char c){
		key = c;
		frequency = 1;
		left_child = NULL;
		right_child = NULL;
	}
};
typedef huff_tree_node* huff_tree_pointer;

struct Mycomparator{
	bool operator() (huff_tree_pointer arg1, huff_tree_pointer arg2){
		return arg1->frequency > arg2->frequency;
	}
};
//make priority queue place lowest frequency top 

struct encode_info{
	char original;
	string code;
	encode_info(){};
};
//store original character and its encoded huffman code pairly

void Encode(string filename);
void except_encode( string filename, vector< huff_tree_pointer> &frequency_list );
void Count_frequency( string filename, vector<huff_tree_pointer>&list );
huff_tree_pointer Make_huffman_tree( vector<huff_tree_pointer>&list );
void Encode_Tree_description( huff_tree_pointer root, vector<encode_info>&list , string outfile_name );
void Encode_File( string filename, vector<encode_info>& list , string outfile_name );
void Make_encode_list( vector<unsigned char> &encoded, unsigned int& buffer, int& buffer_len, int inst );
void Decode( string filename );
void Convert_to_binary(unsigned char encoded_char, int* binary, int len);
void Decode_File( string filename, huff_tree_pointer root);
void Delete_Tree( huff_tree_pointer node );

int main( int argc, char *argv[] ){
	string inputfilename;
	char option;
	if( argc != 3 ){
		cout << "Error" << endl;
		return 0;
	}
	inputfilename = argv[2];
	option = argv[1][1];
	if( option == 'c' )
		Encode( inputfilename );
	else if( option == 'd' )
		Decode( inputfilename );
	else
		cout << "Error" << endl;
}

void Encode(string filename){
	huff_tree_pointer root;
	vector<huff_tree_pointer> frequency_list;
	vector<encode_info> list;
	string outfile_name;

	Count_frequency( filename, frequency_list );
	if( (int)frequency_list.size() <= 1 ){
		except_encode( filename, frequency_list );
		return;
	}
	root = Make_huffman_tree( frequency_list );
	outfile_name = filename + ".zz";
	Encode_Tree_description( root, list, outfile_name );
	Delete_Tree( root );
	Encode_File( filename, list, outfile_name );
}

void Count_frequency(string filename, vector<huff_tree_pointer>& frequency_list){
	/*store original character and its frequency together in huff_tree_node.
	  And these huff_tree_node pointer store in frequency_list*/
	FILE* inFile;
	char input_text;
	int char_num=0;
	int index;
	inFile = fopen(filename.c_str(),"r");

	//file is empty
	if( inFile == NULL)
		return;

	while( fscanf(inFile,"%c",&input_text)!=EOF ){
		bool find = false;
		for( index = 0; index < char_num ; index++){//judge whether current character is new or already found
			if( frequency_list[index]->key == input_text ){
				find = true;
				break;
			}
		}
		if( find ){//case : current character is already found
			frequency_list[index]->frequency++;
		}
		else{//case : current character is new
			char_num++;
			huff_tree_pointer New = new huff_tree_node(input_text);
			frequency_list.push_back(New);
		}
	}
	fclose(inFile);
}
void except_encode( string filename, vector< huff_tree_pointer> &frequency_list ){
	//call when file is empty or file consists of only one character
	string outfile_name = filename + ".zz";
	FILE* OutFile;
	
	if((int)frequency_list.size() == 0 ){
		cout<< "Error: File is empty"<<endl;
	}
	else{
		OutFile = fopen( outfile_name.c_str() , "w" );
		fprintf( OutFile, "1 %c %d", frequency_list[0]->key, frequency_list[0]->frequency);
		fclose(OutFile);
	}
}

huff_tree_pointer Make_huffman_tree( vector<huff_tree_pointer>&frequency_list ){
	priority_queue<huff_tree_node, vector<huff_tree_pointer>, Mycomparator> Q;
	//In priority_queue, huff_tree_node that has lowest frequency is located top

	int list_len = (int)frequency_list.size();
	int freq_sum;
	huff_tree_pointer insert_node;
	huff_tree_pointer root;

	//initial step for huffman algorithm: any node doesn't union
	for(int i=0; i < list_len ; i++){
		Q.push(frequency_list[i]);
	}
	frequency_list.clear();
		
	//union lowest frequency node recursively until only one node is left
	while(1){
		huff_tree_pointer first = Q.top();
		Q.pop();
		huff_tree_pointer second = Q.top();
		Q.pop();
		huff_tree_pointer union_tree_node = new huff_tree_node();
		union_tree_node->left_child = first;
		union_tree_node->right_child = second;
		freq_sum = first->frequency;
		freq_sum += second->frequency;
		union_tree_node->frequency = freq_sum;
		Q.push(union_tree_node);
		int size = (int)Q.size();
		if( size == 1){
			root = Q.top();
			break;
		}	
	}
	return root;
}

void Encode_Tree_description( huff_tree_pointer root, vector<encode_info>&list , string outfile_name){
	stack< pair< huff_tree_pointer, string > > traverse_tree;
	//store unvisited tree node address and its huffman code
 	FILE* outFile;
	vector<unsigned char> tree_description;
	unsigned int buffer = 0;
	int buffer_len = 0;
	encode_info insert;
	huff_tree_pointer cur;
	
	//traverse tree like dfs process and prefer left child node.
	traverse_tree.push( make_pair( root, insert.code ) );
	while(!traverse_tree.empty()){
		cur = traverse_tree.top().first;
		insert.code = traverse_tree.top().second;
		traverse_tree.pop();
		while( cur->left_child != NULL ){//have child
			Make_encode_list( tree_description, buffer, buffer_len, 1 );
			traverse_tree.push( make_pair(cur->right_child, insert.code+'1') );//store node to visit 
			cur = cur->left_child;// move to left child
			insert.code.push_back('0');
		}
		//arrive leaf node, then store this in list
		Make_encode_list( tree_description, buffer, buffer_len, 0 );
		insert.original = cur->key;
		list.push_back(insert);
	}
	if( buffer_len != 0 )
		tree_description.push_back( (char)buffer );

	outFile = fopen( outfile_name.c_str(),"w" );
	fprintf( outFile, "%d ",(int)list.size() );
	//ordered by visited order while tree traversing
	for(int i = 0; i < (int)list.size(); i++)
		fprintf( outFile, "%c", list[i].original );
	
	fprintf( outFile, "%d%d ", (int)tree_description.size(),buffer_len);
	for(int i = 0; i < (int)tree_description.size() ; i++)
		fprintf( outFile, "%c", tree_description[i] );
	fclose( outFile );
}

void Make_encode_list( vector<unsigned char> &encoded, unsigned int& buffer, int& buffer_len, int inst){
	/*what I want to write is 'inst' 
	  It collects in buffer
	  When buffer size is 8, buffer is converted to one ASCII code and push into 'encoded'
	  */
	buffer = buffer<<1;
	buffer += inst;
	buffer_len++;
	if( buffer_len == 8 ){
		encoded.push_back( (unsigned char)buffer );
		buffer = 0;
		buffer_len = 0;
	}
}

void Delete_Tree( huff_tree_pointer node){
	if( node->left_child != NULL){
		Delete_Tree( node->left_child );
		Delete_Tree( node->right_child );
	}
	delete node;
}

void Encode_File( string filename, vector<encode_info>& list, string outfile_name ){
	FILE *inFile;
	FILE *outFile;
	vector<unsigned char> encoded;
	unsigned int buffer = 0;
	int buffer_len = 0;
	char input_text;
	int pos;
	
	inFile = fopen(filename.c_str(),"r");

	//recursively read one character and convert it to huff man code
	while( fscanf( inFile, "%c" , &input_text) != EOF ){
		for( pos = 0; pos < (int)list.size(); pos++){
			if( list[pos].original == input_text )
				break;
		}
		string put = list[pos].code; //huffman code corresponding to character
		for(int i = 0; i < (int)put.length(); i++ )
			Make_encode_list( encoded, buffer, buffer_len, put[i]-'0' );
	}
	fclose( inFile );
	if( buffer_len != 0 )
		encoded.push_back( (unsigned char)buffer );

	outFile = fopen( outfile_name.c_str(), "a" );
	fprintf( outFile, "%d%d ", (int)encoded.size(), buffer_len );
	for(int i = 0; i < (int)encoded.size(); i++ )
		fprintf( outFile, "%c", encoded[i] );
	fclose( outFile );
	list.clear();
}

void Decode( string filename ){
	huff_tree_pointer root;
	int original_char_num;
	FILE *inFile;
	FILE *OutFile;

	inFile = fopen( filename.c_str(), "r" );
	if( inFile == NULL ){//case : compressed file is empty
		cout<<"ERROR: Input File is empty"<<endl;
		return;
	}
	fscanf( inFile, "%d%*c", &original_char_num );
	if( original_char_num == 1 ){//case : original file consists of only one character
		char ch;
		int frequency;
		string outfilename = filename + ".yy";
		fscanf( inFile, "%c %d", &ch, &frequency );
		fclose( inFile );
		OutFile = fopen( outfilename.c_str() , "w" );
		for( int i = 0; i < frequency ; i++ )
			fprintf( OutFile, "%c", ch );
		fclose ( OutFile );
		return;
	}
	fclose( inFile );

	root = new huff_tree_node();
	Decode_File( filename , root);
	Delete_Tree( root );
}

void Decode_File( string filename, huff_tree_pointer root ){
	stack< huff_tree_pointer > tree_designing;
	vector<char> originals;

	FILE* inFile;
	FILE* OutFile;
	string outfile_name;
	int original_char_num;
	int originals_index=0;
	int remain;
	int tree_description_len;
	int total_len;
	int encode_text_len;
	unsigned char encoded_text;
	int binary[8];
	int binary_len;
	huff_tree_pointer cur;


	//decode tree
	inFile = fopen( filename.c_str(), "r" );

	fscanf( inFile, "%d%*c", &original_char_num );
	originals.resize( original_char_num );
	//read original character that ordered by visit order( when traversing tree )
	for( int i = 0; i < original_char_num ; i++ )
		fscanf( inFile, "%c", &originals[i] );
	fscanf( inFile, "%d%*c", &total_len );
	cur = root;
	tree_description_len = total_len/10;
	remain = total_len%10;

	//design tree by reading tree description
	while( tree_description_len != 0 ){
		fscanf( inFile, "%c" , &encoded_text );
		binary_len = 8;
		if( tree_description_len == 1 && remain != 0)
			binary_len = remain;
		Convert_to_binary( encoded_text, binary, binary_len);
		for( int i = 0; i < binary_len ; i++){
			if( binary[i] == 0 ){//current node is leaf node. This node has to store original character
				cur->left_child = NULL;
				cur->right_child = NULL;
				cur->key = originals[originals_index++];
				if( !tree_designing.empty() ){
					cur = tree_designing.top();
					tree_designing.pop();
				}
			}
			else{//current node has children
				cur->left_child = new huff_tree_node();
				cur->right_child = new huff_tree_node();
				tree_designing.push( cur->right_child );
				cur = cur->left_child;//move to left child
			}
		}
		tree_description_len--;
	}

	//decode_file
	outfile_name = filename + ".yy";
	OutFile = fopen( outfile_name.c_str(), "w");

	fscanf( inFile, "%d%*c", &total_len );
	encode_text_len = total_len/10;
	remain = total_len%10;

	cur = root;
	while(encode_text_len != 0){
		fscanf( inFile, "%c", &encoded_text );
		binary_len = 8;
		if( encode_text_len == 1 && remain != 0){
			binary_len = remain;
		}
		Convert_to_binary( encoded_text, binary, binary_len );
		for( int i = 0; i < binary_len ; i++){
			if( binary[i] == 0 )
				cur = cur->left_child;
			else
				cur = cur->right_child;
			if( cur->left_child == NULL && cur->right_child == NULL ){
				/*arrive at leaf node. This is where original character is stored
				  in huff man Tree
				  */
				fprintf( OutFile, "%c", cur->key );
				cur = root;
			}
		}
		encode_text_len--;
	}
	fclose(inFile);
	fclose(OutFile);
}

void Convert_to_binary(unsigned char encoded_char, int* binary, int len){
	//character which I read convert to binary list
	unsigned int num;
	num = (unsigned int)encoded_char;
	for( int i = len-1 ; i>= 0 ; i-- ){
		binary[i] = num%2;
		num /= 2;
	}
}
