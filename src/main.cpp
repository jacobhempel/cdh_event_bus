#include "octopos.h"
#include "tenticle.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

int main(int argc, char const *argv[]) {
    pid_t pid;

    try {
        octopOS::getInstance();
    } catch (std::system_error e) {
        std::cerr << e.what();
        throw;
    }

    tenticle test(MSGKEY);

    if (!(pid = fork())) {
        execl("../../testModule/build/bin/testModule",
            std::to_string(MSGKEY).c_str(), (char*)0);
    }

    // test.write(1, "This is a test string");
    std::pair<long, std::string> tmp(-1, "");
    std::pair<unsigned, key_t> shit;
    tmp = test.read(1);
    std::cout << tmp.first << " : " << tmp.second << std::endl;

    std::istringstream iss(tmp.second);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>{}};

    shit = octopOS::getInstance().create_new_topic(tokens[1], std::stoi(tokens[0]));

    std::stringstream ss;
    ss << shit.first << " " << shit.second;

    test.write(2, ss.str());
    octopOS::getInstance().subscribe_to_topic("test", 0);

    tmp = test.read(3);
    std::cout << tmp.first << " : " << tmp.second << std::endl;

    octopOS::getInstance().propagate_to_subscribers(tmp.second);

    sleep(5);

    wait(NULL);

    // octopOS::getInstance().create_new_topic("test", sizeof(int));
    // octopOS::getInstance().create_new_topic("foo", sizeof(int));
    //
    // std::cout << octopOS::getInstance().subscribe_to_topic("test", 0) << std::endl;
    // std::cout << octopOS::getInstance().subscribe_to_topic("test", 1) << std::endl;
    // std::cout << octopOS::getInstance().subscribe_to_topic("foo", 1) << std::endl;
    // octopOS::getInstance().propagate_to_subscribers("test");
    // octopOS::getInstance().propagate_to_subscribers("foo");

    return 0;
}
