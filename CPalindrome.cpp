#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include "CPalindrome.h"

using namespace std;

//**********************************************************************************************

CPalindrome::CPalindrome()
{
}

CPalindrome::~CPalindrome()
{
}

//checks correctness of provided string
bool CPalindrome::if_correct(const std::string &s)
{
    if (s.size() < 2)
    {
        return false;
    }
    for (auto i = s.begin(); i < s.end(); i++)
    {
        if (!(*i >= 'a' && *i <= 'z'))
        {
            return false;
        }
    }
    return true;
}


//**********************************************************************************************
//checks if given string is a palindrome
bool CPalindrome::if_palindrome(const std::string &s)
{   
    if (s.size() < 2)
    {
        return false;
    }
    auto itb {s.begin()};
    auto ite (s.end() - 1);
    while (*itb == *ite && ite > itb)
    {
        ite--;
        itb++;
    }
    if (ite > itb)
    {
        return false;
    }
    else
    {
        return true;
    }
}
//**********************************************************************************************



//**********************************************************************************************
//returns minimum number of changes that are needed to change the given string into a palidrome (possible only increasing or decreasing of letters)
int CPalindrome::to_palindrome(const std::string &s)
{
    int sum {};
    for (auto i = s.begin(), j = s.end() - 1; j > i; i++, j--)
    {
        sum += difference(*i, *j); 
    }
    return sum;
}
//**********************************************************************************************

//**********************************************************************************************
//returns minimum number of changes that are needed to change the given string into a near-palidrome (possible only increasing or decreasing of letters)
int CPalindrome::to_near_palindrome(std::string &s)
{
    //count which letters appear odd number of times in s
    map<char, int> chars;
    count_letters(s, chars);
    size_t odds = count_odds(chars);

    //int to count the amount of steps in the exercise
    int steps {};
    
    //at which position are odd letters and what letter is that
    map<size_t, char> pos_odds;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (chars.find(s[i]) != chars.end() && chars[s[i]] % 2 != 0)
        {
            pos_odds.insert(pair<size_t, char>(i, s[i]));
            chars.erase(s[i]);
        }
    }

    //exact algorithm
    map<size_t, char>::iterator fir, sec;
    int diff, tmp;
    while (odds > 1 && odds <= s.size())
    {
        fir = pos_odds.begin();
        sec = fir;
        sec++;
        diff = difference(fir->second, sec->second);

        
        for (auto i = pos_odds.begin(); i != pos_odds.end(); i++)
        {
            for (auto j = i; j != pos_odds.end(); j++)
            {
                if (j != i)
                {
                    tmp = difference(j->second, i->second);
                    if (tmp < diff)
                    {
                        fir = i;
                        sec = j;
                        diff = tmp;
                    }
                }
            }
        }

        if (fir->second > sec->second)
        {
            s[fir->first] = sec->second;

        }
        else
        {
            s[sec->first] = fir->second;
        }

        steps += diff;

        pos_odds.erase(fir);
        pos_odds.erase(sec);
        
        odds -= 2;
    }

    return steps;
}
//**********************************************************************************************


//**********************************************************************************************
//checks if there can be a removal of one letter to change the given string into a palidrome
bool CPalindrome::if_one_sign_away(const std::string &s)
{
    for (int i = 0, n = s.size(); i < n; i++)
    {
        if (if_palindrome(s.substr(0,i) + s.substr(i+1, n)))
        {
            return true;
        }
    }
    return false;
}
//**********************************************************************************************

//**********************************************************************************************
//checks if there can be a removal of one letter to change the given string into a near-palidrome
bool CPalindrome::if_one_sign_away_from_np(const std::string &s)
{
    map<char,int> set;
    count_letters(s, set);
    size_t odd_count {count_odds(set)};
    if ((s.size() % 2 == 0 && odd_count < 3) || (s.size() % 2 == 1 && odd_count < 2))
    {
        return true;
    }
    return false;
}
//**********************************************************************************************


//**********************************************************************************************
//rearranges letters in given string to make a lowest palindrome, if it is not possible return "None"
string CPalindrome::find_palindrome(std::string &s)
{
    //create a vector counts the quantity of each letter in given string
    vector<int> chars;
    count_letters(s, chars);

    //check if there is a possibily to create a palidrome
    size_t odd_count {count_odds(chars)};

    if (s.size() % 2 == 0 && odd_count == 0 || s.size() % 2 == 1 && odd_count == 1)
    {
        string ret(s.size(), ' ');
        int i {};
        for (size_t j = 0, n = s.size(); j < n / 2; j++)
        {
            while (chars[i] < 2 && i < chars.size())
            {
                i++;
            }
            ret[j] = 'a' + i;
            ret[n - 1 - j] = ret[j];
            chars[i] -= 2;
        }
        if (odd_count)
        {
            i = 0;
            while (chars[i] == 0)
            {
                i++;
            }
            ret[s.size() / 2] = 'a' + i;
        }
        return ret;
    }
    else
    {
        return "None";
    }
}
//**********************************************************************************************


