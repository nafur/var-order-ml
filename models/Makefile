DEPS := $(wildcard *.h)

all: evaluate_cw_krr evaluate_cw_svr evaluate_krr evaluate_mlp \
	train_cw_krr train_cw_svr train_krr train_mlp

dlib:
	git clone https://github.com/davisking/dlib.git
	cd dlib \
		&& mkdir build && cd build \
		&& cmake -DDLIB_JPEG_SUPPORT=OFF -DDLIB_PNG_SUPPORT=OFF ../ \
		&& make -j4 dlib


OPTS := -std=c++11 -g -Wno-unknown-attributes -I dlib/ -L dlib/build/dlib/ -ldlib -lblas -llapack

evaluate_cw_krr: evaluate.cpp $(DEPS)
	clang++ -DCLASSIFIER="ClassificationWrapper<KRRRegression>" -o $@ $< $(OPTS)

evaluate_cw_svr: evaluate.cpp $(DEPS)
	clang++ -DCLASSIFIER="ClassificationWrapper<SVRRegression>" -o $@ $< $(OPTS)

evaluate_krr: evaluate.cpp $(DEPS)
	clang++ -DCLASSIFIER="KRRClassifier" -o $@ $< $(OPTS)

evaluate_mlp: evaluate.cpp $(DEPS)
	clang++ -DCLASSIFIER="MLPClassifier" -o $@ $< $(OPTS)

train_cw_krr: train.cpp $(DEPS)
	clang++ -DCLASSIFIER="ClassificationWrapper<KRRRegression>" -o $@ $< $(OPTS)

train_cw_svr: train.cpp $(DEPS)
	clang++ -DCLASSIFIER="ClassificationWrapper<SVRRegression>" -o $@ $< $(OPTS)

train_krr: train.cpp $(DEPS)
	clang++ -DCLASSIFIER="KRRClassifier" -o $@ $< $(OPTS)

train_mlp: train.cpp $(DEPS)
	clang++ -DCLASSIFIER="MLPClassifier" -o $@ $< $(OPTS)
