#!/usr/bin/env python
import os
import sys
import time
import logging
import argparse
from datetime import datetime

from DevTools.Ntuplizer.validationParams import params

logging.basicConfig(level=logging.INFO, stream=sys.stderr)

def add_job(jobname,args,name,params):
    '''Add a single validation job'''
    kwargs = params['kwargs']
    outdir = 'validation/{0}/{1}'.format(jobname,name)
    if not args.full: kwargs['maxEvents'] = 1000
    kwargs['outputFile'] = '{0}_miniTree.root'.format(name)
    if args.save: kwargs['outputFile'] = '{0}/miniTree.root'.format(outdir)
    config = args.cfg
    command = 'cmsRun {0} {1}'.format(config,' '.join(['{0}={1}'.format(a,b) for a,b in kwargs.iteritems()]))
    logging.info('Running command: {0}'.format(command))
    if args.save:
        os.system('mkdir -p {0}'.format(outdir))
        if args.bg:
            command += ' > {0}/miniTree.log 2>&1 &'.format(outdir)
        else:
            command += ' 2>&1 | tee {0}/miniTree.log'.format(outdir)
    os.system(command)

def validate(args):
    '''Validate ntuplizer'''
    jobname = datetime.now().strftime('%Y%m%d_%H%M%S')
    keys = params.keys()
    if args.jobs: keys = args.jobs
    for p in keys:
        if p not in params.keys():
            logging.warning('Unrecognized job string {0}. Allowed strings: {1}'.format(p,' '.join(sorted(params.keys()))))
            continue
        add_job(jobname,args,p,params[p])


def parse_command_line(argv):
    parser = argparse.ArgumentParser(description='Validate Ntuples')

    parser.add_argument('--full', action='store_true', help='Run full jobs')
    parser.add_argument('--bg', action='store_true', help='Run in the background')
    parser.add_argument('--save', action='store_true', help='Save output into directory')
    parser.add_argument('jobs', nargs='*', help='Job strings to run, default to run all')
    parser.add_argument('--cfg', default='DevTools/Ntuplizer/test/MiniTree_cfg.py', help='Config to validate')

    return parser.parse_args(argv)

def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]

    args = parse_command_line(argv)

    validate(args)

if __name__ == "__main__":
    status = main()
    sys.exit(status)
