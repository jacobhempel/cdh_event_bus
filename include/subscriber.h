#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>


#include <list>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <queue>
#include <functional>

#include "tentacle.h"

typedef std::function<void(std::tuple<sem_id_t, shm_object*, generic_t*>&)> callback;

class subscriber_manager : protected tentacle {
public:
    static void* wait_for_data(void* data);
protected:
    static std::mutex topic_ids_lock, topic_memory_lock, registered_callbacks_lock;

    // Registers CB to TOPIC; CB will be invoked when new messages are available
    // on TOPIC
    static bool register_cb(callback cb, std::string topic, uint size, subscriber_manager* sub);

    // A map
    static std::map<std::string, std::tuple<sem_id_t, shm_object*, generic_t*>> topic_memory;

    static std::map<std::string, octopOS_id_t> topic_ids;

    // A map from topics to registered callbacks
    static std::map<std::string, std::vector<callback> > registered_callbacks;

    subscriber_manager(key_t shared_queue) : tentacle(shared_queue) {};
};

template <typename T>
class subscriber : public subscriber_manager {
public:
    subscriber(std::string topic_name, key_t shared_queue)
        : subscriber_manager(shared_queue),
        topic(topic_name) {

        pthread_mutex_init(&data_queue_lock, NULL);
        pthread_cond_init(&data_queue_condition, NULL);

          // Callback: push the new data onto this subscriber's data queue
    if (!subscriber::register_cb([this](std::tuple<sem_id_t, shm_object*, generic_t*> &data) {
        sem_id_t sem_id = std::get<0>(data);
        shm_object* rw = std::get<1>(data);


        if (p(sem_id, 3) < 0)
            exit(1);
        if (p(sem_id, 0) < 0)
            exit(1);
        rw->rw_array[0] += 1;
        if (rw->rw_array[0] == 1)
            if (p(sem_id, 2) < 0)
                exit(1);
        v(sem_id, 0);
        v(sem_id, 3);

        pthread_mutex_lock(&data_queue_lock);
        data_queue.push(*(T*)std::get<2>(data));
        pthread_mutex_unlock(&data_queue_lock);

        if(p(sem_id, 0) < 0)
            exit(1);
        rw->rw_array[0] -= 1;
        if (rw->rw_array[0] == 0)
            if (v(sem_id, 2) < 0)
                exit(1);
        v(sem_id, 0);
        pthread_cond_signal(&data_queue_condition);

    }, topic_name, sizeof(T), this))
        throw std::runtime_error("Failed to register topic");
  }

  // Read the data queue if there's anything there
  // Blocks if no data available
    T get_data() {
        T return_value;
        pthread_mutex_lock(&data_queue_lock);

        while (data_queue.empty())
             pthread_cond_wait(&data_queue_condition, &data_queue_lock);

        return_value = data_queue.front();
        data_queue.pop();

        pthread_mutex_unlock(&data_queue_lock);
        return return_value;
    }

    ~subscriber() {
        pthread_mutex_destroy(&data_queue_lock);
        pthread_cond_destroy(&data_queue_condition);
    }
private:
  std::string topic;
  pthread_mutex_t data_queue_lock;
  pthread_cond_t data_queue_condition;
  std::queue<T> data_queue;
  long id;
};
