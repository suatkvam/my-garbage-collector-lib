# Project Configuration
NAME        = garbage_collecter.a
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -I$(INC_DIR)
AR          = ar rcs
RM          = rm -f

# ANSI Color Codes
GREEN       := \033[0;32m
YELLOW      := \033[1;33m
BLUE        := \033[1;34m
RESET       := \033[0m

# Directory Structure
SRC_DIR     = src
INC_DIR     = include
OBJ_DIR     = obj

# Source Files - Core Collector
SRCS_CORE   = $(SRC_DIR)/core/collector_init.c \
              $(SRC_DIR)/core/collector_close.c \
              $(SRC_DIR)/core/gc_state.c

# Source Files - Memory Management
SRCS_MEM    = $(SRC_DIR)/memory/gc_malloc.c \
              $(SRC_DIR)/memory/gc_calloc.c \
              $(SRC_DIR)/memory/gc_realloc.c \
              $(SRC_DIR)/memory/gc_memory_utils.c \
              $(SRC_DIR)/memory/get_header_from_ptr.c

# Source Files - Collection
SRCS_COL    = $(SRC_DIR)/collector/gc_collect.c \
              $(SRC_DIR)/collector/gc_mark.c \
              $(SRC_DIR)/collector/gc_mark_utils.c \
              $(SRC_DIR)/collector/gc_sweep.c

# All Sources
SRCS        = $(SRCS_CORE) $(SRCS_MEM) $(SRCS_COL)

# Object Files
OBJS        = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Wrapper Flags (eski projelere entegrasyon için)
WRAP_FLAGS  = -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=free

# Rules
all: banner $(NAME) show_usage

banner:
	@printf '$(BLUE)╔════════════════════════════════════════╗\n'
	@printf '║    GARBAGE COLLECTOR LIBRARY BUILD    ║\n'
	@printf '╚════════════════════════════════════════╝$(RESET)\n\n'

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf '$(GREEN)► Compiling: $(RESET)%-35s' "$(notdir $<)"
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf '$(GREEN)✓$(RESET)\n'

$(NAME): $(OBJS)
	@printf '\n$(YELLOW)► Creating static library: $(NAME)$(RESET)\n'
	@$(AR) $(NAME) $(OBJS)
	@printf '$(YELLOW)✓ Library created: $(NAME)$(RESET)\n\n'

show_usage:
	@printf '$(GREEN)═══════════════════════════════════════════════════════════════════════$(RESET)\n'
	@printf '$(YELLOW)Eski projenize entegre etmek için:$(RESET)\n\n'
	@printf '  GC_LIB = path/to/garbage_collecter.a\n'
	@printf '  GC_WRAP = $(WRAP_FLAGS)\n\n'
	@printf '  $$(NAME): $$(OBJS) $$(GC_LIB)\n'
	@printf '  	$$(CC) $$(OBJS) $$(GC_LIB) $$(GC_WRAP) -o $$(NAME)\n\n'
	@printf '$(GREEN)═══════════════════════════════════════════════════════════════════════$(RESET)\n'

clean:
	@printf '$(BLUE)► Cleaning object files...$(RESET)\n'
	@$(RM) -r $(OBJ_DIR)
	@printf '$(BLUE)✓ Cleaned$(RESET)\n'

fclean: clean
	@printf '$(BLUE)► Cleaning library...$(RESET)\n'
	@$(RM) $(NAME)
	@printf '$(BLUE)✓ Cleaned$(RESET)\n'

re: fclean all

show:
	@printf '$(YELLOW)Source Files:$(RESET)\n'
	@printf '  Core:       $(words $(SRCS_CORE)) files\n'
	@printf '  Memory:     $(words $(SRCS_MEM)) files\n'
	@printf '  Collector:  $(words $(SRCS_COL)) files\n'
	@printf '  $(BLUE)Total:      $(words $(SRCS)) files$(RESET)\n'

.PHONY: all clean fclean re banner show_usage show