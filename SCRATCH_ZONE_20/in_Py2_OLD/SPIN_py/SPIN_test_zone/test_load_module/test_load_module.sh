. ../.env_test

set -x
ls -l $SPIN_py_SP_lib
type spin_py_so_HERE

python2 -c "import spin_py"

cd $SPIN_py_SP_lib

python2 -c "import spin_py"
