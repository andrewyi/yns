# yi's net server

# customize:
* src/server/conn.cpp
    * ```try_handle_request```: procedure execute for only a short time
    * ```try_handle_request_async```: long-running task

* ```Timer::register_handler```: time event
