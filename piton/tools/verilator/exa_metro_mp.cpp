#include "Vmetro_chipset.h"
#include "Vmetro_tile.h"
#include "verilated.h"
#include <iostream>
#include <vector>

#define VERILATED_VCD
#include "verilated_vcd_c.h"
#endif

#include <iomanip>
#include <omp.h>

const int YUMMY_NOC_1  = 0;
const int DATA_NOC_1   = 1;
const int YUMMY_NOC_2  = 2;
const int DATA_NOC_2   = 3;
const int YUMMY_NOC_3  = 4;
const int DATA_NOC_3   = 5;
const int TEST_FINISH  = 6;
const int DATA_ALL_NOC = 7;
const int ALL_YUMMY    = 8;
const int ALL_NOC      = 9;

const int PITON_X_TILES=X_TILES;
const int PITON_Y_TILES=Y_TILES;


uint64_t main_time = 0; // Current simulation time
uint64_t clk = 0;
int rank, dest, size;
int rankN, rankS, rankW, rankE;
int tile_x, tile_y;//, PITON_X_TILES, PITON_Y_TILES;

std::vector<Vmetro_tile* > top_tiles(PITON_X_TILES*PITON_Y_TILES,Vmetro_tile*);

Vmetro_chipset* top_chipset=new Vmetro_chipset;

void initialize();

void chipset_tick() {
    top_chipset->core_ref_clk = !top->core_ref_clk;
    chipset_main_time += 250;
    top_chipset->eval();
#ifdef VERILATOR_VCD
    tfp->dump(main_time);
#endif
    top_chipset->core_ref_clk = !top->core_ref_clk;
    chipset_main_time += 250;
    top_chipset->eval();
#ifdef VERILATOR_VCD
    tfp->dump(chipset_main_time);
#endif
}


void chipset_reset_and_init() {
    
    //    fail_flag = 1'b0;
    //    stub_done = 4'b0;
    //    stub_pass = 4'b0;

    // Clocks initial value
    top_chipset->core_ref_clk = 0;

    // Resets are held low at start of boot
    top_chipset->sys_rst_n = 0;
    top_chipset->pll_rst_n = 0;

    top_chipset->ok_iob = 0;

    // Mostly DC signals set at start of boot
    //    clk_en = 1'b0;
    top_chipset->pll_bypass = 1; // trin: pll_bypass is a switch in the pll; not reliable
    top_chipset->clk_mux_sel = 0; // selecting ref clock
    // rangeA = x10 ? 5'b1 : x5 ? 5'b11110 : x2 ? 5'b10100 : x1 ? 5'b10010 : x20 ? 5'b0 : 5'b1;
    top_chipset->pll_rangea = 1; // 10x ref clock
    // pll_rangea = 5'b11110; // 5x ref clock
    // pll_rangea = 5'b00000; // 20x ref clock
    
    // JTAG simulation currently not supported here
    //    jtag_modesel = 1'b1;
    //    jtag_datain = 1'b0;

    top_chipset->async_mux = 0;

    top_chipset->processor_offchip_noc1_valid = 0;
    top_chipset->processor_offchip_noc1_data  = 0;
    top_chipset->offchip_processor_noc1_yummy = 0;
    top_chipset->processor_offchip_noc2_valid = 0;
    top_chipset->processor_offchip_noc2_data  = 0;
    top_chipset->offchip_processor_noc2_yummy = 0;
    top_chipset->processor_offchip_noc3_valid = 0;
    top_chipset->processor_offchip_noc3_data  = 0;
    top_chipset->offchip_processor_noc3_yummy = 0;
    top_chipset->test_ena = 0;

    chipset_test_end=0;

    init_jbus_model_call((char *) "mem.image", 0);

    //std::cout << "Before first ticks" << std::endl << std::flush;
    chipset_tick();
    //std::cout << "After very first tick" << std::endl << std::flush;
    // Reset PLL for 100 cycles
    //    repeat(100)@(posedge core_ref_clk);
    //    pll_rst_n = 1'b1;
    for (int i = 0; i < 100; i++) {
        chipset_tick();
    }
    top_chipset->pll_rst_n = 1;

    //std::cout << "Before second ticks" << std::endl << std::flush;
    // Wait for PLL lock
    //    wait( pll_lock == 1'b1 );
    //while (!top_chipset->pll_lock) {
    //    chipset_tick();
    //}

    //std::cout << "Before third ticks" << std::endl << std::flush;
    // After 10 cycles turn on chip-level clock enable
    //    repeat(10)@(posedge `CHIP_INT_CLK);
    //    clk_en = 1'b1;
    for (int i = 0; i < 10; i++) {
        chipset_tick();
    }
    top_chipset->clk_en = 1;

    // After 100 cycles release reset
    //    repeat(100)@(posedge `CHIP_INT_CLK);
    //    sys_rst_n = 1'b1;
    //    jtag_rst_l = 1'b1;
    for (int i = 0; i < 100; i++) {
        chipset_tick();
    }
    top_chipset->sys_rst_n = 1;

    // Wait for SRAM init, trin: 5000 cycles is about the lowest
    //    repeat(5000)@(posedge `CHIP_INT_CLK);
    for (int i = 0; i < 5000; i++) {
        chipset_tick();
    }

    //    top_chipset->diag_done = 1;

    //top_chipset->ciop_fake_iob.ok_iob = 1;
    top_chipset->ok_iob = 1;
    std::cout << "Reset complete (Chipset)" << std::endl << std::flush;
}

