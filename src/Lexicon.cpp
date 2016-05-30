#include <Lexicon.h>
using namespace std;

Lexicon::Lexicon() :
  root_(nullptr),
  size_(0)
{}

void Lexicon::add(const string& word) {
 ensureNodeExists(word)->isWord = true;
 ++size_;
}
 
void Lexicon::addWordsFromFile (std::istream& input) {}
void Lexicon::addWordsFromFile (const std::string& filename) {}
void Lexicon::clear() {} 
 
bool Lexicon::containsPrefix(const string& prefix) const {
  return findNode(prefix) != nullptr;
}
 
bool Lexicon::contains(const string& word) const {
 const node *found = findNode(word);
 return found != nullptr && found->isWord;
}

bool Lexicon::isEmpty() const {
  return (size() == 0);
}

void Lexicon::mapAll(function<void (string)> func) const {}

bool Lexicon::remove (const string& word) {
  return removePrefix(word);
}

bool Lexicon::removePrefix (const string& prefix) {
  /* In order to remove all words with a prefix:
   * Step 1: the (key,node) pair stored at  last character of the prefix must 
   * be removed from the node containing it.
   * Step 2: all other prefix letters */
/*  map<char, unique_ptr<node>>::iterator key_at_prefix;
  node* node_containing_key;;
  tie(key_at_prefix, node_containing_key) = findNodeAndKeyAtPrefix(prefix);
  if (node_containing_key) {
    size_ -= node_containing_key->suffixes.size();
    node_containing_key->suffixes.erase(key_at_prefix);
    return true;
  }
  return false;*/
  return removePrefixHelper(prefix, 0, root_.get());
}

bool Lexicon::removePrefixHelper(const string& prefix, int prefix_index, 
    node* curr, bool isPrefix) {
  /* curr only becomes null when we cannot find the prefix */
  if (curr != nullptr) {
    map<char, unique_ptr<node>>::iterator suffixes_itr = 
      curr->suffixes.find(prefix[prefix_index]);
    /* if current prefix letter is not found then quit recursion and return false */
    if (suffixes_itr != curr->suffixes.end()) {
      /* base case: we have matched the prefix or word to remove */
      /* curr is now at the node that contains the last letter in the prefix */
      if (prefix_index == prefix.size() - 1) {
        /* if..else logic only removes the word from the tree if it is not a prefix
         * when it is a prefix the entire substree of the prefix is removed */
        if (!isPrefix) {
          /* if word is a leaf then remove it, otherwise simply mark it as not a word
           * when it is not a leaf it means that it is a word but it is also a prefix
           * for bigger words (e.g cat is a prefix for catepillar). That is why we 
           * can't remove it */
          if (suffixes_itr->second->suffixes.empty())
            curr->suffixes.erase(suffixes_itr);
          else
            suffixes_itr->second->isWord = false;
          words_set_.remove(prefix);
          --size_;
        }
        else { //isPrefix
          stack<pair<node*,SuffixTItr>> delete_stack;;
          delete_stack..push(make_pair(suffix_itr->second.get(),
                suffix_itr->second->suffixes.begin()));
          char first_node_suffix = suffix_itr->second->suffixes.begin()->first;
          /* Remove subtree at current suffix (this updates size and word_set) */
          removeSubtree(prefix + first_node_suffix, &delete_stack);
          /* After removeSubstree node at at last letter of prefix is empty.
           * Therefore the last letter of the prefix must also be removed
           * from the tree */
          curr->suffixes.erase(suffix_itr);
        }
        return true;
      }
      /* recursive case: advance prefix position and traverse the rest */
      if (removePrefixHelper(prefix, ++prefix_index, suffixes_itr->second.get())) {
        /* if this node is now a leaf then we can remove this node as well */
        if (suffixes_itr->second->suffixes.empty())
          curr->suffixes.erase(suffixes_itr);
        return true;
      }
    }
  }
  return false;
}

