
TARGET      := sysmon
BUILD       := build

SRC_ROOT    := .
SRC         := main.c \
               model/model.c \
               model/logger/logger.c \
               model/cpu/cpu.c \
               model/memory/memory.c \
               model/network/network.c \
               model/storage/storage.c \
               model/system/system.c

CC          ?= cc
CSTD        ?= -std=c11
WARN        ?= -Wall -Wextra -Wformat=2 -Wshadow -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes
OPT         ?= -O2
CFLAGS      ?= $(CSTD) $(WARN) $(OPT) -MMD -MP
CPPFLAGS    ?= $(INCLUDES) -D_GNU_SOURCE
LDFLAGS     ?=
LDLIBS      ?=

INCLUDES    := -Imodel -Imodel/logger -Imodel/cpu -Imodel/memory -Imodel/network -Imodel/storage -Imodel/system

UNAME_S     := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDLIBS  += -pthread
endif

OBJ         := $(patsubst %.c,$(BUILD)/%.o,$(SRC))
DEP         := $(OBJ:.o=.d)
BUILD_DIRS  := $(sort $(dir $(OBJ)))

.PHONY: all debug release clean run print

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD)/%.o: %.c | $(BUILD_DIRS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIRS):
	@mkdir -p $@

debug: CFLAGS := $(CSTD) $(WARN) -O0 -g3 -fsanitize=address,undefined -fno-omit-frame-pointer -MMD -MP
debug: LDLIBS += -fsanitize=address,undefined
debug: $(TARGET)

release: CFLAGS := $(CSTD) $(WARN) -O3 -DNDEBUG -MMD -MP
release: $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET)

-include $(DEP)
