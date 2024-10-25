#include <iostream>
#include <string>
#include <string_view>
#include <stack>
#include <iomanip>

static bool is_uppercase(char c){
    return (c >= 'A' && c <= 'Z');
}

enum class State{
    start,

    tag_content,

};

class Solution {
public:


      bool isValid(const std::string_view code) {
        //minimum tag: "<a></a>" has 7 symbols.
       std::stack<std::string> tags;
       State state = State::start;
       std::string tag;
       const int n = static_cast<int>(code.size());

       for (int i = 0; i < n; ) {

            switch (state) {
                case State::start:
                {
                    if ( code[i] != '<' ) return false;
                    // <a></a>
                    if ( i + 6 >= n) return false;
                    if (code[i+1] == '!') return false; //after start CDATA not allowed.
                    tag.clear();
                    ++i;
                    while (i < n && code[i] != '>') {
                        if ( !is_uppercase(code[i]) ) {
                            return false;
                        }
                        tag += code[i];
                        i++;
                        if (tag.size() > 9) return false;
                    }

                    if (tag.empty()) return false;

                    if (i >= n) return false;

                    ++i; // skip '>'.

                    tags.push(tag);

                    state = State::tag_content;

                }
                break;

                case State::tag_content: {


                    while ( i < n && code[i] != '<' ) {
                        ++i;
                    }
                    if (i >= n) return false;

                    // open tag , close tag or CDATA
                    // </a>
                    if (i + 3 >= n) return false;

                    ++i;//skip '<'

                    if (code[i] == '!') {

                        ++i; // skip '!'

                        //cdata
                        //[CDATA]
                        if (i + 6 >= n){ return false; }
                        bool cdata = code[i] == '[' && code[i+1] == 'C' && code[i+2] == 'D' && code[i+3] == 'A' && code[i+4] == 'T' && code[i+5] == 'A' &&
                            code[i+6] == '[';
                        if (!cdata) return false;
                        i += 7;

                        bool end = false;
                        while (i + 2< n && !(code[i] == ']' && code[i+1] == ']' && code[i+2] == '>')) {
                            ++i;
                        }

                        end = (i + 2 < n && code[i] == ']' && code[i+1] == ']' && code[i+2] == '>');
                        if (!end) return false;
                        i += 3;

                        state = State::tag_content;

                    } else if (code[i] == '/') {
                        //close tag
                        ++i;//skip '/'

                        tag.clear();
                        while (i < n && code[i] != '>') {
                            if ( !is_uppercase(code[i]) ) {
                                return false;
                            }
                            tag += code[i];
                            i++;
                            if (tag.size() > 9) return false;
                        }

                        if (i >= n) return false;
                        ++i;//skip '>'.

                        if (tag.empty()) return false;

                        if (tags.empty()) return false;
                        if (tags.top() != tag) return false;
                        tags.pop();
                        if (tags.empty()) {
                            if (i < n) return false;
                        }
                        state = State::tag_content;
                    } else {
                        //open tag

                        tag.clear();

                        while (i < n && code[i] != '>') {
                            if ( !is_uppercase(code[i]) ) {
                                return false;
                            }
                            tag += code[i];
                            i++;
                            if (tag.size() > 9) return false;
                        }

                        if (tag.empty()) return false;

                        if (i >= n) return false;
                        ++i;//skip '>'.
                        tags.push(tag);

                        state = State::tag_content;
                    }
                } // end case tag_open

            } //end switch case


       } // end for (int i = 0; i < n)


        if (!tags.empty()) return false;

        if (state != State::tag_content) return false;

        return true;
    } // end isValid


};// end solution
int main() {
    //std::string code ="<DIV>This is the first line <![CDATA[<div>]]></DIV>";
    //https://leetcode.com/problems/tag-validator/submissions/1433473207/
    const std::string code = "<A></A><B></B>";
    Solution solution{};
    std::cout << std::boolalpha << solution::isValid(code) << std::endl;
    return 0;
}
