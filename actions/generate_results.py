import logging
import os
import subprocess

def generate_results(args):
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
