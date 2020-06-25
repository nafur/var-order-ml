import logging
import subprocess

def export_theory_calls(args):
    cmd = [
        args.script,
        '-p', args.partition,
        '--time-limit', args.timeout,
        '--solver-options', args.solver_options,
        '--benchmark-sets', args.benchmarks,
        '--working-dir', '{}'.format(args.target),
        args.solver
    ]
    logging.info('Executing {}'.format(' '.join(map(str, cmd))))
    # subprocess.call(cmd)