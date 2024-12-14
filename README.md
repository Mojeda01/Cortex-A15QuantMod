# Quant mod for CoreTile-A15x2

I am using the CoreTile-A15x2 to build a low-level system for probability models, stochastic variables, calculus, ML, information theory, statistics, optimziation problem analysis, numerical solvers (ODEs and PDEs), matrix factorizations, and sparse matrix support. This low-level approach deepends the understanding by tackling these topics at their most fundamental and challenging level. So, this is a challenge for myself, of course. But if successful, I would like to utilize it as an tool for solving investment problems.

## Core Components
1. Probability Models:
    * Foundational probability distributions (e.g., normal, binomial, Poisson).
    * Bayesian inference and Monte Carlo Simulation methods.
    * Stochasstic processes such as Markov Chains and Brownian Motion.

2. Stochastic Variables and Calculus:
    * Random variable generation and transformation.
    * Stochastic differential equations (SDEs) solvers (e.g., Euler-Maruyama).

3. Machine Learning:
    * Linear models (linear and logistic regression):
    * Algorithms like decision trees and basic neural networks.

4. Information Theory and statistics:
    * Entropy, mutual information, and coding theory algorithms.
    * Descriptive and inferential statistical tools.

5. Optimization Problem analysis:
    * Solvers for both constrained and unconstrained optimization.
    * Gradient-based methods (e.g., L-BFGS) and gradient-free methods (e.g., genetic algorithms).

6. Heavy Equation solving:
    * Numeric solvers for differential equations (ODEs and PDEs).
    * Symbolic math capabilities (e.g., symbolic integrationand SDE solutions).

7. Numerical Linear Algebra:
    * matrix factorizations (LU, QR, SVD).
    * Support for sparse matrices and operations for efficient computation.

8. Low-level Optimziations
    * Hand-tuned assembly routines for critical performance bottlenecks.
    * Use of Cortex-A15's NEON SIMD instructions for vectorized matrix operations.

9. Integration with scheduler:
    * Task scheduling for efficient parallel or sequential computation.
    * Multi-core capabilities leveraging Cortex-A15's architecture for load distribution.

10. Networking capabilities:
    * Protocol support: Develop low-level support for basic networking protocols (TCP/IP stack implementation or lightweight alternatives like LWIP).
    * Real-Time Communication: Support for data exchange and distributed computing tasks.
    * Data Transmission Security: Add encryption mechanisms for secure data communication.
    * Integration with platform: Enable real-time updates or remote access to platform computations.

11. Debugging and profiling tools
    * Basic utilities for debugging, such as memory usage tracking.
    * Performance profiling using Cortex-A15's Performance Monitor Unit (PMU).

12. I/O system
    * Low-level interface for reading/writing datasets directly to memory.
    * Support for real-time data streams.

---> These are the things that I would like to integrate into my project, but reality * time = very unlikely, so for now, this will be purely a inspiration board and nothing else. I would also like to include a sort-of half-roadmap:

### Phase 1: Foundation:
1. Environment Setup
    * Setup Cortex-A15 simulation,
    * Configure Barebox bootloader for program loading.

2. Low-Level Framework
    * Develop a custom memory allocator and basic math routines (e.g., floating-point operations).
    * Build a low-level API for matrix and vector operations.

3. Core Computaions:
    * Implement basic probability distributions and statistical methods.
    * Develop random variable generators.

### Phase 2: Intermediate Functionality
1. Stochastic Calculus and Optimization
    * Implement stochastic process simulations.
    * Build a library for solving constrained and unconstrained optimization problems.

2. Integration with Machine Learning
    * Add simple models like linear regression and decision trees.
    * Optimize performance using Cortex-A15's hardware features.

3. Equation Solvers
    * Implement numeric solvers for ODEs and PDEs.
    * Add symbolic computation for differentiation and integration.

### Phase 3: Advanced capabilities
1. Numerical Linear Algebra:
    * Implement matrix factorizations (LU, QR, SVD).
    * Add sparse matrix operations for efficient computation.

2. Scalability and Performance:
    * Optimize solvers and matrix operations with NEON SIMD instructions.
    * Add multi-core support for parallel computations.

3. Information Theory Tools :
    * Develop entropy-based metrics and mutual information calculations.

4. Debugging and Profiling;:
    * Integrate debugging utilities and performance profiling using Cortex-A15's PMU.

### Phase 4: Finalization and Testing 
1. Comprehensive Testing 
    * Validate implementations with well-known benchmarks.
    * Stress-test the platform with large datasets and high-load scenarios.

2. Documentation
    * Create user documentation and code-level comments.

3. Real-world applications:
    * Test the platform with investment problems and gambling scenarios.
