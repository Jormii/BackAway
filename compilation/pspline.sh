ELF=$1
ADDR=$2

psp-addr2line -e $ELF -f -C $ADDR
