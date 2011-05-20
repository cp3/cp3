rm -rf trogdor.tar.gz
tar cvf trogdor.tar trogdor.c
gzip trogdor.tar
./makeai trogdor
