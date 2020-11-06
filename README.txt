COL761 Assignment 1: Report

----------------------------------------------

Rishabh Ranjan      2018CS10416
Rohan Debbarma      2018CS50417
Chirag Bansal       2018CS50402

------------------------------------------------

Code Explanation:


There are 2 cpp files which contain the implementation of the FP-tree and the apriori algorithm. 
The shell files compile.sh compiles the executables and the 2018CS50402.sh will run do as mentioned. 
The plot.py is a python script responsible for plotting the graphs. 
The Makefile compiles the files, this is used by compile.sh.
Our implementation offers very good performance.
However, there is a drastic increase in the runtime on webdocs.dat when the support threshold is changed from 11% to 10% (10% does not return even after an hour). 

We have investigated this issue by analysing CPU and memory usage characteristics and come to the conclusion that this happens because at 10% the FP-tree is huge and the system runs out of memory to perform efficient construction and mining.

This problem cannot be avoided as with such large datasets at a sufficiently low threshold the FP-tree becomes unreasonably large. We prioritize runtime efficiency for the thresholds of common interests and choose not to optimize memory usage at the cost of time, this makes only a superficial difference as main memory limits are arbitrary and finite anyway.

--------------------------------------------------

Explanation of Execution Time Plot (Part B):


The running time for the FP-tree is lesser than the apriori as in the apriori tree we generate many duplicated candidate sets which is not time efficient, apriori algorithm uses a Breadth-first search approach, generating candidate itemsets which increase the execution time.
On the other hand, the FP-growth algorithm doesn’t have any candidate set generation.

The running time for apriori rises exponentially as the support threshold is reduced, whereas for FP-tree the running time is linearly increasing. As the support threshold is reduced, there is a greater chance to get larger frequent itemsets, the pattern matching operation of comparing candidate sets with transactions becomes expensive. 

This means that the database will be read a larger number of times, as opposed to a constant of 2 scans for FP-tree. Thus the time for apriori increases accordingly.

At higher support thresholds, we observe comparable running time for both the algorithms. 
However, the FP-growth algorithm is still slightly better as the time complexity of apriori is O(m^2 * n), where m is the number of distinct items and n is the number of transactions while the FP-growth algorithm is O(n) which is faster.

In the Fp-tree, there is no candidate generation and hence it requires less memory. It removes the need to calculate the pairs to be counted. 
The FP-Growth algorithm stores in memory a compact version of the database. 

