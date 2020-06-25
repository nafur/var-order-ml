import logging
import os
import subprocess

from actions import utils

def generate_results(args):
    utils.assert_is_executable(args.script)

    cmd = [
        args.script,
        '-p', args.partition,
        '--time-limit', args.timeout,
        '--solver-options', args.solver_options,
        '--benchmark-sets', args.benchmarks,
        '--working-dir', args.target.format(solver = os.path.basename(args.solver)),
        args.solver
    ]
    logging.info('Executing {}'.format(' '.join(map(str, cmd))))
    # subprocess.call(cmd)
