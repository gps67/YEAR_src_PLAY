. ../.env_test

. fns_ASKS

ls -l $SPIN_py_SP_lib
echo SP_lib is not on PATH but is on PYTHONPATH
echo this will fail not found
type spin_py_so_HERE

say_do python2 -c "import spin_py"

say_do python2 -c "

import sys
import pprint

pprint.pprint(sys.path)

import spin_py
print('IMPORTED')
"

cd $SPIN_py_SP_lib
pwd

say_do python2 -c "import spin_py"
