/*******************************************************************************
 * Name        : commonwordfinder.cpp
 * Author      : Adam Undus
 * Version     : 1.0
 * Date        : 11/26/18
 * Description : Find the k most common words in a text file
 * Pledge      : I pledge my honor that I have abided by the stevens honor system.
 ******************************************************************************/
#include "rbtree.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool compPair(pair<int,Node<string,int>*> a, pair<int,Node<string,int>*> b){
	if(a.first == b.first){
		return a.second->key() < b.second->key();
	}
	return a.first > b.first;
}
int limit = 10;
vector<pair<int,Node<string,int>*>> vec;
RedBlackTree<string, int> *rbt = new RedBlackTree<string, int>();
size_t maxWordLen = 0;

int num_digits(int num) {
     //  determine how many digits are in an integer
     int digits = 1;
    while(num >= 10){
       	digits ++;
    	num /= 10;
    }
    return digits;
}


void printHeap(){
	int size = vec.size();
	cout << "Total unique words: " << size << endl;
	pair<int,Node<string,int>*> p;
	for(int j = 0; j < limit; ++j){
		if(abs(j) == vec.size()){
			break;
		}
		maxWordLen = max(maxWordLen,vec[j].second->key().length());
	}
	for(int i = 0;i<limit;++i){
		if(abs(i) == vec.size()){
			return;
		}
		int indent = min(limit,size);
		cout << setw(num_digits(indent)) << right << i+1 << ". ";
		p = vec[i];
		cout << setw(maxWordLen) << left << p.second->key()  << " "<< p.first << endl;
	}
}

void createHeap(){
	RedBlackTree<string, int>::iterator it = rbt->begin();
    while (it != rbt->end()) {
        vec.push_back(make_pair(it->value(),&*it));
        ++it;
    }
    sort(vec.begin(),vec.end(),compPair);
}

void insertWord(string s){
	RedBlackTree<string, int>::iterator it = rbt->find(s);
	if(it == rbt->end()){
		rbt->insert(s,1);
	}else{
		it->set_value(it->value() + 1);
	}

}

void processLine(string s){
	string t = "";
	for(auto c : s){
		if(c == ' ' && t!=""){
			//maxWordLen = max(maxWordLen,t.length());
			insertWord(t);
			t = "";
		}
		else if(!(c< 'a' || c>'z')){
			t += c;
		}else if(c== '-' || c == '\''){
			t+= c;
		}
		else if(c>='A' && c<='Z'){
			t += c+32;
		}
	}
	if(t!= ""){
		insertWord(t);
	}
}

int main(int argc, char *argv[]) {
	// Make sure the right number of command line arguments exist.
	if (!(argc == 2 || argc ==3)) {
		cerr << "Usage: " << argv[0] << " <filename> [limit]" << endl;
		return 1;
	}
	if(argc == 3){
		istringstream iss;
		iss.str(argv[2]);
		if(!(iss >> limit)){
			cerr << "Error: Invalid limit '"<< argv[2] << "' received."<< endl;
			return 1;
		}
		if(limit < 0){
			cerr << "Error: Invalid limit '"<< argv[2] << "' received."<< endl;
			return 1;
		}
	}
	// Create an ifstream object.
	ifstream input_file(argv[1]);
	// If it does not exist, print an error message.
	if (!input_file) {
		cerr << "Error: Cannot open file '" << argv[1] << "' for input." << endl;
		return 1;
	}
	// Add read errors to the list of exceptions the ifstream will handle.
	input_file.exceptions(ifstream::badbit);
	string line;
	try {
		unsigned int line_number = 1;
		// Use getline to read in a line.
		// See http://www.cplusplus.com/reference/string/string/getline/
		while (getline(input_file, line)) {
			//cout << line_number << ":\t" << line << endl;
			processLine(line);
			++line_number;
		}
		// Don't forget to close the file.
		input_file.close();
	} catch (const ifstream::failure &f) {
		cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
		return 1;
	}
	//cout << rbt->to_ascii_drawing() << endl;
	createHeap();
	printHeap();
	delete rbt;
	return 0;
}
