#!/usr/bin/env python3
# wash zoo

import sys

ifs = open(sys.argv[1], "r")
ofs = open(sys.argv[2], "w")
s = '\t'

for line in ifs:
    split_line = line.split(";")
    ss = s.join(split_line)
    
    ofs.write("%s\n" % ss)