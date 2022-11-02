#include <sys/time.h>
#include "interrupt_timout_handler.hpp"

#include <iostream>

InterruptTimoutHandler::InterruptTimoutHandler(Timestamp timeout_ms) : timeout_ms(timeout_ms) {
    this->reset();
}

void InterruptTimoutHandler::reset() {
//    std::cout << "[DEBUG] reset called" << std::endl;
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
//    std::cout << "[DEBUG] current_time : " << current_time  << std::endl;
//    std::cout << "[DEBUG] this->last_reset_time : " << this->last_reset_time  << std::endl;
//    std::cout << "[DEBUG] is_timeout delay: " << delay << std::endl;
    return delay > this->timeout_ms;
}
