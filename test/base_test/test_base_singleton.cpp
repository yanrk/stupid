#include "test_base.h"
#include "base/utility/uncopy.h"
#include "base/utility/singleton.h"

USING_NAMESPACE_STUPID_BASE

class identifier : private Uncopy
{
public:
    void set(const std::string & id)
    {
        m_id = id;
    }

    const std::string & get() const
    {
        return(m_id);
    }

private:
    identifier()
    {

    }

    ~identifier()
    {

    }

    friend class Singleton<identifier>;

private:
    std::string      m_id;
};

void test_base_singleton(void)
{
    Singleton<identifier>::instance().set("364321198712154135");
    std::cout << "identifier: " << Singleton<identifier>::instance().get() << std::endl;
}
