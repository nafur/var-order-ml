DEPS := $(wildcard *.h)

all: evaluate_cw_krr evaluate_cw_svr evaluate_krr train_cw_krr train_cw_svr train_krr

%: %.cpp $(DEPS)
	clang++ -std=c++11 -g -o $@ $< -ldlib -lblas -llapack