import logging
import os
import sys

def assert_is_executable(filename):
    if not os.path.isfile(filename):
        logging.error('given file {} does not exist'.format(filename))
        sys.exit(1)
    if not os.access(filename, os.X_OK):
        logging.error('given file {} is not executable'.format(filename))
        sys.exit(1)

def assert_is_empty_dir(dir):
    if not os.path.isdir(dir):
        os.makedirs(dir, exist_ok = True)
    elif len(os.listdir(dir)) > 0:
        logging.error('the directory {} is not empty'.format(dir))
        sys.exit(1)
