#include <Lexicon.h>
#include <fstream>
#include <sstream>

using namespace std;

Lexicon::Lexicon() :
  root_(nullptr),
  size_(0)
{}

void Lexicon::add(const string& word) {
 ensureNodeExists(word)->isWord = true;
 word_set_.insert(word);
 ++size_;
}
 
void Lexicon::addWordsFromFile (std::istream& input) {
  string word;
  while(getline(input,word)) {
    add(word);
  }
}

void Lexicon::addWordsFromFile (const std::string& filename) {
  ifstream file(filename);
  if (file)
    addWordsFromFile(file);
  file.close();
}

void Lexicon::clear() {
  root_.reset(nullptr);
  word_set_.clear();
  size_ = 0;
} 
 
bool Lexicon::containsPrefix(const string& prefix) const {
  return findNode(prefix) != nullptr;
}
 
bool Lexicon::contains(const string& word) const {
 const TrieNode* found = findNode(word);
 return found != nullptr && found->isWord;
}

bool Lexicon::isEmpty() const {
  return (size() == 0);
}

void Lexicon::mapAll(function<void (string)> func) const {}

bool Lexicon::remove (const string& word) {
  return removePrefixHelper(word, 0, root_.get(), false);
}

bool Lexicon::removePrefix (const string& prefix) {
  return removePrefixHelper(prefix, 0, root_.get(), true);
}

int Lexicon::size() const {
  return size_;
}

set<string> Lexicon::toSTLSet() {
  return word_set_;
}

string Lexicon::toString() const {
  string lexicon_str;
  for (auto& word:word_set_) {
    lexicon_str += "{" + word + "}, ";
  }
  return lexicon_str.substr(0,lexicon_str.length()-2);
}

ostream& operator <<(ostream& out, const Lexicon& lex) {
  string lexicon_str = lex.toString();
  out << lexicon_str;;
  return out;
}

bool Lexicon::removePrefixHelper(const string& prefix, int prefix_index, 
    TrieNode* curr, bool isPrefix) {
  /* curr only becomes null when we cannot find the prefix */
  if (curr != nullptr) {
    SuffixesTItr suffixes_itr = curr->suffixes.find(prefix[prefix_index]);
    /* if current prefix letter is not found then quit recursion and return false */
    if (suffixes_itr != curr->suffixes.end()) {
      /* base case: we have matched the prefix or word to remove */
      /* curr is now at the node that contains the last letter in the prefix */
      if (prefix_index == prefix.size() - 1) {
        /* if..else logic only removes the word from the tree if it is not a prefix
         * when it is a prefix the entire substree of the prefix is removed */
        if (!isPrefix) {
          /* if word is a leaf then remove it, otherwise simply mark it as not a word.
           * When not a leaf it means that it is also a prefix
           * for bigger words (e.g cat is a prefix for catepillar). That is why we 
           * can't remove it */
          if (suffixes_itr->second->suffixes.empty())
            curr->suffixes.erase(suffixes_itr);
          else
            suffixes_itr->second->isWord = false;
          /* in both cases the word needs to be removed from the word set */
          word_set_.erase(prefix);
          --size_;
        }
        else { /* isPrefix */
          stack<StackElement> delete_stack;;
          char first_suffix = suffixes_itr->second->suffixes.begin()->first;
          delete_stack.push({suffixes_itr->second.get(), 
              prefix + first_suffix,
              suffixes_itr->second->suffixes.begin()});
          /* Remove subtree of current suffix (this updates size and word_set_) */
          removeSubtree(&delete_stack);
          /* After removeSubstree node at at last letter of prefix is empty.
           * Therefore the last letter of the prefix must also be removed
           * from the tree */
          curr->suffixes.erase(suffixes_itr);
        }
        return true;
      }
      /* recursive case: advance prefix position and traverse deeper into the tree */
      if (removePrefixHelper(prefix, ++prefix_index, suffixes_itr->second.get(),
          isPrefix)) {
        /* if this node is now a leaf then we can remove this node as well */
        if (suffixes_itr->second->suffixes.empty())
          curr->suffixes.erase(suffixes_itr);
        return true;
      }
    }
  }
  return false;
}

