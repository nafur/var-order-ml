#!/usr/bin/env python3

import argparse
import datetime
import glob
import logging
import os
import progressbar
import re
import shutil
import subprocess
import sys

from actions import export_theory_calls, collect_theory_calls, generate_results, collect_results

def guess_scratch_dir():
    match = re.match('(/barrett/scratch/[0-9a-zA-Z]+).*', os.getcwd())
    if match is not None:
        return match.group(1)
    return os.getcwd()


scratch_dir = guess_scratch_dir()

# Utility parser that are reused later via the parents option.
slurm_options_parser = argparse.ArgumentParser(add_help=False)
slurm_options_parser.add_argument('--partition', default='octa', help='slurm partition to use (default: %(default)s)')
slurm_options_parser.add_argument('--script', default='/barrett/scratch/local/bin/submit-solver-runexec.sh', help='submit script (default: %(default)s)')
slurm_options_parser.add_argument('--timeout', type=int, default=600, help='timeout for the slurm job (default: %(default)s)')

# Main parser
parser = argparse.ArgumentParser(description='use machine learning to learn variable orderings')
parser.add_argument('-v', '--verbose', action='count', default=0, help = 'be more verbose')
parser.add_argument('--dry', action = 'store_true', help = 'do not actually execute anything')
subparsers = parser.add_subparsers(dest='subcommand')
subparsers.required = True

# Subparser for export-theory-calls
parser_etc = subparsers.add_parser('export-theory-calls', help='runs a solver that exports all theory calls to separate files', parents=[slurm_options_parser])
parser_etc.add_argument('--solver', default=os.path.join(scratch_dir, 'CVC4/build/bin/cvc4-tc-export'), help='full path to a solver binary that exports theory calls (default: %(default)s)')
parser_etc.add_argument('--solver-options', default='--no-nl-ext-inc-prec --nl-cad --theoryof-mode=type', help='options passed to the solver (default: %(default)s)')
parser_etc.add_argument('--benchmarks', action='append', default='non_incremental_QF_NRA', help='benchmark set to use (default: %(default)s)')
parser_etc.add_argument('--target', default=os.path.join(scratch_dir, 'slurmwd', '{:%m%d-%H%M}-theory-calls'.format(datetime.datetime.now())), help='Target directory (default: %(default)s)')

# Subparser for collect-theory-calls
parser_ctc = subparsers.add_parser('collect-theory-calls', help='collects the exported theory calls into one directory')
parser_ctc.add_argument('source', nargs='+', help='source directory')
parser_ctc.add_argument('--target-dir', default='theory-calls', help='where to collect all theory calls (default: %(default)s)')

# Subparser for generate-results
parser_gr = subparsers.add_parser('generate-results', help='run one solver on the exported theory calls', parents=[slurm_options_parser])
parser_gr.add_argument('--target', default='slurmwd/{:%m%d-%H%M}-{{solver}}'.format(datetime.datetime.now()), help='target directory (default: %(default)s)')
parser_gr.add_argument('--solver', default=os.path.join(scratch_dir, 'CVC4/build/bin/cvc4'), help='full path to a solver binary (default: %(default)s)')
parser_gr.add_argument('--solver-options', default='--no-nl-ext-inc-prec --nl-cad --theoryof-mode=type --simplification=none', help='options passed to the solver (default: %(default)s)')
parser_gr.add_argument('--benchmarks', default='theory_calls', help='benchmark set to use (default: %(default)s)')

# Subparser for collect-results
parser_cr = subparsers.add_parser('collect-results', help='Collects results from the solver runs')
parser_cr.add_argument('benchmarks', help='benchmark directory')
parser_cr.add_argument('source', nargs='+', help='source directory')
parser_cr.add_argument('--script', default='/barrett/scratch/local/bin/cmpr.py', help='analysis script for slurm benchmarks (default: %(default)s)')
parser_cr.add_argument('--result', default='data.json', help='data file with all collected results (default: %(default)s)')

# Subparser for train
parser_t = subparsers.add_parser('train', help='Train the classifiers')

# Subparser for evaluate
parser_e = subparsers.add_parser('evaluate', help='Evaluate the classifiers')

args = parser.parse_args()

if args.verbose == 0:
    logging.basicConfig(level=logging.WARN)
elif args.verbose == 1:
    logging.basicConfig(level=logging.INFO)
elif args.verbose == 2:
    logging.basicConfig(level=logging.DEBUG)

if args.subcommand == 'export-theory-calls':
    export_theory_calls.export_theory_calls(args)
elif args.subcommand == 'collect-theory-calls':
    collect_theory_calls.collect_theory_calls(args)
elif args.subcommand == 'generate-results':
    generate_results.generate_results(args)
elif args.subcommand == 'collect-results':
    collect_results.collect_results(args)
else:
    print('Unknown: {}'.format(args.subcommand))
