

OBJS    += main.o
OBJS    += tun.o
OBJS    += ipv6.o
OBJS    += layer1.o
OBJS    += udp.o
OBJS    += session.o
OBJS    += PersistentQueue.o

CFLAGS  += -g
CFLAGS  += -DWOLFSSL_DTLS
CFLAGS  += -DHAVE_SUPPORTED_CURVES


all: main

main: $(OBJS)
	gcc -o $@ $^ -lwolfssl -lpthread

clean:
	@-rm $(OBJS)
	@-rm main

%.o: %.c
	gcc $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@ -MMD -MP -MF $(notdir $(patsubst %.o,%.d,$@) )

