using Timestamp = unsigned long long;

class InterruptTimoutHandler {

public:
    InterruptTimoutHandler(Timestamp timeout_ms);

    void reset();
    static int interrupt_callback(void * context);

private:
    unsigned int timeout_ms;
    Timestamp last_reset_time;

    Timestamp get_current_time();
    bool is_timeout();
};