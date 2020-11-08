#pragma once

#include "priam/cpp_driver.hpp"

#include <string>

namespace priam
{
template<typename type>
using ptr = type*;

using uuid = CassUuid;

enum class data_type : int64_t
{
    unknown   = -1,
    custom    = CASS_VALUE_TYPE_CUSTOM,
    ascii     = CASS_VALUE_TYPE_ASCII,
    bigint    = CASS_VALUE_TYPE_BIGINT,
    blob      = CASS_VALUE_TYPE_BLOB,
    boolean   = CASS_VALUE_TYPE_BOOLEAN,
    counter   = CASS_VALUE_TYPE_COUNTER,
    decimal   = CASS_VALUE_TYPE_DECIMAL,
    double_t  = CASS_VALUE_TYPE_DOUBLE,
    float_t   = CASS_VALUE_TYPE_FLOAT,
    int_t     = CASS_VALUE_TYPE_INT,
    text      = CASS_VALUE_TYPE_TEXT,
    timestamp = CASS_VALUE_TYPE_TIMESTAMP,
    uuid      = CASS_VALUE_TYPE_UUID,
    varchar   = CASS_VALUE_TYPE_VARCHAR,
    varint    = CASS_VALUE_TYPE_VARINT,
    timeuuid  = CASS_VALUE_TYPE_TIMEUUID,
    inet      = CASS_VALUE_TYPE_INET,
    date      = CASS_VALUE_TYPE_DATE,
    time      = CASS_VALUE_TYPE_TIME,
    smallint  = CASS_VALUE_TYPE_SMALL_INT,
    tinyint   = CASS_VALUE_TYPE_TINY_INT,
    duration  = CASS_VALUE_TYPE_DURATION,
    list      = CASS_VALUE_TYPE_LIST,
    map       = CASS_VALUE_TYPE_MAP,
    set       = CASS_VALUE_TYPE_SET,
    udt       = CASS_VALUE_TYPE_UDT,
    tuple     = CASS_VALUE_TYPE_TUPLE
};

/**
 * @param type Converts this CassValueType into a std::string representation.
 * @return The string representation.
 */
auto to_string(data_type type) -> const std::string&;

} // namespace priam
