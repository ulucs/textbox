
include(Pkg.dir("Knet/examples/mnist.jl"))

function main()
	#============================
	MNIST.xtrn, MNIST.ytrn
	MNIST.xtst, MNIST.ytst
	============================#
	
	# Size of input vector (MNIST images are 28x28)
	ninputs = 28 * 28

	# Number of classes (MNIST images fall into 10 classes)
	noutputs = 10

	ytrn = MNIST.ytrn
	ytst = MNIST.ytst
		
	xtrn = reshape(MNIST.xtrn,28*28,60000)
	xtst = reshape(MNIST.xtst,28*28,10000)
	
	W, b = init_params(ninputs, noutputs)
	
	debug = false #Turn this parameter off, after gradient checking passed
	
	if debug
		grad_check(W, b, xtrn[:, 1:100], ytrn[:, 1:100])
	end
	
	#Training parameters
	lr = 0.15
	epochs = 100

	for i=1:epochs
		cost, W, b = train(W, b, xtrn, ytrn, lr)
		
		ypred = softmax_forw(W, b, xtrn)
		trnacc = accuracy(ytrn, ypred)

		ypred = softmax_forw(W, b, xtst)
		tstacc = accuracy(ytst, ypred)
		
		@printf("epoch: %d softloss: %g trn accuracy: %g tst accuracy: %g\n", i, cost, trnacc, tstacc)
	end

end


function init_params(ninputs, noutputs)
	#takes number of inputs and number of outputs(number of classes)
	#returns randomly generated W and b (must be zeros vector) params of softmax
	#W matrix must be drawn from a gaussian distribution (mean=0, std=0.001)
	
	return randn(noutputs, ninputs), zeros(noutputs, 1)
end

function softmax_cost(W, b, X, Y)
	#takes W, b paremeters, X and Y (correct labels)
	#calculates soft loss, gradient of W and gradient of b
	#W and b are model parameters
	#W: (10 x 784)
	#b: (10 x 1)
	#X: (784 x m) m: number of instances
	#Y: (10 x m)
	#
	#returned gradients must have same sizes with W and b

	# Thank you, documentation!
	yi = softmax_forw(W,b,X)
	dy = yi - Y
	# assuming there are no complex inputs, ' is a good shorthand for transpose
	return -sum(Y.*log(yi))/size(dy,2), dy*X'/size(dy,2), sum(dy,2)./size(dy,2)
end

function softmax_forw(W, b, X)
	#applies affine transformation and softmax function
	#returns predicted probabilities
	#W and b are model parameters
	#W: (10 x 784)
	#b: (10 x 1)
	#X: (784 x m) m: number of instances
	#
	#Returned probabilities variable must be a matrix with size (10 x m)
	
	y = W*X.+b
	return exp(y)./sum(exp(y),1)
end

function grad_check(W, b, X, Y)
	#W and b are model parameters
	#W: (10 x 784)
	#b: (10 x 1)
	#X: (784 x m) m: number of instances
	#Y: (10 x m)

	function numeric_gradient()
		epsilon = 0.0001

		gw = zeros(size(W))
		gb = zeros(size(b))
		
		soft = y -> exp(y)./sum(exp(y),1)
		loss = y -> -sum(Y.*log(y))
		f = (w,c,x) -> loss(soft(w*x.+c))/size(X,2)
		dw = zeros(size(W))
		for i in eachindex(W)
			dw[i] = epsilon
			gw[i] = sum(f(W+dw,b,X)-f(W-dw,b,X))/epsilon/2
			dw[i] = 0
		end
		db = zeros(size(b))
		for i in eachindex(b)
			db[i] = epsilon
			gb[i] = sum(f(W,b+db,X)-f(W,b-db,X))/epsilon/2
			db[i] = 0
		end

		return gw, gb
	end

	_,gradW,gradB = softmax_cost(W, b, X, Y)
	gw, gb = numeric_gradient()
	
	diff = sqrt(sum((gradW - gw) .^ 2) + sum((gradB - gb) .^ 2))
	println("Diff: $diff")
	if diff < 1e-7
		println("Gradient Checking Passed")
	else
		println("Gradient Checking Failed!")
		println("Diff must be < 1e-7")
	end

end

function train(W, b, X, Y, lr=0.15)
	#W and b are model parameters
	#W: (10 x 784)
	#b: (10 x 1)
	#X: (784 x m) m: number of instances
	#Y: (10 x m)

	batchsize = 100
	
	# basic error avoidance for our crude way
	errsum = 0
	nbatches = floor(size(X,2)/batchsize)
	for i in batchsize*(0:nbatches-1)+1
		Xb = X[:,i:i+batchsize-1]
		Yb = Y[:,i:i+batchsize-1]
		err, gw, gb = softmax_cost(W, b, Xb, Yb)
		errsum += err
		# update parameters
		W -= lr*gw
		b -= lr*gb
	end
	return errsum/nbatches, W, b
end

function accuracy(ygold, ypred)
	correct = 0.0
	for i=1:size(ygold, 2)
		correct += indmax(ygold[:,i]) == indmax(ypred[:, i]) ? 1.0 : 0.0
	end
	return correct / size(ygold, 2)
end

main()