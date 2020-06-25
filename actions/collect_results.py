import csv
import glob
import json
import logging
import os
import progressbar
import subprocess
import sys

def collect_features(benchmark_dir):
    files = glob.glob("{}/*.smt2".format(benchmark_dir))
    features = {}

    bar = progressbar.ProgressBar(maxval = len(files)).start()
    for file in bar(files):
        logging.debug('parsing features from {}'.format(file))
        lines = open(file, 'r').readlines()
        lines = filter(lambda s: s[0] == ';', lines)
        lines = map(lambda s: s[2:], lines)
        features[os.path.basename(file)] = json.loads("".join(lines))
    bar.finish()
    return features

def export_as_csv(args):
    bar = progressbar.ProgressBar(maxval = len(args.source)).start()
    csvs = {}
    for source in bar(args.source):
        csvfilename = os.path.join(source, 'data.csv')
        logging.info('generating csv from {} to {}'.format(source, csvfilename))
        output = open(csvfilename, 'w')
        subprocess.call([args.script, '--csv', '--no-vbs', source], stdout = output)
        solvername = os.path.basename(source)
        csvs[solvername] = csvfilename
    bar.finish()
    return csvs

def merge_solver_results(results, solvername, csvfilename):
    file = open(csvfilename)
    reader = csv.DictReader(file)
    for row in reader:
        if not row['benchmark'] in results:
            logging.info('found results for {} on {}, but no features were parsed for this file'.format(solvername, row['benchmark']))
            continue
        if row['time_cpu'] == '':
            logging.info('no cpu time was given for {} on {}'.format(solvername, row['benchmark']))
            continue
        assert row['benchmark'] in results
        results[row['benchmark']]['results'][solvername] = float(row['time_cpu'])
        logging.debug('loaded result for {} on {}: {}'.format(solvername, row['benchmark'], float(row['time_cpu'])))

def collect_results(args):
    if not os.path.isfile(args.script):
        logging.error('given script {} does not exist'.format(args.script))
        sys.exit(1)

    features = collect_features(args.benchmarks)
    csvs = export_as_csv(args)
    results = { f: {'features': features[f], 'results': {}} for f in features }
    for solver in csvs:
        merge_solver_results(results, solver, csvs[solver])
    json.dump(results, open(args.result, 'w'), indent = 2)
