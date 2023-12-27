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
    UNKNOWN = 0, // 未知状态
    OK = 200, // 成功
    NO_CONTENT = 204, // 无内容
    PARTIAL_CONTENT = 206, // 部分内容
    MOVED_PERMANENTLY = 301, // 永久重定向
    FOUND = 302, // 临时重定向
    SEE_OTHER = 303, // 临时重定向
    NOT_MODIFIED = 304, // 未修改
    TEMPORARY_REDIRECT = 307, // 临时重定向
    BAD_REQUEST = 400, // 错误请求
    UNAUTHORIZED = 401, // 未授权
    FORBIDDEN = 403, // 禁止访问 
    NOT_FOUND = 404,    // 未找到
    INTERNAL_SERVER_ERROR = 500, // 服务器内部错误
    NOT_IMPLEMENTED = 501, // 未实现
};

}// namespace server