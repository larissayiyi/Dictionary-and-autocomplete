/* DictionaryTrie is implemented with a ternary search tries structure and 
   we also implemented an autocomplete method from provided prefixes */
#include "util.h"
#include "DictionaryTrie.h"
#include <iostream>
#include <stack>
#include <ctype.h>
using namespace std;

/* Node constructor for letters that make up words in the dictionary */
Node::Node(char letter)
{
  // the character stored in the node
  this -> letter = letter;
  this -> left = NULL;
  this -> right = NULL;
  this -> center = NULL;
  this -> parent = NULL;
  // whether it is a word node 
  this -> isWord = false;
  // frequency of the word
  this -> freq = 0;
}

/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie()
{

  this -> root = NULL;

}

/* Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string) */
bool DictionaryTrie::insert(std::string word, unsigned int freq)
{

  Node * curr;  // used to find where to insert letters
  char letter = word.at(0);  // go through the word one letter at a time

  // if root does not exist
  if (!root) {
    // add a new root
    root = new Node (letter);
    // start from the root
    curr = root;
  }
  else {
    // start from the root;
    curr = root;
    // while letters are not the same
    while (letter != curr -> letter) {
      // go left or right accordingly if the node does not exist then make new
      // node
      if (letter < curr -> letter) {
        if (! (curr -> left)) {
          curr -> left = new Node (letter);
          curr -> left ->parent = curr;
        }
        curr = curr -> left;
      }
      else {
        if (! (curr -> right)) {
          curr -> right = new Node (letter);
          curr -> right -> parent = curr; 
        }
        curr = curr -> right;
      }
    }
  }
  // for letters after root
  for (unsigned int i = 1; i < word.length(); i++) {
    // loop through the letters
    char letter = word.at(i);
    // if there is no center node, make new center node
    // if they are equal then go down to the center
    if (! (curr -> center)) {
      curr -> center = new Node(letter);
      curr -> center -> parent = curr;
      curr = curr -> center;
    }

    else {
      // go down to the center 
      curr = curr -> center;
      // while the letters dont match 
      while (letter != curr -> letter) {
        // go left or right and make new nodes as necessary
        if (letter < curr -> letter) {
          if (! (curr -> left)) {
            curr -> left = new Node (letter);
            curr -> left -> parent = curr;
          }
          curr = curr -> left;
        }
        if (letter > curr -> letter) {
          if (! (curr -> right)) {
            curr -> right = new Node (letter);
            curr -> right -> parent = curr;
          }
          curr = curr -> right;
        }
      }
    }
  }
  // if the word exist then insertion failed
  if (curr -> isWord) {
    return false;
  }
  // add the freqency to the word
  curr -> freq = freq;
  // isWord is now true
  curr -> isWord = true;
  // succesful insertion
  return true;
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryTrie::find(std::string word) const
{

  Node * curr;  // used to find letters
  char letter = word.at(0);  // starting from the first letter
  // if the Trie doesnt exist
  if (!root) {
    return false;
  }
  else {
    // starting from the root
    curr = root;
    // while current 
    while (letter != curr -> letter) {
      // either go left or right or return false if left or right node doesnt
      // exist
      if (letter < curr -> letter) {
        if (! (curr -> left))
          return false;
        curr = curr -> left;
      }
      else {
        if (! (curr -> right))
          return false;
        curr = curr -> right;
      }
    }
  }

  // for all the nodes after the first
  for (unsigned int i = 1; i < word.length(); i++) {
    // go through every letter
    char letter = word.at(i);
    // if center child doesnt exist return false
    if (! (curr -> center)) {
      return false;
    }

    else {
      // go to center
      curr = curr -> center;
      // while not equal to the letter
      while (letter != curr -> letter) {
        // either go left or right or return false if the child does not exist
        if (letter < curr -> letter) {
          if (! (curr -> left))
            return false;
          curr = curr -> left;
        }
        if (letter > curr -> letter) {
          if (! (curr -> right))
            return false;
          curr = curr -> right;
        }
      }
    }
  }
  // return isWord on the last node which indicates whether it's a word in the
  // tries or not
  return curr -> isWord;

}

/* Return up to num_completions of the most frequent completions
 * of the prefix, such that the completions are words in the dictionary.
 * These completions should be listed from most frequent to least.
 * If there are fewer than num_completions legal completions, this
 * function returns a vector with as many completions as possible.
 * If no completions exist, then the function returns a vector of size 0.
 * The prefix itself might be included in the returned words if the prefix
 * is a word (and is among the num_completions most frequent completions
 * of the prefix)
 */
std::vector<std::string> DictionaryTrie::predictCompletions(std::string prefix,
                                                            unsigned int 
                                                            num_completions)
{
  // vector we will be using to return our list of words
  vector<std::string> words;
  vector<Node *> isWordNodes;
  // temporary pointer to nodes that are words
  Node * temp;
  // used to reverse the order of letters in the words
  stack<char> letterStack;
  // for storing the characters to make strings of words
  char buffer [BUFSIZ];
  // string to store in the vector
  string newWord;
  int count;
  // check if its a valid input and the number of words is not zero
  if ( prefix.empty() ) {
    // error message
    cout << "Invalid Input. Please retry with correct input";
    // return empty vector
    return words;
  }
  // check for any invalid characters
  for (unsigned int i = 0; i < prefix.length(); i++) {
    if (!isalpha(prefix.at(i)) && prefix.at(i) != ' ') {
      // error message
      cout << "Invalid Input. Please retry with correct input";
      // return empty vector
      return words;
    }
  }
  
  Node * curr;  // used to find letters
  char letter = prefix.at(0);  // go through the word one letter at a time
  // this is similar to find method
  // if the trie doesn't exist
  if (!root) {
    // return empty vector
    return words;
  }
  else {
    // starting from the root
    curr = root;
    // if not equal
    while (letter != curr -> letter) {
      // go to either left or right or return empty array if they dont exist
      if (letter < curr -> letter) {
        if (! (curr -> left))
          return words;
        curr = curr -> left;
      }
      else {
        if (! (curr -> right))
          return words;
        curr = curr -> right;
      }
    }
  }
  // after finding the first character
  for (unsigned int i = 1; i < prefix.length(); i++) {
    // go through each character
    char letter = prefix.at(i);
    // if there is no center node return empty vector
    if (! (curr -> center)) {
      return words;
    }
    else {
      // go to the center
      curr = curr -> center;
      // if not equal either go left or right
      while (letter != curr -> letter) {
        if (letter < curr -> letter) {
          if (! (curr -> left))
            return words;
          curr = curr -> left;
        }
        if (letter > curr -> letter) {
          if (! (curr -> right))
            return words;
          curr = curr -> right;
        }
      }
    }
  }
  // now we found where the last node is
  // check if it is a word
  if (curr -> isWord) {
    // add to the node * vector for sorting later
    isWordNodes.push_back(curr);
  }


  // go down to the center and call our helper method to get all the isWord
  // node
  if (curr -> center) {
    findWords(curr -> center, &isWordNodes);
  }

  unsigned int size = isWordNodes.size();
  if (num_completions < isWordNodes.size()) {
    size = num_completions;
  }

  // we would like to sort the vector of node pointers by their frequency
  // bubble sort
  for (unsigned int i = 0; i < size; i++) {
    unsigned int max = i;
    for (unsigned int j = i+1; j < isWordNodes.size(); j++) {
      if (isWordNodes[j]->freq > isWordNodes[max]->freq) {
        max = j;
      }
    }
    Node * temp = isWordNodes[i];
    isWordNodes[i] = isWordNodes[max];
    isWordNodes[max] = temp;
  }

  
  // for the largest num_completions numbers of node pointers in the vector we
  // will turn them into words and insert them into the vector
  for (unsigned int i = 0; i < size; i++) {
    // loop through the pointers
    temp = isWordNodes[i];
    // add the node to the stack
    letterStack.push(temp -> letter);
    // while they have parents
    while (temp -> parent) {
      // if they are the parent's center child
      if (temp -> parent ->center == temp){
        // add the letter to the stack
        letterStack.push(temp -> parent -> letter);
      }      
      // now temp is parent
      temp = temp -> parent;
    }
    // retart the count
    count = 0;
    // now we going to empty the stack to the buffer
    while (!letterStack.empty()) {
      // add letter to the buffer
      buffer[count] = letterStack.top();
      // remove from the stack
      letterStack.pop();
      // increment
      count++;
    }
    buffer[count] = '\0';
    // copy the letter and make into a string
    newWord = buffer;
    // add new word to the vector
    words.push_back(newWord);
    
  }
  return words;
}

/* helper method for predictCompletions to find every isWord node by calling 
   itself recursively and add the node pointer to the node pointer vector
 */
void DictionaryTrie::findWords(Node * node, vector <Node *> * isWordNodes){
  // add node if it is a word node
  if (node -> isWord) {
    isWordNodes -> push_back(node);
  }
  // call from its left child
  if (node -> left) {
    findWords(node -> left, isWordNodes);
  }
  // call from its center child
  if (node -> center) {
    findWords(node -> center, isWordNodes);
  }
  // call from its right child
  if (node -> right) {
    findWords(node -> right, isWordNodes);
  }
  return;
}
/* Destructor */
DictionaryTrie::~DictionaryTrie()
{
  deleteAllNodes (root);
}

/* call bt the destructor to delete the nodes recursively */
void DictionaryTrie::deleteAllNodes(Node * n)
{
  if (!n) {
    return;
  }
  
  if (n -> left) {
    deleteAllNodes (n -> left);
  }
  if (n -> center) {
    deleteAllNodes (n -> center);
  }
  if (n -> right) {
    deleteAllNodes (n -> right);
  }

  delete n;

}
