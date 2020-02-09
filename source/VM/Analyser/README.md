# The Analyzer

Allows you instances of benchmarking, code analysis and code debugging through the IR of the RCL.

## Benchmark
There are two types of benchmarks:
 - **Inter**: analyse performances about the program
 - **Intra**: analyse performances about the VM

You might be interested by the *Inter Benchmark*.

## Debugger
There are also two types of debugger;
 - **Common Simplest Debugger (CSB)**: it analyzes the program from a high level point of view, without dwelling on technical details, it identifies among others bad combinations, heavy / infinite recursions ... The simplest and most common elements in fact.
 - **In-Depth Details Debugger (IDD)**: It analyzes, traces and debugs the program in greater depth, analyzing the generated assembler code and taking into account the system resources. It detects and prevents errors such as: segmentation, overflow, uninitialized memory area, pointer errors...

Both offer solutions, if possible, to the errors detected.
Depending on what you do, one or the other will be more appreciated. In general, you would prefer **CSB**; **IDD** is more specific to RCL contributors or low-level stuff.

## Diagnostic
The diagnostic returns a complete report of all the implementation information that was collected during the evaluation of a programme:
 - Performance (Inter + Intra)
 - Debug Analysis Report
 - Errors, warnings and information
 - Performance comparisons according to different optimizations
  
It regroups everything above, in a way, in a single report (human readable, in Markdown, or easily analyzable by a parser, in .txt).
