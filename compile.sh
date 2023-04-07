#!/bin/bash
curdir=$(pwd)
builddir=${curdir}/build
installdir=${curdir}/install
thirdpartydir=${curdir}/3rd

mkdir -p ${builddir} ${installdir}

# demo-izk
cd ${builddir}
mkdir -p demo-izk
cd demo-izk
cmake ${curdir} -DCMAKE_INSTALL_PREFIX=${installdir} -DCMAKE_PREFIX_PATH=${installdir}
make -j4

cd ${curdir}
