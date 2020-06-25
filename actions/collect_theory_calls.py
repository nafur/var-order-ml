import glob
import logging
import os
import progressbar
import shutil
import sys

def collect_theory_calls(args):
    target = args.target_dir
    if not os.path.isdir(target):
        os.makedirs(target, exist_ok = True)
    elif len(os.listdir(target)) > 0:
        logging.error('The given target directory {} is not empty.'.format(target))
        sys.exit(1)
    for s in args.source:
        logging.info('Processing source "{}"'.format(s))
        files = glob.glob("{}/**/*.smt2".format(s), recursive = True)
        bar = progressbar.ProgressBar(maxval = len(files)).start()
        for file in bar(files):
            logging.debug('Processing file "{}"'.format(file))
            shutil.copy(file, target)
        bar.finish()