EXEC		= build/game

CC 			= gcc
EXTENSION 	= c

CORE_SRC_DIR = ../core/src
CORE_OBJ_DIR = ../core/build/objs

SRCS_DIR	= src
OBJS_DIR	= build/objs

INCS_DIR	= -Iinc -I/mnt/data/code/c/libs/cglm/include -I$(CORE_SRC_DIR) 

CFLAGS		+= -std=gnu99 -Wall -Wextra $(INCS_DIR)
LFLAGS		+= -lm -lglfw -lGL -ldl
SRCS		= $(shell find $(SRCS_DIR) -type f -name *.$(EXTENSION))
OBJS		= $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%,$(SRCS:.$(EXTENSION)=.o))
CORE_SRCS	= $(shell find $(CORE_SRC_DIR) -type f -name *.$(EXTENSION))
CORE_OBJS	= $(patsubst $(CORE_SRC_DIR)/%,$(CORE_OBJ_DIR)/%,$(CORE_SRCS:.$(EXTENSION)=.o))
DEPS		= $(OBJS:.o=.d)
CORE_DEPS	= $(CORE_OBJS:.o=.d)

DIRS := . $(shell find $(OBJS_DIR) -type d)
GARBAGE_PATTERNS := *.o *.d
GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))

MAKE		= make --no-print-directory -C
ECHO		= echo

all		: $(EXEC)


$(EXEC)		: $(OBJS) $(CORE_OBJS)
		  @$(CC) $^ -o $@ $(LFLAGS)

$(OBJS_DIR)/%.o	: $(SRCS_DIR)/%.$(EXTENSION)
		  @$(CC) $(CFLAGS) -MD -MF $(@:.o=.d) -c -o $@ $<

run		: all
		  ./$(EXEC)

clean		:
		  @rm -rf $(GARBAGE)


-include $(DEPS)

.PHONY		: all clean 