uint64_t chipset_main_timei=0;
void mpi_tick() {
    top_chipset->core_ref_clk = !top_chipset->core_ref_clk;
    chipset_main_time += 250;
    top_chipset->eval();
#ifdef MPI_OPT_4
    mpi_work_opt_4_chipset();
#endif
    top_chipset->eval();
//#ifdef VERILATOR_VCD
//    tfp->dump(chipset_main_time);
//#endif
    top_chipset->core_ref_clk = !top_chipset->core_ref_clk;
    chipset_main_time += 250;
    top_chipset->eval();
}

int getRank()
{
	//get the threadid 

}
int getSize()
{
	// num of threads in the program
}

void finalize()
{
	//finish all the process 
}

double chipset_sc_time_stamp()
{
	return chipset_main_time;
}

int main()
{
    Verilated::commandArgs(argc, argv);

    assert(argc >= 3 && "Add argument how many cycles to start checking and later frequency");

//#ifdef VERILATOR_VCD
//    Verilated::traceEverOn(true);
//    tfp = new VerilatedVcdC;
//    top->trace (tfp, 99);
//    tfp->open ("my_metro_chipset.vcd");
//
//    Verilated::debug(1);
//#endif

    initialize(); //TODO
    rank = getRank(); // threadid
    size = getSize(); // number of threads 
    
    std::cout << "CHIPSET size: " << size << ", rank: " << rank <<  std::endl;
    //thread id ==0 
    if (rank==0) {
        dest = 1;
    } else {
        dest = 0;
    }


    chipset_reset_and_init();

    top_chipset->test_ena = 1;

    bool chipset_test_exit = false;
    uint64_t cyclesToCheckEnd=std::stoi(argv[1]); //Let this variables be private to threads 
    uint64_t CyclesToCheckEndAfter=std::stoi(argv[2]);
    while (!Verilated::gotFinish() and !chipset_test_exit) { 
        mpi_tick();
        if (cyclesToCheckEnd==0) {
            mpi_send_finish(test_end, rank);// finish signal by threaid 0
            cyclesToCheckEnd=CyclesToCheckEndAfter;
            chipset_test_exit=chipset_test_end;
        }
        else {
            cyclesToCheckEnd--;
        }
    }

    std::cout << "ticks: " << std::setprecision(10) << chipset_sc_time_stamp() << " , cycles: " << chipset_sc_time_stamp()/500 << std::endl;


    finalize();

    delete top_chipset;
    exit(0);

}
