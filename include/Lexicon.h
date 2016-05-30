#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <tuple>
#include <memory>
#include <functional>
 
class Lexicon {
public:
  Lexicon();
  Lexicon(std::istream& input);
  Lexicon(const std::string& filename);
//~Lexicon() { delete root; }
  void add(const std::string& word);
  void addWordsFromFile (std::istream& input);
  void addWordsFromFile (const std::string& filename);
  void clear(); 
  bool contains(const std::string& word) const;
  bool containsPrefix(const std::string& prefix) const;
  //bool equals (const Lexicon& lex2) const;
  bool isEmpty() const;
  void mapAll(std::function<void (std::string)> func) const;
  bool remove (const std::string& word);
  bool removePrefix (const std::string& prefix);
  int size() const;
  std::set<std::string> toSTLSet();
  void toString() const;
  friend std::ostream& operator <<(std::ostream& os, const Lexicon& lex);
  friend std::istream& operator >>(std::istream& is, Lexicon& lex);
  

private:
  typedef struct TrieNode {
    bool isWord;
    std::map<char,std::unique_ptr<TrieNode>> suffixes;
  } TrieNode;
  
  typedef map<char, std::unique_ptrTrieNode>> SuffixesT;
  typedef SuffixesT::iterator SuffixesTItr;
  typedef SuffixesT::const_iterator SuffixesTConstItr;

  typedef struct StackElement {
    TrieNode* tnode;
    string prefix;
    SuffixesTItr curr_suffix;
  } StackElement;

  constTrieNode* findNode(const std::string& str) const;
  
  std::pair<std::map<char, std::unique_ptrTrieNode>>::iterator,TrieNode*> 
  findNodeAndKeyAtPrefix(const std::string& str);
  bool removePrefixHelper(const std::string& prefix, int prefix_index, 
     TrieNode* curr, bool is_prefix);
  bool removeSubtreeTrieNode* curr);
  std::vector<std::string> toSTLHelperTrieNode*TrieNode); 
 TrieNode* ensureNodeExists(const std::string& str);

  /* Members */ 
  std::unique_ptrTrieNode> root_;
  int size_;
};
 
 

