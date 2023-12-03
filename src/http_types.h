#pragma once

namespace server
{

enum class HTTPMethod
{
    GET,
    POST,
    HEAD,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    UNKNOWN
};

enum class HTTPVersion
{
    HTTP_10,
    HTTP_11,
    HTTP_20,
    UNKNOWN
};

}// namespace server