TARGET_EXEC := scheduler

BUILD_DIR := ./build
TEST_DIR := ./test
SRC_DIRS := ./src


SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)


DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -g -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -g -c $< -o $@




.PHONY: clean test
clean:
	rm -r $(BUILD_DIR)
	rm -f Scheduler_test minheap_test simulation_test
test:
	gcc test/Scheduler_test.c src/Scheduler/Scheduler.c src/Queue/Queue.c src/Queue/MinHeap.c -g -o Scheduler_test
	gcc ${TEST_DIR}/MinHeap_test.c src/Queue/MinHeap.c src/Job/Job.c src/IO/IO.c -g -o minheap_test
	gcc ${TEST_DIR}/Simulation_test.c src/Simulation/Simulation.c src/Queue/Queue.c  src/IO/IO.c src/Queue/MinHeap.c src/Scheduler/Scheduler.c src/Job/Job.c -g -o simulation_test
	gcc ${TEST_DIR}/IO_test.c src/IO/IO.c src/Job/Job.c -g -o io_test
-include $(DEPS)