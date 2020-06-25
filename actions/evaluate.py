import glob
import logging
import subprocess

def evaluate_model(args, binary, modelfile):
    logging.info('evaluating {} with {}'.format(binary, modelfile))
    if not args.dry:
        subprocess.call([binary, args.data, modelfile])

def evaluate(args):
    if args.models:
        for m in args.models:
            evaluate_model(args, 'models/evaluate_{}'.format(m), 'evaluate_{}.model'.format(m))
    else:
        for modelfile in glob.glob('train_*.model'):
            binary = 'models/evaluate_{}'.format(modelfile[6:-6])
            evaluate_model(args, binary, modelfile)
