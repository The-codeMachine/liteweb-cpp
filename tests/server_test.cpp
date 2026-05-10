#include "../include/Server.hpp"
#include <iostream>

int main() {

    liteweb_cpp::Server srv;

    srv.loadPage("/", "resources/index_test.html");

    srv.get("/cool_info", [](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
        res.set_content("Mankirat is really really fun to be around", "text/html");
    });

    srv.post("/funny_stuff", [](const httplib::Request& req, httplib::Response& res) {
        auto json = liteweb_cpp::Server::parseJson(req);
        if (json.contains("error")) {
            res.status = 500;
        } else {
            res.status = 200;
        }
        
        res.set_content("guess what? " + json.value("cool", ""), "text/html");
    });

    std::cout << "Server listening on port 8080 (localhost)\n";
    srv.listen("0.0.0.0", 8080);

    return 0;
}