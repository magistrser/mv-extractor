#include <sys/time.h>
#include "interrupt_timout_handler.hpp"

InterruptTimoutHandler::InterruptTimoutHandler(Timestamp timeout_ms) : timeout_ms(timeout_ms) {
    this->reset();
}

void InterruptTimoutHandler::reset() {
    this->last_reset_time = this->get_current_time();
}

// static
int InterruptTimoutHandler::interrupt_callback(void * context) {
    return context && static_cast<InterruptTimoutHandler *>(context)->is_timeout();
}

Timestamp InterruptTimoutHandler::get_current_time() {
    struct timeval te;
    gettimeofday(&te, NULL);
    return te.tv_sec*1000LL + te.tv_usec/1000;
}

bool InterruptTimoutHandler::is_timeout(){
    const Timestamp current_time = this->get_current_time();
    const Timestamp delay = current_time - this->last_reset_time;
    return delay > this->timeout_ms;
}
