#pragma once
#include <functional>
#include <server/HttpRequest.h>
#include <server/HttpResponse.h>

namespace maziogra_http {

using Route = std::function<void(HttpRequest &, HttpResponse &)>;
using Middleware =
    std::function<void(HttpRequest &, HttpResponse &, std::function<void()>)>;

} // namespace maziogra_http
