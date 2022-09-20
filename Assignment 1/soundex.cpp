/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 *
 * The function did not consider the first letter
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}
/* This function is intended to encode input letter
 * as a digit using the table provided
 */

string transCodes(char s){
    if ('a'<=s && s<='z'){
        s -= 32;
    }
    Vector<string> dict = {"AEIOUHWY","BFPV","CGJKQSXZ","DT","L","MN","R"};
    for (int i = 0; i < dict.size(); i++){
        if (dict[i].find(s) != string::npos){
            return to_string(i);
        }
    }
    return 0;
}

/* This function is intended to return the encoded string
 */
string transToNumbers(string s){
    string result = "";
    for (int i = 0; i < s.size(); i++){
        result += transCodes(s[i]);
    }
    return result;
}


/* This function is intended to coalesce adjacent
 *  duplicate digits from the code
 */
string mergeNumbers(string s){
    string result = "";
    int i = 0;
    int j = 0;
    while (j<s.size()){
        if (s[i] != s[j]){
            result += s[i];
            i = j;
        }
        j++;
    }
    return result + s[i];
}

/* This function is intended to replace the first digit of the code
 * with the first letter of the original name, converting to uppercase
 * and remove all zeros from the code.
 */
string moveZero(string s,char c) {
    string result = to_string(c);
    for (int i = 1; i < s.size(); i++) {
        if (s[i] != '0') {
            result += s[i];
        }
    }
    return result;
}

/* This function is intended to Make the code exactly length 4
 * by padding with zeros or truncating the excess.
 */
string makeLengthFour(string s) {
    string result = s.substr(0,4);
    int len = result.size();
    if (len < 4) {
        for (int i = 0; i < 4-len; i++) {
            result += '0';
        }
    }
    return result;
}


/* This function is intended to return soundex
 */
string soundex(string s) {
    char c = s[0];
    if (c>='a' && c<='z') {
        c -= 32;
    }
    s = removeNonLetters(s);
    s = transToNumbers(s);
    s = mergeNumbers(s);
    s = moveZero(s,c);
    return makeLengthFour(s);
}



/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames
    while (true) {
        string inputname = getLine("Enter a surname (RETURN to quit):");
        if (inputname != "") {
            string code = soundex(inputname);
            cout << "Soundex code is " << code << endl;
            Vector <string> matches = {};
            for (long i = 0; i < databaseNames.size(); i++) {
                if (soundex(databaseNames[i]) == code) {
                    matches.add(databaseNames[i]);
                }
            }
            matches.sort();
            cout << "Matches from database: " << matches << '\n' << endl;
        }
        else {
            cout << "All done!" << endl;
            break;
        }
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("Test removing first letter") {
    string s= "Curie";
    EXPECT_EQUAL(removeNonLetters(s),"Curie");
    s = "O'Conner";
    EXPECT_EQUAL(removeNonLetters(s),"OConner");
}

STUDENT_TEST("Confirm removeNonLetters is correct") {
    string s = " 'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Hara");
}

STUDENT_TEST("Confirm transCodes is correct") {
    char c = 'C';
    EXPECT_EQUAL(transCodes(c), "2");
    c = 'l';
    EXPECT_EQUAL(transCodes(c), "4");
}

STUDENT_TEST("Confirm transToNumbers is correct") {
    string s= "Curie";
    EXPECT_EQUAL(transToNumbers(s), "20600");
    s = "OConner";
    EXPECT_EQUAL(transToNumbers(s), "0205506");
}

STUDENT_TEST("Confirm mergeNumbers is correct") {
    string s= "20600";
    EXPECT_EQUAL(mergeNumbers(s), "2060");
    s = "0205506";
    EXPECT_EQUAL(mergeNumbers(s), "020506");
}

STUDENT_TEST("Confirm moveZero is correct") {
    string s= "2060";
    EXPECT_EQUAL(moveZero(s,'C'), "C6");
    s = "O20506";
    EXPECT_EQUAL(moveZero(s,'O'), "O256");
}

STUDENT_TEST("Confirm makeLengthFour is correct") {
    string s= "C6";
    EXPECT_EQUAL(makeLengthFour(s), "C600");
    s = "O256";
    EXPECT_EQUAL(makeLengthFour(s), "O256");
}

STUDENT_TEST("Q10") {
    EXPECT_EQUAL(soundex("Angelou"), "A524");
    EXPECT_EQUAL(soundex("Wu"), "W000");
}




