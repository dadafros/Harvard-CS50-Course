def main():
    card_number = input("Number: ")
    if verify_check_sum(card_number):
        if len(card_number) == 15 and int(card_number[0:2]) in (34, 37):
            print("AMEX")
        elif len(card_number) == 16 and int(card_number[0:2]) in range(51, 56):
            print("MASTERCARD")
        elif len(card_number) in (13, 16) and card_number[0] == "4":
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")


def verify_check_sum(num):
    check_sum = 0
    length = len(num)
    for i in reversed(range(length)):
        if (i - (length - 1)) % 2 != 0:
            if 2 * int(num[i]) > 9:
                check_sum += 2 * int(num[i]) % 10
                check_sum += 2 * int(num[i]) // 10
            else:
                check_sum += 2 * int(num[i])
        else:
            check_sum += int(num[i])
    if check_sum % 10 == 0:
        return True
    else:
        return False


main()