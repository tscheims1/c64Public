./cbp2make.linux-x86_64 -in ../c64/MemoryManager.cbp
cd ../c64/
make -B -f ../c64/MemoryManager.cbp.mak debug
cd ../cpu-emu2/
make -B -f graphicmake
echo project successful build
