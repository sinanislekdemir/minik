CC=gcc
LIBS=-lbluetooth -lreadline
TARGET=blueterm

OBJS = \
	bluescan.o

REBUILDABLES = $(OBJS) $(TARGET)

all: $(TARGET)
	echo All done

clean:
	rm -f $(REBUILDABLES)
	echo Clean done

$(TARGET) : $(OBJS)
	$(CC) $^ $(LIBS) -o $@

%.o : %.c
	$(CC) -o $@ -c $<
