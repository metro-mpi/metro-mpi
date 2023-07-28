/*
Copyright (c) 2023 Barcelona Supercomputing Center (BSC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Barcelona Supercomputing Center nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Barcelona Supercomputing Center "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Barcelona Supercomputing Center BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <mpi.h>

#define MPI_OPT_4 1
//#define MPI_OPT_MULTI 1
using namespace std;

unsigned long long message_async;
MPI_Status status_async;
MPI_Request request_async;

const int nitems=2;
int          blocklengths[2] = {1,1};
MPI_Datatype types[2] = {MPI_UNSIGNED_SHORT, MPI_UNSIGNED_LONG_LONG};
MPI_Datatype mpi_data_type;
MPI_Aint     offsets[2];

const int nitems_noc=6;
int          blocklengths_noc[6] = {1,1,1,1,1,1};
MPI_Datatype types_noc[6] = {MPI_UNSIGNED_LONG_LONG, MPI_UNSIGNED_LONG_LONG, MPI_UNSIGNED_LONG_LONG,
                         MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT};
MPI_Datatype mpi_noc_type;
MPI_Aint     offsets_noc[6];

const int nitems_yummy=3;
int          blocklengths_yummy[3] = {1,1,1};
MPI_Datatype types_yummy[3] = {MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT};
MPI_Datatype mpi_yummy_type;
MPI_Aint     offsets_yummy[3];

const int nitems_all=9;
int          blocklengths_all[9] = {1,1,1,1,1,1,1,1,1};
MPI_Datatype types_all[9] = {MPI_UNSIGNED_LONG_LONG, MPI_UNSIGNED_LONG_LONG, MPI_UNSIGNED_LONG_LONG,
                         MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT,
                         MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT};
MPI_Datatype mpi_all_type;
MPI_Aint     offsets_all[9];

const int nitems_multi_all=10;
int          blocklengths_multi_all[10] = {1,1,1,1,1,1,1,1,1,1};
MPI_Datatype types_multi_all[10] = {MPI_UNSIGNED_LONG_LONG, MPI_UNSIGNED_LONG_LONG, MPI_UNSIGNED_LONG_LONG,
                         MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT,
                         MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT, MPI_UNSIGNED_SHORT,MPI_UNSIGNED_SHORT};
MPI_Datatype mpi_multi_all_type;
MPI_Aint     offsets_multi_all[10];


typedef struct {
    unsigned short valid;
    unsigned long long data;
} mpi_data_t;

typedef struct {
    unsigned short yummy_0;
    unsigned short yummy_1;
    unsigned short yummy_2;
} mpi_yummy_t;

typedef struct {
    unsigned long long data_0;
    unsigned long long data_1;
    unsigned long long data_2;
    unsigned short valid_0;
    unsigned short valid_1;
    unsigned short valid_2;
} mpi_noc_t;

typedef struct {
    unsigned long long data_0;
    unsigned long long data_1;
    unsigned long long data_2;
    unsigned short valid_0;
    unsigned short valid_1;
    unsigned short valid_2;
    unsigned short yummy_0;
    unsigned short yummy_1;
    unsigned short yummy_2;
    
} mpi_all_t;


typedef struct {
    unsigned long long data_0;
    unsigned long long data_1;
    unsigned long long data_2;
    unsigned short valid_0;
    unsigned short valid_1;
    unsigned short valid_2;
    unsigned short yummy_0;
    unsigned short yummy_1;
    unsigned short yummy_2;
    unsigned short dest_id;    
} mpi_multi_all_t;

void initialize(){
    MPI_Init(NULL, NULL);
    //cout << "initializing" << endl;
    
#if defined(MPI_OPT_0)
    // Initialize the struct data&valid
    offsets[0] = offsetof(mpi_data_t, valid);
    offsets[1] = offsetof(mpi_data_t, data);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_data_type);
    MPI_Type_commit(&mpi_data_type);
#endif

#if defined(MPI_OPT_1) || defined(MPI_OPT_3) 
    // Initialize the struct data&valid
    offsets_noc[0] = offsetof(mpi_noc_t, data_0);
    offsets_noc[1] = offsetof(mpi_noc_t, data_1);
    offsets_noc[2] = offsetof(mpi_noc_t, data_2);
    offsets_noc[3] = offsetof(mpi_noc_t, valid_0);
    offsets_noc[4] = offsetof(mpi_noc_t, valid_1);
    offsets_noc[5] = offsetof(mpi_noc_t, valid_2);

    MPI_Type_create_struct(nitems_noc, blocklengths_noc, offsets_noc, types_noc, &mpi_noc_type);
    MPI_Type_commit(&mpi_noc_type);
#endif
#if defined(MPI_OPT_2) || defined(MPI_OPT_3)
    // Initialize the struct data&valid
    offsets_yummy[0] = offsetof(mpi_yummy_t, yummy_0);
    offsets_yummy[1] = offsetof(mpi_yummy_t, yummy_1);
    offsets_yummy[2] = offsetof(mpi_yummy_t, yummy_2);

    MPI_Type_create_struct(nitems_yummy, blocklengths_yummy, offsets_yummy, types_yummy, &mpi_yummy_type);
    MPI_Type_commit(&mpi_yummy_type);
#endif
#ifdef MPI_OPT_4
    // Initialize the struct data&valid
    offsets_all[0] = offsetof(mpi_all_t, data_0);
    offsets_all[1] = offsetof(mpi_all_t, data_1);
    offsets_all[2] = offsetof(mpi_all_t, data_2);
    offsets_all[3] = offsetof(mpi_all_t, valid_0);
    offsets_all[4] = offsetof(mpi_all_t, valid_1);
    offsets_all[5] = offsetof(mpi_all_t, valid_2);
    offsets_all[6] = offsetof(mpi_all_t, yummy_0);
    offsets_all[7] = offsetof(mpi_all_t, yummy_1);
    offsets_all[8] = offsetof(mpi_all_t, yummy_2);

    MPI_Type_create_struct(nitems_all, blocklengths_all, offsets_all, types_all, &mpi_all_type);
    MPI_Type_commit(&mpi_all_type);
#endif
#ifdef MPI_OPT_MULTI
    // Initialize the struct data&valid
    offsets_multi_all[0] = offsetof(mpi_multi_all_t, data_0);
    offsets_multi_all[1] = offsetof(mpi_multi_all_t, data_1);
    offsets_multi_all[2] = offsetof(mpi_multi_all_t, data_2);
    offsets_multi_all[3] = offsetof(mpi_multi_all_t, valid_0);
    offsets_multi_all[4] = offsetof(mpi_multi_all_t, valid_1);
    offsets_multi_all[5] = offsetof(mpi_multi_all_t, valid_2);
    offsets_multi_all[6] = offsetof(mpi_multi_all_t, yummy_0);
    offsets_multi_all[7] = offsetof(mpi_multi_all_t, yummy_1);
    offsets_multi_all[8] = offsetof(mpi_multi_all_t, yummy_2);
    offsets_multi_all[9] = offsetof(mpi_multi_all_t, dest_id);

    MPI_Type_create_struct(nitems_multi_all, blocklengths_multi_all, offsets_multi_all, types_multi_all, &mpi_multi_all_type);
    MPI_Type_commit(&mpi_multi_all_type);
#endif
}

// MPI finish functions
unsigned short mpi_receive_finish(){
    unsigned short message;
    int message_len = 1;
    //cout << "[DPI CPP] Block Receive finish from rank: " << origin << endl << std::flush;
    MPI_Bcast(&message, message_len, MPI_UNSIGNED_SHORT, 0, MPI_COMM_WORLD);

    return message;
}

void mpi_send_finish(unsigned short message, int rank){
    int message_len = 1;

    MPI_Bcast(&message, message_len, MPI_UNSIGNED_SHORT, rank, MPI_COMM_WORLD);
}

// MPI Send 3 NoC messages
void mpi_send_all_noc(unsigned long long data_0, unsigned char valid_0, unsigned long long data_1, unsigned char valid_1, unsigned long long data_2, unsigned char valid_2, int dest, int rank, int flag){
    int message_len = 1;
    mpi_noc_t message;

    message.valid_0 = valid_0;
    message.data_0  = data_0;
    message.valid_1 = valid_1;
    message.data_1  = data_1;
    message.valid_2 = valid_2;
    message.data_2  = data_2;

    MPI_Send(&message, message_len, mpi_noc_type, dest, flag, MPI_COMM_WORLD);

}

mpi_noc_t mpi_receive_all_noc(int origin, int flag){
    int message_len = 1;
    MPI_Status status;
    mpi_noc_t message;

    MPI_Recv(&message, message_len, mpi_noc_type, origin, flag, MPI_COMM_WORLD, &status);
        
    return message;
}

// MPI Send 3 NoC messages
void mpi_send_all_yummy(unsigned short yummy_0, unsigned short yummy_1, unsigned short yummy_2, int dest, int rank, int flag){
    int message_len = 1;
    mpi_yummy_t message;

    message.yummy_0 = yummy_0;
    message.yummy_1 = yummy_1;
    message.yummy_2 = yummy_2;

    MPI_Send(&message, message_len, mpi_yummy_type, dest, flag, MPI_COMM_WORLD);

}

mpi_yummy_t mpi_receive_all_yummy(int origin, int flag){
    int message_len = 1;
    MPI_Status status;
    mpi_yummy_t message;

    MPI_Recv(&message, message_len, mpi_yummy_type, origin, flag, MPI_COMM_WORLD, &status);
        
    return message;
}

// MPI Send 3 NoC messages
void mpi_send_all(mpi_all_t message, int dest, int rank, int flag){
    int message_len = 1;
    MPI_Request request;
    //MPI_Isend(&message, message_len, mpi_all_type, dest, flag, MPI_COMM_WORLD, &request);

    MPI_Send(&message, message_len, mpi_all_type, dest, flag, MPI_COMM_WORLD);
}

mpi_all_t mpi_receive_all(int origin, int flag){
    int message_len = 1;
    MPI_Status status;
    mpi_all_t message;

    MPI_Recv(&message, message_len, mpi_all_type, origin, flag, MPI_COMM_WORLD, &status);
    
    return message;
}

// MPI Send MULTI-TILE all messages
void mpi_send_multi_all(mpi_multi_all_t message, int dest, int rank, int flag){
    int message_len = 1;

    MPI_Send(&message, message_len, mpi_multi_all_type, dest, flag, MPI_COMM_WORLD);
}
// MPI Send MULTI-TILE all messages
mpi_multi_all_t mpi_receive_multi_all(int origin, int flag){
    int message_len = 1;
    MPI_Status status;
    mpi_multi_all_t message;

    MPI_Recv(&message, message_len, mpi_multi_all_type, origin, flag, MPI_COMM_WORLD, &status);
    
    return message;
}

int getRank(){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return rank;
}

int getSize(){
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &size);
    return size;
}

void finalize(){
    //cout << "[DPI CPP] Finalizing" << endl;
    MPI_Finalize();
}

