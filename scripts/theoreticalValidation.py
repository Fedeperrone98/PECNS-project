import math

# Given values
T = 10e-9
c = 1.6e-9
M = 25000
k_mean = 4.6e-5

k = 1/k_mean

# Calculate 'a'
a = T * M * math.sqrt(2) / 2
#a = T * M / 2

# Calculate mean (E[S])
mean_S = c * (2 * math.pi) / (3 * T**2) * (a / 2)**3

# Calculate second moment (E[S^2])
second_moment_S = c * (2 * math.pi) / (4 * T**2) * (a / 2)**4

# Calculate variance (Var[S])
variance_S = second_moment_S - mean_S**2

mu = 1/mean_S

rho = k / mu

# Print the results
print("Mean (E[S]):", mean_S)
print("mu:", mu)
print("Variance (Var[S]):", variance_S)

print("rho:", rho)

print("k:", k)

e_n = rho + ((rho**2 + k**2 * variance_S)/(2*(1-rho)))
print("E[N]:", e_n)

e_nq = e_n - rho
print("E[Nq]:", e_nq)

e_r = e_n / k
print("E[R]:", e_r)
