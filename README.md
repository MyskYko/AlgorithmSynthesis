# AlgorithmSynthesis

Everything is written in C++.
"src" is source directory, "include" is include directory.
"program" is directory for ather tools like graphing tool.
All the other directories are testcase. Note that some testcases are outdated and not able to be synthesized.

First type "make" inside the directory. Executable will be generated as "/bin/gen".

For example, move to "matrix2".
Type "../bin/gen spec.blif data.txt out.blif setting.txt"
It generates out.blif which is dataflow(expressed in BLIF) of 2x2 matrix vector multiplication with mutually connected 2 nodes.

"spec.blif" is the specification file that calcultes 2x2 matrix without thinking structure of nodes.
"data.txt" is the adjacency matrix of the target structure for directed graph. https://en.wikipedia.org/wiki/Adjacency_matrix
"setting.txt" is the setting file. It defines the number of register, the number of operand in 1 LUT, and many options. The explanations for options are written in the attched paper.
Currently the configurations of options in "setting.txt" are complicated. If you want to change them, please ask me.

Graphing tool is "programs/dotgen". You can see and compile its code "dotgen.cpp".
Type "./dotgen out.blif out.png". It generates dataflow png in "out.png".
A result for matrix2 is attached.

For more details, see "readme.pdf" or please ask me. 

This program will appear in paper of ISQED 2019, live demo of ISCAS 2019, Ubooth of DATE2019.
