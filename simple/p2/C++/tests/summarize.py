#!/usr/bin/env python

import sys

results = {}

results['success'] = 0
results['errors'] = 0
results['total'] = 0

for fName in sys.argv[1:]:
    f = open(fName,"r")
    
    for line in iter(f.readline, ''):
        s = line.split(',')
        results[s[0]] += int(s[1])

print "Total  = %d" % results['total']
print "Success= %04d / %d" % (results['success'],results['total'])
print "Errors = %04d / %d" % (results['errors'],results['total'])
print "Rate   = %d%%" % (results['success']*100/results['total'])
