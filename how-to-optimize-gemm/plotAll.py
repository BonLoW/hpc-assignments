import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 15})

fig, ax = plt.subplots(figsize=(15, 10))
x = [8, 32, 256, 512, 1024, 4096]
gflops_pthread  = [7.293447e-04, 1.702234e-01, 3.658355e+00, 2.559624e+00, 2.181210e+00, 6.002578e-01]
gflops_cblas  = [4.778348e-04, 5.041231e+00, 1.259078e+01, 8.494793e+01, 8.819235e+01, 7.292089e+01]
gflops_openmp = [.482993e-03, 1.489455e+00, 3.111790e+00, 2.617350e+00, 2.629049e+00, 1.165917e+00]

plt.plot(x, gflops_pthread, label='pthread', color='blue', linestyle='-', linewidth=3.0)
plt.plot(x, gflops_cblas, label='cblas', color='green', linestyle='-.', linewidth=3.0)
plt.plot(x, gflops_openmp, label='openmp', color='purple', linestyle=':', linewidth=3.0)

plt.title('dgemm')
plt.xlabel('matrix size')
plt.ylabel('gflops')
plt.legend()

plt.savefig('result.png')
plt.show()
