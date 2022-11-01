#include <time.h>
#include <interrupt_timout_handler.hpp>

InterruptTimoutHandler::InterruptTimoutHandler(Timestamp timeout_s) : timeout_s(timeout_s) {}

void InterruptTimoutHandler::reset() {
    this->last_reset_time = this->get_current_time();
}

// static
int InterruptTimoutHandler::interrupt_callback(void * context) {
    return context && static_cast<InterruptTimoutHandler *>(context)->is_timeout();
}

Timestamp InterruptTimoutHandler::get_current_time() {
    return (Timestamp)time(NULL);
}

bool InterruptTimoutHandler::is_timeout(){
    const Timestamp delay = this->get_current_time() - this->last_reset_time;
    return delay > this->timeout_s;
}
