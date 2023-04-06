#!/bin/bash
curdir=$(pwd)
builddir=${curdir}/build
installdir=${curdir}/install
thirdpartydir=${curdir}/3rd

mkdir -p ${builddir} ${installdir}

# emp*
cd ${builddir}
mkdir -p emp-tool
cd emp-tool
cmake ${thirdpartydir}/emp-tool -DCMAKE_INSTALL_PREFIX=${installdir} -DCMAKE_PREFIX_PATH=${installdir}
make -j4
make install

cd ${builddir}
mkdir -p emp-ot
cd emp-ot
cmake ${thirdpartydir}/emp-ot -DCMAKE_INSTALL_PREFIX=${installdir} -DCMAKE_PREFIX_PATH=${installdir}
make -j4
make install

cd ${builddir}
mkdir -p emp-zk
cd emp-zk
cmake ${thirdpartydir}/emp-zk -DCMAKE_INSTALL_PREFIX=${installdir} -DCMAKE_PREFIX_PATH=${installdir}
make -j4
make install

# secp256k1
cd ${builddir}
mkdir -p secp256k1
cd secp256k1
cmake ${thirdpartydir}/secp256k1 -DCMAKE_INSTALL_PREFIX=${installdir} -DCMAKE_PREFIX_PATH=${installdir} \
  -DSECP256K1_BUILD_EXAMPLES=ON -DSECP256K1_ENABLE_MODULE_RECOVERY=ON
make -j4
make install

# cpp-httplib
cd ${builddir}
mkdir -p cpp-httplib
cd cpp-httplib
cmake ${thirdpartydir}/cpp-httplib -DCMAKE_INSTALL_PREFIX=${installdir} -DCMAKE_PREFIX_PATH=${installdir}
make -j4
make install

cd ${curdir}
