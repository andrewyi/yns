CC := g++
AR  := ar
CFLAGS := -ggdb -I..
CPPFLAGS := -std=c++11
LFLAGS := -pthread

lib := libyns.a
objects := server/binder.o server/conn.o server/eventer.o server/poller.o server/server.o server/timer.o \
	util/buffer.o util/thread.o util/exception.o util/syncer.o util/util.o util/worker.o log/logger.o

main: ./bin/main.cpp libyns.a
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -L. -lyns $(LFLAGS) -o $@

server/binder.o: server/binder.cpp server/binder.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

server/conn.o: server/conn.cpp server/conn.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

server/eventer.o: server/eventer.cpp server/eventer.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

server/poller.o: server/poller.cpp server/poller.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

server/server.o: server/server.cpp server/server.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

server/timer.o: server/timer.cpp server/timer.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

util/buffer.o: util/buffer.cpp util/buffer.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

util/thread.o: util/thread.cpp util/thread.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

util/exception.o: util/exception.cpp util/exception.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

util/syncer.o: util/syncer.cpp util/syncer.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

util/util.o: util/util.cpp util/util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

util/worker.o: util/worker.cpp util/worker.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

log/logger.o: log/logger.cpp log/logger.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(lib): $(objects)
	$(AR) cr $@ $^

clean:
	rm -f main $(lib) $(objects)

.PHONY: clean
