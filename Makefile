
X_TILE=2
Y_TILE=2
ARGS=""

rule1:
	sims ${ARGS} -vlt_build -ariane -vlt_build_args="-MAKEFLAGS CXX=mpic++ -MAKEFLAGS LINK=mpic++ " -vlt_build_args='-j 2' -config_rtl=DISABLE_ALL_MONITORS -config_rtl=MINIMAL_MONITORING -sys=metro_chipset -x_tiles=${X_TILE} -y_tiles=${Y_TILE}
	

rule2:
	sims ${ARGS} -vlt_build -ariane -vlt_build_args="-MAKEFLAGS CXX=mpic++ -MAKEFLAGS LINK=mpic++ " -vlt_build_args='-j 2' -config_rtl=DISABLE_ALL_MONITORS -config_rtl=MINIMAL_MONITORING -sys=metro_tile -x_tiles=${X_TILE} -y_tiles=${X_TILE}

rule3:
	sims  -vlt_run -ariane -sys=metro_chipset -x_tiles=${X_TILE} -y_tiles=${Y_TILE} hello_world_token.c -model_dir=`pwd` -cyclesCheckFinish=10000 -cyclesCheckFinishAfter=1000

rule0: rule1 rule2 rule3


rule1_hier:
	python3 vl_hier_graph --f flist -top-module metro_chipset -o metro_chipset_${X_TILE}_${Y_TILE}.dot -Wno-fatal -Wno-BLKANDNBLK -Wno-TIMESCALEMOD
	cp metro_chipset_${X_TILE}_${Y_TILE}.dot ../dots 
	dot -Tpdf metro_chipset_${X_TILE}_${Y_TILE}.dot -o metro_chipset_${X_TILE}_${Y_TILE}.pdf

rule2_hier:
	python3 vl_hier_graph --f flist -top-module metro_tile -o metro_tile_${X_TILE}_${Y_TILE}.dot -Wno-fatal -Wno-BLKANDNBLK -Wno-TIMESCALEMOD
	cp metro_tile_${X_TILE}_${Y_TILE}.dot ../dots
	dot -Tpdf metro_tile_${X_TILE}_${Y_TILE}.dot -o metro_tile_${X_TILE}_${Y_TILE}.pdf
