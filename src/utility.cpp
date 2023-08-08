//
// Created by konstantin on 04.08.23.
//

#include <unistd.h>
#include <fstream>
#include "utility.hpp"



void pinepp::fetch(const std::string& url, const std::string& filepath, pinepp::http_tools tool) {
    const auto reply = fetch(url, tool);
    std::ofstream file{filepath};
    if (file.is_open()) {
        file << reply;
        file.close();
    } else throw;

}

std::string pinepp::fetch(const std::string& url, pinepp::http_tools tool) {
    int fd[2];
    if (pipe(fd) < 0)
        throw std::runtime_error{"Failed to create pipe."};

    if (fork() == 0) {
        // CHILD
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        if (tool == pinepp::http_tools::CURL) {
            execlp("curl", "curl", url.c_str(), "-s", nullptr);
        } else if (tool == pinepp::http_tools::WGET) {
            execlp("wget", "wget", "-q", "-O-", url.c_str(), nullptr);
        }
        exit(-1);
    }

    close(fd[1]);
    std::string http_reply;
    char c;
    while (read(fd[0], &c, 1) > 0)
        http_reply.push_back(c);
    close(fd[0]);
    return http_reply;
}