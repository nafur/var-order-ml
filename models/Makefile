DEPS := $(wildcard *.h)

all: bin/evaluate_cw_krr bin/evaluate_cw_svr bin/evaluate_krr bin/evaluate_mlp \
	bin/train_cw_krr bin/train_cw_svr bin/train_krr bin/train_mlp

dlib:
	git clone https://github.com/davisking/dlib.git
	cd dlib \
		&& mkdir build && cd build \
		&& cmake -DDLIB_JPEG_SUPPORT=OFF -DDLIB_PNG_SUPPORT=OFF ../ \
		&& make -j4 dlib

bin:
	mkdir -p bin/

OPTS := -std=c++11 -g -Wno-unknown-attributes -I dlib/ -L dlib/build/dlib/ -ldlib -lblas -llapack

bin/evaluate_cw_krr: evaluate.cpp $(DEPS) | bin
	clang++ -DCLASSIFIER="ClassificationWrapper<KRRRegression>" -o $@ $< $(OPTS)

bin/evaluate_cw_svr: evaluate.cpp $(DEPS) | bin
	clang++ -DCLASSIFIER="ClassificationWrapper<SVRRegression>" -o $@ $< $(OPTS)

bin/evaluate_krr: evaluate.cpp $(DEPS) | bin
	clang++ -DCLASSIFIER="KRRClassifier" -o $@ $< $(OPTS)

bin/evaluate_mlp: evaluate.cpp $(DEPS) | bin
	clang++ -DCLASSIFIER="MLPClassifier" -o $@ $< $(OPTS)

bin/train_cw_krr: train.cpp $(DEPS) | bin
	clang++ -DCLASSIFIER="ClassificationWrapper<KRRRegression>" -o $@ $< $(OPTS)

bin/train_cw_svr: train.cpp $(DEPS) | bin
	clang++ -DCLASSIFIER="ClassificationWrapper<SVRRegression>" -o $@ $< $(OPTS)

bin/train_krr: train.cpp $(DEPS) | bin
	clang++ -DCLASSIFIER="KRRClassifier" -o $@ $< $(OPTS)

bin/train_mlp: train.cpp $(DEPS) | bin
	clang++ -DCLASSIFIER="MLPClassifier" -o $@ $< $(OPTS)
