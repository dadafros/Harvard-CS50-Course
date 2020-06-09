from sys import argv
import csv

if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")

database = csv.DictReader(open(argv[1]))
sequence = open(argv[2], "r").read()

lruns = []
for STRs in database.fieldnames:
    if STRs != "name":
        aux = 0
        cut = 0
        longest_run = 0
        while cut < len(sequence) and aux != -1:
            run = 0
            first_time = True
            while True:
                aux = sequence.find(STRs, cut)
                if first_time:
                    cut = aux
                    first_time = False
                if aux != -1 and aux == cut:
                    cut = aux + len(STRs)
                    run = run + 1
                else:
                    break
            if run > longest_run:
                longest_run = run
        lruns.append(longest_run)

for row in database:
    ident = []
    equal = 1
    for STRs in database.fieldnames:
        if STRs != "name":
            ident.append(int(row.get(STRs)))
    for i in range(len(lruns)):
        if lruns[i] != ident[i]:
            equal = 0
    if equal:
        print(row["name"])
        quit()
print("No match")