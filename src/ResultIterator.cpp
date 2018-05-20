#include "vidar/ResultIterator.h"

namespace vidar
{

ResultIterator::~ResultIterator()
{
    if(m_cass_iterator != nullptr)
    {
        cass_iterator_free(m_cass_iterator);
        m_cass_iterator = nullptr;
    }
}

auto ResultIterator::GetNextRow() -> Row
{
    if(cass_iterator_next(m_cass_iterator))
    {
        return Row(cass_iterator_get_row(m_cass_iterator));
    }

    throw std::runtime_error("ResultIterator: Iterator has no more rows.");
}

ResultIterator::ResultIterator(
    CassIterator* cass_iterator
)
    : m_cass_iterator(cass_iterator)
{

}

} // namespace vidar
