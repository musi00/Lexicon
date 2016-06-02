#include <iostream>
#include <string>
#include <map>
#include <set>
#include <stack>
#include <utility>
#include <tuple>
#include <memory>
#include <functional>
 
class Lexicon {
public:
  Lexicon();
  Lexicon(std::istream& input);
  Lexicon(const std::string& filename);
  void add(const std::string& word);
  void addWordsFromFile (std::istream& input);
  void addWordsFromFile (const std::string& filename);
  void clear(); 
  bool contains(const std::string& word) const;
  bool containsPrefix(const std::string& prefix) const;
  bool isEmpty() const;
  void mapAll(std::function<void (std::string)> func) const;
  bool remove (const std::string& word);
  bool removePrefix (const std::string& prefix);
  int size() const;
  std::set<std::string> toSTLSet();
  std::string toString() const;
  friend std::ostream& operator <<(std::ostream& os, const Lexicon& lex);

private:
  typedef struct TrieNode {
    bool isWord;
    std::map<char,std::unique_ptr<TrieNode>> suffixes;
  } TrieNode;
  
  typedef std::map<char, std::unique_ptr<TrieNode>> SuffixesT;
  typedef SuffixesT::iterator SuffixesTItr;
  typedef SuffixesT::const_iterator SuffixesTConstItr;

  typedef struct StackElement {
    TrieNode* node;
    std::string prefix;
    SuffixesTItr suffix;
  } StackElement;

  const TrieNode* findNode(const std::string& str) const;
  
  std::pair<std::map<char, std::unique_ptr<TrieNode>>::iterator, TrieNode*> 
  findNodeAndKeyAtPrefix(const std::string& str);
  bool removePrefixHelper(const std::string& prefix, int prefix_index, 
     TrieNode* curr, bool is_prefix);
  void removeSubtree(std::stack<StackElement>* delete_stack_ptr);
  //std::vector<std::string> toSTLHelper(TrieNode* curr); 
  TrieNode* ensureNodeExists(const std::string& str);

  /* Members */ 
  std::unique_ptr<TrieNode> root_;
  int size_;
  std::set<std::string> word_set_;
};
 
 

