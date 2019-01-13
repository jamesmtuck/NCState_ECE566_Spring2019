#!/usr/bin/env python

import sys
import re

Stats = {}

p_name = re.compile('.*/(\w*)\.(\w+)\..*\.bc.*\.stats',re.IGNORECASE)

Ids = {}

argv = sys.argv
Normalize = False
if argv[1] == '-N':
    Normalize = True
    argv = argv[1:]

field = argv[1]

for fName in argv[2:]:
    try:
        f = open(fName,"r")
    except:
        print "Error: could not find file %s" % fName
        sys.exit(1)

    m = p_name.match(fName)
    if m==None:
        continue

    g = m.groups()
    if g==None:
        continue

    if len(g) < 2:
        continue

    name = g[0]
    opt = g[1]
    if opt==None:
        opt = "None"

    Ids[name] = 1
    
    if not Stats.has_key(opt):
        Stats[opt] = {}

    if not Stats[opt].has_key(name):
        Stats[opt][name] = {}

    for line in iter(f.readline, ''):
        s = line.split(',')
        if len(s) != 2:
            continue

        Stats[opt][name][s[0]] = s[1].rstrip()
        

s = "Category".ljust(20,)
keys = Stats.keys()
keys.sort(cmp)
for k in keys:
    s += k.rjust(10)

print s

benchs = Ids.keys()
benchs.sort(cmp)

for i in benchs:
    s = str(i).ljust(20,'.')
    for k in keys:
        if Stats[k].has_key(i):
            if Stats[k][i].has_key(field):
                if Normalize==True and Stats.has_key('None') :
                    if Stats['None'][i][field] > 0:
                        s += str(float(Stats[k][i][field])/float(Stats['None'][i][field]))[0:3].rjust(10,'.')
                    else:
                        s += str('x').rjust(10,'.');
                else:
                    s += str(Stats[k][i][field]).rjust(10,'.')
            else:
                s += '(missing)'.rjust(10,'.')
        else:
            s += '(missing)'.rjust(10,'.')
    print s


#for i in Ids.keys():
#    s = str(i).ljust(20,'.')
#    for k in Stats.keys():
#        if Stats[k].has_key(i):
#            if Stats[k][i].has_key(sys.argv[1]):
#                s += str(Stats[k][i][sys.argv[1]]).rjust(10,'.')
#            else:
#                s += '(missing)'.rjust(10,'.')
#        else:
#            s += '(missing)'.rjust(10,'.')
#    print s
