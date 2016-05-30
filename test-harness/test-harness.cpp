/*************************************************
 * File: test-harness.cpp 
 * Author: Mustafa Mohamad (mustafa.mohamad@gmail.com) 
 * adapted from Keith Schwarz's code for unit testing
 *
 * File containing several unit test cases that can be
 * used to verify the correctness of the Lexicon
 * implementation. 
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cstdarg>
#include <set>
#include <exception>
#include <Lexicon.h>
using namespace std;

/* These flags control which tests will be run.   */
#define EmptyLexiconTestEnabled 1
#define BasicLexiconTestEnabled  1


/* Utility function that pauses until the user hits ENTER. */
void PressEnterToContinue() {
  /* Use getline to stall until receiving input. */
  string line;
  getline(cin, line);
}

/* This function is what the test suite uses to ensure that the KDTree works
 * correctly.  It takes as parameters an expression and description, along
 * with a file and line number, then checks whether the condition is true.
 * If so, it prints that the test passed.  Otherwise, it reports that the
 * test fails and points the caller to the proper file and line.
 */
void DoCheckCondition(bool expr, const string& rationale, const string& file, int line) {
  /* It worked!  Congrats. */
  if (expr) {
    cout << "PASS: " << rationale << endl;
    return;
  }

  /* Uh oh!  The test failed! */
  cout << "FAIL: " << rationale << endl;
  cout << "  Error at " << file << ", line " << line << endl;
  cout << "  (ENTER to continue)" << endl;

  /* Pause so that the test fail stands out. */
  PressEnterToContinue();
}

/* Reports that an unexpected error occurred that caused a test to fail. */
void FailTest(const exception& e) {
  cerr << "TEST FAILED: Unexpected exception: " << e.what() << endl;
  PressEnterToContinue();
}

/* This macro takes in an expression and a string, then invokes
 * DoCheckCondition passing in the arguments along with the file
 * and line number on which the macro was called.  This makes it
 * easier to track down the source of bugs if a test case should
 * fail.
 */
#define CheckCondition(expr, rationale) DoCheckCondition(expr, rationale, __FILE__, __LINE__)

/* Utility function to delimit the start and end of test cases. */
void PrintBanner(const string& header) {
  cout << "\nBeginning test: " << header << endl;
  cout << setw(40) << setfill('-') << "" << setfill(' ') << endl;
}

/* Utility function to signal that a test isn't begin run. */
void TestDisabled(const string& header) {
  cout << "== Test " << header << " NOT RUN: press ENTER to continue ==" << endl;

  /* Pause for the user to hit enter. */
  PressEnterToContinue();
}

/* Utility function to signal the end of a test. */
void EndTest() {
  cout << "== end of test: press ENTER to continue ==" << endl;
  PressEnterToContinue();
}

/* Checking operations an on empty Leixcon */
void EmptyLexiconTest() try {
#if EmptyLexiconTestEnabled
  Lexicon lex;
  CheckCondition(true, "Lexicon construction completed");

  /* Check the basic properties of the Lexicon */
  CheckCondition(lex.size() == 0, "New Lexicon has no elements");
  CheckCondition(lex.isEmpty(),   "New Lexicon is empty");

  /* Find and remove methods should all return false for empty lexicon */
  string word = "word";
  CheckCondition(!lex.contains(word),       "Word not found in empty Lexicon");
  CheckCondition(!lex.containsPrefix(word), "Prefix not found in empty Lexicon");
  CheckCondition(!lex.remove(word),         "Word can't be removed from empty Lexicon");
  CheckCondition(!lex.removePrefix(word),   "Prefix can't be removed from empty Lexicon");

  /* Calling clear() on empty Lexicon should not cause any errors */
  lex.clear();

  CheckCondition(true, "Calling clear on empty Lexicon succeeded");

  EndTest();
#else 
  TestDisabled("EmptyLexiconTest");
#endif
} catch (const exception& e) {
  FailTest(e);
}

