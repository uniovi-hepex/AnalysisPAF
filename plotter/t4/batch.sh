# Run me from outside the plotter.

# Running with -q proof is only different in that it has a small time limit (~2h). -q batch has a 2 days limit
qsub -q batch -l nodes=1:ppn=30 plotter/t4/submit.sh
