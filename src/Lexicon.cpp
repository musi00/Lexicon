#include<Lexicon.h>

void Lexicon::add(const string& word) {
 ensureNodeExists(word)->isWord = true;
}
 
bool Lexicon::containsPrefix(const string& prefix) const {
  return findNode(prefix) != nullptr;
}
 
bool Lexicon::contains(const string& word) const {
 const node *found = findNode(word);
 return found != nullptr && found->isWord;
}
 
const  Lexicon::node* Lexicon::findNode(const std::string& str) const {
  node* curr = root_.get();
  for (char c : str) {
    curr = curr->suffixes.at(c).get();
    if (!curr)
      return nullptr;
  }
  return curr;
}
 
Lexicon::node* Lexicon::ensureNodeExists(const std::string& str) {
  if (!root_)
    root_.reset(new node);
 
  node* curr = root_.get();
  for (char c: str) {
    /* if current character is not found in the node, add the character to this node
     * and create a child node for this character */
    map<char,unique_ptr<node>>::iterator map_itr = curr->suffixes.find(c);
    if (map_itr == curr->suffixes.end()) 
      curr->suffixes.at(c) = unique_ptr<node> (new node);
    
    /* move down the tree */
    curr = curr->suffixes.at(c).get();
  }
  return curr;
}

