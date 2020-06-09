from cs50 import SQL
from sys import argv

if len(argv) != 2:
    print("Usage: python roster.py housename")
    exit()

db = SQL("sqlite:///students.db")
result = db.execute("SELECT first, middle, last, birth from students WHERE house = ? ORDER BY last, first", argv[1])

for row in result:
    if row.get("middle") == None:
        print(row.get("first"), row.get("last") + ", born", row.get("birth"))
    else:
        print(row.get("first"), row.get("middle"), row.get("last") + ", born", row.get("birth"))