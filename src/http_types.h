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

enum class HTTPStatus
{
    UNKNOWN = 0,
    OK = 200,
    NO_CONTENT = 204,
    PARTIAL_CONTENT = 206,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    TEMPORARY_REDIRECT = 307,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
};

}// namespace server