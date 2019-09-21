#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"
// Using a C library requires extern "C" to prevent function managling
extern "C" {
#include <dyn_array.h>
}


#define NUM_PCB 30
#define QUANTUM 4 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment {
    public:
        virtual void SetUp() {
            score = 0; 
            total = 160;
        }
        virtual void TearDown() {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};


/* 
    LOAD PROCESS CONTROL BLOCKS TEST CASES
 */

//dyn_array_t *load_process_control_blocks(const char *input_file); 

TEST (load_process_control_blocks, nullFilePath) {
	dyn_array_t* da = load_process_control_blocks (NULL);
	ASSERT_EQ(da,(dyn_array_t*) NULL);
}

TEST (load_process_control_blocks, TrickyBadFileName){
    const char* input_file = "";
    dyn_array_t* da = load_process_control_blocks (input_file);
    ASSERT_EQ(da,(dyn_array_t*) NULL);
}

TEST (load_process_control_blocks, emptyFile) {
    char filename[] = "test.txt";
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
    close(fd);
    dyn_array_t* da = load_process_control_blocks (filename);
    ASSERT_EQ(da,(dyn_array_t*) NULL);
}

/*
first come first serve
*/
// Runs the First Come First Served Process Scheduling algorithm over the incoming ready_queue
// \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
// \param result used for first come first served stat tracking \ref ScheduleResult_t
// \return true if function ran successful else false for an error
//bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result);

TEST(first_come_first_serve, nullReadyQueue){
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* da = NULL;
    bool res = first_come_first_serve(da,sr);
    ASSERT_EQ(false , res);
    delete sr;
}

TEST(first_come_first_serve, nullResult){
    ScheduleResult_t *sr = NULL;  
    dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    bool res = first_come_first_serve (da,sr);
	EXPECT_EQ(false,res);
	dyn_array_destroy(da);
}

TEST(first_come_first_serve, nullResultAndNullReadyQueue){
    ScheduleResult_t *sr = NULL;  
    dyn_array_t* da = NULL;
    bool res = first_come_first_serve (da,sr);
	EXPECT_EQ(false,res);
}

TEST(first_come_first_serve, goodInput){
        ScheduleResult_t *sr = new ScheduleResult_t;
        dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
        memset(sr,0,sizeof(ScheduleResult_t));
        ProcessControlBlock_t times[] = {{4,0,false},{7,0,false},{3,0,false}
        };	
        dyn_array_push_back(da,&times[2]);
        dyn_array_push_back(da,&times[1]);
        dyn_array_push_back(da,&times[0]);
        bool res = first_come_first_serve (da,sr);
        ASSERT_EQ(true,res);  // stop if not

        float answers[3] = {5,9.67,14};
	    EXPECT_EQ(answers[0],sr->average_latency_time);
	    EXPECT_EQ(answers[1],sr->average_wall_clock_time);
	    EXPECT_EQ(answers[2],sr->total_run_time);
	    dyn_array_destroy(da);
	    delete sr;
}

/*
bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result);
*/
TEST(priority, nullReadyQueue){
    ScheduleResult_t *sr = new ScheduleResult_t;
    dyn_array_t* da = NULL;
    bool res = priority(da,sr);
    ASSERT_EQ(false , res);
}

TEST(priority, nullResult){
    ScheduleResult_t *sr = NULL;  
    dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    bool res = priority (da,sr);
	EXPECT_EQ(false,res);
	dyn_array_destroy(da);
}

TEST(priority, nullResultAndNullReadyQueue){
    ScheduleResult_t *sr = NULL;  
    dyn_array_t* da = NULL;
    bool res = priority (da,sr);
	EXPECT_EQ(false,res);
}


TEST(priority, goodInput){
        ScheduleResult_t *sr = new ScheduleResult_t;
        dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
        memset(sr,0,sizeof(ScheduleResult_t));
        ProcessControlBlock_t times[] = {{4,1,false},{7,2,false},{3,3,false}
        };	
        dyn_array_push_back(da,&times[2]);
        dyn_array_push_back(da,&times[1]);
        dyn_array_push_back(da,&times[0]);
        bool res = priority (da,sr);
        ASSERT_EQ(true,res);  // stop if not

        float answers[3] = {4.33,9,14};
	    EXPECT_EQ(answers[0],sr->average_latency_time);
	    EXPECT_EQ(answers[1],sr->average_wall_clock_time);
	    EXPECT_EQ(answers[2],sr->total_run_time);
	    dyn_array_destroy(da);
	    delete sr;
}

/*
bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum);
*/
// Runs the Round Robin Process Scheduling algorithm over the incoming ready_queue
// \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
// \param result used for round robin stat tracking \ref ScheduleResult_t
// \param the quantum
// \return true if function ran successful else false for an error
TEST(round_robin , nullReadyQueue){
    dyn_array_t* da = NULL;
    ScheduleResult_t* sr = new ScheduleResult_t;
    size_t q = 5;
    bool res = round_robin(da,sr,q);
    EXPECT_EQ(false, res);
    delete sr;

}
TEST(round_robin , nullResult){
    dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    ScheduleResult_t* sr = NULL;
    size_t q = 5;
    bool res = round_robin(da,sr,q);
    EXPECT_EQ(false,res);
    dyn_array_destroy(da);
}
TEST(round_robin , zeroQuantum){
    dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    ScheduleResult_t* sr = new ScheduleResult_t;
    size_t q = 0;
    bool res = round_robin(da,sr,q);
    EXPECT_EQ(false,res);
    dyn_array_destroy(da);
    delete sr;
}

/*
bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result);
*/

TEST(shortest_remaining_time_first , nullReadyQueue){
    dyn_array_t* da = NULL;
    ScheduleResult_t* sr = new ScheduleResult_t;
    bool res = shortest_remaining_time_first(da,sr);
    EXPECT_EQ(false, res);
    delete sr;

}
TEST(shortest_remaining_time_first , nullResult){
    dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
    ScheduleResult_t* sr = NULL;
    bool res = shortest_remaining_time_first(da,sr);
    EXPECT_EQ(false,res);
    dyn_array_destroy(da);
}

TEST(shortest_remaining_time_first , nullReadyQueueAndNullResult){
    dyn_array_t* da = NULL;
    ScheduleResult_t* sr = NULL;
    bool res = shortest_remaining_time_first(da,sr);
    EXPECT_EQ(false, res);
}

TEST(shortest_remaining_time_first, goodInput){
        ScheduleResult_t *sr = new ScheduleResult_t;
        dyn_array_t* da = dyn_array_create(0,sizeof(ProcessControlBlock_t),NULL);
        memset(sr,0,sizeof(ScheduleResult_t));
        ProcessControlBlock_t times[] = {{4,0,false},{7,0,false},{3,0,false}
        };	
        dyn_array_push_back(da,&times[2]);
        dyn_array_push_back(da,&times[1]);
        dyn_array_push_back(da,&times[0]);
        bool res = shortest_remaining_time_first (da,sr);
        ASSERT_EQ(true,res);  // stop if not

        float answers[3] = {3.33,8,14};
	    EXPECT_EQ(answers[0],sr->average_latency_time);
	    EXPECT_EQ(answers[1],sr->average_wall_clock_time);
	    EXPECT_EQ(answers[2],sr->total_run_time);
	    dyn_array_destroy(da);
	    delete sr;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();

}


