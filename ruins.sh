


usage()
{
	echo "tile x , tile y , {rule number for makefile} give for 0 all "
}
cp Makefile vl_hier_graph  build/
cd build 

if [ -z $1]
then
	usage
else
	make X_TILE="$1" Y_TILE="$2" rule$3 ARGS=$4
#	cat fake_uart.log
fi

