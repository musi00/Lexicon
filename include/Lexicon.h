#ifndef LEXICON_H_
#define LEXICON_H_

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <stack>
#include <utility>
#include <memory>
#include <functional>
 
/** \brief An interface representing a Lexicon, or a word list.  The class 
 * supports efficient look up operation on words and prefixes.  It is 
 * implemented using the prefix tree data structure (also referred to as trie)
 *
 * \author Mustafa Mohamad
 */
class Lexicon {
public:
  Lexicon();

  /** \brief Constructor that creates a lexicon from words in an input stream
   *  \param[in] input the input stream of words seperated by a new line
   */
  Lexicon(std::istream& input);

  /** \brief Constructor that creates a lexicon from words stored in a file
   *  \param[in] filename the name of the file
   */
  Lexicon(const std::string& filename);
  
  /** \brief add a word to the lexicon
   *  \param[in] word the word to be added
   */
  void add(const std::string& word);
  
  /**
   * \brief add words to lexicon from an input stream
   * \param[in] input the input stream
   */
  void addWordsFromFile (std::istream& input);

  /** \brief add words stored in a file to the lexicon
   *  \param[in] filename the name of the file
   */
  void addWordsFromFile (const std::string& filename);
  
  /** \brief delete all words from the lexicon */
  void clear(); 

  /** \brief returns whether the lexicon contains a word
   *  \param[in] word the query word
   */
  bool contains(const std::string& word) const;

  /** \brief returns whether the lexicon contains a prefix
   *  \return true if prefix is in lexicon
   */
  bool containsPrefix(const std::string& prefix) const;

  /** \brief returns whether the lexicon contains any words */
  bool isEmpty() const;

  /** \brief apply a function to all words in the lexicon
   *  \param[in] the function to be applied
   */
  void mapAll(std::function<void (std::string)> func) const;

  /** \brief remove a word from the lexicon
   *  \param[in] word the word to be removed
   *  \return true if the word was removed
   */
  bool remove (const std::string& word);
  
  /** \brief remove a word from the lexicon
   *  \param[in] word the word to be removed
   *  \return true if the word was removed
   */
  bool removePrefix (const std::string& prefix);

  /** \brief returns number of words in lexicon */
  size_t size() const;

  /** \brief return an stl set of all words in lexicon in sorted order */
  std::set<std::string> toSTLSet();

  /** \brief return a comma-separated string of all words in lexicon */
  std::string toString() const;

  friend std::ostream& operator <<(std::ostream& os, const Lexicon& lex);

private:
  /** \brief the node of the prefix tree. */
  typedef struct TrieNode {
    bool isWord;
    std::map<char,std::unique_ptr<TrieNode>> suffixes;
  } TrieNode;
  
  typedef std::map<char, std::unique_ptr<TrieNode>> SuffixesT;
  typedef SuffixesT::iterator SuffixesTItr;
  typedef SuffixesT::const_iterator SuffixesTConstItr;

  /** \brief a type used to populate an explicit stack for the removeSubtree recursive
   * function.  It captures the current node being processed, which suffix in that node
   * is currently being processed, and the prefix formed so far by the current traversal
   * path
   */
  typedef struct StackElement {
    TrieNode* node;
    std::string prefix;
    SuffixesTItr suffix;
  } StackElement;

  /** \brief return the TrieNode whose path forms the input string if found
   *  \param[in] str the input string
   *  \return pointer to TrieNode or nullptr if not found
   */
  const TrieNode* findNode(const std::string& str) const;
  
 
  /** \brief Helper function for removePrefix
   *  \param[in] prefix the word or prefix to be removed
   *  \param[in] prefix_index the position of the letter currently being processed
   *  \param[in] curr the current TrieNode being processed
   *  \param[in] is_prefix whether the input prefix is a complete word or just a prefix
   *  \return whether the prefix was successfully removed
   */
  bool removePrefixHelper(const std::string& prefix, int prefix_index, 
     TrieNode* curr, bool is_prefix);

  /** \brief Remove a substree from the prefix tree
   *  \param[in] delete_stack_ptr the explicit recursion stack.  See StackElement for
   *  more details
   */
  void removeSubtree(std::stack<StackElement>* delete_stack_ptr);
  
  /** \brief Check whether there is a path in the prefix tree that forms the input 
   *  string. This method creates new nodes to form this path if they don't exist
   *  \param[in] str the input string
   *  \return A pointer to the TrieNode at the end of the path forming str
   */
  TrieNode* ensureNodeExists(const std::string& str);

  /* \brief the root of the prefix tree */ 
  std::unique_ptr<TrieNode> root_;

  /* \brief number of words in the lexicon */
  int size_;

  /* \brief a set of all valid words in the lexicon */ 
  std::set<std::string> word_set_;
};
 
#endif
