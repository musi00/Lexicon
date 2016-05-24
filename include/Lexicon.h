#include <iostream>
#include <map>
#include <memory>
#include <functional>
using namespace std;
 
class Lexicon {
public:
  Lexicon() { root_.reset(nullptr); }
  Lexicon(std::istream& input);
  Lexicon(const std::string& filename);
//~Lexicon() { delete root; }
  void add(const std::string& word);
  void addWordsFromFile (std::istream& input);
  void addWordsFromFile (const std::string& filename);
  void clear(); 
  bool contains(const std::string& word) const;
  bool containsPrefix(const std::string& prefix) const;
  bool equals (const Lexicon& lex2) const;
  bool isEmpty() const;
  void mapAll(std::function<(std::string)> func) const;
  bool remove (const std::string& word);
  bool removePrefix (const std::string& prefix);
  int size() const;
  std::set<std::string> toSTLSet();
  void toString const;
  friend std::ostream& operator <<(std::ostream& os, const Lexicon& lex);
  friend std::istream& operator >>(std::istream& is, Lexicon& lex);
  

private:
  typedef struct node {
    bool isWord;
    map<char,unique_ptr<node>> suffixes;
  } node;
 
  unique_ptr<node> root_;
  const  node *findNode(const std::string& str) const;
  node *ensureNodeExists(const std::string& str);
};
 
 

