
function clean_up {
  kill $pida
  kill $pidb
}
trap clean_up SIGHUP SIGINT SIGTERM

pythonport=80
pythonbind=192.168.1.219

myname=$(basename ${BASH_SOURCE[0]})
mydir=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)


export BIND=192.168.1.219
export HOST=$BIND
export PORT=443
python3 -m http.server --bind $pythonbind --directory ${mydir}/public $pythonport &
pidb=$!
pida=0

if pgrep > /dev/null 2>&1  wssServer; then
  pida=$(pgrep wssServer)
  echo wsserver already run
else
  echo wsserver start
  ${mydir}/wssServer &
  pida=$!
fi

if wait $pida $pidb; then
  echo 'wssServer returned nonzero'
else:
  echo 'wssServer returned zero(success)'
fi
