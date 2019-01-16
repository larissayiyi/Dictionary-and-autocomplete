/* This function is bench testing the functionality of two hash functions
   found online checking for the number of collisions and average step for
	 search function */
/* functions from http://research.cs.vt.edu/AVresearch/hashing/strings.php 
	 and http://stackoverflow.com/questions/7666509/hash-function-for-string */
#include "util.h"
#include <iostream>
#include <ctime>
#include <string.h>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main (int argc, char** argv) {
	// check the number of arguments
  if(argc != 3) {
    cerr << "Incorrect number of arguments\n";
    return -1;
  }
	// load the arguments 
  char * dictfile = argv[1];
  int num_words = atoi(argv[2]);
  
	// open file
  ifstream stream;
  stream.open(dictfile, ios::binary);
  
	// table size is twice as big
  int tsize = num_words * 2;
  int stepSumOne = 0;
  int stepSumTwo = 0;
  int maxNumHitsOne = 0;
  int maxNumHitsTwo = 0;
  // these are the "hashtables" with the number of collisions in each element
  int hashtableOne [tsize]; 
  int hashtableTwo [tsize];
  // these are for calculating the number of hits
  int numOfHitsOne [tsize];
  int numOfHitsTwo [tsize];
  for (int i = 0; i < tsize; i++){
    hashtableOne[i] = 0;
    hashtableTwo[i] = 0;
    numOfHitsOne[i] = 0;
    numOfHitsTwo[i] = 0;
  }
  char buffer [BUFSIZ];
  // run number of words time
  for (int i = 0; i < num_words; i++){
    // get the word
    stream.getline (buffer, BUFSIZ);
    string s (strchr(buffer,' ') + 1);
    // Hash function 1
    // http://research.cs.vt.edu/AVresearch/hashing/strings.php
    int intLength = s.size() / 4;
    long sum = 0;
    for (int j = 0; j < intLength; j++) {
	    // break words in 4 characters chuncks
      string c = s.substr(j * 4, (j * 4) + 4);
      long mult = 1;
      for (unsigned int k = 0; k < c.length(); k++) {
	      sum += c[k] * mult;
	      mult *= 256;
      }
    }
   
    string c = s.substr(intLength * 4);
    long mult = 1;
    for (unsigned int k = 0; k < c.length(); k++) {
      sum += c[k] * mult;
      mult *= 256;
    }
	  // mod to fit the table size
    hashtableOne[abs(sum) % tsize]++;
  
    // djb2 by Dan Bernstein
    // http://stackoverflow.com/questions/7666509/hash-function-for-string
    
    unsigned long hash = 5381;
    int count = 0;

    for (unsigned int j = 0; j < s.length(); j++) {
      count = (int)s[j];
      hash = ((hash << 5) + hash) + count; /* hash * 33 + c */
    }
    // mod to fit table size
    hashtableTwo[hash% tsize]++;
    
    
  }
  // calculate the number of slots for number of hits 
  for (int i = 0; i < tsize; i++) {
    numOfHitsOne [hashtableOne[i]]++;
    numOfHitsTwo [hashtableTwo[i]]++;
  }
  
  cout << "Printing the statistics for hashFunction1 with hash table size "
    << tsize << '\n';
  cout << "#hits\t#slots receiving the #hits\n";
  // print number of hits
  for (int i = 0; i < tsize; i++) {
    if (numOfHitsOne[i] != 0) {
      cout << i << '\t' << numOfHitsOne[i] << '\n';
	    // find the max number of hits 
      if (i > maxNumHitsOne){
        maxNumHitsOne = i;
      }
      int tempSum = 0;
      for (int j=i; j < tsize; j++) {
        tempSum = numOfHitsOne[j] + tempSum;
      }
	    // get the sum of steps
      stepSumOne = stepSumOne + i * tempSum;      
    }
  }
  cout << "The average number of steps for a successful search for hash "
    << "function 1 would be " << ((float)stepSumOne/(float)num_words) << '\n';
  cout << "The worst case steps that would be needed to find a word is "
    << maxNumHitsOne << '\n';
 
  cout << "Printing the statistics for hashFunction2 with hash table size "
    << tsize << '\n';
  cout << "#hits\t#slots receiving the #hits\n";
	// print number of hits
  for (int i = 0; i < tsize; i++) {
    if (numOfHitsTwo[i] != 0) {
      cout << i << '\t' << numOfHitsTwo[i]<< '\n';
      // find the max number of hits
      if (i > maxNumHitsTwo){
        maxNumHitsTwo= i;
      }
      int tempSum = 0;
      for (int j=i; j < tsize; j++) {
        tempSum = numOfHitsTwo[j] + tempSum;
      }
      //get the sum of steps
      stepSumTwo = stepSumTwo + i * tempSum;
    }
  }
  cout << "The average number of steps for a successful search for hash "
    << "function 2 would be " << (float)stepSumTwo/(float)num_words <<  '\n';
  cout << "The worst case steps that would be needed to find a word is "
    << maxNumHitsTwo <<'\n';
}