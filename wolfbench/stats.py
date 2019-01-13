#!/usr/bin/env python

import sys
import re

Stats = {}

p_name = re.compile('.*\.(\w+)\.bc\.stats',re.IGNORECASE)

Ids = {}

for fName in sys.argv[1:]:
    try:
        f = open(fName,"r")
    except:
        print "Error: could not find file %s" % fName
        sys.exit(1)

    m = p_name.match(fName)
    g = m.groups()

    opt = g[0]
    if not Stats.has_key(opt):
        Stats[opt] = {}

    stats = {}

    for line in iter(f.readline, ''):
        s = line.split(',')
        if stats.has_key(s[0]):
            stats[s[0]] += int(s[1])
        else:
            if not s[0]=='id':
                Ids[s[0]] = 0
                stats[s[0]] = int(s[1])

    for k in stats.keys():
        if Stats[opt].has_key(k):
            Stats[opt][k] += stats[k]
        else:
            Stats[opt][k] = stats[k]


s = "Category".ljust(20,)
for k in Stats.keys():
    s += k.rjust(10)

print s

for i in Ids.keys():
    s = str(i).ljust(20,'.')
    for k in Stats.keys():
        s += str(Stats[k][i]).rjust(10,'.')
    print s