/* tail recursive implementation using a stack variable*/
bool Lexicon::removeSubtree (const string& prefix, stack<node*, SuffixTItr>* delete_stack) {
  /* it's easier to work with a reference */
  stack<node*, SuffixTItr>& delete_stack = *delete_stack_ptr;
  node* curr = delete_stack.top().first;
  SuffixTItr suffix_to_remove = delete_stack.top().second;
  /* base case: curr node is a leaf so delete all its ancestor prefixes*/
  if (curr->suffixes.empty()) {
    delete_stack.pop();
    words_set_.remove(prefix);
    --size_;
    
    while (!delete_stack..empty()) {
      curr = nodes_to_delete.top().first;

      curr->suffixes.clear();
      nodes_to_delete.pop();
    }
  }
  
  /* recursive case: curr not is not a leaf so add all its children onto the stack
   * and traverse the tree */
  SuffixesTItr suffixes_itr = curr->suffixes.begin();
  while (suffixes_itr != curr->suffixes.end()) {
    char suffix = suffixes_itr.first;
    delete_stack.push(make_pair(curr, suffixes_itr));
    removeSubtree (prefix + suffix, delete_stack);
    ++suffixes_itr;
  }


}

/*void Lexicon::mergeNodes (node* target, node* src) {
  // iterate over source key,value pairs 
  for (auto& src_kv : src.suffixes) {
    map<char, unique_ptr<node>>::iterator target_itr;
    target_itr = target.find(src_kv.first);
    if (target_itr == target.end()) {
      target.suffixes[src_kv.first] = move(src_kv.second);
    }
    else {
      mergeNodes(target_itr->second.get(), src_kv.second.get());
    }
  }

  // clear the node 
  src->suffixes.clear();
}*/

int Lexicon::size() const {
  return size_;
}

set<string> Lexicon::toSTLSet() {
  vector<string> word_list = toSTLHelper(root_.get());
  /* convert vector to set without copying elements */
  set<string> word_set (make_move_iterator(word_list.begin()),
      make_move_iterator(word_list.end()));

  return word_set;
}

vector<string> Lexicon::toSTLHelper(node* node) {
  vector<string> node_words;
  for (auto& key_value : node->suffixes) {
    vector<string> child_words = toSTLHelper(key_value.second.get());
    for (auto& cw : child_words) {
      node_words.emplace_back(key_value.first + cw);
    }
    /* when you reach the deepest node, simply add the current letter onto the
     * the list of words */
    if (child_words.empty()) {
      string s = "";
      s += key_value.first;
      node_words.emplace_back(s);
    }
  }
  return node_words;
}

void Lexicon::toString() const {}

ostream& operator <<(std::ostream& os, const Lexicon& lex) {}

istream& operator >>(std::istream& is, Lexicon& lex) {}

const Lexicon::node* Lexicon::findNode(const std::string& str) const {
  node* curr = root_.get();
  for (int i = 0; i < str.size() && curr; ++i) {
    char c = str[i];
    map<char, unique_ptr<node>>::const_iterator suffixes_itr = curr->suffixes.find(c);    
    curr = suffixes_itr != curr->suffixes.end() ? suffixes_itr->second.get() : nullptr;
  }
  return curr;;
}

pair<map<char, unique_ptr<Lexicon::node>>::iterator, Lexicon::node*> 
Lexicon::findNodeAndKeyAtPrefix(const std::string& str) {
  node* parent = nullptr;
  node* curr = root_.get();
  map<char, unique_ptr<node>>::iterator prefix_itr;
  for (int i = 0; i < str.size() && curr; ++i) {
    char c = str[i];
    prefix_itr = curr->suffixes.find(c);
    if (prefix_itr != curr->suffixes.end()) {
      parent = curr;
      curr = prefix_itr->second.get();
    } 
    else {
      curr = nullptr;
    }
  }

  return curr ? make_pair(prefix_itr, parent) : make_pair(prefix_itr, nullptr);
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
      curr->suffixes[c] = unique_ptr<node> (new node);
    
    /* move down the tree */
    curr = curr->suffixes.at(c).get();
  }
  return curr;
}

