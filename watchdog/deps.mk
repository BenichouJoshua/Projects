DEPS_CMD := gcc -MM $(SRC)$(current).c -I$(INCLUDE) | tr -d '\n\\'

DEPS := $(notdir $(filter %.h, $(shell $(DEPS_CMD))))
DEPS := $(filter-out $(current).h, $(DEPS))
DEPS := $(basename $(DEPS))

$(foreach dep,$(DEPS), \
	$(eval OBJ_DBG += $(BIN_DBG)lib$(dep).so) \
	$(eval OBJ_REL += $(BIN_REL)lib$(dep).so) \
	$(eval LDFLAGS += -l$(dep)) \
)