DEPS := $(wildcard *.h)

all: evaluate_cw_krr evaluate_cw_svr evaluate_krr train_cw_krr train_cw_svr train_krr

download_dlib:
	git clone https://github.com/davisking/dlib.git
	cd dlib \
		&& mkdir build && cd build \
		&& cmake -DDLIB_JPEG_SUPPORT=OFF -DDLIB_PNG_SUPPORT=OFF ../ \
		&& make -j4 dlib


OPTS := -Wno-unknown-attributes -I dlib/ -L dlib/build/dlib/

%: %.cpp $(DEPS)
	clang++ -std=c++11 -g -o $@ $< $(OPTS) -ldlib -lblas -llapack
