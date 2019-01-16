/* function for bench test running speed of DictionaryTrie, DictionaryBST
   and DictionaryHashtable 's find method. Min_size,step_size, num_iterations 
   and dictfile are the parameters of this function*/
#include "util.h"
#include <iostream>
#include <ctime>
#include <string.h>
#include <fstream>

using namespace std;
/* main function that runs the test */
int main (int argc, char** argv) {
  // argument size
  int argsiz = 5;
  if (argc != argsiz) {
    cerr << "Incorrect number of arguments\n";
    return -1;
  }
  // get the input 1-4 
  int min_size = atoi (argv[1]);
  int step_size = atoi (argv[2]);
  int num_iterations = atoi (argv[3]);
  char * dictfile = argv[4];
  
  // open the file
  ifstream stream;
  stream.open(dictfile, ios::binary);

  // for recording time for all 100 runs
  int many = 100;
  int times_array [many];
  // new DictionaryTrie object
  DictionaryTrie * dt = new DictionaryTrie();
  cout << "DictionaryTrie\n";

  // run num_iterations times
  for (int i=0; i<num_iterations; i++) {
    // calculate num_words
    int num_words = min_size + i * step_size;
    
    // load the num_words into the dictionary
    Utils u;
    u.load_dict (*dt, stream, num_words);
    
    char buffer [BUFSIZ];
    long long time;

    // loop 100 times
    for (int j=0; j<many; j++) {
      // start the time 
      Timer t;
      t.begin_timer();
      // search next 100 words 
      for (int k=0; k<100; k++) {
        stream.getline (buffer, BUFSIZ);
        char *  s = strchr(buffer,' ');
        if (s) {
          dt->find ( s + 1);
        }
      }
      // end timer
      time = t.end_timer();
      times_array[j] = time;
    }
    int totalTime = 0;
    // find average time
    for (int k = 0; k < many; k++) {
      totalTime = totalTime + times_array[k];
    }
    
    float averageTime = (float) totalTime / (float) many;
    // print dictsize and time
    cout << num_words << "\t" << averageTime << "\n";
  }
  // delete heap object
  delete (dt);
  // new DictionaryBST object
  DictionaryBST * d_bst = new DictionaryBST;
  cout << "\nDictionaryBST\n";
  // run num_iterations times
  for (int i=0; i<num_iterations; i++) {
    int num_words = min_size + i * step_size;
    Utils u;
    //load the words
    u.load_dict (*d_bst, stream, num_words);
    
    char buffer [BUFSIZ];
    long long time;
    
    // run 100 times
    for (int j=0; j<many; j++) {
      // start timing
      Timer t;
      t.begin_timer();
      // next 100 words
      for (int k=0; k<100; k++) {
        stream.getline (buffer, BUFSIZ);
        char *  s = strchr(buffer,' ');
        if (s) {
          d_bst->find ( s + 1);
        }
      }
      // end timing
      time = t.end_timer();
      times_array[j] = time;
    }
    int totalTime = 0;
    // find average time
    for (int k = 0; k < many; k++) {
      totalTime = totalTime + times_array[k];
    }
    float averageTime = (float) totalTime / (float) many;
    // print dictsize and time
    cout << num_words << "\t" << averageTime << "\n";
  }
  delete(d_bst);
  
  //new DictionaryHashtable object
  DictionaryHashtable * d_ht = new DictionaryHashtable();
  cout << "\nDictionaryHashtable\n";
  // run num_iterations times
  for (int i=0; i<num_iterations; i++) {
    // calculate num_words
    int num_words = min_size + i * step_size;
    Utils u;
    // load the words
    u.load_dict (*d_ht, stream, num_words);
    
    char buffer [BUFSIZ];
    long long time;
    
    // run 100 times
    for (int j=0; j<many; j++) {
      // start timer 
      Timer t;
      t.begin_timer();
      // load next 100 words
      for (int k=0; k<100; k++) {
        stream.getline (buffer, BUFSIZ);
        char *  s = strchr(buffer,' ');
        if (s) {
          // call find
          d_ht->find ( s + 1);
        }
      }
      // end timer
      time = t.end_timer();
      times_array[j] = time;
    }
    int totalTime = 0;
    // find average time
    for (int k = 0; k < many; k++) {
      totalTime = totalTime + times_array[k];
    }
    float averageTime = (float) totalTime / (float) many;
    // print dictsize and time
    cout << num_words << "\t" << averageTime << "\n";
  }
  delete (d_ht);
  
}