//**********************************************************************************************
//finds the longest palidrome in given string, if there is none return "None"
string CPalindrome::longest_palindrome(std::string &s)
{
    string ret {};
    for (size_t i = s.size(); i <= s.size(); i--)
    {
        for (size_t j = 0; j < i; j++)
        {
            if ((i - j) > ret.size())
            {
                if (if_palindrome(s.substr(j, i)))
                {
                    ret = s.substr(j, i);
                }
            }
            else if ((i - j) == ret.size())
            {
                if (if_palindrome(s.substr(j, i)) && ret.compare(s.substr(j, i)) < 0)
                {
                    ret = s.substr(j, i);
                }
            }
            else 
            {
                break;
            }
        }
        for (size_t j = i; j < s.size(); j++)
        {
            if ((j - i) > ret.size())
            {
                if (if_palindrome(s.substr(i, j)))
                {
                    ret = s.substr(i, j);
                }
            }else if ((j - i) == ret.size())
            {
                if (if_palindrome(s.substr(i, j)) && ret.compare(s.substr(i, j)) < 0)
                {
                    ret = s.substr(i, j);
                }
            }
            else
            {
                break;
            }
        }
    }
    if (ret == "")
    {
        ret = "None";
    }
    return ret;
}
//**********************************************************************************************


//**********************************************************************************************
//returns the smallest alphabetically string that is a near palindrome, doing as few operations as possible (possible is only changing of letter into a different one)
int CPalindrome::even_palindrome(std::string &s)
{
    if (s.size() % 2 != 0)
    {
        return -1;
    }
    
    //count which letters appear odd number of times in s
    map<char, int> chars;
    count_letters(s, chars);
    size_t odds = count_odds(chars);

    //int to count the amount of steps in the exercise
    int steps {};
    
    //at which position are odd letters and what letter is that
    map<size_t, char> pos_odds;
    for (size_t i = 0; i < s.size(); i++)
    {
        if (chars.find(s[i]) != chars.end() && chars[s[i]] % 2 != 0)
        {
            pos_odds.insert(pair<size_t, char>(i, s[i]));
            chars.erase(s[i]);
        }
    }

    cout << "s = " << s << ", before exact algorithm" << endl;
    //exact algorithm
    map<size_t, char>::iterator fir, sec;
    while (odds > 0 && odds <= s.size())
    {
        fir = pos_odds.begin();
        sec = fir;
        sec++;
        for (auto i = sec; i != pos_odds.end(); i++)
        {
            if (sec->second > i->second)
            {
                sec = i;
            }
        }

        if (fir->second > sec->second)
        {
            s[fir->first] = sec->second;

        }
        else
        {
            s[sec->first] = fir->second;
        }
        
        pos_odds.erase(fir);
        pos_odds.erase(sec);
        steps++;
        odds -= 2;
    }

    cout << "s = " << s << ", after exact algorithm" << endl;
    cout << "steps = " << steps << endl;

    return steps;
}
//**********************************************************************************************


//**********************************************************************************************
//checks if given integer is a palindrome
bool CPalindrome::int_is_palindrome(const int &i)
{
    //count how many numbers has given int
    int num {}, pom {i};
    while (pom > 0)
    {
        num++;
        pom /= 10;
    }
    
    //check sides of i 
    pom = i;
    while (num > 1)
    {
        if (static_cast<int>(pom / pow(10, num - 1)) != (pom % 10))
        {
            return false;
        }
        pom -= static_cast<int>(pom / pow(10, num - 1)) * static_cast<int>(pow(10, num - 1));
        pom /= 10;
        num -= 2;
    }

    return true;
}
//**********************************************************************************************

//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//functions which counts appearances of letters and store that data in a map or a vector
void CPalindrome::count_letters(const std::string &s, std::vector<int> &chars)
{
    chars.resize(26);
    for (auto i = s.begin(); i < s.end(); i++)
    {
        chars[*i - 'a']++;
    }
}

//**********************************************************************************************
void CPalindrome::count_letters(const std::string &s, std::map<char, int> &chars)
{
    for (auto it = s.begin(); it < s.end(); it++)
    {
        if (chars.find(*it) == chars.end())
        {
            chars[*it] = 1;
        }
        else
        {
            chars.find(*it)->second++;
        }
    }
}

//**********************************************************************************************
//functions that count how many letters appear odd number of times
size_t CPalindrome::count_odds(std::vector<int> &chars)
{
    size_t odd_count {};
    for (auto i = chars.begin(); i < chars.end(); i++)
    {
        if (*i % 2 != 0)
        {
            odd_count++;
        }
    }
    return odd_count;
}

//**********************************************************************************************
size_t CPalindrome::count_odds(std::map<char, int> &chars)
{
    size_t odd_count {};
    for (auto it = chars.begin(); it != chars.end(); it++)
    {
        if (it->second % 2 != 0)
        {
            odd_count++;
        }
    }
    return odd_count;
}

//**********************************************************************************************
//function that counts the difference between char a and char b
int CPalindrome::difference(char a, char b)
{
    int dif {};
    if (a > b)
    {
        dif = static_cast<int>(a - b);
    }
    else
    {
        dif = static_cast<int>(b - a);
    }
    if (dif > 13)
    {
        dif = 26 - dif;
    }

    return dif;
}
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************