/* Checking operations on a Lexicon with a few words */
void BasicLexiconTest() try {
#if BasicLexiconTestEnabled
  Lexicon lex;

  /* Add a few words to the Lexicon */
  string words[3] = {"first", "second", "third"};

  for (auto w: words) 
    lex.add(w);

  /* Check Basic properties */
  CheckCondition(lex.size() == 3, "New Lexicon has 3 elements");
  CheckCondition(!lex.isEmpty(),  "New Lexicon is not empty");

  /* Make sure words and prefixes in lexicon can be found */
  for (auto w : words) {
    CheckCondition(lex.contains(w),                   "Lexicon contains the "
        + w + " word");
    CheckCondition(lex.containsPrefix(w.substr(0,3)), "Lexicon contains a " 
        "prefix of the " + w + "word");
  }

  /* Make sure different length prefixes can be found */
  for (auto i: {1,2,3,5}) {
    string prefix = words[0].substr(0,i);
    CheckCondition(lex.containsPrefix(prefix), "Lexicon constains prefix "
        + prefix);
  }

  /* Make sure words not lexicon are not found */
  CheckCondition(!lex.contains("test"),       "Word 'test' not in Lexicon");
  CheckCondition(!lex.contains("fir"),        "Word 'fir' not in Lexicon");
  CheckCondition(!lex.contains("firstblah"),  "Word 'firstblah' not in "
      "Lexicon");

  /* Make sure prefixes not in lexicon are not found */
  CheckCondition(!lex.containsPrefix("te"),   "Prefix 'te' not in Lexicon");
  CheckCondition(!lex.containsPrefix("fiaa"), "Prefix 'fiaa' not in Lexicon");
  
  /* Make sure a correct set can be generated from the Lexicon */
  set<string> lex_set = lex.toSTLSet();
  bool correct_contents = lex_set.find("first") != lex_set.end() &&
    lex_set.find("second") != lex_set.end() && 
    lex_set.find("third") != lex_set.end();
  CheckCondition(lex_set.size() == 3 && correct_contents, "Correct Set" 
      " generated from Lexicon");

  /* Make sure words can be removed  */
  CheckCondition(lex.remove("first"), "Word 'first' removed from Lexicon");
 
  /* Check properties after remove() */
  CheckCondition(lex.size() == 2,     "Lexicon reporting correct size after "
      "removal");
  lex_set = lex.toSTLSet();
  correct_contents = lex_set.find("second") != lex_set.end() && 
    lex_set.find("third") != lex_set.end();;
  CheckCondition(correct_contents, "Correct lexicon contents after remove");

  /* Make sure we can remove prefixes */
  lex.add("first");
  CheckCondition(lex.removePrefix("fi"), "Prefix 'fi' removed");

  /* Check properties after removePrefix() */
  CheckCondition(lex.size() == 2, "Lexicon has correct size");
  lex_set = lex.toSTLSet();
  correct_contents = lex_set.find("second") != lex_set.end() && 
    lex_set.find("third") != lex_set.end() &&
    lex_set.find("first") == lex_set.end();
  CheckCondition(correct_contents, "Correct Lexicon contents after removePrefix");
  
  /* Make sure clearing non-empty lexicon works */
  lex.clear();
  CheckCondition(lex.size() == 0,         "Lexicon has correct size after clear()");
  CheckCondition(lex.isEmpty(),           "Lexicon is empty after clear()");
  CheckCondition(lex.toSTLSet().empty(),  "Lexicon has correct contents after clear()");

  EndTest();
#else
  TestDisabled("BasicLexiconTest");
#endif
} catch (const exception& e) {
  FailTest(e);
}

//void BasicLexiconTest() try {
//#if BasicLexiconTestEnabled
//
//  EndTest();
//#else
//  TestDisabled("BasicLexiconTest");
//#endif
//} catch (const exception& e) {
//  FailTest(e);
//}
//
//void BasicLexiconTest() try {
//#if BasicLexiconTestEnabled
//
//  EndTest();
//#else
//  TestDisabled("BasicLexiconTest");
//#endif
//} catch (const exception& e) {
//  FailTest(e);
//}



/* Main entry point simply runs all the tests.  Note that these functions might be no-ops
 * if they are disabled by the configuration settings at the top of the program.
 */
int main() {
  EmptyLexiconTest();
  BasicLexiconTest();
#if (BasicKDTreeTestEnabled && \
     ModerateKDTreeTestEnabled && \
     HarderKDTreeTestEnabled &&   \
     EdgeCaseKDTreeTestEnabled && \
     MutatingKDTreeTestEnabled && \
     ThrowingKDTreeTestEnabled &&  \
     NearestNeighborTestEnabled &&  \
     MoreNearestNeighborTestEnabled && \
     BasicCopyTestEnabled && \
     ModerateCopyTestEnabled)
  cout << "All tests completed!  If they passed, you should be good to go!" << endl << endl;
#else
  cout << "Not all tests were run.  Enable the rest of the tests, then run again." << endl << endl;
#endif
  
  PressEnterToContinue();
}
