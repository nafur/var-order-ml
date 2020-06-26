import glob
import logging
import subprocess

def build_models(args):
    logging.info('download and build dlib')
    if not args.dry:
        subprocess.call(['make', 'dlib'], cwd = 'models')
    logging.info('building model binaries')
    if not args.dry:
        subprocess.call(['make', '-j4'], cwd = 'models')

def train_model(args, binary, modelfile):
    logging.info('training {} to {}'.format(binary, modelfile))
    if not args.dry:
        subprocess.call([binary, args.data, modelfile])

def train(args):
    build_models(args)
    if args.models:
        for m in args.models:
            train_model(args, 'models/bin/train_{}'.format(m), 'train_{}.model'.format(m))
    else:
        for binary in glob.glob('models/bin/train_*'):
            modelfile = binary[11:] + '.model'
            train_model(args, binary, modelfile)
