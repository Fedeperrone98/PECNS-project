import math

############### HANDOVER #################
print("############### HANDOVER #################")

T = 10e-9
c = 1.6e-9
M = 25000
k_mean = 0.00002
t_mean = 0.00003

k = 1/k_mean
t = 1/t_mean

lambda_ = k+t

a = T * M / 2

# Calculate mean (E[S])
mean_S = c * (2 * math.pi) / (3 * T**2) * (a / 2)**3

# Calculate second moment (E[S^2])
second_moment_S = c * (2 * math.pi) / (4 * T**2) * (a / 2)**4

# Calculate variance (Var[S])
variance_S = second_moment_S - mean_S**2

mu = 1/mean_S

rho = lambda_ / mu

# Print the results
print("Mean (E[S]):", mean_S)
print("mu:", mu)
print("Variance (Var[S]):", variance_S)

print("rho:", rho)

print("k:", k)
print("t:", t)
print("lambda:", lambda_)

e_n = rho + ((rho**2 + (lambda_**2 * variance_S))/(2*(1-rho)))
print("E[N]:", e_n)

e_nq = e_n - rho
print("E[Nq]:", e_nq)

e_r = e_n / lambda_
print("E[R]:", e_r)
e_w = e_nq / lambda_
print("E[W]:", e_w)


############### NO HANDOVER #################
print("############### NO HANDOVER #################")

k_mean = 0.000012

k = 1/k_mean

lambda_ = k

rho = lambda_ / mu
print("rho:", rho)

print("lambda:", lambda_)

e_n = rho + ((rho**2 + (lambda_**2 * variance_S))/(2*(1-rho)))
print("E[N]:", e_n)

e_nq = e_n - rho
print("E[Nq]:", e_nq)

e_r = e_n / lambda_
print("E[R]:", e_r)

e_w = e_nq / lambda_
print("E[W]:", e_w)