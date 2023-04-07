# ethbeijing-demo-izk


## Quick Start

```sh
git clone --recursive https://github.com/pado-labs/ethbeijing-demo-izk.git
cd ethbeijing-demo-izk
git submodule update --init
bash ./compile_deps.sh
bash ./compile.sh
```

## Running

```sh
bash ./run_demo.sh
```

**OR**

```sh
# prepare
cd build/demo-izk/bin
mkdir -p data
cp ../../../install/lib/lib* ./

# run
# ./test_demo <party> <mpc_port> <mpc_host> <http_port>
```

<br/>

- party: `1(Prover)` or `2(Verifier)`.
- mpc_port: default is `12345`.
- mpc_host: default is `127.0.0.1`.
- http_port: default is `8000`.


<br/>

for example:

- run `./test_demo 1` on one termial/machine.
- run `./test_demo 2` on the other termial/machine.


