
#编译使用的平台 Windows Linux
HOST_OS = Windows
BIN_NAME = main

#要编译的文件
obj-y += main.o
obj-y += catsh.o
obj-y += catsh_port.o

#相关配置
ifeq ($(HOST_OS), Windows)
	CC  	= gcc
	LD  	= gcc
	DBG		= gdb

	RM 		= del
	MK_DIR 	= mkdir
	RM_DIR 	= rmdir /s /q

	LINK_SCRIPT = link_tp.ld
	
	OUT_BIN = $(BIN_NAME).exe

	CINCLUDE_FILE_FLAG = -Iinclude
else
	CC  	= gcc
	LD  	= gcc
	DBG		= gdb

	RM 		= rm
	MK_DIR 	= mkdir
	RM_DIR 	= rm -rf

	# LINK_SCRIPT = link_linux.ld

	OUT_BIN = $(BIN_NAME)

	CINCLUDE_FILE_FLAG = -Iinclude
endif

# 可以使用链接脚本
CLINK_FLAGS += -g -ffunction-sections -fdata-sections -Wl,-T "$(LINK_SCRIPT)"
# CLINK_FLAGS += -g
CFLAGS		+= -g

#不需要处理的目录路径
SRC_DIR = src
OUT_DIR = output
#需要处理的目录路径
OBJ_DIR_LINUX = $(OUT_DIR)/obj
BIN_DIR_LINUX = $(OUT_DIR)/bin

#处理要编译的文件
OBJ_TARGET_LINUX = $(patsubst %,$(OBJ_DIR_LINUX)/%,$(obj-y))

ifeq ($(HOST_OS), Windows)
OBJ_DIR_WINDOWS = $(subst /,\\,$(OBJ_DIR_LINUX))
BIN_DIR_WINDOWS = $(subst /,\\,$(BIN_DIR_LINUX))
OBJ_TARGET_WINDOWS = $(subst /,\\,$(OBJ_TARGET_LINUX))
endif

all: $(OUT_BIN)

ifeq ($(HOST_OS), Windows)
$(OUT_BIN): $(BIN_DIR_WINDOWS) $(OBJ_TARGET_WINDOWS)
	$(LD) $(CLINK_FLAGS) -Wl,-Map,$(BIN_DIR_WINDOWS)\\$(BIN_NAME).map -o $(BIN_DIR_WINDOWS)\\$@ $(OBJ_TARGET_WINDOWS)

$(BIN_DIR_WINDOWS):
	$(MK_DIR) $(OUT_DIR)
	$(MK_DIR) $(OBJ_DIR_WINDOWS)
	$(MK_DIR) $(BIN_DIR_WINDOWS)

$(OBJ_DIR_WINDOWS)\\%.o: $(SRC_DIR)\\%.c
	$(CC) $(CINCLUDE_FILE_FLAG) $(CFLAGS) -o $@ -c $<

run: $(BIN_DIR_WINDOWS)\\$(OUT_BIN)
	@$(BIN_DIR_WINDOWS)\\$(OUT_BIN)

dbg: $(BIN_DIR_WINDOWS)\\$(OUT_BIN)
	gdb $(BIN_DIR_WINDOWS)\\$(OUT_BIN)
else
$(OUT_BIN): $(BIN_DIR_LINUX) $(OBJ_TARGET_LINUX)
	$(LD) $(CLINK_FLAGS) -Wl,-Map,$(BIN_DIR_LINUX)/$(BIN_NAME).map -o $(BIN_DIR_LINUX)/$@ $(OBJ_TARGET_LINUX)

$(BIN_DIR_LINUX):
	$(MK_DIR) $(OUT_DIR)
	$(MK_DIR) $(OBJ_DIR_LINUX)
	$(MK_DIR) $(BIN_DIR_LINUX)

$(OBJ_DIR_LINUX)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CINCLUDE_FILE_FLAG) $(CFLAGS) -o $@ -c $<

run: $(BIN_DIR_LINUX)/$(OUT_BIN)
	@$(BIN_DIR_LINUX)/$(OUT_BIN)

dbg: $(BIN_DIR_LINUX)/$(OUT_BIN)
	gdb $(BIN_DIR_LINUX)/$(OUT_BIN)
endif


clean:
	$(RM_DIR) $(OUT_DIR)


