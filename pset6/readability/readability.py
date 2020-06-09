from cs50 import get_string

text = get_string("Text: ")
numLetters = 0
numWords = 1
numSetences = 0

for i in range(len(text)):
    if text[i].isalpha():
        numLetters = numLetters + 1
    elif text[i] == " ":
        numWords = numWords + 1
    elif text[i] in (".", "!", "?"):
        numSetences = numSetences + 1

grade = round(5.88 * numLetters / numWords - 29.6 * numSetences / numWords - 15.8)

if grade >= 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print("Grade ", grade)