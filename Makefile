TARGET = libalsa-reroute.so
TARGET_SH = alsa_reroute

PREFIX = /usr/local

OBJ = alsa-reroute.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $< -ldl -shared

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

clean:
	rm -rf $(TARGET)
	rm -rf $(OBJ)

install:
	install -m755 $(TARGET) $(PREFIX)/lib/
	install -m755 $(TARGET_SH) $(PREFIX)/bin/

uninstall:
	rm -rf $(PREFIX)/$(TARGET)
	rm -rf $(PREFIX)/$(TARGET_SH)

