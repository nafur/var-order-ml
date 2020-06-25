import glob
import logging
import subprocess

def build_models(args):
    logging.info('download and build dlib')
    if not args.dry:
        subprocess.call(['make', 'dlib'], cwd = 'models')
    logging.info('building model binaries')
    if not args.dry:
        subprocess.call(['make'], cwd = 'models')

def train_model(args, binary, modelfile):
    logging.info('training {} to {}'.format(binary, modelfile))
    if not args.dry:
        subprocess.call([binary, args.data, modelfile])

def train(args):
    build_models(args)
    if args.models:
        for m in args.models:
            train_model(args, 'models/train_{}'.format(m), 'train_{}.model'.format(m))
    else:
        for model in glob.glob('models/train_*.cpp'):
            binary = model[:-4]
            modelfile = binary[7:] + '.model'
            train_model(args, binary, modelfile)
