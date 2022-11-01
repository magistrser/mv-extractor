using Timestamp = unsigned long;

class InterruptTimoutHandler {

public:
    InterruptTimoutHandler(Timestamp timeout_s);

    void reset();
    static int interrupt_callback(void * context);

private:
    unsigned int timeout_s;
    Timestamp last_reset_time;

    Timestamp get_current_time();
    bool is_timeout();
};