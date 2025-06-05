CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iinc -fPIC
AR = ar
ARFLAGS = rcs

#Dicrectories
CUR_DIR = .
BIN_DIR = $(CUR_DIR)/bin
INC_DIR = $(CUR_DIR)/inc
LIB_DIR = $(CUR_DIR)/lib
OBJ_DIR = $(CUR_DIR)/obj
SRC_DIR = $(CUR_DIR)/src
STATICLIB_DIR = $(LIB_DIR)/staticlib
SHAREDLIB_DIR = $(LIB_DIR)/sharedlib

#Files
MODULES = facade hvacsystem lighting security
SRCS = $(foreach module,$(MODULES),$(wildcard $(SRC_DIR)/$(module)/*.c))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

STATICLIB = $(STATICLIB_DIR)/libmain.a
SHAREDLIB = $(SHAREDLIB_DIR)/libmain.so

MAIN = main.c
TARGET_STATIC = $(BIN_DIR)/main_static
TARGET_SHARED = $(BIN_DIR)/main_shared

all: $(TARGET_SHARED) $(TARGET_STATIC)

#object
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#static lib
$(STATICLIB): $(OBJS)
	@mkdir -p $(STATICLIB_DIR)
	$(AR) $(ARFLAGS) $@ $^	

#shared lib
$(SHAREDLIB): $(OBJS)
	@mkdir -p $(SHAREDLIB_DIR)
	$(CC) -shared -o $@ $^
#executalbe static_lib

$(TARGET_STATIC): $(STATICLIB) $(MAIN)
	@mkdir -p $(BIN_DIR)
	$(CC) -Iinc $(MAIN) -L$(STATICLIB_DIR) -lmain -o $@
$(TARGET_SHARED): $(SHAREDLIB) $(MAIN)
	@mkdir -p $(BIN_DIR)
	$(CC) -Iinc $(MAIN) -L$(SHAREDLIB_DIR) -lmain -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(SHAREDLIB_DIR) $(SHAREDLIB_DIR)

.PHONY: all clean
