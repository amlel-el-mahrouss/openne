#!/bin/sh

cd dev/Boot/Mod/SysChk
btb amd64.json
cd ../../
make -f amd64-desktop.make efi
make -f amd64-desktop.make epm-img
cd ../../dev/Kernel/HALKit/AMD64
./make_ap_blob.sh
