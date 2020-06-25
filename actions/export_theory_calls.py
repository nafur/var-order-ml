import logging
import os
import subprocess
import sys

from actions import utils

def export_theory_calls(args):
    utils.assert_is_executable(args.script)
    
    cmd = list(map(str, [
        args.script,
        '-p', args.partition,
        '--time-limit', args.timeout,
        '--solver-options', args.solver_options,
        '--benchmark-sets', args.benchmarks,
        '--working-dir', '{}'.format(args.target),
        args.solver
    ]))
    logging.info('Executing {}'.format(' '.join(cmd)))
    if not args.dry:
        subprocess.call(cmd)

    print('Once this slurm job has finished, continue with collecting the theory calls with:')
    print('\t{} collect-theory-calls {}'.format(sys.argv[0], args.target))
