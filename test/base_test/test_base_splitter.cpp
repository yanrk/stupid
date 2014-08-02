#include "test_base.h"
#include "base/string/splitter.h"

USING_NAMESPACE_STUPID_BASE

void test_base_splitter(void)
{
    std::string element_set("  abc  , bcd\t, \r cde,  def \t, efg\n ,\r\nfgh\t\n");
    StringSplitter splitter(element_set);

    std::cout << "element count bound : " << splitter.count_bound() << std::endl;

    while (splitter.has_element())
    {
        std::string element(splitter.pop_element());
        std::cout << '[' << element << ']' << "   ";
    }
    std::cout << std::endl;

    splitter.rewind();

    while (!splitter.eof())
    {
        std::string element;
        splitter >> element;
        std::cout << '[' << element << ']' << "   ";
    }
    std::cout << std::endl;

    std::cout << "element count bound : " << splitter.count_bound() << std::endl;
}
