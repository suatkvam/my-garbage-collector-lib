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
RED         := \033[0;31m
RESET       := \033[0m

# Directory Structure
SRC_DIR     = src
INC_DIR     = include
OBJ_DIR     = obj

# Source Files (organized by functionality)
SRCS        = $(wildcard $(SRC_DIR)/*.c)

# Object Files
OBJS        = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Header Files
HEADERS     = $(INC_DIR)/garbage_collector.h

# Rules
all: banner $(NAME)
	@printf '$(GREEN)✓ Build completed successfully!$(RESET)\n\n'

banner:
	@printf '$(BLUE)'
	@printf '╔════════════════════════════════════════╗\n'
	@printf '║    GARBAGE COLLECTOR LIBRARY BUILD    ║\n'
	@printf '╔════════════════════════════════════════╗\n'
	@printf '$(RESET)\n'

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	@printf '$(GREEN)► Compiling: $(RESET)%-30s' "$(notdir $<)"
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf '$(GREEN)✓$(RESET)\n'

$(NAME): $(OBJS)
	@printf '\n$(YELLOW)► Creating static library: $(NAME)$(RESET)\n'
	@$(AR) $(NAME) $(OBJS)
	@printf '$(YELLOW)✓ Library created: $(NAME)$(RESET)\n'

clean:
	@printf '$(BLUE)► Cleaning object files...$(RESET)\n'
	@$(RM) -r $(OBJ_DIR)
	@printf '$(BLUE)✓ Object files cleaned$(RESET)\n'

fclean: clean
	@printf '$(BLUE)► Cleaning library files...$(RESET)\n'
	@$(RM) $(NAME)
	@printf '$(BLUE)✓ Library files cleaned$(RESET)\n'

re: fclean all

# Utility rules
show:
	@printf '$(YELLOW)Project Configuration:$(RESET)\n'
	@printf '  NAME:     $(NAME)\n'
	@printf '  CC:       $(CC)\n'
	@printf '  CFLAGS:   $(CFLAGS)\n'
	@printf '  SRCS:     $(words $(SRCS)) files\n'
	@printf '  OBJS:     $(words $(OBJS)) files\n'

help:
	@printf '$(BLUE)Available targets:$(RESET)\n'
	@printf '  all      - Build the library (default)\n'
	@printf '  clean    - Remove object files\n'
	@printf '  fclean   - Remove object files and library\n'
	@printf '  re       - Rebuild everything\n'
	@printf '  show     - Show project configuration\n'
	@printf '  help     - Show this help message\n'

.PHONY: all clean fclean re banner show help
