#! /bin/bash
if [ -z "$BASH_VERSION" ]; then exec bash "$0" "$@"; fi;
# csmake.sh
# Copyright (C) 2014 spaceconcordia <spaceconcordia@mustang>
#
# Distributed under terms of the MIT license.
#
# colors: echo -e "${red}Text${NC}"
NC='\e[0m';black='\e[0;30m';darkgrey='\e[1;30m';blue='\e[0;34m';lightblue='\e[1;34m';green='\e[0;32m';lightgreen='\e[1;32m';cyan='\e[0;36m';lightcyan='\e[1;36m';red='\e[0;31m';lightred='\e[1;31m';purple='\e[0;35m';lightpurple='\e[1;35m';orange='\e[0;33m';yellow='\e[1;33m';lightgrey='\e[0;37m';yellow='\e[1;37m';

declare -a SysReqs=('dialog' 'whiptail')
for item in ${SysReqs[*]}; do command -v $item >/dev/null 2>&1 || { echo >&2 "I require $item but it's not installed.  Aborting."; exit 1; }; done

confirm () { #https://stackoverflow.com/a/3232082
    read -r -p "${1:-[y/N]} [y/N] " response
    case $response in
        [yY][eE][sS]|[yY]) 
            true
            ;;
        *)
            false
            ;;
    esac
}

fail () {
    echo -e "${red}$1 ABORTING...${NC}" 
    exit 1
}
quit () {
    echo -e "$1 exiting..."
    exit 0
}

ensure-directories () {
  mkdir -p $CS1_DIR/HE100-lib/C/lib
}

mbcc-static-c () {
  make buildQ6  
  ar rcs lib/libhe100-c-mbcc.a lib/he100-mbcc.o
}

mbcc-static-cpp () {
  make buildQ6Cpp
  ar rcs lib/libhe100-mbcc.a lib/he100-mbcc.o
}

x86-static-c () {
  make buildBin
  ar rcs lib/libhe100-c.a lib/he100.o
}

x86-static-cpp () {
  make buildBinCpp
  ar rcs lib/libhe100.a lib/he100.o
}

bb-static-cpp () {
  make buildBB
  ar rcs lib/libhe100-BB.a lib/he100-BB.o
}

make-run-gtest () {
  cd test
  bash csmaketest.sh $1 #will pass gdm argument to csmaketest.sh
  cd ..
}

for arg in "$@"; do
    case $arg in
        "Q6")
            mbcc-static-cpp; quit;
        ;;
        "PC")
            x86-static-cpp; quit;
        ;;
        "BB")
            bb-static-cpp; quit;
        ;;
        "test")
            make-run-gtest
        ;;
        "gdb")
            make-run-gtest gdb
    esac
done

CURRENT_DIR="${PWD##*/}"
if [ "$CURRENT_DIR" != "C" ]; then fail "This script must be run from HE100_lib/C, not $CURRENT_DIR"; fi;
ensure-directories

usage="usage: csmake.sh [options] "
if [ $# -eq 0 ]; then 
    echo "No arguments supplied... $usage"
    echo "Running default conditions"
    x86-static-cpp
    mbcc-static-cpp
    make-run-gtest
fi 

quit # exit cleanly
