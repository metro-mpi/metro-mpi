.PHONY: all setup update_march update_version update_jobs replace_verilator

export PITON_ROOT := $(shell pwd)
NUM_CPUS := $(shell nproc)
dir=$(pwd)

#=======================================================================================

all: setup update_march update_version replace_verilator update_jobs

setup:
	git submodule update --init --recursive

update_march:
	@echo "Updating architecture flags..."
	@find $(PITON_ROOT)/piton -type f \
		-exec sed -i \
			-e 's/-march=rv32im /-march=rv32imafdc_zicsr /g' \
			-e 's/-march=rv32i /-march=rv32imafdc_zicsr /g' \
			-e 's/-march=rv64imc/-march=rv64imafdc_zicsr /g' \
			-e 's/-mabi=ilp32 /-mabi=ilp32d /g' \
			-e 's/-mabi=lp64 /-mabi=lp64d /g' \
			-e 's/-march=rv64imac /-march=rv64imafdc_zicsr /g' \
			-e 's/-march=rv32imc /-march=rv32imafdc_zicsr /g' \
			-e 's/-4.014/-5.036/g' \
			{} +

update_version:
	@echo "Updating GCC configuration..."
	@sed -i '28s|.*|      ./configure --prefix=$$RISCV --with-arch=rv64imafdc_zicsr --with-abi=lp64d|' \
		$(PITON_ROOT)/piton/design/chip/tile/ariane/ci/build-riscv-gcc.sh

replace_verilator:
	@echo "Replacing Verilator Installation Script..."
	@rm $(PITON_ROOT)/piton/design/chip/tile/ariane/ci/install-verilator.sh
	@cp $(PITON_ROOT)/install-verilator.sh $(PITON_ROOT)/piton/design/chip/tile/ariane/ci/.
	@chmod +x $(PITON_ROOT)/piton/design/chip/tile/ariane/ci/install-verilator.sh

update_jobs:
	@echo "Updating job count to $(NUM_CPUS)..."
	@find $(PITON_ROOT)/piton -type f -exec sed -i 's/NUM_JOBS=1/NUM_JOBS=$(NUM_CPUS)/g' {} +
	@find $(PITON_ROOT)/piton -type f -exec sed -i 's/NUM_JOBS=4/NUM_JOBS=$(NUM_CPUS)/g' {} +
	@find $(PITON_ROOT)/piton -type f -exec sed -i 's/NUM_JOBS=8/NUM_JOBS=$(NUM_CPUS)/g' {} +