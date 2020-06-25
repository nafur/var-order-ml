import logging
import os
import subprocess

from actions import utils

def generate_results(args):
    utils.assert_is_executable(args.script)

    target_dir = args.target.format(solver = os.path.basename(args.solver))
    cmd = list(map(str, [
        args.script,
        '-p', args.partition,
        '--time-limit', args.timeout,
        '--solver-options', args.solver_options,
        '--benchmark-sets', args.benchmarks,
        '--working-dir', target_dir,
        args.solver
    ]))
    logging.info('Executing {}'.format(' '.join(map(str, cmd))))
    if not args.dry:
        subprocess.call(cmd)

    print('When all jobs for the solvers has finished, collect the results as follows:')
    print('\t{} collect-results {} ...'.format(sys.argv[0], target_dir))

