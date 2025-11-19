$VERILATOR_ROOT=""

chipset:
	mpic++ -g  -I.  -MMD -I$(VERILATOR_ROOT)/verilator/include -I$(VERILATOR_ROOT)/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=0 -DVM_TRACE_FST=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -DVERILATOR -DPITON_DPI -lstdc++ -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/verilator -DX_TILES=2 -DY_TILES=2 -ggdb -pg -DPITON_DPI -I/home/regaltos/dev/fossi/metro-mpi/build/metro_chipset/rel-0.1/obj_dir -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/pli/iop  -std=gnu++14 -Os -c -o exa_metro_chipset.o exa_metro_chipset.cpp

tile:
	mpic++ -g  -I.  -MMD -I$(VERILATOR_ROOT)/verilator/include -I$(VERILATOR_ROOT)/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=0 -DVM_TRACE_FST=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -DVERILATOR -DPITON_DPI -lstdc++ -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/verilator -DX_TILES=2 -DY_TILES=2 -ggdb -pg -DPITON_DPI -I/home/regaltos/dev/fossi/metro-mpi/build/metro_tile/rel-0.1/obj_dir -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/pli/iop  -std=gnu++14 -Os -c -o exa_metro_tile.o exa_metro_tile.cpp

metro_mp:
	 mpic++ -g  -I.  -MMD -I$(VERILATOR_ROOT)/verilator/include -I$(VERILATOR_ROOT)/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=0 -DVM_TRACE_FST=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -DVERILATOR -DPITON_DPI -lstdc++ -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/verilator -DX_TILES=2 -DY_TILES=2 -ggdb -pg -DPITON_DPI -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/pli/iop  -std=gnu++14 -Os -c -o metro_mpi.o /home/regaltos/dev/fossi/metro-mpi/piton/tools/verilator/exa_mpi_lib.cpp


veril_dpi:
	mpic++  -I. -g  -MMD -I$(VERILATOR_ROOT)/verilator/include -I$(VERILATOR_ROOT)/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=0 -DVM_TRACE_FST=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -DVERILATOR -DPITON_DPI -lstdc++ -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/verilator -DX_TILES=2 -DY_TILES=2 -ggdb -pg -DPITON_DPI -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/pli/iop  -std=gnu++14 -Os -c -o verilated_dpi.o $(VERILATOR_ROOT)/verilator/include/verilated_dpi.cpp

veril_v:
	mpic++  -I. -g  -MMD -I$(VERILATOR_ROOT)/verilator/include -I$(VERILATOR_ROOT)/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=0 -DVM_TRACE_FST=0 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -DVERILATOR -DPITON_DPI -lstdc++ -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/verilator -DX_TILES=2 -DY_TILES=2 -ggdb -pg -DPITON_DPI -I/home/regaltos/dev/fossi/metro-mpi/piton/tools/pli/iop  -std=gnu++14 -Os -c -o verilated.o $(VERILATOR_ROOT)/verilator/include/verilated.cpp

link_l:
	mpic++ -openmp -g  b_ary.o bw_lib.o cpx.o iob.o iob_main.o pcx.o metro_mpi.o exa_metro_chipset.o   verilated.o    verilated_dpi.o  /home/regaltos/dev/fossi/metro-mpi/build/metro_chipset/rel-0.1/obj_dir/Vmetro_chipset__ALL.a  exa_metro_tile.o /home/regaltos/dev/fossi/metro-mpi/build/metro_tile/rel-0.1/obj_dir/Vmetro_tile__ALL.a exa_metro_mpi.o -o comb_bin

all : chipset tile veril_v metro_mp veril_dpi  link_l 
