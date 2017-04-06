#include <cstdlib>
#include <string>
#include <iomanip>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <tuple>
#include <type_traits>

template<typename _Type>
struct Printer {
    void operator()(_Type instance) {
        std::cout << instance << std::endl;
    }
};

typedef Printer<std::string> StringPrinter;

int
main(int argc, char **argv) {
    std::string line;
    std::vector<std::string> lines;

    std::cout << std::boolalpha;
#if 0
    do {
        std::getline(std::cin, line);
        lines.push_back(line);
        line.clear();
    } while (lines.size() < 5);
#else
    lines.push_back("30");
    lines.push_back("50");
    lines.push_back("20");
    lines.push_back("10");
    lines.push_back("40");
#endif

    if (!std::is_sorted(lines.begin(), lines.end())) {
        std::sort(lines.begin(), lines.end(), [](const std::string s1, const std::string &s2) {
            return s1 < s2;
        });
    }
    std::for_each(lines.begin(), lines.end(), StringPrinter());

    std::map<std::string, int> word_count;
    std::set<std::string> words;
    std::set<std::string> maine;

    line = "incrementing the iterator after the regex was destroyed results";
    const std::regex re{"\\s+"};
    for (auto it = std::sregex_token_iterator(line.begin(), line.end(), re, -1);
         it != std::sregex_token_iterator(); ++it) {

        word_count[(*it)]++;
        if (words.find((*it)) != words.end())
            std::cout << (*it) << " already exists" << std::endl;
        words.emplace((*it));
    }


    for (auto it = word_count.begin(); it != word_count.end(); ++it) {
        std::cout << (*it).first << " = " << (*it).second << std::endl;
    }

    maine.emplace("the");
    maine.emplace("and");

    std::vector<std::string> diff(std::max(maine.size(), words.size()));
    std::set_intersection(maine.begin(), maine.end(),
                          words.begin(), words.end(),
                          diff.begin());
    std::cout << "DIFF" << std::endl;
    for (auto it = diff.begin(); it != diff.end() && !(*it).empty(); ++it) {
        std::cout << (*it) << std::endl;
    }

    //queue
    //stack

    std::tuple<std::string, int> man("name", 10);
    std::string name;
    int age;

    std::tie(name, age) = man; //?
    std::cout << name << std::endl;
    std::cout << std::get<0>(man) << std::endl;
    std::cout << std::get<1>(man) << std::endl;



    std::cout << std::is_function<int(void)>::value << std::endl;
    std::cout << std::is_function<int(*)(void)>::value << std::endl;

    class A {};
    class B {virtual void fn() {}};
    class C: public B {};

    std::cout << std::is_polymorphic<A>::value << std::endl;
    std::cout << std::is_polymorphic<B>::value << std::endl;
    std::cout << std::is_polymorphic<C>::value << std::endl;


#if defined(_WIN32_WINNT)
    system("pause");
#endif
    return 0;
}

