import glob
import logging
import os
import progressbar
import shutil
import sys

from actions import utils

def collect_theory_calls(args):
    target = args.target_dir
    utils.assert_is_empty_dir(target)
    benchmark_list = []
    for s in args.source:
        logging.info('Processing source "{}"'.format(s))
        files = glob.glob("{}/**/*.smt2".format(s), recursive = True)
        bar = progressbar.ProgressBar(maxval = len(files)).start()
        for file in bar(files):
            if not os.path.isfile(file):
                continue

            basepart = os.path.basename(os.path.dirname(file))
            tcpart = os.path.basename(file)
            targetfile = os.path.join(target, "{}-{}".format(basepart, tcpart))
            benchmark_list.append(targetfile)

            logging.debug('Copying "{}" to "{}"'.format(file, targetfile))
            if not args.dry:
                shutil.copy(file, targetfile)
        bar.finish()
    
    benchmarkset_file = 'benchmark_set_{}'.format(target)
    logging.info('Writing list of benchmarks to {}'.format(benchmarkset_file))
    if not args.dry:
        f = open(benchmarkset_file, 'w')
        f.write('\n'.join(benchmark_list) + '\n')
        f.close()
    
    print('Now run every relevant solver on the collect benchmarks as follows:')
    print('\t{} generate-results --benchmarks {} --solver {{solver}}'.format(sys.argv[0], target))
