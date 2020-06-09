from cs50 import SQL
from sys import argv
import csv

if len(argv) != 2:
    print("Usage: python import.py file.csv")
    exit()

file = csv.reader(open(argv[1]))
db = SQL("sqlite:///students.db")

next(file)
student_id = 0
for row in file:
    name = row[0].split()
    if len(name) == 3:
        db.execute("INSERT INTO students VALUES(?, ?, ?, ?, ?, ?)", student_id, name[0], name[1], name[2], row[1], int(row[2]))
    else:
        db.execute("INSERT INTO students VALUES(?, ?, ?, ?, ?, ?)", student_id, name[0], None, name[1], row[1], int(row[2]))
    student_id += 1