/* tail recursive implementation using an explicit stack*/
void Lexicon::removeSubtree (stack<StackElement>* delete_stack_ptr) {
  /* it's easier to work with a reference */
  stack<StackElement>& delete_stack = *delete_stack_ptr;
  /* when the delete stack is empty we are done */
  if (!delete_stack.empty()) {

    StackElement stack_top = delete_stack.top();

    /* Base case: TrieNode of current suffix is empty */
    if (stack_top.suffix->second->suffixes.empty()) {
      /* if this prefix was a word then delete it */
      if (stack_top.suffix->second->isWord) {
        word_set_.erase(stack_top.prefix);
        --size_;
      }
      /* delete this suffix from current node and pop() element from stack */
      SuffixesTItr next_suffix = stack_top.node->suffixes.erase(stack_top.suffix);
      delete_stack.pop();
      /* if there is a next suffix in current node, push onto the stack */
      if (next_suffix != stack_top.node->suffixes.end()) {
        string next_prefix = stack_top.prefix.substr(0,stack_top.prefix.length()-1)
          + next_suffix->first;
        delete_stack.push({stack_top.node, next_prefix, next_suffix});
      }
    }
    /* Recusive case: TrieNode of current suffix has suffixes of its own */
    else {
      /* push child info onto the stack to move deeper into the prefix tree */
      TrieNode* next_node = stack_top.suffix->second.get();
      SuffixesTItr next_suffix = next_node->suffixes.begin();
      string next_prefix = stack_top.prefix + next_suffix->first;
      delete_stack.push({next_node, next_prefix, next_suffix});
    }

    /* tail recursive call to remove subtree */
    removeSubtree(&delete_stack);
  }
}

const Lexicon::TrieNode* Lexicon::findNode(const std::string& str) const {
  TrieNode* curr = root_.get();
  for (int i = 0; i < str.size() && curr; ++i) {
    char c = str[i];
    map<char, unique_ptr<TrieNode>>::const_iterator suffixes_itr = curr->suffixes.find(c);    
    curr = suffixes_itr != curr->suffixes.end() ? suffixes_itr->second.get() : nullptr;
  }
  return curr;;
}

pair<map<char, unique_ptr<Lexicon::TrieNode>>::iterator, Lexicon::TrieNode*> 
Lexicon::findNodeAndKeyAtPrefix(const std::string& str) {
  TrieNode* parent = nullptr;
  TrieNode* curr = root_.get();
  map<char, unique_ptr<TrieNode>>::iterator prefix_itr;
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
 
Lexicon::TrieNode* Lexicon::ensureNodeExists(const std::string& str) {
  if (!root_)
    root_.reset(new TrieNode);
 
  TrieNode* curr = root_.get();
  for (char c: str) {
    /* if current character is not found in the node, add the character to this node
     * and create a child node for this character */
    map<char,unique_ptr<TrieNode>>::iterator map_itr = curr->suffixes.find(c);
    if (map_itr == curr->suffixes.end()) 
      curr->suffixes[c] = unique_ptr<TrieNode> (new TrieNode);
    
    /* move down the tree */
    curr = curr->suffixes.at(c).get();
  }
  return curr;
}

//vector<string> Lexicon::toSTLHelper(TrieNode* curr) {
//  vector<string> node_words;
//  for (auto& key_value : curr->suffixes) {
//    vector<string> child_words = toSTLHelper(key_value.second.get());
//    for (auto& cw : child_words) {
//      node_words.emplace_back(key_value.first + cw);
//    }
//    /* when you reach the deepest node, simply add the current letter onto the
//     * the list of words */
//    if (child_words.empty()) {
//      string s = "";
//      s += key_value.first;
//      node_words.emplace_back(s);
//    }
//  }
//  return node_words;
//}

//istream& operator >>(istream& in, Lexicon& lex) {}


