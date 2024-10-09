# Final Project Report: Coding Schemes for Constrained Systems

## Project Information

- *Project Title*: Coding Schemes for Constrained Systems
- *Project Number*: P-2024-002
- *Department*: Electrical and Computer Engineering, Ben-Gurion University
- *Supervisors*: Ohad Elishco
- *Students*: Yehonatan Arama, Yuval Yacov Said

## Project Overview

This project focuses on developing an innovative encoding algorithm for systems constrained by preventing local repetitions. This type of encoding is critical for applications such as *data compression, **error correction, and **DNA storage systems*.

## Project Objectives

- *Goal*: Develop an efficient and decodable algorithm for generating “KM repeat-free” codes that prevent local repetitions in constrained systems.
- *Innovation*: A two-phase algorithm that compresses and then expands sequences while ensuring no repeated subsequences.
  
## Deliverables

1. *Computer Programs*:
    - *kmnRandomCheck.c*: Generates random sequences and validates them against defined constraints.
    - *CreateAndEliminate256183.c*: Implements the encoding algorithm, creating repeat-free sequences.
    - *Decode256183.c*: Implements the decoding algorithm, reconstructing the original sequences.
    
2. *Academic Document*:  
   Contains the formal notations, algorithm descriptions, and mathematical proofs of correctness for the proposed algorithm.
   
3. *Simulations*:  
   Visualize the performance of the encoding algorithm and the "language capacity" under various constraints. 

## Challenges and Solutions

The project encountered several challenges, including:

- *Long Simulation Time*: Optimized through advanced algorithmic design, reducing the runtime from exponential to linear.
- *Memory Constraints*: Adjustments were made to the algorithm to ensure it could run on modern computing systems with limited memory resources.

## Conclusion

The project succeeded in delivering an encoding algorithm that solves the *KM repeat-free* problem with minimal redundancy. However, improvements can be made to the algorithm's expansion phase, and further optimization for larger sequences is recommended.

## How to Use

1. *Encoding*: Run the CreateAndEliminate256183.c file to encode sequences based on the defined algorithm.
2. *Decoding*: Use the Decode256183.c file to decode sequences and verify the accuracy of the encoding.
3. *Validation*: The kmnRandomCheck.c file can be used to simulate and validate the sequence constraints.

## Contact Information

- *Yehonatan Arama*: yoni1arama@gmail.com
- *Yuval Yacov Said*: yuvalisaid@gmail.com

## References

1. O. Elishco, R. Gabrys, E. Yaakobi, and M. Medard. "Repeat-free codes". arXiv preprint arXiv:1909.05694, vol. 2, 2021.