#include "octopos.h"
#include "tenticle.h"

//#include "subscriber.h" PLEASE DONT FORGET ME
#include "publisher.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <unordered_map>


#include <pthread.h>

int main(int argc, char const *argv[]) {
    pid_t pid;

    try {
        octopOS::getInstance();
    } catch (std::system_error e) {
        std::cerr << e.what();
        throw;
    }
/*
    if (!(pid = fork())) {
        execl("../../testModule/build/bin/testModule",
            std::to_string(MSGKEY).c_str(), (char*)0);
    }
*/
    int x = MSGKEY;
    pthread_t tmp;

    if (pthread_create(&tmp, NULL, octopOS::listen_for_child, &x)) {
        exit(-1);
    }

    octopOS::getInstance().subscribe_to_topic("test", 0, 12234, sizeof(int));

/*
    // test.write(1, "This is a test string");
    // sleep(1);

    // tenticle test(MSGKEY);
    subscriber<int> sub("test", MSGKEY);
    // std::cout << sub.listen_to_topic(4) << std::endl;
    // std::cout << test.read(4).second << std::endl;

*/
    publisher<int> pub("test", x);

    pub.publish(0666);

    if(pthread_join(tmp, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }
    /*

    wait(NULL);

    // octopOS::getInstance().create_new_topic("test", sizeof(int));
    // octopOS::getInstance().create_new_topic("foo", sizeof(int));
    //
    // std::cout << octopOS::getInstance().subscribe_to_topic("test", 0) << std::endl;
    // std::cout << octopOS::getInstance().subscribe_to_topic("test", 1) << std::endl;
    // std::cout << octopOS::getInstance().subscribe_to_topic("foo", 1) << std::endl;
    // octopOS::getInstance().propagate_to_subscribers("test");
    // octopOS::getInstance().propagate_to_subscribers("foo");

*/

    return 0;
}
