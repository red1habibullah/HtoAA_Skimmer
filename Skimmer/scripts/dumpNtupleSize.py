#!/usr/bin/env python
import operator
import argparse
import sys

import ROOT
ROOT.gROOT.SetBatch(True)

def parse_command_line(argv):
    parser = argparse.ArgumentParser(description='Submit analyzers')

    parser.add_argument('fname', type=str, default='', help='Analysis to submit')

    return parser.parse_args(argv)

def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]

    args = parse_command_line(argv)
    
    tfile = ROOT.TFile(args.fname)
    print 'Compression level:', tfile.GetCompressionLevel()
    print 'Compression factor:', tfile.GetCompressionFactor()
    miniTree = tfile.Get('miniTree/MiniTree')
    lumiTree = tfile.Get('miniTree/MiniTree')
    
    sizes = {}
    for branch in miniTree.GetListOfBranches():
        sizes[branch.GetName()] = branch.GetZipBytes()
    
    objects = ['muons','electrons','taus','photons','jets','genParticles','vertices','pfmet']
    
    totalSizes = {}
    for branch in sizes:
        found = False
        for obj in objects:
            if branch.startswith(obj):
                if obj not in totalSizes: totalSizes[obj] = 0
                totalSizes[obj] += sizes[branch]
                found = True
        if not found and (branch.endswith('Pass') or branch.endswith('Prescale')):
            if 'trigger' not in totalSizes: totalSizes['trigger'] = 0
            totalSizes['trigger'] += sizes[branch]
            found = True
        if not found:
            if 'other' not in totalSizes: totalSizes['other'] = 0
            totalSizes['other'] += sizes[branch]
    
    sumSize = sum(totalSizes.values())
    for branch, size in sorted(totalSizes.items(), key=operator.itemgetter(1), reverse=True):
        print '{0:20} {1:10.2f} MB ({2:5.1f} %)'.format(branch, float(size)/1024/1024, float(size)/sumSize*100)
    print '{0:20} {1:10.2f} MB'.format('Total', float(sumSize)/1024/1024)

    return 0

if __name__ == "__main__":
    func = main()
    sys.exit(func)

