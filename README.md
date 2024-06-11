# CHIYODIFF
An automatic differentiation library with support for arbitrary tensor operations. If you define a functions from shape X tensors to shape Y tensors, and also define the derivative, then you can use it in chiyodiff.

Built in support for matmuls, common scalar operations (+,-,\*,/,exp,log,sin,cos) and tensorizing scalar operations.

`make` will compile chiyodiff.c which just has a testing function right now.

Breakdown of code files:
- `A.h`: a dynamically resized array implementation, used for adjacency lists and queues
- `funcs.h`: mathematical functions that you want to compute
- `tensor.h`: defines tensors and various helper functions for them
- `chiyodiff.h`: main computational graph stuff
