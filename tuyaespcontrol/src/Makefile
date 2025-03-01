LDFLAGS:=-llink_core -lmiddleware_implementation -lplatform_port -lutils_modules -lubox -lblobmsg_json -lubus -largp -lserialport

SRCS := $(wildcard src/*.c)

OUTPUT := tuyaespcontrol

$(OUTPUT): $(SRCS)
	$(CC) -I./include $(CFLAGS) -o $(OUTPUT) $^ $(LDFLAGS)

clean:
	rm -f $(OUTPUT)