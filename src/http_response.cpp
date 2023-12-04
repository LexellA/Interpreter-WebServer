

#include "http_response.h"

namespace server
{

const std::unordered_map<HTTPStatus, std::string> HTTPResponse::m_status_map =
{
    {HTTPStatus::UNKNOWN, "Unknown"},
    {HTTPStatus::OK, "Ok"},
    {HTTPStatus::NO_CONTENT, "No Content"},
    {HTTPStatus::PARTIAL_CONTENT, "Partial Content"},
    {HTTPStatus::MOVED_PERMANENTLY, "Moved Permanently"},
    {HTTPStatus::FOUND, "Found"},
    {HTTPStatus::SEE_OTHER, "See Other"},
    {HTTPStatus::NOT_MODIFIED, "Not Modified"},
    {HTTPStatus::TEMPORARY_REDIRECT, "Temporary Redirect"},
    {HTTPStatus::BAD_REQUEST, "Bad Request"},
    {HTTPStatus::UNAUTHORIZED, "Unauthorized"},
    {HTTPStatus::FORBIDDEN, "Forbidden"},
    {HTTPStatus::NOT_FOUND, "Not Found"},
    {HTTPStatus::INTERNAL_SERVER_ERROR, "Internal Server Error"},
    {HTTPStatus::NOT_IMPLEMENTED, "Not Implemented"}
};

} // namespace server
