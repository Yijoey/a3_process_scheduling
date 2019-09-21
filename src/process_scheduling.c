#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) {
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(__attribute__((unused)) dyn_array_t *ready_queue, __attribute__((unused)) ScheduleResult_t *result) {
    // You need to remove this when you begin implementation.
    //abort();  // replace me with implementation.
    return false;
}

bool priority(__attribute__((unused)) dyn_array_t *ready_queue, __attribute__((unused)) ScheduleResult_t *result) {
    // You need to remove this when you begin implementation.
   //abort();  // replace me with implementation.
   return false;   
}

bool round_robin(__attribute__((unused)) dyn_array_t *ready_queue, __attribute__((unused)) ScheduleResult_t *result, __attribute__((unused)) size_t quantum) {
    // You need to remove this when you begin implementation.
    //abort();  // replace me with implementation.
    return false;
}

dyn_array_t *load_process_control_blocks(__attribute__((unused)) const char *input_file) {
    // You need to remove this when you begin implementation.
    //abort();  // replace me with implementation.
    return NULL;
}

bool shortest_remaining_time_first(__attribute__((unused)) dyn_array_t *ready_queue, __attribute__((unused)) ScheduleResult_t *result) {
    // You need to remove this when you begin implementation.
    //abort();  // replace me with implementation.
    return false;
}
