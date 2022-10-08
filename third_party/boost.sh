


set -x
boost_dir=/data/boost/boost_1_80_0
boost_sha='4b2136f98bdd1f5857f1c3dea9ac2018effe65286cf251534b6ae20cc45e1847'
boost_link='https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.tar.gz'
boost=${boost_dir}.tar.gz
boost_log=$(dirname ${boost})/_install.log
mkdir -p /data/boost

#    comma@tici:/data/boost$ sha256sum ./boost_1_80_0.tar.gz
#    4b2136f98bdd1f5857f1c3dea9ac2018effe65286cf251534b6ae20cc45e1847  ./boost_1_80_0.tar.gz
#    comma@tici:/data/boost$ f=($(sha256sum ./boost_1_80_0.tar.gz))
#    comma@tici:/data/boost$ echo ${#f[@]}
#    2

function get_boost {
  set -x
  mkdir -p /data/boost
  test -d ${boost_dir} && echo boost dir exist && exit 0
  test -f "${boost}" || wget -O "${boost}" "${boost_link}"
  sharesult=($(sha256sum "${boost}"))
  test "${#sharesult[@]}" = "2" || (rm -f "${boost}"; echo shafailnumarg ${sharesult[@]}; exit -1)
  test "${sharesult[0]}" = "${boost_sha}" || (rm -f "${boost}"; echo shafailmatch ${sharesult[@]}; exit -2)
  (cd /data/boost; tar -xzvf ${boost}) > ${boost_log}.taroutput.log 2>&1
  return 0
}

(get_boost &) > ${boost_log} 2>&1

